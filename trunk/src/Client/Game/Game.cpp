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
#include "../Map/MapLoader.h"

Game::Game(const char *data_dir, RenderingEngine *re, GamingClient *gc, const char *mapname)
{
	strcpy(this->data_dir, data_dir);
	this->re = re;
	this->gc = gc;
	strcpy(mapfn, mapname);
}

Game::~Game()
{
}

void Game::initGame(void)
{
	map = loadMap(mapfn);

	re->Initialize(&map->initCamPos);

	gc->initialize("default.eventmap", 10);

	player = new Player(map->objList->next);
	re->addObject(player);
	player->load((void*)gc);

	skybox = new SkyBox;
	skybox->load(map->skyboxfn);
	skybox->setSize(map->sizex, map->sizey, map->sizez);
	re->addObject(skybox);

}

void Game::startGame(void)
{
	gc->Connect("127.0.0.1");
}

void Game::stopGame(void)
{
}


