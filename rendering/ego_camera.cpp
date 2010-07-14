/*
 * ego_camera.cpp
 *
 *  Created on: Jul 12, 2010
 *      Author: peter
 */

#include "ego_camera.h"
#include "../main.h"

ego_camera::ego_camera(game_object *go, float x, float y, float z)
{
	this->go = go;
	offsetx = x;
	offsety = y;
	offsetz = z;
}

void ego_camera::apply_rotation()
{
	//glRotatef(180.0f, 0.0, 1.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-go->orientation(), 0.0f,0.0f,1.0f);
}

void ego_camera::apply_translation()
{
	glRotatef(go->orientation(), 0.0f,0.0f,1.0f);
	glTranslatef(offsetx, offsety, offsetz);
	glRotatef(-go->orientation(), 0.0f,0.0f,1.0f);
 	glTranslatef(go->posx(), go->posy(), go->posz());
}

void ego_camera::active(int i)
{
}
