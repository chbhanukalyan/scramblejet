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


#ifndef		__OBJECTS_FLAME_H__
#define		__OBJECTS_FLAME_H__

#include "../RenderingEngine/Renderable.h"
#include "../RenderingEngine/Texture.h"

class Flame : public Renderable {
	private:
		Texture *t;
		float size;
		float width, tipwidth, length;
		bool redflame;
		float texXStart;
		float texXRange;

		float *sinarr, *cosarr;
		int numTris;
		float x, y, z;

	public:
		Flame(void);
		~Flame();

		inline void setSize(float w, float l)
		{
			width = w;
			length = l;
		}

		int load(const char *path);
		void unload(void);

		void render(Camera *c);
};

#endif	/*	__OBJECTS_FLAME_H__	*/

