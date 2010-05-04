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


#ifndef		__OBJ_PLAYER_H__
#define		__OBJ_PLAYER_H__

#include "StaticModel.h"
#include "../../protocol.h"
#include "../GamingClient/EventMap.h"

class Player : public StaticModel {
	private:
		float locx, locy, locz;
	public:
		Player();
		~Player();

		void load(void *gc);

		void render(Camera *c);
		void update(struct updateObj *upObj);

};

#endif	/*	__OBJ_PLAYER_H__	*/

