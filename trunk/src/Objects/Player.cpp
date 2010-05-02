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

#include "Player.h"
#include "../GamingClient/GamingClient.h"

Player::Player()
	:StaticModel("Player")
{
}

Player::~Player()
{
}

void Player::load(void *gcptr)
{
	StaticModel::load("../data/staticmodels/F-16.dae");
	GamingClient *gc = (GamingClient *)gcptr;
	gc->player = this;
}

void Player::render(Camera *c)
{
	glPushMatrix();
	StaticModel::render(c);
	glPopMatrix();
}

void Player::handle(EventMap *evmap)
{
	if (evmap->eventInProgress(FUNCID_ACCELERATE)) {
		transz += 0.01;
	}
	if (evmap->eventInProgress(FUNCID_DECCELERATE)) {
		transz -= 0.01;
	}

	if (evmap->eventInProgress(FUNCID_ROTZP))
		roll += 0.1;
	if (evmap->eventInProgress(FUNCID_ROTZM))
		roll -= 0.1;
	if (evmap->eventInProgress(FUNCID_ROTYP))
		yaw += 0.1;
	if (evmap->eventInProgress(FUNCID_ROTYM))
		yaw -= 0.1;
	if (evmap->eventInProgress(FUNCID_ROTXP))
		pitch += 0.1;
	if (evmap->eventInProgress(FUNCID_ROTXM))
		pitch -= 0.1;
}

