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
#include <math.h>

#include "Player.h"
#include "../../funcids.h"

Player::Player(ObjInfo *o, const char *name)
{
	id = o->id;
	strncpy(playerName, name, 16);
	dir[0] = o->dirx;
	dir[1] = o->diry;
	dir[2] = o->dirz;
	vel = o->initialVelocity;
	loc[0] = o->locx;
	loc[1] = o->locy;
	loc[2] = o->locz;
	maxspeed = o->maxspeed;
	minspeed = o->minspeed;
	maxanglerot = o->maxanglerot;
	minanglerot = o->minanglerot;

	/* Unit Vector to angle */
	pitch = asinf(dir[1]);
	yaw = asinf(dir[0]/cosf(pitch));
}

Player::~Player()
{
}

void Player::handleEvent(int funcid, int count) {
	fprintf(stderr, "Player(%s) got an event:%d count:%d\n", playerName, funcid, count);
	switch(funcid) {
		case FUNCID_ACCELERATE: {
			setSpeed(vel + 0.01 * count);
			break;
		}
		case FUNCID_DECCELERATE: {
			setSpeed(vel - 0.01 * count);
			break;
		}
		case FUNCID_BANKRIGHT: {
			loc[0] += 0.01 * count;
			break;
		}
		case FUNCID_BANKLEFT: {
			loc[0] -= 0.01 * count;
			break;
		}
		case FUNCID_CLIMBUP: {
			loc[1] += 0.01 * count;
			break;
		}
		case FUNCID_CLIMBDOWN: {
			loc[1] -= 0.01 * count;
			break;
		}
		case FUNCID_FIRE1: break;
		case FUNCID_FIRE2: break;

		case FUNCID_ROTXP: {
			pitch += minanglerot * count;
			break;
		}
		case FUNCID_ROTXM: {
			pitch -= minanglerot * count;
			break;
		}
		case FUNCID_ROTYP: {
			yaw += minanglerot * count;
			break;
		}
		case FUNCID_ROTYM: {
			yaw -= minanglerot * count;
			break;
		}
		case FUNCID_ROTZP: {
			roll += 0.01 * count;
			break;
		}
		case FUNCID_ROTZM: {
			roll -= 0.01 * count;
			break;
		}

		default:
		   break;
	}
}

#define	SERIALIZE_ELEM(x)	\
	do {\
		memcpy(b + len, &(x), sizeof(x)); len += sizeof(x);\
	} while (0)

int Player::serializeState(void *buf) {
	int len = 0;
	unsigned char *b = (unsigned char *)buf;
	b[1] = (unsigned char)id;					// update player id
	len = 2;

	SERIALIZE_ELEM(loc);
	SERIALIZE_ELEM(pitch);
	SERIALIZE_ELEM(yaw);
	SERIALIZE_ELEM(roll);

	b[0] = len;

	return len;
}

void Player::doTick(void) {
	/* Change angles to direction vector */
	dir[0] = cosf(pitch) * sinf(yaw);
	dir[1] = -sinf(pitch);
	dir[2] = cosf(pitch) * cosf(yaw);

	/* go to next position */
	loc[0] += dir[0] * vel;
	loc[1] += dir[1] * vel;
	loc[2] += dir[2] * vel;
}

