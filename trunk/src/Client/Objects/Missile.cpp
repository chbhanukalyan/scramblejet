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

#include <cassert>
#include "Missile.h"
#include "../GamingClient/GamingClient.h"

Missile::Missile(int id, StaticModel *model)
	:Renderable("Missile"), IndObj("Missile")
{
	this->id = id;
	assert(id >= 32);
	locx = locy = locz = 0;

	pitch = yaw = roll = 0;

	basemodel = model;
	flame = new Flame(0.019, 0.016, -1.45);
	flame->load("textures/flame.tga");

	numSections = 12;
	sinArr = new float[numSections+1];
	cosArr = new float[numSections+1];

	for (int i = 0; i <= numSections; i++) {
		sinArr[i] = sin(2*i*PI/(numSections) + PI/2);
		cosArr[i] = cos(2*i*PI/(numSections) + PI/2);
	}
	barrelLength = 0.3;
	coneLength = 0.06f;
	width = 0.010f;

	flame->transx = flame->transy = flame->transz = 0;
	flame->width = 0.010;
	flame->tipwidth = 0.005;
}

Missile::~Missile()
{
	basemodel = NULL;
	delete flame;
}

void Missile::followCam(CamPos *cp)
{
	cp->pointx = locx;
	cp->pointy = locy;
	cp->pointz = locz;

	cp->upx = 0;
	cp->upy = 10000;
	cp->upz = 0;

	cp->distance = 2;
	cp->height = 0.4;
	/* Follow only horizontal movements */
	cp->angle = 3.141 + yaw;
}

void Missile::render(Camera *c)
{
	/* 1 radian = 57.296 degrees */
#define	RADIAN2DEG(x)	((x) * 57.29578f)
	glPushMatrix();
	glTranslatef(locx, locy, locz);

	glDisable(GL_BLEND);
	
	/* Rotate Around Y-AXIS */
	glRotatef(RADIAN2DEG(yaw), 0, 1, 0);

	/* Now figure out a vector which will allow it to rotate on the specific axis */
	glRotatef(RADIAN2DEG(pitch), 1, 0, 0);

	glPushMatrix();

	/* Body */
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor3f(.7,.71,.71);
	for (int i = 0; i < numSections; i++) {
		glTexCoord2f(0, 1.0f);
		glVertex3f(width*sinArr[i], width*cosArr[i], 0);
		glTexCoord2f(1, 1.0f);
		glVertex3f(width*sinArr[i+1], width*cosArr[i+1], 0);
		glTexCoord2f(1, 0.0f);
		glVertex3f(width*sinArr[i+1], width*cosArr[i+1], barrelLength);
		glTexCoord2f(0, 0.0f);
		glVertex3f(width*sinArr[i], width*cosArr[i], barrelLength);
	}
	glEnd();
	/* Cone */
	glBegin(GL_TRIANGLES);
	glColor3f(1,.5,.5);
	for (int i = 0; i < numSections; i++) {
		glTexCoord2f(0, 1.0f);
		glVertex3f(width*sinArr[i], width*cosArr[i], barrelLength);
		glTexCoord2f(1, 1.0f);
		glVertex3f(width*sinArr[i+1], width*cosArr[i+1], barrelLength);
		glTexCoord2f(1, 0.0f);
		glVertex3f(0, 0, barrelLength + coneLength);
	}
	glEnd();
	/* Fins */
	int finwidth = 3*width;
	glBegin(GL_TRIANGLES);
//	glColor3f(.7,.71,.71);
	glColor3f(1,0,0);
	for (int i = 0; i < numSections; i+=numSections/4) {
		glTexCoord2f(0, 1.0f);
		glVertex3f(width*sinArr[i], width*cosArr[i], barrelLength/5);
		glTexCoord2f(1, 1.0f);
		glVertex3f(width*sinArr[i], width*cosArr[i], 0.0);
		glTexCoord2f(1, 0.0f);
		glVertex3f((width + finwidth)*sinArr[i], (width +finwidth)*cosArr[i], -0.005);
	}
	glEnd();

	glEnable(GL_BLEND);
	/* Smoke Trail */
	glBegin(GL_QUADS);
	for (int i = 0; i < numSections; i++) {
		glColor4f(0,.0,.0,1);
		glTexCoord2f(0, 1.0f);
		glVertex3f(width*sinArr[i], width*cosArr[i], 0);
		glColor4f(0,.0,.0,1);
		glTexCoord2f(1, 1.0f);
		glVertex3f(width*sinArr[i+1], width*cosArr[i+1], 0);
		glTexCoord2f(1, 0.0f);
		glColor4f(.4,.41,.41,0);
		glVertex3f(3*width*sinArr[i+1], 3*width*cosArr[i+1], -10*barrelLength);
		glTexCoord2f(0, 0.0f);
		glColor4f(.4,.41,.41,0);
		glVertex3f(3*width*sinArr[i], 3*width*cosArr[i], -10*barrelLength);
	}
	glEnd();


	glDisable(GL_TEXTURE_2D);

	flame->render(c);
	
	glPopMatrix();
	glPopMatrix();
}

void Missile::update(struct updateObj *upObj)
{
	assert(upObj->updateFieldID == id);

	float *vals = (float *)(upObj->vals);
	locx = vals[0];
	locy = vals[1];
	locz = vals[2];
	pitch = vals[3];
	yaw = vals[4];
	roll = vals[5];
//	printf("NEW Missile(%d) Vals = %f, %f, %f, %f %f %f\n", id, vals[0], vals[1], vals[2], roll, pitch, yaw);
}

