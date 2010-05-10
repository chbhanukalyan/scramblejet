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


#ifndef		__PANEL_PANEL_H__
#define		__PANEL_PANEL_H__

#include <cassert>
#include <sys/time.h>

#include "../RenderingEngine/TTFFont.h"
#include "../RenderingEngine/TextObject.h"
#include "../RenderingEngine/Camera.h"
#include "../Objects/IndObj.h"
#include "RadarMap.h"

class Panel {
	private:
		bool displayPanel;
		RadarMap *horizRadarMap;
		RadarMap *vertRadarMap;

		IndObj *objList;

		TTFFont *font;

		/* FPS calculation */
		TextObject *fps;
		int renderCount;
		struct timeval lasttmval;

		void RenderTextObject(TextObject *tob, float x, float y, float w, float h);

	public:
		Panel(void);
		~Panel();

		inline void showPanel(bool show) {
			this->displayPanel = show;
		}

		void render(long curtick, Camera *c);

		void addObject(IndObj *obj) {
			assert(obj);
			obj->inext = objList;
			objList = obj;
		}
		void removeObject(IndObj *obj) {
			assert(obj);
			IndObj *tmp = objList;
			while (tmp) {
				if (tmp->inext == obj) {
					tmp->inext = tmp->inext->inext;
					break;
				}
				tmp = tmp->inext;
			}
		}


};

#endif	/*	__PANEL_PANEL_H__	*/

