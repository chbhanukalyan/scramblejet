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
#define	DEFAULT_WAIT_BEFORE_START	10

#include "SentientObject.h"
#include "../Objects/Player.h"

class GamingEngine {
	private:
		SentientObject *objList;
	public:
		Player *p;
		GamingEngine(void);
		~GamingEngine();

		int initialize(void);
		int startGame(void *server);
		void stop(void);

		void addPlayer(int id, const char *name);
		void removePlayer(int id);

		void addObject(SentientObject *seo);
		void removeObject(SentientObject *seo);

};

#endif	/*	__GE_GAMINGENGINE_H__	*/

