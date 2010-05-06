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
	:StaticModel("Player")
{
	id = oi->id;
	transx = oi->locx;
	transy = oi->locy;
	transz = oi->locz;

	StaticModel::load(oi->modelfn);

}

Player::~Player()
{
}

void Player::load(void *gcptr)
{
}

void Player::render(Camera *c)
{
	glPushMatrix();
	StaticModel::render(c);
	glPopMatrix();
}

void Player::update(struct updateObj *upObj)
{
	assert(upObj->updateFieldID == id);

	float *vals = (float *)(upObj->vals);
	transx = locx = vals[0];
	transy = locy = vals[1];
	transz = locz = vals[2];
	roll = vals[3];
	pitch = vals[4];
	yaw = vals[5];
	printf("New TRANS Vals = %f, %f, %f, %f %f %f\n", vals[0], vals[1], vals[2], roll, pitch, yaw);
}

