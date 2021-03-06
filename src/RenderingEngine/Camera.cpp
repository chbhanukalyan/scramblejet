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


#include "Camera.h"

Camera::Camera(void)
{
	dirty = false;
}

Camera::~Camera()
{
}

void Camera::Initialize(void)
{
	distance = 1;
	height = 0;
	angle = 0;

	pointx = 0;
	pointy = 0;
	pointz = 0;

	upx = 0;
	upy = 10000;
	upz = 0;

	
	camx = distance * sin(angle);
	camy = height;
	camz = distance * cos(angle);

	Move();
	dirty = true;
}

void Camera::Rotate(float a)
{
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

