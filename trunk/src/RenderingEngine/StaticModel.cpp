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

#include "StaticModel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define CBK_FILEFORMAT_MAGIC		0xDEADBEEF
#define CBK_FILEFORMAT_CUR_VERSION	0x2

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

#define CBK_MODEL_MATERIAL_TYPE_INVALID		0x0
#define CBK_MODEL_MATERIAL_TYPE_DIFFUSE		0x1
#define CBK_MODEL_MATERIAL_TYPE_SPECULAR	0x2
#define CBK_MODEL_MATERIAL_TYPE_AMBIENT		0x3
#define CBK_MODEL_MATERIAL_TYPE_EMISSION	0x4
#define CBK_MODEL_MATERIAL_TYPE_SHININESS	0x5
struct material {
	int type;
	float c[4];
};
struct facehdr {
	unsigned int numFaces;
	int nummats;
	struct material mats[16];
};

StaticModel::StaticModel(void)
{
	data = NULL;
	transx = transy = transz = 0;
	scalefactor = 1.0f;
}

StaticModel::~StaticModel()
{
	if (data)
		free(data);
}

int StaticModel::load(const char *f)
{
	int err = -1;
	int fd;
	int rem = 0;
	unsigned char *filedata = NULL;

	int i, j, k;
	int *ptr = NULL;
	int curpos = 0;

	strncpy(fn, f, 256);

	fd = open(fn, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Warning: Unable to load %s: %s\n", fn, strerror(errno));
		return -1;
	}

	struct cbkModelHeader hdr;
	read(fd, &hdr, sizeof(hdr));
	if (hdr.version != CBK_FILEFORMAT_CUR_VERSION) {
		fprintf(stderr, "Warning: Unable to load %s: Unknown version(%d)\n", fn, hdr.version);
		goto out;
	}
	if (hdr.magic != CBK_FILEFORMAT_MAGIC) {
		fprintf(stderr, "Warning: Unable to load %s: Unknown version(%d)\n", fn, hdr.version);
		goto out;
	}

	fprintf(stderr, "Info: Loaded(%s) with triangles:%d faces:%d meshes:%d "
			"filesz:%d\n", fn, hdr.totTris, hdr.totFaces, hdr.numMeshes,
			hdr.totFileSize);

	numMeshes = hdr.numMeshes;

	filedata = (unsigned char *)malloc(hdr.totFileSize);
	if (filedata == NULL) {
		fprintf(stderr, "Warning: Unable to load %s: No Memory\n", fn);
		goto out;
	}
	data = filedata;

	rem = hdr.totFileSize - sizeof(hdr);
	while (rem) {
		int red = read(fd, filedata, rem);
		if (red < 0) {
			fprintf(stderr, "Warning: Unable to read(num:%d) data of %s: %s\n", rem, fn, strerror(errno));
			goto out;
		}
		if (red == 0) {
			fprintf(stderr, "Warning: EOF recieved while reading data of %s. Remaining to read:%d\n", fn, rem);
			goto out;
		}
		rem -= red;
		filedata += red;
	}

	transx = -(hdr.max[0] + hdr.min[0]) / 2;
	transy = -(hdr.max[1] + hdr.min[1]) / 2;
	transz = -(hdr.max[2] + hdr.min[2]) / 2;

	float expx, expy, expz;
	expx = hdr.max[0] - hdr.min[0];
	expy = hdr.max[1] - hdr.min[1];
	expz = hdr.max[2] - hdr.min[2];
	scalefactor = expx;
	if (expx < expy)
		scalefactor = expy;
	if (expy < expz)
		scalefactor = expz;
	scalefactor /= 200.0f;
	
	fprintf(stderr,"max:%f %f %f min:%f %f %f exp:%f %f %f scf:%f\n", hdr.max[0], hdr.max[1], hdr.max[2], hdr.min[0], hdr.min[1], hdr.min[2],
			expx, expy, expz, scalefactor);
	err = 0;

	/* OK Create a display list */
	ptr = (int*)data;
	list = glGenLists(1);
	glNewList(list, GL_COMPILE);
	for (i = 0; i < numMeshes; i++) {

		struct facehdr *fhdr = (struct facehdr*)&ptr[curpos];
		
		GLint whichface = GL_FRONT;
		for (j = 0 ; j < fhdr->nummats; j++) {
			switch (fhdr->mats[j].type) {
				case CBK_MODEL_MATERIAL_TYPE_DIFFUSE:
					glMaterialfv(whichface, GL_DIFFUSE, fhdr->mats[j].c);
					break;
				case CBK_MODEL_MATERIAL_TYPE_SPECULAR:
					glMaterialfv(whichface, GL_SPECULAR, fhdr->mats[j].c);
					break;
				case CBK_MODEL_MATERIAL_TYPE_AMBIENT:
					glMaterialfv(whichface, GL_AMBIENT, fhdr->mats[j].c);
					break;
				case CBK_MODEL_MATERIAL_TYPE_EMISSION:
					glMaterialfv(whichface, GL_EMISSION, fhdr->mats[j].c);
					break;
				case CBK_MODEL_MATERIAL_TYPE_SHININESS:
					glMaterialf(whichface, GL_SHININESS, fhdr->mats[j].c[0]);
					break;
				case CBK_MODEL_MATERIAL_TYPE_INVALID:
				default:
					break;
			}
		}

		curpos += sizeof(*fhdr)/sizeof(int);

		glBegin(GL_TRIANGLE_FAN);
		glColor3f(1.0f, 1.0f, 1.0f);
		for (j = 0; j < (int)fhdr->numFaces; j++) {
			int numIndices = ptr[curpos++];
			float *fp = (float*)&ptr[curpos];
			for (k = 0; k < numIndices; k++) {
				glVertex3f((fp[0] + transx)/scalefactor, (fp[1] + transy)/scalefactor, (fp[2] + transz)/scalefactor);
				fprintf(stderr, "%f %f %f\n", (fp[0] + transx)/scalefactor, (fp[1] + transy)/scalefactor, (fp[2] + transz)/scalefactor);
				fp += 3;
			}
			curpos += numIndices * 3;
		}
		glEnd();
	}
	glEndList();

	fprintf(stderr, "Info: Loaded(%s) with triangles:%d faces:%d meshes:%d "
			"filesz:%d\n", fn, hdr.totTris, hdr.totFaces, hdr.numMeshes,
			hdr.totFileSize);
	fprintf(stderr,"max:%f %f %f min:%f %f %f exp:%f %f %f scf:%f\n", hdr.max[0], hdr.max[1], hdr.max[2], hdr.min[0], hdr.min[1], hdr.min[2],
			expx, expy, expz, scalefactor);
 out:
	close(fd);
	if (data) {
		free(data);
		data = NULL;
	}
	return err;
}

extern float degx,degy,degz;
void StaticModel::render(Camera *c)
{
//	static float x,y,z;

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	float light_pos[4] = {c->camx, c->camy, c->camz, 0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	glPushMatrix();
	glTranslatef(c->pointx + degx, c->pointy, c->pointz + degz - 300);
	glRotatef(-90, 1, 0, 0);
	glRotatef(180 + degy, 0, 0, 1);

	glCallList(list);

	glPopMatrix();
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
}

