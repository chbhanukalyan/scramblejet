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


#ifndef		__TYPES_H__
#define		__TYPES_H__

#include <GL/gl.h>

#define		PI	(3.14159)

class Vec2D {
	public:
		float x, y;
		float magnitude(void) {
			return x*x + y*y;
		}
};

class Vec3D {
	public:
		float x, y, z;
		Vec3D(void) {
			this->x = this->y = this->z = 0;
		}
		Vec3D(float x, float y, float z) {
			this->x = x; this->y = y; this->z = z;
		}
		float magnitude(void) {
			return x*x + y*y + z*z;
		}
		Vec3D operator+(Vec3D &o) {
			return Vec3D(x+o.x, y+o.y, z+o.z);
		}
		Vec3D operator-(Vec3D &o) {
			return Vec3D(x-o.x, y-o.y, z-o.z);
		}
		Vec3D operator*(float s) {
			return Vec3D(x*s, y*s, z*s);
		}
		Vec3D operator/(float s) {
			return Vec3D(x/s, y/s, z/s);
		}

		void GlVertex3f(Vec3D v) {
			glVertex3f(v.x, v.y, v.z);
		}

};

class Vec4D {
	public:
		float x, y, z, w;
		Vec4D(void) {
			x = y = z = w = 0;
		}
		Vec4D(float x, float y, float z, float w) {
			this->x = x; this->y = y; this->z = z; this->w = w;
		}
};


#endif	/*	__TYPES_H__	*/

