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

#include "Missile.h"
#include "../../funcids.h"

Missile::Missile(void)
{
	this->id = -1;
	maxspeed = .01;
	minspeed = .005;
	maxanglerot = 0.1;
	minanglerot = 0.1;
}

Missile::~Missile()
{
}

#define	SERIALIZE_ELEM(x)	\
	do {\
		memcpy(b + len, &(x), sizeof(x)); len += sizeof(x);\
	} while (0)

int Missile::serializeState(void *buf) {
	int len = 0;
	unsigned char *b = (unsigned char *)buf;
	b[1] = (unsigned char)id;					// update missile id
	len = 2;

	SERIALIZE_ELEM(loc);
	SERIALIZE_ELEM(pitch);
	SERIALIZE_ELEM(yaw);
	SERIALIZE_ELEM(roll);

	b[0] = len;

	return len;
}

void Missile::doTick(void) {
	if (vel < maxspeed)
		vel += 0.005;

	/* Change angles to direction vector */
	dir[0] = cosf(pitch) * sinf(yaw);
	dir[1] = -sinf(pitch);
	dir[2] = cosf(pitch) * cosf(yaw);

	/* go to next position */
	loc[0] += dir[0] * vel;
	loc[1] += dir[1] * vel;
	loc[2] += dir[2] * vel;

}

