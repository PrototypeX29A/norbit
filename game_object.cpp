#include "malloc.h"
#include "string.h"
#include "physics.h"
#include "object.h"
#include "shadow.h"
#include "game_object.h"
#include <GL/gl.h>
#include <assert.h>


simulation_world * game_object::world= NULL;

using namespace std;


void game_object::draw()
{
	glPushMatrix();
	glTranslatef(-posx(),-posy(),-posz());
	glRotatef(body->aConfigurations[body->SourceConfigurationIndex].Orientation*180.0/PI, 0.0f,0.0f,1.0f);
	glRotatef(90,1.0f,0.0f,0.0f);	
	glScalef(0.2f, 0.2f, 0.2f); 
	object_shape->draw();
	glPopMatrix();
}




void game_object::set_shape(shape *s)
{
	object_shape = s;
}

void game_object::set_position(float x, float y, float z)
{
	assert(body);
	assert(world);
	body->aConfigurations[body->SourceConfigurationIndex].CMPosition = vector_2(r(x),r(y));
}


float game_object::posx(){
	assert(world);
	return body->aConfigurations[body->SourceConfigurationIndex].CMPosition.X;
}

float game_object::posy(){
	assert(world);
	return body->aConfigurations[body->SourceConfigurationIndex].CMPosition.Y;

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


// specified in local coords
void game_object::apply_force(vector_2 const &F, vector_2 const &Pl){
	assert(body);
	body->apply_force(F, Pl);
}

//apply force, specified in global coords
void game_object::apply_force_G(vector_2 const &F, vector_2 const &Pl){
	assert(body);
	body->apply_force_G(F, Pl);
}

float game_object::mass(){
	return body->Mass;
}

shape::shape()
{
}

shape::~shape()
{
	glDeleteTextures(1, &TexId);
	free(Normals);
	free(TexCoords);
	free(Vertices);
	free(Faces);
}

