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

#include "Player.h"
#include "../GamingClient/funcids.h"

Player::Player(int id, const char *name)
{
	this->id = id;
	strncpy(playerName, name, 16);
	loc[0] = loc[1] = loc[2] = 0;
	dir[0] = dir[1] = dir[2] = 0;
	vel[0] = vel[1] = vel[2] = 0;
}

Player::~Player()
{
}

void Player::handleEvent(int funcid, int count) {
	fprintf(stderr, "Player(%s) got an event:%d count:%d\n", playerName, funcid, count);
	switch(funcid) {
		case FUNCID_ACCELERATE: {
			loc[0] += 0.1 * count;
			loc[1] += 0.1 * count;
			loc[2] += 0.1 * count;
			break;
		}
		case FUNCID_DECCELERATE: {
			loc[0] -= 0.1 * count;
			loc[1] -= 0.1 * count;
			loc[2] -= 0.1 * count;
			break;
		}
		default:
		   break;
	}
}

int Player::serializeState(void *buf) {

	unsigned char *b = (unsigned char *)buf;
	b[0] = 2 + sizeof(loc);		//size
	b[1] = 0x1;					// update field id
	memcpy(b + 2, loc, sizeof(loc));

	return 2 + sizeof(loc);
}

void Player::doTick(void) {
}

