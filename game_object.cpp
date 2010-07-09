#include "physics.h"
#include "object.h"
#include "shadow.h"
#include "game_object.h"
#include <GL/gl.h>


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

void shape::set_object(Object *obj)
{
	object = obj;
}

void shape::draw()
{
	int nf, i;
	Face *face;
	Point *n, *t, *v;
	
	glBindTexture(GL_TEXTURE_2D, object->TexId);
	glBegin(GL_TRIANGLES);
	for(nf=0; nf < object->nFaces; nf++) {
		face = &object->Faces[nf];
		for(i=0; i<3; i++) {
			n = &object->Normals[face->NorIdx[i]];
			t = &object->TexCoords[face->TexIdx[i]];
			v = &object->Vertices[face->VertIdx[i]];
			
			glNormal3f(n->x, n->y, n->z);
			glTexCoord3f(t->x, t->y, t->z);
			glVertex3f(v->x, v->y, v->z);
		}
	}
	glEnd();
}
