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

#include "../../protocol.h"

#include "GamingEngine.h"
#include "../GamingServer/Server.h"

GamingEngine::GamingEngine(Map *map)
{
	objList = NULL;
	memset(playerList, 0, sizeof(playerList));
	this->map = map;
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
	Server *server = (Server *)serv;
	while (true) {
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
			printf("SEO SERIAL len=%d ret=%d\n", len, l);
			len += l;
			seo = seo->next;
			count++;
		}
		server->broadcastUpdatePacket(buf, len, count);
		/* sleep some time before starting again TODO Make wakeup */
		usleep(50000);
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
	removeObject(playerList[id]);
	delete playerList[id];
	playerList[id] = NULL;
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

