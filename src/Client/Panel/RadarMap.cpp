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

#include "math.h"
#include "RadarMap.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>

RadarMap::RadarMap(int displayType)
{
	this->displayType = displayType;
	startAngle = rand()%6;
	angle = 0;
	size = 100;

	bg = new Texture("textures/radarbg.png");
	bg->Load();

	scalex = scaley = scalez = 1;
}

RadarMap::~RadarMap()
{
	bg->Unload();
	delete bg;
}

bool RadarMap::scalevec3d(Camera *c, Vec3D *v3, Vec2D *v)
{
	v3->x -= c->pointx;
	v3->y -= c->pointy;
	v3->z -= c->pointz;
	if (displayType == RADARMAP_TYPE_HORIZONTAL) {
		v->x = -v3->x / scalex;
		v->y = v3->z / scalez;
	} else {
		v->x = -v3->x / scalex;
		v->y = v3->y / scaley;
	}
	if (size*size <= v->magnitude())
		return false;
	return true;
}

void RadarMap::render(long curtick, Camera *c, IndObj *iolist)
{
	angle = startAngle + curtick/40.0f;

	glPushMatrix();
	glTranslatef(cx, cy, 0);

	if (displayType == RADARMAP_TYPE_HORIZONTAL) {
		glPushMatrix();
		glRotatef(-c->angle * 57.296 + 180, 0, 0, 1);
	}

	/* Draw the Radar Screen */
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glColor4f(1, 1, 1, 0.7);
	glBindTexture(GL_TEXTURE_2D, bg->texid);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex2f(-size, - size);
		glTexCoord2f(1, 0);
		glVertex2f(+ size, - size);
		glTexCoord2f(1, 1);
		glVertex2f(+ size, + size);
		glTexCoord2f(0, 1);
		glVertex2f(- size, + size);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	/* Draw the reducing glow of the rotating line */
	glEnable(GL_BLEND);
	glColor4f(0, 1, 0, 1);
	glLineWidth(1);
	glBegin(GL_TRIANGLES);
		glColor4f(0, 1, 0, 1);
		glVertex2f(+ size*sin(angle),  + size*cos(angle));
		glColor4f(0, 1, 0, 0);
		glVertex2f(+ size*sin(angle-0.4f), + size*cos(angle-0.4f));
		glColor4f(0, 1, 0, 1);
		glVertex2f(0, 0);
	glEnd();
	glDisable(GL_BLEND);

	/* Draw the rotating line */
	glColor4f(0, 1, 0, 1);
	glLineWidth(1);
	glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(size*sin(angle), size*cos(angle));
	glEnd();

	/* Draw the points */
	glColor4f(0, 1, 0, 1);
	glPointSize(3);
	IndObj *io = iolist;
	glBegin(GL_POINTS);
	while (io) {
		Vec3D v3;
		Vec2D v;
		io->getPos(&v3);
		if (scalevec3d(c, &v3, &v))
			glVertex2f(v.x, v.y);
		io = io->inext;
	}
	glEnd();

	if (displayType == RADARMAP_TYPE_HORIZONTAL)
		glPopMatrix();
	glPopMatrix();
}


