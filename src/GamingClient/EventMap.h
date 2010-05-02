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


#ifndef		__GC_EVENTMAP_H__
#define		__GC_EVENTMAP_H__

#include "funcids.h"

typedef void (EventCallbackFunc)(void);

#define	MAX_EVENTS_PER_FUNC	4
#define	MAX_NUM_FUNCS	256
#define	MAX_NUM_EVENTS	1024
struct function {
	/* For polling */
	int inprogress;
	/* For Callback */
	EventCallbackFunc *cbs;
	int timestamp;
};

class EventMap {
	private:
		unsigned long long timestamp;
		int getKeyID(const char *key);

		int ev2funcid[MAX_NUM_EVENTS];

		struct function funcArr[MAX_NUM_FUNCS];

		inline void registerEvent(int key, int func) {
			ev2funcid[key] = func;
		}

	public:
		EventMap(void);

		void updateTimer(void)
		{
			timestamp++;
		}

		int loadKeymap(const char *evmapfn);

		inline void registerFunction(int funcid, EventCallbackFunc *cb) {
			funcArr[funcid].cbs = cb;
		}
		inline int eventInProgress(int funcid) {
			return funcArr[funcid].inprogress;
		}
		inline void keyUp(int key) {
			int func = ev2funcid[key];
			funcArr[func].inprogress--;
		}
		inline void keyDown(int key) {
			int func = ev2funcid[key];
			if (funcArr[func].inprogress == 0) {
				funcArr[func].timestamp = timestamp;
			}
			funcArr[func].inprogress++;
			if (funcArr[func].cbs) 
				(*funcArr[func].cbs)();
		}

		/* Serializes list of events to a buffer */
		int serializeEvMap(unsigned char *buf, int maxlen);

};

#endif	/*	__GC_EVENTMAP_H__	*/

