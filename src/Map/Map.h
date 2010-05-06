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


#ifndef		__MAP_MAP_H__
#define		__MAP_MAP_H__

#include "CamPos.h"
#include "ObjInfo.h"

class Map {
	public:
		char fn[256];
		char name[32];
		char skyboxfn[256];

		float sizex, sizey, sizez;
		CamPos initCamPos;

		ObjInfo *objList;

	public:
		Map();
		~Map();

		void insertObjInfo(ObjInfo *o);

};

#endif	/*	__MAP_MAP_H__	*/

