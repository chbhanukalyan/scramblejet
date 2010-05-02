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


#ifndef		__RE_STATICMODEL_H__
#define		__RE_STATICMODEL_H__

#include "../RenderingEngine/Renderable.h"

class StaticModel : public Renderable {
	private:
		char fn[256];

		GLint list;
		float scalefactor;

	protected:
		float transx;
		float transy;
		float transz;

		float roll;
		float pitch;
		float yaw;

	public:
		StaticModel(void);
		~StaticModel();

		int load(const char *fn);
		int unload(void);

		void render(Camera *c);

};

#endif	/*	__RE_STATICMODEL_H__	*/

