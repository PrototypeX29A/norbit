#include <string>
#include <cstring>

#include "malloc.h"
#include "obj_shape.h"

extern "C" {
#include "object.h"
}

obj_shape::~obj_shape()
{
	glDeleteTextures(1, &TexId);
	free(Normals);
	free(TexCoords);
	free(Vertices);
	free(Faces);
}

obj_shape::obj_shape(string *name, double scale) 
{
	char *cname = (char*) malloc(200);
	strcpy (cname, name->c_str()); 
	if((object = InitObject(cname)) == (Object*)NULL) {

		printf("Cannot load the object!\n");
	}
	set_object(object);
	this->scale = scale;
}


void obj_shape::set_object(Object *obj)
{
	TexId = obj->TexId;
	nFaces = obj->nFaces;
	Faces = obj->Faces;       
	Vertices = obj->Vertices;
	TexCoords = obj->TexCoords;
	Normals = obj->Normals;
}

void obj_shape::draw()
{
	int nf, i;
	Face *face;
	Point *n, *t, *v;

	glScaled(scale, scale, scale);
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
	glScaled(1.0/scale, 1.0/scale, 1.0/scale);	
	glEnd();
}
