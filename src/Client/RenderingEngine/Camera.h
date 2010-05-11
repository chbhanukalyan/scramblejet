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

#include "../../Map/CamPos.h"

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
	float vertangle;

	float yfactor;

	bool dirty;

	void copyCamPos(CamPos *p);

 public:
	 Camera(void);
	~Camera();

	void Initialize(CamPos *pos = NULL);
	void dumpCurPos(void);

	void Update(CamPos *p);
	inline void setView(void) {
		// Set the projection matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45, 640/480.0, 0.005, 5000.0f);

		// Reset the model view matrix
		glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
		glLoadIdentity();   // Reset The Modelview Matrix
	//	glOrtho(-5000, 5000, -5000, 5000, -5000, 5000);
	}

	inline void Update(void) {
		gluLookAt(camx, camy, camz, pointx, pointy, pointz, upx,
				  upy, upz);
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
