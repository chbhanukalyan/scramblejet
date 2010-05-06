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
	waitBeforeStart = DEFAULT_WAIT_BEFORE_START;
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

int Server::waitForClients(void)
{
	int numClients = 0;
	struct timeval tm;
	tm.tv_sec = waitBeforeStart;
	tm.tv_usec = 0;

	fprintf(stderr, "INFO: Waiting for Clients\n");

	while (true) {
		int ret;
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(commSocket, &fds);

		/* Recv packets */
		ret = select(commSocket+1, &fds, NULL, NULL, &tm);
		if (ret > 0) {
			if (FD_ISSET(commSocket, &fds)) {
				welcomeClient();
				numClients++;
			}
		}
		else if (ret == 0)
			break;
		else
			return -1;
	}

	return numClients;
}

void *event_recver_thread(void *arg)
{
	Server *srvr = (Server*)arg;
	printf("Event Reciever Thread Started\n");
	while (true) {
		srvr->getEventList();
	}

	return NULL;
}

int Server::startEventRecver(void)
{
	if (pthread_create(&eventReceiverThread, NULL, event_recver_thread, this) < 0) {
		fprintf(stderr, "Unable to start Event recevier thread\n");
		return -1;
	}

	pthread_detach(eventReceiverThread);

	return 0;
}

void Server::stop(void)
{
	close(commSocket);
}

void Server::welcomeClient(void)
{
	char buf[MAX_PACKET_SIZE];
	int len = MAX_PACKET_SIZE;

	struct sockaddr_in st;
	socklen_t slen = sizeof(st);

	if ((len = recvfrom(commSocket, buf, len, 0, (struct sockaddr*)&st, &slen)) < 0) {
		fprintf(stderr, "Server socket Recv failed: %s\n", strerror(errno));
		return;
	}
	if (lookupClientID(&st) != INVALID_CLIENT_ID)
		return;

	struct cliJoinPkt *cjp = (struct cliJoinPkt*)buf;
	if ((cjp->phdr.version != CUR_PROTOCOL_VERSION) &&
			(cjp->phdr.magic != CUR_PROTOCOL_MAGIC) &&
			(cjp->phdr.type != PKTTYPE_CLI2SRV_JOINGAME))
		return;

	ClientInfo *ci = &cliinfo[curclients];
	memcpy(&ci->sendst, &st, sizeof(st));
	inet_ntop(AF_INET, &st.sin_addr, ci->IP, 16);
	strncpy(ci->playerName, cjp->playerName, 16);

	struct srvWelcomePkt *swp = (struct srvWelcomePkt*)buf;
	swp->phdr.version = CUR_PROTOCOL_VERSION;
	swp->phdr.magic = CUR_PROTOCOL_MAGIC;
	swp->phdr.type = PKTTYPE_SRV2CLI_JOINEDGAME;
	swp->generated_id = curclients;

	if (sendto(commSocket, buf, sizeof(*swp), 0,
				(struct sockaddr *)&ci->sendst, sizeof(ci->sendst)) < 0) {
		fprintf(stderr, "Error sending welcome packet to Client IP(%s) "
				"Player(%s)\n", ci->IP, ci->playerName);
	}

	ge->addPlayer(curclients, ci->playerName);

	fprintf(stderr, "Client connected from Client IP(%s) Player(%s)\n",
			ci->IP, ci->playerName);
	curclients++;
	return;
}

int Server::getEventList(void)
{
	int len = MAX_PACKET_SIZE;
	unsigned char buf[MAX_PACKET_SIZE];
	ClientID cid = recvPacket(buf, &len);
	if (cid == INVALID_CLIENT_ID)
		return -1;


	struct cliEventPacket *evp = (struct cliEventPacket *)buf;
	if ((evp->phdr.version != CUR_PROTOCOL_VERSION) &&
			(evp->phdr.magic != CUR_PROTOCOL_MAGIC) &&
			(evp->phdr.type != PKTTYPE_CLI2SRV_EVENTLIST))
		return -1;

	printf("Got Event List from Client: %d\n", evp->numfuncs);
	int i;
	for (i = 0; i < evp->numfuncs; i++) {
		printf("Event(%d) is: %d, %d\n",i, evp->fnlist[i].funcid, evp->fnlist[i].count);
		ge->p->handleEvent(evp->fnlist[i].funcid, evp->fnlist[i].count);
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
		if ((cliinfo[i].sendst.sin_addr.s_addr == st->sin_addr.s_addr) && (cliinfo[i].sendst.sin_port == st->sin_port))
			return i;

	return INVALID_CLIENT_ID;
}

ClientID Server::recvPacket(void *buf, int *len)
{
	int l = *len;
	struct sockaddr_in st;
	socklen_t slen = sizeof(st);

	if ((l = recvfrom(commSocket, buf, l, 0, (struct sockaddr*)&st, &slen)) < 0) {
		fprintf(stderr, "Client socket Recv failed: %s\n", strerror(errno));
		return INVALID_CLIENT_ID;
	}
	*len = l;

	/* Lookup the client information */
	return lookupClientID(&st);
}

