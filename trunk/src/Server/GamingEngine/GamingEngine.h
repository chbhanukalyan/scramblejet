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


#ifndef		__GE_GAMINGENGINE_H__
#define		__GE_GAMINGENGINE_H__

#define	MAX_CLIENT_ID			32
#define	INVALID_CLIENT_ID		(-1)
#define	DEFAULT_WAIT_BEFORE_START_USEC	(20*1000)

#include "../../Map/Map.h"
#include "SentientObject.h"
#include "../Objects/Player.h"
#include "../Objects/Missile.h"

class GamingEngine {
	private:
		SentientObject *sentObjList;
		MovableObject *moveObjList;
		long timer_loop_time;

	public:
		Map *map;

		static GamingEngine *g_GamingEngine;

		Player *playerList[MAX_CLIENT_ID];

		GamingEngine(Map *map);
		~GamingEngine();

		int initialize(void);
		int startGame(void *server);
		void stop(void);

		void addPlayer(int id, const char *name);
		void removePlayer(int id);

		void addMissile(Missile *m);

		void addSentObject(SentientObject *seo);
		void removeSentObject(SentientObject *seo);

		void addMovableObject(MovableObject *mov);
		void removeMovableObject(MovableObject *mov);
};

#endif	/*	__GE_GAMINGENGINE_H__	*/

