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


#ifndef		__GE_PLAYER_H__
#define		__GE_PLAYER_H__

#include "../../Map/ObjInfo.h"
#include "../GamingEngine/SentientObject.h"

class Player : public SentientObject {
	private:
		float loc[3];
		float vel;
		float accel;
		float dir[3];

		float maxspeed, minspeed;
		float minanglerot, maxanglerot;

		inline void setSpeed(float s) {
			if (s > maxspeed) s = maxspeed;
			if (s < minspeed) s = minspeed;
			vel = s;
		}
		inline void setAngle(float a, int type) {
			if (a > maxanglerot) a = maxanglerot;
			if (a < minanglerot) a = minanglerot;
		}

	public:
		int id;
		char playerName[16];

		Player(ObjInfo *o, const char *name);
		~Player();

		void handleEvent(int funcid, int count);
		int serializeState(void *buf);

		void doTick(void);
};

#endif	/*	__GE_PLAYER_H__	*/

