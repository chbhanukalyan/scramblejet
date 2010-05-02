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


#ifndef		__PROTOCOL_H__
#define		__PROTOCOL_H__

#define	MAX_PACKET_SIZE		512

/* Server-Client Protocol
 * ======================
 */
struct protocolHeader {
	unsigned char version;
	unsigned char type;
	unsigned short magic;

	unsigned int serialno;
};

#define		CUR_PROTOCOL_VERSION	0x01
#define		CUR_PROTOCOL_MAGIC		0xBADB

#define	PKTTYPE_CLI2SRV_EVENTLIST	0x01
struct cliEvObj {
	unsigned short funcid;
	unsigned short count;
};

struct cliEventPacket {
	struct protocolHeader phdr;

	int numfuncs;
	/* Current function status */
	struct cliEvObj fnlist[];
};

#define	PKTTYPE_SRV2CLI_OBJUPDTLIST	0x02
struct updateObj {
	unsigned char size;
	unsigned char updateFieldID;
	unsigned char vals[];
};

struct srvUpdatePacket {
	struct protocolHeader phdr;

	/* Object Update List */
	struct updateObj lst[];
};

#define	PKTTYPE_CLI2SRV_JOINGAME	0xF0
struct cliJoinPkt {
	struct protocolHeader phdr;
	char playerName[16];
};

#define	PKTTYPE_SRV2CLI_JOINEDGAME	0xF1
struct srvWelcomePkt {
	struct protocolHeader phdr;
};

#endif	/*	__PROTOCOL_H__	*/

