/*
 * scRamble - 3D Flight Racer
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

#include "Flame.h"

Flame::Flame(float x, float y, float z)
	: Renderable("Flame")
{
	tipwidth = .01f;
	width = .025f;
	length = .3f;
	t = NULL;
	redflame = 1;
	texXStart = 0.22;
	texXRange = 0.02;
	numTris = 12;

	transx = x;
	transy = y;
	transz = z;

	sinarr = new float[numTris+1];
	cosarr = new float[numTris+1];

	for (int i = 0; i <= numTris; i++) {
		sinarr[i] = sin(2*i*PI/(numTris) + PI/2);
		cosarr[i] = cos(2*i*PI/(numTris) + PI/2);
	}
}

int Flame::load(const char *path)
{
	t = new Texture(path);
	t->Load();

	return 0;
}

void Flame::unload(void)
{
	delete t;
	t = NULL;
}

Flame::~Flame()
{
}

void Flame::render(Camera *c)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, t->texid);

	glBegin(GL_TRIANGLES);
	glColor4f(1,1,1,1);
	for (int i = 0; i < numTris; i++) {
		glTexCoord2f( texXStart, 0.9f);
		glVertex3f(transx+width*sinarr[i], transy+width*cosarr[i], transz-length/9);
		glTexCoord2f( texXStart + texXRange, 0.9f);
		glVertex3f(transx+width*sinarr[i+1], transy+width*cosarr[i+1], transz-length/9);
		glTexCoord2f( texXStart + texXRange, 0.8f);
		glVertex3f(transx, transy, transz-length/9);
	}
	glEnd();

	glBegin(GL_QUADS);
	for (int i = 0; i < numTris; i++) {
		glColor4f(1,1,1,1);
		glTexCoord2f( texXStart, 1.0f);
		glVertex3f(transx+width*sinarr[i], transy+width*cosarr[i], transz);
		glColor4f(1,1,1,1);
		glTexCoord2f( texXStart + texXRange, 1.0f);
		glVertex3f(transx+width*sinarr[i+1], transy+width*cosarr[i+1], transz);
		glColor4f(1,1,1,.7);
		glTexCoord2f( texXStart + texXRange, 0.0f);
		glVertex3f(transx+tipwidth*sinarr[i+1], transy+tipwidth*cosarr[i+1], transz-length);
		glColor4f(1,1,1,.7);
		glTexCoord2f( texXStart, 0.0f);
		glVertex3f(transx+tipwidth*sinarr[i], transy+tipwidth*cosarr[i], transz-length);
	}
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

}

