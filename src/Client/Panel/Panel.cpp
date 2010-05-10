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

	font = new TTFFont("fonts/panelfont.ttf", 12);

	fps = new TextObject(font);

	fps->setText("FPS: 0.0");
	gettimeofday(&lasttmval, NULL);

	renderCount = 0;
}

Panel::~Panel()
{
	delete horizRadarMap;
}

void Panel::RenderTextObject(TextObject *tob, float x, float y, float w, float h)
{
	float x1 = (x);
	float y1 = (y);

	float x2 = x1 + tob->getLen() * (w);
	float y2 = y1 + (h);

	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GEQUAL, 0.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tob->texid);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 0);
		glVertex2f(x2, y2);
		glTexCoord2f(1, 1);
		glVertex2f(x2, y1);
		glTexCoord2f(0, 1);
		glVertex2f(x1, y1);
		glTexCoord2f(0, 0);
		glVertex2f(x1, y2);
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
}

void Panel::render(long curtick, Camera *c)
{
	if (renderCount % 20 == 19) {
		struct timeval newtv;
		char buf[16];
		gettimeofday(&newtv, NULL);
		float f = (newtv.tv_sec - lasttmval.tv_sec) * 1000000 + (newtv.tv_usec - lasttmval.tv_usec);
		sprintf(buf, "FPS:%3.1f", 20.0*1000000/f);
		lasttmval = newtv;
		renderCount = -1;
		fps->setText(buf);
	}
	renderCount++;

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

	/* Render the FPS value */
	RenderTextObject(fps, 750, 950, 25, 25);

	glPopMatrix();
}

