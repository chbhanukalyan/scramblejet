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


#ifndef		__RE_RENDERINGENGINE_H__
#define		__RE_RENDERINGENGINE_H__

#include "Renderable.h"
#include "Terrain.h"
#include "../Panel/Panel.h"

class RenderingEngine {
	private:
		Camera *camera;

		int hres;
		int vres;

		Renderable *renderList;
		Terrain *terrain;
		Panel *panel;

	public:
		RenderingEngine(void);
		~RenderingEngine();

		int Initialize(CamPos *initCamPos);
		void render(CamPos *followCam, long curTicks);
		void Destroy(void);

		void addPanel(Panel *p) {
			panel = p;
		}
		void addObject(Renderable *r);
		void removeObject(Renderable *r);
		void clearRenderList(void);

};

#endif	/*	__RE_RENDERINGENGINE_H__	*/
