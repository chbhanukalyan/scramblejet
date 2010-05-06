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

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>

#include "../../protocol.h"

GamingClient::GamingClient(void)
{
	evmap = new EventMap;
	commSocket = -1;
	updateInterval = DEFAULT_NETWORK_EVENT_UPDATE_INTERVAL;
	networkUpdateCounter = 0;
	connected = false;
}

GamingClient::~GamingClient()
{
	if (connected)
		Disconnect();
	delete evmap;
}

unsigned int timer_callback(unsigned int tm, void *param)
{
	GamingClient *gc = (GamingClient*)param;

	gc->updateTimerCb();

	return gc->timerInterval;
}

void GamingClient::updateTimerCb(void)
{
	evmap->updateTimer();

	if (connected) {
		networkUpdateCounter++;
		if (networkUpdateCounter >= updateInterval) {
			sendEventList();
			networkUpdateCounter = 0;
		}
	}
}

void Quit(void);

int GamingClient::initialize(const char *evmapfn, int timerInt)
{
	evmap->loadKeymap(evmapfn);

	evmap->registerFunction(FUNCID_QUIT, Quit);

	timerInterval = (timerInt / 10) * 10;
	if (timerInterval != timerInt) {
		fprintf(stderr, "WARNING: Timer Interval(%d) rounded off to %d\n",
				timerInt, timerInterval);
	}

	if ((timerID = SDL_AddTimer(timerInterval, timer_callback, this)) == NULL) {
		fprintf(stderr, "WARNING: Unable to set SDL Timer\n");
		return -1;
	}

	return 0;
}

void GamingClient::handleEvents(Player **playerList)
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

	handleNetworkEvents(playerList);

}

void GamingClient::handleNetworkEvents(Player **playerList)
{
	/* Recieve Network Packets */
	char buf[MAX_PACKET_SIZE];
	int len = MAX_PACKET_SIZE;
	while (true) {
		struct timeval tm;
		tm.tv_sec = 0;
		tm.tv_usec = 0;
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(commSocket, &fds);
		int ret = select(commSocket + 1, &fds, NULL, NULL, &tm);
		if (ret > 0) {
			if (recvPacket(buf, &len) < 0)
				continue;
			printf("recved packet from server len=%d\n", len);

			struct srvUpdatePacket *sup = (struct srvUpdatePacket *)buf;
			if ((sup->phdr.version != CUR_PROTOCOL_VERSION) ||
					(sup->phdr.magic != CUR_PROTOCOL_MAGIC) ||
					(sup->phdr.type != PKTTYPE_SRV2CLI_OBJUPDTLIST))
				continue;

			unsigned char *b = (unsigned char *)sup->lst;

			/* TODO call update objects HERE */
			for (int i = 0; i < sup->num_updtobjs; i++) {
				struct updateObj *updt = (struct updateObj*)b;
				printf("sup->num_updtobjs = %d updtfid = %d siz=%d\n", sup->num_updtobjs, updt->updateFieldID, updt->size);
				if (playerList[updt->updateFieldID])
					playerList[updt->updateFieldID]->update(updt);
				b += updt->size;
			}

		}
		if (ret == 0)
			break;
	}
}

int GamingClient::doHandshake(int *localPlayerID, char *mapName)
{
	char buf[MAX_PACKET_SIZE];
	int len = MAX_PACKET_SIZE;

	struct cliJoinPkt *cjp = (struct cliJoinPkt*)buf;
	cjp->phdr.version = CUR_PROTOCOL_VERSION;
	cjp->phdr.magic = CUR_PROTOCOL_MAGIC;
	cjp->phdr.type = PKTTYPE_CLI2SRV_JOINGAME;
	strncpy(cjp->playerName, "Bhanu", 16);

	if (send(commSocket, buf, sizeof(*cjp), 0) < 0) {
		fprintf(stderr, "Error sending join packet to Server\n");
		return -1;
	}

	if ((len = recv(commSocket, buf, len, 0)) < 0) {
		fprintf(stderr, "Server socket Recv failed: %s\n", strerror(errno));
		return -1;
	}

	struct srvWelcomePkt *swp = (struct srvWelcomePkt*)buf;
	if ((swp->phdr.version != CUR_PROTOCOL_VERSION) ||
			(swp->phdr.magic != CUR_PROTOCOL_MAGIC) ||
			(swp->phdr.type != PKTTYPE_SRV2CLI_JOINEDGAME))
		return -1;

	*localPlayerID = swp->generated_id;
	strncpy(mapName, swp->mapName, 32);

	fprintf(stderr, "Client was successfully able to join game with ID=%d\n", *localPlayerID);
	return 0;
}

int GamingClient::Connect(const char *serverIP, int port, int *localPlayerID, char *mapName)
{
	int err = -1;

	strncpy(this->serverIP, serverIP, sizeof(this->serverIP));

	commSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (commSocket < 0) {
		fprintf(stderr, "Client socket Creation failed: %s\n", strerror(errno));
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
	
	if (connect(commSocket, (struct sockaddr*)&st, sizeof(st)) < 0) {
		fprintf(stderr, "Client socket Connect failed: %s\n", strerror(errno));
		goto out;
	}

	if (doHandshake(localPlayerID, mapName) < 0) {
		fprintf(stderr, "Unable to join Server\n");
		goto out;
	}

	err = 0;
	connected = true;

 out:
	if (err) {
		close(commSocket);
		commSocket = -1;
	}
	return err;
}

void GamingClient::Disconnect(void)
{
	if (connected) {
		connected = false;
		close(commSocket);
		commSocket = -1;
	}
}

int GamingClient::sendEventList(void)
{
	unsigned char buf[MAX_PACKET_SIZE];

	struct cliEventPacket *evp = (struct cliEventPacket *)buf;
	evp->phdr.version = CUR_PROTOCOL_VERSION;
	evp->phdr.type = PKTTYPE_CLI2SRV_EVENTLIST;
	evp->phdr.magic = CUR_PROTOCOL_MAGIC;

	int len = evmap->serializeEvMap((unsigned char*)(evp->fnlist), (int)(MAX_PACKET_SIZE - sizeof(struct protocolHeader)));

	if (len == 0)
		return 0;

	evp->numfuncs = len/sizeof(struct cliEvObj);

	fprintf(stderr, "sending event list(%d) len(%d) to server\n", evp->numfuncs, len);

	return sendPacket(buf, sizeof(cliEventPacket) + len);
}

int GamingClient::sendPacket(void *buf, int len)
{
	fprintf(stderr, "Client socket Send(%d bytes)\n", len);
	if (send(commSocket, buf, len, 0) != len) {
		fprintf(stderr, "Client socket Send(%d bytes) failed: %s\n",
				len, strerror(errno));
		return -1;
	}
	return 0;
}

int GamingClient::recvPacket(void *buf, int *len)
{
	int l = *len;
	if ((l = recv(commSocket, buf, l, 0)) < 0) {
		fprintf(stderr, "Client socket Recv failed: %s\n", strerror(errno));
		return -1;
	}
	*len = l;
	return l;
}

