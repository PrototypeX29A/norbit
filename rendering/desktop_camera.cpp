/*
 * desktop_camera.cpp
 *
 *  Created on: Jul 11, 2010
 *      Author: peter
 */

#include "desktop_camera.h"
#include "../main.h"

void desktop_camera::apply_rotation()
{

}

void desktop_camera::apply_translation()
{
	glTranslatef(0.0f, 0.0f, -7.5f); /* Negative Zoom */
	vector_2 mid((ship1->posx() +ship2->posx())/2.0,(ship1->posy() +ship2->posy())/2.0);
	float dist = sqrt(pow(ship1->posx() - ship2->posx(),2)+ pow(ship1->posy() - ship2->posy(),2)+pow(ship1->posz() - ship2->posz(),2));
	glTranslatef(mid.X,mid.Y,-dist);
}



void desktop_camera::active(int i)
{
}


