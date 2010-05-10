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

	re->Initialize(&map->initCamPos);

	panel = new Panel;
	re->addPanel(panel);

	re->loadModels();

	ObjInfo *o = map->objList;
	while (o) {
		Player *p = new Player(o, re->modelList[MODELID_F16_FIGHTERJET]);
		re->addObject(p);
		panel->addObject(p);
		playerList[p->id] = p;
		o = o->next;
	}

	skybox = new SkyBox;
	skybox->load(map->skyboxfn);
	skybox->setSize(map->sizex, map->sizey, map->sizez);
	re->addObject(skybox);

	return 0;
}

void Game::startGame(void)
{
}

void Game::runGameLoop(void)
{
	CamPos cp;
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
		Missile *m = new Missile(id, re->modelList[MODELID_F16_FIGHTERJET]);
		missileList[id] = m;
		re->addObject(m);
		panel->addObject(m);
	}
	missileList[id]->update(updt);
}

