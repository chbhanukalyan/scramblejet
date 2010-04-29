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


#ifndef				__RE_CAMERA_H__
#define				__RE_CAMERA_H__

#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>

class Camera {
 public:
	float camx;
	float camy;
	float camz;

	float pointx;
	float pointy;
	float pointz;

	float upx;
	float upy;
	float upz;

	float distance;
	float height;
	float angle;
	float initx;
	float initz;

	float yfactor;

	bool dirty;

 public:
	 Camera(void);
	~Camera();

	void Initialize();

	inline void Update(void) {
		if (dirty) {
			glLoadIdentity();
			gluLookAt(camx, camy, camz, pointx, pointy, pointz, upx,
				  upy, upz);
			dirty = false;
		}
	}
	void Move();
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	void Rotate(float a);

	void Zoom(float d);

	int ConvertCoordinates(int x, int y, float &, float &);

	int ScrollOver(float x, float y);

};

#endif				/*      __RE_CAMERA_H__       */
