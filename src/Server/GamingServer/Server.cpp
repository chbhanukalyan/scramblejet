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
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>

#include "../../protocol.h"

#include "../GamingEngine/GamingEngine.h"
#include "Server.h"

Server::Server(GamingEngine *ge)
{
	commSocket = -1;
	curclients = 0;
	memset(&cliinfo, 0, sizeof(cliinfo));
	this->ge = ge;
}

Server::~Server()
{
}

int Server::initialize(const char *serverIP, int port)
{
	int err = -1;

	if ((commSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		fprintf(stderr, "Creating Server Socket failed: %s\n", strerror(errno));
		goto out;
	}

	struct sockaddr_in st;
	memset(&st, 0, sizeof(st));
	st.sin_family = AF_INET;
	st.sin_port = htons(port);
	if (inet_pton(AF_INET, serverIP, &st.sin_addr) != 1) {
		fprintf(stderr, "Server IP: %s is invalid\n", serverIP);
		goto out;
	}
	
	if (bind(commSocket, (struct sockaddr*)&st, sizeof(st)) < 0) {
		fprintf(stderr, "Server socket bind failed: %s\n", strerror(errno));
		goto out;
	}

	err = 0;

 out:
	if (err) close(commSocket);
	return err;
}

int Server::handleIncomingPackets(const char *mapName, long waitTime)
{
	struct timeval tm;
	tm.tv_sec = 0;
	tm.tv_usec = waitTime;

	while (true) {
		int ret;
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(commSocket, &fds);

		/* Recv packets */
		ret = select(commSocket+1, &fds, NULL, NULL, &tm);
		if (ret > 0) {
			if (FD_ISSET(commSocket, &fds)) {
				processIncomingPacket(mapName);
			}
		}
		else if (ret == 0)
			break;
		else
			return -1;
	}

	return 0;
}

int Server::processIncomingPacket(const char *mapName)
{
	unsigned char buf[MAX_PACKET_SIZE];
	int len = MAX_PACKET_SIZE;
	struct sockaddr_in st;
	socklen_t slen = sizeof(st);

	if ((len = recvfrom(commSocket, buf, len, 0, (struct sockaddr*)&st, &slen)) < 0) {
		fprintf(stderr, "Client socket Recv failed: %s\n", strerror(errno));
		return -1;
	}

	struct protocolHeader *phdr = (struct protocolHeader *)buf;

	if ((phdr->version != CUR_PROTOCOL_VERSION) ||
			(phdr->magic != CUR_PROTOCOL_MAGIC)) {
		fprintf(stderr, "Illegal packet recvd from client:%x port:%d ver:%d magic:%#x\n",
				st.sin_addr.s_addr, htons(st.sin_port), phdr->version, phdr->magic);
		return -1;
	}

	ClientID cid = lookupClientID(&st);

	switch (phdr->type) {
		case PKTTYPE_CLI2SRV_EVENTLIST: {
			if (cid != INVALID_CLIENT_ID) {
				getEventList(cid, buf, len);
			} else {
				fprintf(stderr, "Invalid client:%x port:%d sent eventlist\n",
						st.sin_addr.s_addr, htons(st.sin_port));
				return -1;
			}

			break;
		}
		case PKTTYPE_CLI2SRV_JOINGAME: {
			welcomeClient(cid, &st, mapName, buf, len);
			break;
		}
	}

	return 0;
}

void Server::stop(void)
{
	close(commSocket);
}

void Server::welcomeClient(ClientID cid, struct sockaddr_in *st, const char *mapName, unsigned char *buf, int len)
{
	struct cliJoinPkt *cjp = (struct cliJoinPkt*)buf;
	ClientInfo *ci = NULL;

	/* Handles case where UDP packet was lost, so reattempting to join */
	if (cid == INVALID_CLIENT_ID) {
		ci = &cliinfo[curclients];
		memcpy(&ci->sendst, st, sizeof(struct sockaddr_in));
		inet_ntop(AF_INET, &st->sin_addr.s_addr, ci->IP, 16);
		strncpy(ci->playerName, cjp->playerName, 16);
		cid = curclients;
		ge->addPlayer(cid, ci->playerName);

		fprintf(stderr, "Client connected from Client IP(%s) Player(%s)\n",
				ci->IP, ci->playerName);
	} else {
		ci = &cliinfo[cid];

		fprintf(stderr, "Client re-connected from Client IP(%s) Player(%s)\n",
				ci->IP, ci->playerName);
	}

	struct srvWelcomePkt *swp = (struct srvWelcomePkt*)buf;
	swp->phdr.version = CUR_PROTOCOL_VERSION;
	swp->phdr.magic = CUR_PROTOCOL_MAGIC;
	swp->phdr.type = PKTTYPE_SRV2CLI_JOINEDGAME;
	swp->generated_id = cid;
	strncpy(swp->mapName, mapName, 32);

	if (sendto(commSocket, buf, sizeof(*swp), 0,
				(struct sockaddr *)&ci->sendst, sizeof(ci->sendst)) < 0) {
		fprintf(stderr, "Error sending welcome packet to Client IP(%s) "
				"Player(%s)\n", ci->IP, ci->playerName);
	}

	curclients++;
	return;
}

int Server::getEventList(ClientID cid, unsigned char *buf, int len)
{
	struct cliEventPacket *evp = (struct cliEventPacket *)buf;

	printf("Got Event List from Client: %d\n", evp->numfuncs);
	int i;
	for (i = 0; i < evp->numfuncs; i++) {
		printf("Event from %d (%d) is: %d, %d\n", cid, i, evp->fnlist[i].funcid, evp->fnlist[i].count);
		ge->playerList[cid]->handleEvent(evp->fnlist[i].funcid, evp->fnlist[i].count);
	}

	return 0;
}

int Server::broadcastUpdatePacket(void *buf, int len, int count)
{
	int i;
	struct srvUpdatePacket *sup = (struct srvUpdatePacket*)buf;
	sup->phdr.version = CUR_PROTOCOL_VERSION;
	sup->phdr.magic = CUR_PROTOCOL_MAGIC;
	sup->phdr.type = PKTTYPE_SRV2CLI_OBJUPDTLIST;
	sup->num_updtobjs = count;
	fprintf(stderr, "Broadcasting Packet(len=%d) to all clients\n", len);
	for (i = 0 ; i < curclients; i++) {
		sendPacket(i, buf, len);
	}
	return 0;
}

int Server::sendPacket(ClientID cid, void *buf, int len)
{
	if (sendto(commSocket, buf, len, 0, (struct sockaddr*)&cliinfo[cid].sendst,
				sizeof(struct sockaddr_in)) != len) {
		fprintf(stderr, "Client socket Send(%d bytes) failed: %s\n",
				len, strerror(errno));
		return -1;
	}
	return 0;
}

ClientID Server::lookupClientID(struct sockaddr_in *st)
{
	int i;
	for (i = 0; i < curclients; i++)
		if ((cliinfo[i].sendst.sin_addr.s_addr == st->sin_addr.s_addr) &&
				(cliinfo[i].sendst.sin_port == st->sin_port))
			return i;

	return INVALID_CLIENT_ID;
}

