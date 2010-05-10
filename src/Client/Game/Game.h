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


#ifndef		__GAME_GAME_H__
#define		__GAME_GAME_H__

#define	MAX_NUM_PLAYERS		16
#define	MAX_NUM_MISSILES	256

#include "../RenderingEngine/RenderingEngine.h"
#include "../Objects/SkyBox.h"
#include "../Panel/Panel.h"
#include "../GamingClient/GamingClient.h"
#include "../../Map/Map.h"
#include "../Objects/Player.h"
#include "../Objects/Missile.h"

class Game {
	public:
		char data_dir[256];
		char mapName[32];
		char mapfn[256];

		static Game *g_game;

		int localPlayerID;

		RenderingEngine *re;
		GamingClient *gc;
		Map *map;

		SkyBox *skybox;
		Player *playerList[MAX_NUM_PLAYERS];
		Missile *missileList[MAX_NUM_MISSILES];
		Panel *panel;

	public:
		long curticks;

		Game(const char *base_dir, RenderingEngine *re, GamingClient *gc);
		~Game();

		int initGame(void);

		void startGame(void);
		void runGameLoop(void);
		void stopGame(void);

		void addNewPlayer(void) {}
		void updateMissile(struct updateObj *updt);

};

#endif	/*	__GAME_GAME_H__	*/

