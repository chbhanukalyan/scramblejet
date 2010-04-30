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
	skybox = new SkyBox;
	fighterjet = new StaticModel;
}

RenderingEngine::~RenderingEngine()
{
	delete fighterjet;
	delete skybox;
	delete camera;
}

int RenderingEngine::Initialize(void)
{
	hres = 640;
	vres = 480;
	camera->Initialize();
	skybox->load("../data/skybox/dry/");

	skybox->setSize(1000, 1000, 1000);

	fighterjet->load("../data/staticmodels/F-16.dae");

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
//	gluPerspective(45, 640/480.0, 0.005, 5000.0f);

	// Reset the model view matrix
    glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
    glLoadIdentity();   // Reset The Modelview Matrix
	glOrtho(-5000, 5000, -5000, 5000, -5000, 5000);

	camera->Update();

	glEnable(GL_DEPTH_TEST);

	/* Just clear the depth buffer, color is overwritten anyway */
	glClear(GL_DEPTH_BUFFER_BIT);

	/* Render the objects in order */
//	skybox->render(camera);

	fighterjet->render(camera);

	glFlush();
}

void RenderingEngine::Destroy(void)
{
	skybox->unload();
}

