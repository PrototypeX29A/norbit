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
	glTranslatef(-posx()*0.2,-posy()*0.2,-posz()*0.2);
	glRotatef(-body->aConfigurations[world->SourceConfigurationIndex].Orientation*57.324840764f, 0.0f,0.0f,1.0f);
		
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
	body->aConfigurations[world->SourceConfigurationIndex].CMPosition = vector_2(r(x),r(y));
}


float game_object::posx(){
	assert(world);
	return body->aConfigurations[world->SourceConfigurationIndex].CMPosition.X;
}

float game_object::posy(){
	assert(world);
	return body->aConfigurations[world->SourceConfigurationIndex].CMPosition.Y;

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



shape::~shape()
{
	glDeleteTextures(1, &TexId);
	free(Normals);
	free(TexCoords);
	free(Vertices);
	free(Faces);
}

shape::shape(string *name)
{
	char *cname = (char*) malloc(200);
	strcpy (cname, name->c_str()); 
	if((object = InitObject(cname)) == (Object*)NULL) {

		printf("Cannot load the object!\n");
	}
	set_object(object);
}


void shape::set_object(Object *obj)
{
	TexId = obj->TexId;
	nFaces = obj->nFaces;
	Faces = obj->Faces;       
	Vertices = obj->Vertices;
	TexCoords = obj->TexCoords;
	Normals = obj->Normals;
}

void shape::draw()
{
	int nf, i;
	Face *face;
	Point *n, *t, *v;
	
	glBindTexture(GL_TEXTURE_2D, TexId);
	glBegin(GL_TRIANGLES);
	for(nf=0; nf < nFaces; nf++) {
		face = &Faces[nf];
		for(i=0; i<3; i++) {
			n = &Normals[face->NorIdx[i]];
			t = &TexCoords[face->TexIdx[i]];
			v = &Vertices[face->VertIdx[i]];
			
			glNormal3f(n->x, n->y, n->z);
			glTexCoord3f(t->x, t->y, t->z);
			glVertex3f(v->x, v->y, v->z);
		}
	}
	glEnd();
}
