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
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>

#include "../../protocol.h"

#include "GamingEngine.h"
#include "../GamingServer/Server.h"

GamingEngine::GamingEngine(Map *map)
{
	objList = NULL;
	memset(playerList, 0, sizeof(playerList));
	this->map = map;
	timer_loop_time = DEFAULT_WAIT_BEFORE_START_USEC;
}

GamingEngine::~GamingEngine()
{
}

int GamingEngine::initialize(void)
{
	return 0;
}

int GamingEngine::startGame(void *serv)
{
	int wait_time = timer_loop_time;

	fprintf(stderr, "Game Started\n");

	Server *server = (Server *)serv;
	while (true) {
		struct timeval starttm, stoptm;

		gettimeofday(&starttm, NULL);

		for (int i = 0; i < MAX_CLIENT_ID; i++) 
			if (playerList[i])
				playerList[i]->doTick();
		
		/* Dispatch updates after some time */
		char buf[MAX_PACKET_SIZE];
		int len = sizeof(struct protocolHeader) + sizeof(int);

		int count = 0;
		SentientObject *seo = objList;
		while (seo) {
			int l = seo->serializeState(buf + len);
			len += l;
			seo = seo->next;
			count++;
		}
		server->broadcastUpdatePacket(buf, len, count);

		gettimeofday(&stoptm, NULL);
		wait_time = timer_loop_time - (stoptm.tv_usec - starttm.tv_usec);
		if (wait_time < 0)
			wait_time = 0;

		/* Gather all events from queued packets */
		server->handleIncomingPackets(map->name, wait_time);
	}
	return 0;
}

void GamingEngine::addPlayer(int id, const char *name)
{
	ObjInfo *o = map->getObj(OBJTYPE_PLAYER, id);
	playerList[id] = new Player(o, name);
	addObject(playerList[id]);
}

void GamingEngine::removePlayer(int id)
{
	Player *p = playerList[id];
	playerList[id] = NULL;
	removeObject(p);
	delete p;
}

void GamingEngine::addObject(SentientObject *seo)
{
	seo->next = objList;
	objList = seo;
}

void GamingEngine::removeObject(SentientObject *seo)
{
	if (objList == NULL)
		return;

	if (objList == seo) {
		objList = seo->next;
		seo->next = NULL;
		return;
	}
	SentientObject *tmp = objList;
	while (tmp) {
		if (tmp->next == seo) {
			tmp->next = tmp->next->next;
			seo->next = NULL;
			return;
		}
		tmp = tmp->next;
	}
	fprintf(stderr, "WARNING: Trying to remove Sentient Object: "
			"not in list\n");
}

