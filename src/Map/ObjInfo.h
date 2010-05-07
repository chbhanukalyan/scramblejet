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


#ifndef		__MAP_OBJINFO_H__
#define		__MAP_OBJINFO_H__

#include <stdio.h>
#include <string.h>
#include "../objtype.h"

struct ObjInfo {
	int type;
	int id;
	float accel;
	float locx, locy, locz;
	float dirx, diry, dirz;
	float initialVelocity;
	float maxspeed, minspeed;
	float minanglerot, maxanglerot;
	char modelfn[256];

	ObjInfo *next;

	inline void dump(void) {
		fprintf(stderr, "type(%d), id(%d), loc(%f,%f,%f), modelfn(%s)\n"
				"speed(max:%f min:%f) anglerot(max:%f min:%f)\n",
				type, id, locx, locy, locz, modelfn,
				maxspeed, minspeed, maxanglerot, minanglerot);
	}
};

#endif	/*	__MAP_OBJINFO_H__	*/

