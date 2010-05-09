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

#ifndef		__PANEL_RADARMAP_H__
#define		__PANEL_RADARMAP_H__

#include "../Objects/IndObj.h"
#include "../RenderingEngine/Camera.h"
#include "../RenderingEngine/Texture.h"

enum {
	RADARMAP_TYPE_HORIZONTAL,
	RADARMAP_TYPE_VERTICAL
};
class RadarMap {
	private:
		float startAngle;
		float angle;
		Texture *bg;

		float cx, cy;
		float size;
		int displayType;

		float scalex, scaley, scalez;
		bool scalevec3d(Camera *c, Vec3D *v3, Vec2D *v);

	public:
		RadarMap(int type);
		~RadarMap();

		void placeRadarMap(float centx, float centy) {
			cx = centx; cy = centy;
		}

		void render(long curtick, Camera *c, IndObj *objList);

};

#endif	/*	__PANEL_RADARMAP_H__	*/

