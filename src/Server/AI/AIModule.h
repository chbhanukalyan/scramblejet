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


#ifndef		__AI_AIMODULE_H__
#define		__AI_AIMODULE_H__

#include <string.h>
#include "../Objects/Player.h"
#include "../../types.h"

#define	MAX_AI_CLIENTS		32

struct AIClient {
	int id;
	Vec3D pos;
	float pitch;
	float yaw;
	float roll;

	int accel;
};

class AIModule {
	private:
		AIClient *aic[MAX_AI_CLIENTS];

	public:
		AIModule(void);
		~AIModule();

		void startNewClient(int id);

		void run(unsigned char *buf, int count, Player **playerList);

};

#endif	/*	__AI_AIMODULE_H__	*/

