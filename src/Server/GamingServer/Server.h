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


#ifndef		__GE_SERVER_H__
#define		__GE_SERVER_H__

#include <arpa/inet.h>

#include "../GamingEngine/GamingEngine.h"

struct ClientInfo {
	char IP[16];
	char playerName[16];

	struct sockaddr_in sendst;
};

typedef int ClientID;

class Server {
	private:
		int curclients;
		ClientInfo cliinfo[MAX_CLIENT_ID];

		GamingEngine *ge;

	public:
		Server(GamingEngine *ge);
		~Server();

		int initialize(const char *serverIP, int port = 6501);
		int handleIncomingPackets(const char *mapName, long waitTime);

		void stop(void);

		/* Network Information */
		int broadcastUpdatePacket(void *buf, int len, int count_objs);

	private:
		int commSocket;
		int processIncomingPacket(const char *mapName);
		void welcomeClient(ClientID cid, struct sockaddr_in *st, const char *mapName, unsigned char *buf, int len);
		int getEventList(ClientID cid, unsigned char *buf, int len);

		int sendPacket(ClientID cid, void *buf, int len);
		ClientID lookupClientID(struct sockaddr_in *st);

};

#endif	/*	__GE_SERVER_H__	*/

