#include "physics.h"
#include "object.h"
#include "shadow.h"
#include "game_object.h"
#include <GL/gl.h>
#include <assert.h>


simulation_world * game_object::world= NULL;

void game_object::draw()
{
	glPushMatrix();
	glTranslatef(-posx(),-posy(),-posz());
	glScalef(0.2f, 0.02f, 0.02f); 
	DrawObject(object);
	glPopMatrix();
}

void game_object::set_object(Object *obj)
{
	assert(obj);
	object = obj;
}

void game_object::set_position(float x, float y, float z)
{
	assert(body);
	assert(world);
	body->aConfigurations[world->SourceConfigurationIndex].CMPosition = vector_2(r(x),r(y));
}

float game_object::posx(){
	assert(world);
	return body->aConfigurations[world->SourceConfigurationIndex].CMPosition.Y;
}

float game_object::posy(){
	assert(world);
	return body->aConfigurations[world->SourceConfigurationIndex].CMPosition.X;

}

float game_object::posz(){
	return 0.0f;
}

void game_object::set_simulation_world(simulation_world *w){ assert(w); world = w; }

void game_object::set_rigid_body(rigid_body * b){
	assert(world);
	assert(b);
	body = b;
}
