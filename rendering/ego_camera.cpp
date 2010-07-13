/*
 * ego_camera.cpp
 *
 *  Created on: Jul 12, 2010
 *      Author: peter
 */

#include "ego_camera.h"
#include "../main.h"

void ego_camera::apply_rotation()
{
	//glRotatef(180.0f, 0.0, 1.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-ship1->orientation(), 0.0f,0.0f,1.0f);
}

void ego_camera::apply_translation()
{
	glRotatef(ship1->orientation(), 0.0f,0.0f,1.0f);
	glTranslatef(0.0f, -0.4f, -0.15f);
	glRotatef(-ship1->orientation(), 0.0f,0.0f,1.0f);
 	glTranslatef(ship1->posx(), ship1->posy(), ship1->posz());
}

void ego_camera::active(int i)
{
}
