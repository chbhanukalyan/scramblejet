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


#ifndef		__GC_GAMINGCLIENT_H__
#define		__GC_GAMINGCLIENT_H__

#include <SDL/SDL_timer.h>
#include "EventMap.h"

#define	DEFAULT_NETWORK_EVENT_UPDATE_INTERVAL	2


class GamingClient {
	private:
		SDL_TimerID timerID;
		bool connected;

	public:
		long curTicks;

		EventMap *evmap;
		int timerInterval;

		GamingClient(void);
		~GamingClient();

		int initialize(const char *eventmap, int timerInterval);

		void handleEvents(void);

		void updateTimerCb(void);

		/* Networking Info */
	private:
		char serverIP[16];
		int serverPort;
		int localPort;
		int commSocket;

		char pktEventBuf[512];

		int doHandshake(int *localPlayerID, char *mapName);
		int sendPacket(void *buf, int len);
		int recvPacket(void *buf, int *len);

		void handleNetworkEvents(void);

		int updateInterval;
		int networkUpdateCounter;

	public:
		int Connect(const char *serverIP, int port, int *localPlayerID, char *mapName);
		void Disconnect(void);

		int sendEventList(void);

};

#endif	/*	__GC_GAMINGCLIENT_H__	*/

