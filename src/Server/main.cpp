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

#include <stdio.h>
#include <unistd.h>
#include "../Map/MapLoader.h"
#include "GamingServer/Server.h"
#include "GamingEngine/GamingEngine.h"

int main(int argc, char **argv)
{
	GamingEngine *ge = NULL;
	Server *srv = NULL;
	Map *map = NULL;
	const char *mapname = "maps/default.map";

	if (argc > 2) {
		fprintf(stderr, "Usage: %s map\n", argv[0]);
		return -1;
	}
	if (argc == 2) {
		mapname = argv[1];
	}

	chdir("../../data");

	map = loadMap(mapname);
	if (map == NULL) {
		fprintf(stderr, "Unable to load Map: %s\n", mapname);
		return -1;
	}

	ge = new GamingEngine(map);
	srv = new Server(ge);

	srv->initialize("0.0.0.0", 6501, 7);
	
	ge->startGame(srv);

	srv->stop();

	return 0;
}

