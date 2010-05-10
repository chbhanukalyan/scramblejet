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


#ifndef		__RE_TERRAIN_H__
#define		__RE_TERRAIN_H__

#include "../../types.h"
#include "Camera.h"

class Terrain {
	private:
		char fn[256];
		unsigned char *data;

		int imgWidth;
		int imgHeight;

		GLint displayList;

		void getVertex(int i, int j, Vec3D *v, float *c) {
			v->x = 8.0 * (i - imgWidth/2);
			v->y = data[i * imgHeight + j] - 200.0;
			v->z = 8.0 * (j - imgHeight/2);
			*c = ((float)data[i * imgHeight + j]) / 256.0;
		}

	public:
		Terrain(void);
		~Terrain();

		int load(const char *name);
		int unload(void);

		void render(Camera *c);

};

#endif	/*	__RE_TERRAIN_H__	*/

