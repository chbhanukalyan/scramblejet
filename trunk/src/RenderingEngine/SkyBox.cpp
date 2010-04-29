/*
 * scRamble - 3D Flight Racer
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

#include "SkyBox.h"

const char *const SkyBox::skybox_fn[6] = {
	"right.png" /* SKYBOX_FACE_X_PLUS */,
	"left.png" /* SKYBOX_FACE_X_MINUS */,
	"top.png" /* SKYBOX_FACE_Y_PLUS */,
	"bottom.png" /* SKYBOX_FACE_Y_MINUS */,
	"back.png" /* SKYBOX_FACE_Z_PLUS */,
	"front.png" /* SKYBOX_FACE_Z_MINUS */,
};

SkyBox::SkyBox(void)
{
	loaded = 0;
	for (int i = 0 ; i < 6; i++) {
		t[i] = NULL;
	}
	size = 100.0f;
}

int SkyBox::loadTexture(const char *dirPath, int type)
{
	char buf[256];
	snprintf(buf, 255, "%s/%s", dirPath, skybox_fn[type]);
	t[type] = new Texture(buf);
	t[type]->Load();
	return 0;
}

int SkyBox::load(const char *dirPath)
{
	for (int i = 0 ; i < 6; i++) {
		loadTexture(dirPath, i);
	}
	loaded = 1;
	return 0;
}

void SkyBox::unload(void)
{
	if (loaded) {
		for (int i = 0 ; i < 6; i++) {
			delete t[i];
			t[i] = NULL;
		}
		loaded = 0;
	}
}

SkyBox::~SkyBox()
{
	if (loaded)
		unload();
}

void SkyBox::render(Camera *c)
{
	glColor4f(1,1,1,0);

	glEnable(GL_TEXTURE_2D);

	// Front
	glBindTexture(GL_TEXTURE_2D, t[SKYBOX_FACE_Z_MINUS]->texid);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(c->pointx - width, c->pointy - height, c->pointz + depth);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(c->pointx - width, c->pointy + height, c->pointz + depth);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(c->pointx + width, c->pointy + height, c->pointz + depth);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(c->pointx + width, c->pointy - height, c->pointz + depth);
	glEnd();

	// Back
	glBindTexture(GL_TEXTURE_2D, t[SKYBOX_FACE_Z_PLUS]->texid);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(c->pointx - width, c->pointy - height, c->pointz - depth);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(c->pointx - width, c->pointy + height, c->pointz - depth);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(c->pointx + width, c->pointy + height, c->pointz - depth);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(c->pointx + width, c->pointy - height, c->pointz - depth);
	glEnd();

	// Bottom
	glBindTexture(GL_TEXTURE_2D, t[SKYBOX_FACE_Y_MINUS]->texid);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(c->pointx - width, c->pointy - height, c->pointz - depth);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(c->pointx - width, c->pointy - height, c->pointz + depth);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(c->pointx + width, c->pointy - height, c->pointz + depth);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(c->pointx + width, c->pointy - height, c->pointz - depth);
	glEnd();

	// TOP
	glBindTexture(GL_TEXTURE_2D, t[SKYBOX_FACE_Y_PLUS]->texid);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(c->pointx - width, c->pointy + height,	c->pointz - depth);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(c->pointx - width, c->pointy + height,	c->pointz + depth);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(c->pointx + width, c->pointy + height, c->pointz + depth);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(c->pointx + width, c->pointy + height, c->pointz - depth);

	glEnd();

	// LEFT
	glBindTexture(GL_TEXTURE_2D, t[SKYBOX_FACE_X_MINUS]->texid);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(c->pointx - width, c->pointy - height, c->pointz - depth);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(c->pointx - width, c->pointy - height, c->pointz + depth);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(c->pointx - width, c->pointy + height,	c->pointz + depth);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(c->pointx - width, c->pointy + height,	c->pointz - depth);
	glEnd();

	// RIGHT
	glBindTexture(GL_TEXTURE_2D, t[SKYBOX_FACE_X_PLUS]->texid);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(c->pointx + width, c->pointy - height, c->pointz - depth);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(c->pointx + width, c->pointy - height, c->pointz + depth);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(c->pointx + width, c->pointy + height,	c->pointz + depth);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(c->pointx + width, c->pointy + height,	c->pointz - depth);
	glEnd();

	glDisable(GL_TEXTURE_2D);

}

