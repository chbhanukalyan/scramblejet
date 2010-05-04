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
#include <string.h>

#include "../../funcids.h"

struct funcst {
	const char *name;
	int id;
};

static const struct funcst funcarr[] = {
	{ "INVALID", FUNCID_INVALID },
	{ "ACCELERATE", FUNCID_ACCELERATE },
	{ "DECCELERATE", FUNCID_DECCELERATE },
	{ "BANKLEFT", FUNCID_BANKLEFT },
	{ "BANKRIGHT", FUNCID_BANKRIGHT },
	{ "CLIMBUP", FUNCID_CLIMBUP },
	{ "CLIMBDOWN", FUNCID_CLIMBDOWN },
	{ "FIRE1", FUNCID_FIRE1 },
	{ "FIRE2", FUNCID_FIRE2 },
	{ "ROTXP", FUNCID_ROTXP },
	{ "ROTXM", FUNCID_ROTXM },
	{ "ROTYP", FUNCID_ROTYP },
	{ "ROTYM", FUNCID_ROTYM },
	{ "ROTZP", FUNCID_ROTZP },
	{ "ROTZM", FUNCID_ROTZM },
	{ "QUIT", FUNCID_QUIT },
};

int getFuncIDfromStr(const char *str)
{
	int i;
	for (i = 0; i < (int)(sizeof(funcarr)/sizeof(funcarr[0])); i++) {
		if (!strcmp(funcarr[i].name, str))
			return funcarr[i].id;
	}
	return FUNCID_INVALID;
}

