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

GamingEngine *GamingEngine::g_GamingEngine = NULL;

GamingEngine::GamingEngine(Map *map)
{
	sentObjList = NULL;
	moveObjList = NULL;
	memset(playerList, 0, sizeof(playerList));
	this->map = map;
	timer_loop_time = DEFAULT_WAIT_BEFORE_START_USEC;

	g_GamingEngine = this;
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
		
		MovableObject *mov = moveObjList;
		while (mov) {
			mov->doTick();
			mov = mov->moveNext;
		}
		
		/* Dispatch updates after some time */
		char buf[MAX_PACKET_SIZE];
		int len = sizeof(struct protocolHeader) + sizeof(int);

		int count = 0;
		SentientObject *seo = sentObjList;
		while (seo) {
			int l = seo->serializeState(buf + len);
			len += l;
			seo = seo->sentNext;
			count++;
		}
		mov = moveObjList;
		while (mov) {
			int l = mov->serializeState(buf + len);
			len += l;
			mov = mov->moveNext;
			count++;
		}
		server->broadcastUpdates(buf, len, count);

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
	if (id != o->id)
		playerList[id]->id = id;
	addSentObject(playerList[id]);
}

void GamingEngine::removePlayer(int id)
{
	Player *p = playerList[id];
	playerList[id] = NULL;
	removeSentObject(p);
	delete p;
}

void GamingEngine::addSentObject(SentientObject *seo)
{
	seo->sentNext = sentObjList;
	sentObjList = seo;
}

void GamingEngine::removeSentObject(SentientObject *seo)
{
	if (sentObjList == NULL)
		return;

	if (sentObjList == seo) {
		sentObjList = seo->sentNext;
		seo->sentNext = NULL;
		return;
	}
	SentientObject *tmp = sentObjList;
	while (tmp) {
		if (tmp->sentNext == seo) {
			tmp->sentNext = tmp->sentNext->sentNext;
			seo->sentNext = NULL;
			return;
		}
		tmp = tmp->sentNext;
	}
	fprintf(stderr, "WARNING: Trying to remove Sentient Object: "
			"not in list\n");
}

void GamingEngine::addMissile(Missile *m)
{

	fprintf(stderr, "Launching Missile\n");
	addMovableObject(m);
}

void GamingEngine::addMovableObject(MovableObject *mov)
{
	mov->moveNext = moveObjList;
	moveObjList = mov;
}

void GamingEngine::removeMovableObject(MovableObject *mov)
{
	if (moveObjList == NULL)
		return;

	if (moveObjList == mov) {
		moveObjList = mov->moveNext;
		mov->moveNext = NULL;
		return;
	}
	MovableObject *tmp = moveObjList;
	while (tmp) {
		if (tmp->moveNext == mov) {
			tmp->moveNext = tmp->moveNext->moveNext;
			mov->moveNext = NULL;
			return;
		}
		tmp = tmp->moveNext;
	}
	fprintf(stderr, "WARNING: Trying to remove Sentient Object: "
			"not in list\n");
}

