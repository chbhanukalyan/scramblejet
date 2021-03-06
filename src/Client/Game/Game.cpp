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

#include "Game.h"
#include "../../Map/MapLoader.h"

Game *Game::g_game = NULL;

Game::Game(const char *data_dir, RenderingEngine *re, GamingClient *gc)
{
	strcpy(this->data_dir, data_dir);
	this->re = re;
	this->gc = gc;
	memset(playerList, 0, sizeof(playerList));
	localPlayerID = -1;
	memset(missileList, 0, sizeof(missileList));
	g_game = this;
}

Game::~Game()
{
	g_game = NULL;
}

int Game::initGame(void)
{
	gc->initialize("default.eventmap", 10);
	if (gc->Connect("127.0.0.1", 6501, &localPlayerID, mapName) < 0)
		return -1;

	sprintf(mapfn, "maps/%s.map", mapName);

	map = loadMap(mapfn);

	re->Initialize(map);

	panel = new Panel;
	re->addPanel(panel);

	re->loadModels();

	Player *p = new Player(localPlayerID, re->modelList[MODELID_F16_FIGHTERJET]);
	re->addObject(p);
	panel->addObject(p);
	playerList[p->id] = p;

	return 0;
}

void Game::startGame(void)
{
}

void Game::runGameLoop(void)
{
	static CamPos cp;

	gc->handleEvents();
	playerList[localPlayerID]->followCam(&cp);
	re->render(&cp, gc->curTicks);
}

void Game::stopGame(void)
{
}

void Game::updateMissile(struct updateObj *updt) {
	int id = updt->updateFieldID;
	if (missileList[id] == NULL) {
		Missile *m = new Missile(id, re->modelList[MODELID_MISSILE]);
		missileList[id] = m;
		re->addObject(m);
		panel->addObject(m);
	}
	missileList[id]->update(updt);
}

void Game::updatePlayer(struct updateObj *updt) {
	int id = updt->updateFieldID;
	if (playerList[id] == NULL) {
		printf("New PLAYER JOINED\n");
		Player *p = new Player(id, re->modelList[MODELID_F16_FIGHTERJET]);
		playerList[id] = p;
		re->addObject(p);
		panel->addObject(p);
	}
	playerList[id]->update(updt);
}

void Game::updateObject(struct updateObj *updt) {
	if (updt->updateFieldID < 16) {
		updatePlayer(updt);
	} else {
		updateMissile(updt);
	}
}

