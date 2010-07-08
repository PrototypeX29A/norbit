#include "physics.h"
#include "object.h"
#include "shadow.h"
#include "game_object.h"
#include <GL/gl.h>

void game_object::draw()
{
	glPushMatrix();
	glTranslate(-posx,-posy,-posz);
	glScalef(0.2f, 0.2f, 0.2f); 
	DrawObject(object);
//	CastShadow(object, lp1_o2);
//	CastShadow(object, lp2_o2);
	glPopMatrix();
}

void game_object::set_object(Object *obj)
{
	object = obj;
}

void game_object::set_position(float x, float y, float z)
{
	posx = x; 
	posy = y;
	posz = z;
}