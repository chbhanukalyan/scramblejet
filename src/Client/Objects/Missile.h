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


#ifndef		__OBJ_MISSILE_H__
#define		__OBJ_MISSILE_H__

#include "IndObj.h"
#include "StaticModel.h"
#include "../../protocol.h"
#include "../GamingClient/EventMap.h"
#include "Flame.h"

class Missile : public Renderable, public IndObj {
	private:
		float locx, locy, locz;
		float pitch, yaw, roll;

		Flame *flame;

		StaticModel *basemodel;

	public:
		int id;

		Missile(int id, StaticModel *model);
		~Missile();

		void followCam(CamPos *cp);

		void render(Camera *c);
		void getPos(Vec3D *v) {
			v->x = locx;
			v->y = locy;
			v->z = locz;
		}
		void update(struct updateObj *upObj);

};

#endif	/*	__OBJ_MISSILE_H__	*/

