/*
 * gl_viewable.cpp
 *
 *  Created on: Jul 11, 2010
 *      Author: peter
 */

#include "gl_drawable.h"
#include "GL/gl.h"

gl_drawable::gl_drawable(game_object *go, shape *sh)
{
	this->go = go;
	this->sh = sh;
}

void gl_drawable::draw()
{
	glPushMatrix();
	glTranslatef(-go->posx(),-go->posy(),-go->posz());
	glRotatef(go->orientation(), 0.0f,0.0f,1.0f);
	glRotatef(90,1.0f,0.0f,0.0f);
	glScalef(0.2f, 0.2f, 0.2f);
	sh->draw();
	glPopMatrix();
}

