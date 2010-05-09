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
#include "Player.h"
#include "../GamingClient/GamingClient.h"

Player::Player(ObjInfo *oi)
	:StaticModel("Player"), IndObj("Player")
{
	id = oi->id;
	transx = oi->locx;
	transy = oi->locy;
	transz = oi->locz;

	dirx = oi->dirx;
	diry = oi->diry;
	dirz = oi->dirz;

	StaticModel::load(oi->modelfn);
	flame = new Flame(0.019, 0.016, -1.45);
	flame->load("textures/flame.tga");
}

Player::~Player()
{
}

void Player::load(void *gcptr)
{
}

void Player::followCam(CamPos *cp)
{
	cp->pointx = transx;
	cp->pointy = transy;
	cp->pointz = transz;

	cp->upx = 0;
	cp->upy = 10000;
	cp->upz = 0;

	cp->distance = 2;
	cp->height = 0.4;
	cp->angle = 3.141;
}

void Player::render(Camera *c)
{
	/* 1 radian = 57.296 degrees */
#define	RADIAN2DEG(x)	((x) * 57.29578f)
	glPushMatrix();
	glTranslatef(locx, locy, locz);
	glPushMatrix();
	
	/* Rotate Around Y-AXIS */
	glRotatef(RADIAN2DEG(yaw), 0, 1, 0);

	/* Now figure out a vector which will allow it to rotate on the specific axis */
	glRotatef(RADIAN2DEG(pitch), 1, 0, 0);

	glPushMatrix();
	glRotatef(RADIAN2DEG(roll), 0, 0, 1);
	glPushMatrix();

	StaticModel::render(c);
	
	flame->render(c);
	
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void Player::update(struct updateObj *upObj)
{
	assert(upObj->updateFieldID == id);

	float *vals = (float *)(upObj->vals);
	transx = locx = vals[0];
	transy = locy = vals[1];
	transz = locz = vals[2];
	pitch = vals[3];
	yaw = vals[4];
	roll = vals[5];
	printf("New TRANS Vals = %f, %f, %f, %f %f %f\n", vals[0], vals[1], vals[2], roll, pitch, yaw);
}

