/*
 * ScRamble - 3D Flight Racer
 * Copyright (C) 2010  Bhanu Chetlapalli
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>        // Plain-C interface
#include <assimp.h>        // Plain-C interface
#include <aiScene.h>       // Output data structure
#include <aiPostProcess.h> // Post processing flags
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char **argv)
{
	const aiScene *scene;
	if (argc != 3) {
		printf("Usage: %s srcmodel outputfile\n", argv[0]);
		return -1;
	}
	
	scene = aiImportFile(argv[1],
						aiProcess_CalcTangentSpace       | 
						aiProcess_Triangulate            |
						aiProcess_JoinIdenticalVertices  |
						aiProcess_SortByPType);

	if (!scene) {
		printf("Unable to load scene!\n");
		return -1;
	}

	int fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0755);
	if (fd < 0) {
		printf("Unable to open dest: %s\n", argv[2]);
		return -1;
	}

	struct cbkModelHeader {
		unsigned int version;
		unsigned int magic;
		unsigned int totFileSize;
		unsigned int totTris;
		unsigned int totFaces;
		unsigned int numMeshes;
		float max[3];
		float min[3];
	};
	cbkModelHeader hdr;
	memset(&hdr, 0, sizeof(hdr));
	hdr.version = 2;
	hdr.magic = 0xDEADBEEF;
	hdr.numMeshes = (int)scene->mNumMeshes;
	hdr.max[0] = hdr.max[1] = hdr.max[2] = -10000000.0f;
	hdr.min[0] = hdr.min[1] = hdr.min[2] = +10000000.0f;

	write(fd, &hdr, sizeof(hdr));

	/* Load into OpenGL */
	for (int i = 0; i < (int)scene->mNumMeshes; i++) {
		const aiMesh *mesh = scene->mMeshes[i];
		const aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];

		unsigned int numFaces = (int)mesh->mNumFaces;

#define	CBK_MODEL_MATERIAL_TYPE_INVALID			0x0
#define	CBK_MODEL_MATERIAL_TYPE_DIFFUSE			0x1
#define	CBK_MODEL_MATERIAL_TYPE_SPECULAR		0x2
#define	CBK_MODEL_MATERIAL_TYPE_AMBIENT			0x3
#define	CBK_MODEL_MATERIAL_TYPE_EMISSION		0x4
#define	CBK_MODEL_MATERIAL_TYPE_SHININESS		0x5
		struct material {
			int type;
			float c[4];
		};
		struct facehdr {
			unsigned int numFaces;
			int nummats;
			struct material mats[16];
		} fhdr;
		memset(&fhdr, 0, sizeof(fhdr));

		aiColor4D aic;
		float aif;
		int matnum = 0;
		if (aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &aic) == AI_SUCCESS) {
			fhdr.mats[matnum].type = CBK_MODEL_MATERIAL_TYPE_DIFFUSE;
			fhdr.mats[matnum].c[0] = aic.r;
			fhdr.mats[matnum].c[1] = aic.g;
			fhdr.mats[matnum].c[2] = aic.b;
			fhdr.mats[matnum].c[3] = aic.a;
			matnum++;
		}
		if (aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &aic) == AI_SUCCESS) {
			fhdr.mats[matnum].type = CBK_MODEL_MATERIAL_TYPE_SPECULAR;
			fhdr.mats[matnum].c[0] = aic.r;
			fhdr.mats[matnum].c[1] = aic.g;
			fhdr.mats[matnum].c[2] = aic.b;
			fhdr.mats[matnum].c[3] = aic.a;
			matnum++;
		}
		if (aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &aic) == AI_SUCCESS) {
			fhdr.mats[matnum].type = CBK_MODEL_MATERIAL_TYPE_AMBIENT;
			fhdr.mats[matnum].c[0] = aic.r;
			fhdr.mats[matnum].c[1] = aic.g;
			fhdr.mats[matnum].c[2] = aic.b;
			fhdr.mats[matnum].c[3] = aic.a;
			matnum++;
		}
		if (aiGetMaterialColor(mat, AI_MATKEY_COLOR_EMISSIVE, &aic) == AI_SUCCESS) {
			fhdr.mats[matnum].type = CBK_MODEL_MATERIAL_TYPE_EMISSION;
			fhdr.mats[matnum].c[0] = aic.r;
			fhdr.mats[matnum].c[1] = aic.g;
			fhdr.mats[matnum].c[2] = aic.b;
			fhdr.mats[matnum].c[3] = aic.a;
			matnum++;
		}
		if (aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &aif) == AI_SUCCESS) {
			fhdr.mats[matnum].type = CBK_MODEL_MATERIAL_TYPE_SHININESS;
			fhdr.mats[matnum].c[0] = aic.r;
			matnum++;
		}
		fhdr.numFaces = numFaces;
		fhdr.nummats = matnum;

		write(fd, &fhdr, sizeof(fhdr));
		hdr.totFaces += numFaces;

		for (int j = 0; j < (int)mesh->mNumFaces; j++) {
			const aiFace *face = &mesh->mFaces[j];
			const aiVector3D *v = mesh->mVertices;
		
			unsigned int numIndices = face->mNumIndices;
			write(fd, &numIndices, sizeof(numIndices));
			hdr.totTris += numIndices;

			for (int k = 0; k < (int)face->mNumIndices; k++) {
				unsigned int idx = face->mIndices[k];

				float a[3];
				a[0] = v[idx].x;
				a[1] = v[idx].y;
				a[2] = v[idx].z;

				int l;
				for (l = 0; l < 3; l++) {
					if (a[l] > hdr.max[l]) hdr.max[l] = a[l];
					if (a[l] < hdr.min[l]) hdr.min[l] = a[l];
				}

				write(fd, a, sizeof(a));
			}
		}
	}
	hdr.totFileSize = lseek(fd, 0, SEEK_END);

	lseek(fd, 0, SEEK_SET);
	write(fd, &hdr, sizeof(hdr));

	close(fd);

	return 0;
}

