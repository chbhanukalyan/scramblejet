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
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <signal.h>

#include <unistd.h>
#include <errno.h>

#include "Game/Game.h"
#include "RenderingEngine/RenderingEngine.h"
#include "GamingClient/GamingClient.h"
//#include "Menu/Menu.h"
#include "../Map/MapLoader.h"

int hres = 640, vres=480;
#define	TIME_INTERVAL	10

pid_t srvpid = -1;

void Quit(void)
{
	if (srvpid != -1)
		kill(srvpid, SIGKILL);

	SDL_SetTimer(TIME_INTERVAL, NULL);
	SDL_Quit();
	exit(0);
}

int fork_server(const char *path, const char *mapname)
{
	srvpid = fork();
	if (srvpid < 0) {
		fprintf(stderr, "Unable to fork server: %s", strerror(errno));
		return -1;
	}

	if (srvpid)
		return 0;

	execlp(path, path, mapname, NULL);
	fprintf(stderr, "Unable to exec server: %s", strerror(errno));
	exit(0);
}

int main(int argc, char **argv)
{
	int VideoFlags = 0;
	SDL_Surface *MainWindow;
	bool fullscreen = false;

	if (argc >= 2) {
		if (!strcmp(argv[1], "-s")) {
			fork_server("../Server/scRambleSrv", "maps/default.map");
		}
	}

	atexit(Quit);

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)	// try to initialize SDL video module
	{
		printf("SDL ERROR:%s\n", SDL_GetError());	// report error if it fails
		Quit();
	}

	const SDL_VideoInfo *VideoInfo = SDL_GetVideoInfo();	// query SDL for information about our video hardware

	if (VideoInfo == NULL)	// if this query fails
	{
		printf("Failed getting Video Info : %s\n", SDL_GetError());	// report error
		Quit();
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	// tell SDL that the GL drawing is going to be double buffered

	VideoFlags = SDL_OPENGL | SDL_HWPALETTE;
	VideoFlags |= (fullscreen) ? SDL_FULLSCREEN : SDL_RESIZABLE;

	MainWindow = SDL_SetVideoMode(hres, vres, 16, VideoFlags);

	if (MainWindow == NULL)	// if window creation failed
	{
		printf("Failed to Create Window : %s\n", SDL_GetError());	// report error
		Quit();
	}
	SDL_WM_SetCaption("scRamble", "scRamble");
	SDL_ShowCursor( SDL_DISABLE );

	Map *m = loadMap();
	if (m == NULL) {
		Quit();
	}

	RenderingEngine *re = new RenderingEngine();

	GamingClient *gc = new GamingClient();

	chdir("../../data/");

	Game *game = new Game("../../data/", re, gc);
	if (game->initGame()) {
		fprintf(stderr, "Failed to Init Game\n");
		Quit();
	}

//	Menu *menu = new Menu;

	game->startGame();

	while (1) {
		game->runGameLoop();
//		menu->render();

		SDL_GL_SwapBuffers();
	}

	return 0;
}

