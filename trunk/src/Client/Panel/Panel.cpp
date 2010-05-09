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

#include "Panel.h"

#include <GL/gl.h>
#include <GL/glu.h>

Panel::Panel(void)
{
	displayPanel = true;
	objList = NULL;
	horizRadarMap = new RadarMap(RADARMAP_TYPE_HORIZONTAL);
	horizRadarMap->placeRadarMap(850, 150);
	vertRadarMap = new RadarMap(RADARMAP_TYPE_VERTICAL);
	vertRadarMap->placeRadarMap(150, 150);
}

Panel::~Panel()
{
	delete horizRadarMap;
}

void Panel::render(long curtick, Camera *c)
{
	glDisable(GL_DEPTH_TEST);
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glLineWidth(20);
	glDisable(GL_TEXTURE_2D);
	glColor3f(1,1,1);
	glBegin(GL_LINES);
		glVertex2f(-10,-10);
		glVertex2f(10,10);
	glEnd();

	vertRadarMap->render(curtick, c, objList);
	horizRadarMap->render(curtick, c, objList);

	glPopMatrix();
}

