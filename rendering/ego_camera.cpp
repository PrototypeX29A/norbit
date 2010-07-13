/*
 * ego_camera.cpp
 *
 *  Created on: Jul 12, 2010
 *      Author: peter
 */

#include "ego_camera.h"
#include "../main.h"

void ego_camera::apply()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f, 0.2f, 0.0f);
//	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-ship1->orientation(), 0.0f,0.0f,1.0f);
	glTranslatef(ship1->posx(), ship1->posy(), ship1->posz());
}



void ego_camera::active(int i)
{
}
