#include "malloc.h"
#include "string.h"
#include "physics.h"
#include "object.h"
#include "shadow.h"
#include "game_object.h"
#include <GL/gl.h>

using namespace std;


void game_object::draw()
{
	glPushMatrix();
	glTranslatef(-posx,-posy,-posz);
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
	posx = x; 
	posy = y;
	posz = z;
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
