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
}

RenderingEngine::~RenderingEngine()
{
	delete camera;
}

int RenderingEngine::Initialize(void)
{
	hres = 640;
	vres = 480;
	camera->Initialize();

	return 0;
}

void RenderingEngine::render(void)
{
	static float a = 0;
//	camera->Rotate(a);
//	camera->Rotate(2.5);
	a+= 0.01;
	// Set the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(45, 640/480.0, 0.005, 5000.0f);

	// Reset the model view matrix
    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
    glLoadIdentity();   // Reset The Modelview Matrix
//	glOrtho(-5000, 5000, -5000, 5000, -5000, 5000);

	camera->Update();

	glEnable(GL_DEPTH_TEST);

	/* Just clear the depth buffer, color is overwritten anyway */
	glClear(GL_DEPTH_BUFFER_BIT);

#if 0 //Enable Fog
	glEnable(GL_FOG);
	{
		GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};

		int	fogMode = GL_EXP;
		glFogi (GL_FOG_MODE, fogMode);
		glFogfv (GL_FOG_COLOR, fogColor);
		glFogf (GL_FOG_DENSITY, 0.35);
		glHint (GL_FOG_HINT, GL_DONT_CARE);
		glFogf (GL_FOG_START, 1.0);
		glFogf (GL_FOG_END, 5.0);
	}
	glClearColor(0.5, 0.5, 0.5, 1.0);  /* fog color */
#endif

	/* Render the objects in order */
	Renderable *renobj = renderList;
	while (renobj) {
		renobj->render(camera);
		renobj = renobj->next;
	}

	glFlush();
}

void RenderingEngine::Destroy(void)
{
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

