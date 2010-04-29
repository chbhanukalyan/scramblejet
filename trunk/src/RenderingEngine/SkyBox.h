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


#ifndef		__RE_SKYBOX_H__
#define		__RE_SKYBOX_H__

#include "Texture.h"
#include "Camera.h"

enum {
	SKYBOX_FACE_X_PLUS = 0,
	SKYBOX_FACE_X_MINUS,
	SKYBOX_FACE_Y_PLUS,
	SKYBOX_FACE_Y_MINUS,
	SKYBOX_FACE_Z_PLUS,
	SKYBOX_FACE_Z_MINUS,
};

class SkyBox {
	static const char *const skybox_fn[6];
	private:
		Texture *t[6];
		int loaded;
		float size;
		float width, height, depth;

		int loadTexture(const char *dirPath, int type);

	public:
		SkyBox(void);
		~SkyBox();

		inline void setSize(float w, float h, float d)
		{
			width = w;
			height = h;
			depth = d;
		}

		int load(const char *dirPath);
		void unload(void);

		void render(Camera *c);
};

#endif	/*	__RE_SKYBOX_H__	*/

