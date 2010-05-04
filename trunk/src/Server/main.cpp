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
#include "GamingServer/Server.h"
#include "GamingEngine/GamingEngine.h"

int main(int argc, char **argv)
{
	GamingEngine *ge = NULL;
	Server *srv = NULL;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s map\n", argv[0]);
		return -1;
	}

	ge = new GamingEngine;
	srv = new Server(ge);

	srv->initialize("127.0.0.1");

	while (srv->waitForClients() == 0) {
		fprintf(stderr, "Still waiting for clients\n");
	}

	srv->startEventRecver();

	ge->startGame(srv);

	srv->stop();

	return 0;
}

