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

#include <stdlib.h>

#include "AIModule.h"
#include "../../protocol.h"
#include "../../funcids.h"

AIModule::AIModule(void)
{
	memset(aic, 0, sizeof(aic));
}

AIModule::~AIModule()
{
}

void AIModule::run(unsigned char *buf, int count, Player **playerList)
{
	int i;
	for (i = 0; i < count; i++) {
		struct updateObj *ob = (struct updateObj *)buf;
		if (aic[ob->updateFieldID]) {
			AIClient *ac = aic[ob->updateFieldID];
			float *f = (float*)ob->vals;
			ac->pos.x = f[0];
			ac->pos.y = f[1];
			ac->pos.z = f[2];
			ac->pitch = f[3];
			ac->yaw = f[4];
			ac->roll = f[5];
		}

		buf += ob->size;
	}

	for (i = 0; i < MAX_AI_CLIENTS; i++) {
		if (aic[i]) {
			playerList[i]->handleEvent(FUNCID_ROTYP, 2);
			if (rand() % 100 > 95) {
				if (rand() % 10 > 5) {
					playerList[i]->handleEvent(FUNCID_ROTXP, 1);
				} else {
					playerList[i]->handleEvent(FUNCID_ROTXM, 1);
				}
			}
			if (aic[i]->accel) {
				playerList[i]->handleEvent(FUNCID_ACCELERATE, 1);
				aic[i]->accel--;
				if (rand() % 10 > 5) {
					playerList[i]->handleEvent(FUNCID_ACCELERATE, 2);
					playerList[i]->handleEvent(FUNCID_ROTXP, 1);
				} else {
					playerList[i]->handleEvent(FUNCID_ROTXM, 1);
				}
			}
		}
	}

}

void AIModule::startNewClient(int id)
{
	aic[id] = new AIClient;
	aic[id]->id = id;
	aic[id]->accel = 5;
}

