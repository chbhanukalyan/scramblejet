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

#include "RenderingEngine.h"

RenderingEngine::RenderingEngine(void)
{
	camera = new Camera;
	renderList = NULL;
	terrain = new Terrain;
	skybox = new SkyBox;
	memset(modelList, 0, sizeof(modelList));

	disableFog = true;
}

RenderingEngine::~RenderingEngine()
{
	delete skybox;
	delete terrain;
	delete camera;
}

int RenderingEngine::Initialize(Map *map)
{
	hres = 640;
	vres = 480;
	camera->Initialize(&map->initCamPos);

	terrain->load(map->terrainfn);
	skybox->load(map->skyboxfn);
	skybox->setSize(map->sizex, map->sizey, map->sizez);

	camera->dumpCurPos();

	return 0;
}

void RenderingEngine::loadModels(void)
{
	/* Load list of models */
	for (int i = 0; i < MAX_MODELID; i++) {
		if (getModeFileFromID(i)) {
			modelList[i] = new StaticModel(getModeFileFromID(i));
			modelList[i]->load(getModeFileFromID(i));
		}
	}
}

void RenderingEngine::renderFog(void)
{
	if (disableFog)
		return;
	
	glClearColor(0.5, 0.5, 0.5, 1.0);  /* fog color */
	glEnable(GL_FOG);
	{
		GLfloat fogColor[4] = {1, 1, 1, 1};

		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogfv (GL_FOG_COLOR, fogColor);
//		glHint (GL_FOG_HINT, GL_NICEST);
		glHint (GL_FOG_HINT, GL_DONT_CARE);
		glFogf (GL_FOG_START, 300.0);
		glFogf (GL_FOG_END, 1000.0);
	}
}

void RenderingEngine::render(CamPos *cp, long curTicks)
{
	camera->setView();

	camera->Update(cp);

	glEnable(GL_DEPTH_TEST);
	/* Just clear the depth buffer, color is overwritten anyway */
	glClear(GL_DEPTH_BUFFER_BIT);

	skybox->render(camera);

	renderFog();

	terrain->render(camera);

	/* Render the objects in order */
	Renderable *renobj = renderList;
	while (renobj) {
		renobj->render(camera);
		renobj = renobj->next;
	}

	/* This should be the last element as we disable depth buffer while drawing this */
	panel->render(curTicks, camera);

	glFlush();
}

void RenderingEngine::Destroy(void)
{
	terrain->unload();
}

void RenderingEngine::addObject(Renderable *r)
{
	r->next = renderList;
	renderList = r;
}

void RenderingEngine::removeObject(Renderable *r)
{
	if (renderList == NULL)
		return;

	if (renderList == r) {
		renderList = r->next;
		r->next = NULL;
		return;
	}

	Renderable *tmp = renderList;
	while (tmp != NULL) {
		if (tmp->next == r) {
			tmp->next = tmp->next->next;
			r->next = NULL;
			return;
		}
		tmp = tmp->next;
	}
	fprintf(stderr, "WARNING: Trying to remove Renderable Object: %s "
			"not in list\n", r->id);
}

void RenderingEngine::clearRenderList(void)
{
	Renderable *tmp = renderList;
	renderList = NULL;

	while (tmp != NULL) {
		Renderable *r = tmp->next;
		delete tmp;
		tmp = r;
	}
}

