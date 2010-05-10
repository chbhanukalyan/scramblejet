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
		float magnitude(void) {
			return x*x + y*y + z*z;
		}
};


#endif	/*	__TYPES_H__	*/

