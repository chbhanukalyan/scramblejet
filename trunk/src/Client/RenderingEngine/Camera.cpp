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

#include <cassert>
#include "Camera.h"

Camera::Camera(void)
{
	dirty = false;
}

Camera::~Camera()
{
}

void Camera::copyCamPos(CamPos *p)
{
	pointx = p->pointx;
	pointy = p->pointy;
	pointz = p->pointz;
	upx = p->upx;
	upy = p->upy;
	upz = p->upz;
	height = p->height;
	angle = p->angle;
	vertangle = p->vertangle;
	distance = p->distance;
}

void Camera::dumpCurPos(void)
{
	fprintf(stderr, "Camera Cur Pos: POINT(%.2f,%.2f,%.2f) UP(%.2f,%.2f,%.2f) "
			"Height(%.2f) ANGLE(%.2f) VERTANGLE(%.2f) DIST(%.2f) "
			"CAM(calculated:%.2f,%.2f,%.2f)\n",
			pointx, pointy, pointz, upx, upy, upz,
			height, angle, vertangle, distance,
			camx, camy, camz);
}

void Camera::Initialize(CamPos *p)
{
	copyCamPos(p);
	
	camx = distance * sin(angle) * cos(vertangle);
	camy = height + distance * sin(vertangle);
	camz = distance * cos(angle) * cos(vertangle);

	Move();
	dirty = true;
}

void Camera::Update(CamPos *p)
{
	if (p) {
		copyCamPos(p);
		
		camx = pointx + distance * sin(angle) * cos(vertangle);
		camy = pointy + height + distance * sin(vertangle);
		camz = pointz + distance * cos(angle) * cos(vertangle);


		dirty = true;
		Update();
	}
}

void Camera::Rotate(float a)
{
	assert(0);
	angle = a;
	camx = distance * sin(angle);
	camz = distance * cos(angle);

	pointx = 0;
	pointz = 0;
	dirty = true;
}

void Camera::Move(void)
{
	dirty = true;
}

void Camera::MoveUp(void)
{
	dirty = true;
}

void Camera::MoveDown(void)
{
	dirty = true;
}

void Camera::MoveLeft(void)
{
	dirty = true;
}

void Camera::MoveRight(void)
{
	dirty = true;
}

void Camera::Zoom(float d)
{
	dirty = true;
}

