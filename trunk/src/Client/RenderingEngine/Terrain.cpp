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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Terrain.h"
#include "DebugGL.h"

Terrain::Terrain(void)
{
	imgWidth = imgHeight = 0;
}

Terrain::~Terrain()
{
}

int Terrain::load(const char *name, const char *texName)
{
	strncpy(fn, name, 256);

	int err = -1;
	struct _TgaHeader {
		unsigned char id;
		unsigned char colormaptype;
		unsigned char imagetype;
		unsigned char colormapspec[5];
		struct {
			unsigned short xorg;
			unsigned short yorg;
			unsigned short imgWidth;
			unsigned short imgHeight;
			unsigned char pixelDepth;
			unsigned char imgDesc;
		};

	} tgaHdr;

	t = new Texture(texName);
	t->Load();

	FILE *fp = fopen(fn, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Unable to open terrain map file: %s\n", fn);
		goto out;
	}

	if (fread(&tgaHdr, sizeof(_TgaHeader), 1, fp) != 1) {
		fprintf(stderr, "Unable to read terrain map file header: %s\n", fn);
		goto out;
	}
	printf("TGA Header Info(%s): ID(%d) colormap(%d) width(%d) height:(%d)\n",
			fn, tgaHdr.id, tgaHdr.colormaptype, tgaHdr.imgWidth, tgaHdr.imgHeight);

	imgWidth = tgaHdr.imgWidth;
	imgHeight = tgaHdr.imgHeight;

	texRepeatX = texRepeatX = 8;
	texXf = imgWidth / texRepeatX;
	texYf = imgWidth / texRepeatX;

	data = (unsigned char *)malloc(imgWidth * imgHeight);
	if (data == NULL) {
		fprintf(stderr, "Unable to allocate terrain map data size(%d)\n", imgHeight * imgWidth);
		goto out;
	}

	if (fread(data, imgWidth * imgHeight, 1, fp) != 1) {
		fprintf(stderr, "Unable to read terrain map data: %s\n", fn);
		goto out;
	}

	fprintf(stderr, "Successfully loaded Terrain map from %s\n", fn);

	displayList = glGenLists(1);
	glNewList(displayList, GL_COMPILE);
	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, t->texid);

	for (int i = 0 ; i < imgWidth - 1; i++) {
		glBegin(GL_QUADS);
		for (int j = 0 ; j < imgHeight - 1; j++) {
			Vec3D a,b,c,d;
//			float c1, c2, c3, c4;
			Vec2D ta, tb, tc, td;

			/* Changed order enables backface culling */
			getVertex(i, j, &a, &ta);
			getVertex(i, j+1, &b, &tb);
			getVertex(i+1, j+1, &c, &tc);
			getVertex(i+1, j, &d, &td);

//			glColor3f(c1, c1, c1);

			glTexCoord2f(ta.x, ta.y);
			glVertex3f(a.x, a.y, a.z);
//			glColor3f(c2, c2, c2);
			glTexCoord2f(tb.x, tb.y);
			glVertex3f(b.x, b.y, b.z);
			glTexCoord2f(tc.x, tc.y);
			glVertex3f(c.x, c.y, c.z);
			glTexCoord2f(td.x, td.y);
			glVertex3f(d.x, d.y, d.z);

		}
		glEnd();
	}
	glEndList();

	err = 0;

 out:
	if (err) {
		if (data) free(data);
		data = NULL;
	}
	if (fp) fclose(fp);
	return err;
}

int Terrain::unload(void)
{
	free(data);
	data = NULL;
	return 0;
}

void Terrain::render(Camera *c)
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, t->texid);

	glCallList(displayList);

	glDisable(GL_TEXTURE_2D);

	glDisable(GL_CULL_FACE);
}

