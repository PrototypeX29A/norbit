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
	glRotated(go->orientation(), 0.0,0.0,1.0);
	glRotated(90,1.0,0.0,0.0);
	glScaled(0.2, 0.2, 0.2);
	sh->draw();
	glPopMatrix();
}

