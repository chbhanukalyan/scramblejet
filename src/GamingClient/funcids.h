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


#ifndef		__GC_FUNCIDS_H__
#define		__GC_FUNCIDS_H__

enum {
	FUNCID_INVALID		=	0,

	FUNCID_ACCELERATE	=	1,
	FUNCID_DECCELERATE	=	2,
	FUNCID_BANKLEFT		=	3,
	FUNCID_BANKRIGHT	=	4,
	FUNCID_CLIMBUP		=	5,
	FUNCID_CLIMBDOWN	=	6,

	FUNCID_FIRE1		=	11,
	FUNCID_FIRE2		=	12,

	FUNCID_ROTXP		=	21,
	FUNCID_ROTXM		=	22,
	FUNCID_ROTYP		=	23,
	FUNCID_ROTYM		=	24,
	FUNCID_ROTZP		=	25,
	FUNCID_ROTZM		=	26,

	FUNCID_QUIT			=	51,

	MAX_FUNCID			=	128
};

extern int getFuncIDfromStr(const char *str);


#endif	/*	__GC_FUNCIDS_H__	*/

