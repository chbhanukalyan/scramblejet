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

#include "GamingClient.h"
#include <SDL/SDL.h>

GamingClient::GamingClient(void)
{
	evmap = new EventMap;
}

GamingClient::~GamingClient()
{
	delete evmap;
}

int GamingClient::initialize(const char *evmapfn)
{
	evmap->loadKeymap(evmapfn);

	return 0;
}

void GamingClient::handleEvents(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_KEYDOWN: 
			{
				evmap->keyDown(event.key.keysym.sym);
				break;
			}
			case SDL_KEYUP: 
			{
				evmap->keyUp(event.key.keysym.sym);
				break;
			}
			case SDL_QUIT:
			{
				exit(0);
			}
			default:
				break;
		}
	}
}

int connect(void)
{

	return 0;
}
