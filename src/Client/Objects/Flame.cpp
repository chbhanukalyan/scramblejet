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
	length = .2f;
	t = NULL;
	redflame = 1;
	texXStart = 0.15;
	texXRange = 0.2;
	numTris = 12;

	transx = x;
	transy = y;
	transz = z;

	sinarr = new float[numTris+1];
	cosarr = new float[numTris+1];

	for (int i = 0; i <= numTris; i++) {
		sinarr[i] = sin(2*i*3.141/(numTris));
		cosarr[i] = cos(2*i*3.141/(numTris));
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
	glColor4f(1,1,1,1);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	
//	glEnable(GL_ALPHA_TEST);
//	glAlphaFunc(GL_GREATER, 0.5);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Front
	glBindTexture(GL_TEXTURE_2D, t->texid);
/*	glBegin(GL_QUADS);
		glTexCoord2f(texXStart + 0, 1.0f); glVertex3f(-length, 0, width);
		glTexCoord2f(texXStart + 0, 0.0f); glVertex3f(-length, 0, -width);
		glTexCoord2f(texXStart + 0.5f, 0.0f); glVertex3f(length, 0, -width);
		glTexCoord2f(texXStart + 0.5f, 1.0f); glVertex3f(length, 0, width);
	glEnd();*/

/*	glBegin(GL_TRIANGLES);
	for (int i = 0; i < numTris; i++) {
		glTexCoord2f(texXStart + 0, 1.0f);
		glVertex3f(width*sinarr[i], width*cosarr[i], 0);
		glTexCoord2f(texXStart + 0.4, 1.0f);
		glVertex3f(width*sinarr[i+1], width*cosarr[i+1], 0);
		glTexCoord2f(texXStart + 0.2, 0.0f);
		glVertex3f(0, 0, -length);
	}
	glEnd();
*/
/*	glBegin(GL_QUADS);
	for (int i = 0; i < numTris; i++) {
		glTexCoord2f(texXStart + 0, 1.0f);
		glVertex3f(width*sinarr[i], width*cosarr[i], 0);
		glTexCoord2f(texXStart + 0.3, 1.0f);
		glVertex3f(width*sinarr[i+1], width*cosarr[i+1], 0);
		glTexCoord2f(texXStart + 0.3, 0.0f);
		glVertex3f(tipwidth*sinarr[i+1], tipwidth*cosarr[i+1], -length);
		glTexCoord2f(texXStart + 0, 0.0f);
		glVertex3f(tipwidth*sinarr[i], tipwidth*cosarr[i], -length);
	}
	glEnd();*/
	glBegin(GL_QUADS);
	for (int i = 0; i < numTris; i++) {
		glTexCoord2f( texXStart + texXRange * ((float)i)/numTris, 1.0f);
		glVertex3f(transx+width*sinarr[i], transy+width*cosarr[i], transz);
		glTexCoord2f( texXStart + texXRange * ((float)i+1)/numTris, 1.0f);
		glVertex3f(transx+width*sinarr[i+1], transy+width*cosarr[i+1], transz);
		glTexCoord2f( texXStart + texXRange * ((float)i+1)/numTris, 0.0f);
		glVertex3f(transx+tipwidth*sinarr[i+1], transy+tipwidth*cosarr[i+1], transz-length);
		glTexCoord2f( texXStart + texXRange * ((float)i)/numTris, 0.0f);
		glVertex3f(transx+tipwidth*sinarr[i], transy+tipwidth*cosarr[i], transz-length);
	}
	glEnd();


	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

}

