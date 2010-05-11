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
	glColor3f(1,1,1);
	/* 1 radian = 57.296 degrees */
#define	RADIAN2DEG(x)	((x) * 57.29578f)
	glPushMatrix();
	glTranslatef(locx, locy, locz);
	glPushMatrix();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	
	/* Rotate Around Y-AXIS */
	glRotatef(RADIAN2DEG(yaw), 0, 1, 0);

	/* Now figure out a vector which will allow it to rotate on the specific axis */
	glRotatef(RADIAN2DEG(pitch), 1, 0, 0);

	glPushMatrix();
	glRotatef(RADIAN2DEG(roll), 0, 0, 1);
	glPushMatrix();

	basemodel->render(c);

	flame->render(c);
	
	glPopMatrix();
	glPopMatrix();
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
//	printf("NEW Missile Vals = %f, %f, %f, %f %f %f\n", vals[0], vals[1], vals[2], roll, pitch, yaw);
}

