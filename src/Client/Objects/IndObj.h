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


#ifndef		__INDOBJ_H__
#define		__INDOBJ_H__

#include <string.h>

#include "../../types.h"

class IndObj {
	public:
		char id[16];
		IndObj *inext;

	public:
		IndObj(const char *id) {
			strncpy(this->id, id, 15);
			this->id[15] = '\0';
			inext = NULL;
		}
		virtual void getPos(Vec3D *) = 0;

};

#endif	/*	__INDOBJ_H__	*/

