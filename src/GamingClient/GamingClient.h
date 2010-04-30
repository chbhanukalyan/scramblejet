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

#include "EventMap.h"

class GamingClient {
	private:

	public:
		EventMap *evmap;

		GamingClient(void);
		~GamingClient();

		int initialize(const char *eventmap);

		int connect(void);
		void handleEvents(void);

};

#endif	/*	__GC_GAMINGCLIENT_H__	*/

