/* GL_shadow: an implementation of the stencil shadow volumes technique
 * Copyright (C) 2005 Angelo "Encelo" Theodorou
 *
 * test_obj.c: OBJ loader test program
 */


#include "object.h"

int main(int argc, char **argv)
{
	Object *obj;
	int nf, i;
	Face *face;
	Point *n, *t, *v;

	if (argc != 2){
		printf("One argument is needed!\n");
		return -1;
	}
	if((obj = InitObject(argv[1])) == NULL){
		printf("Error loading the object!\n");
		return -1;
	}

	printf("Name: %s\n", obj->Name);
	printf("TexId: %u\n", obj->TexId);
	printf("Diffuse Color: <%f, %f, %f, %f>\n", obj->Diffuse[0], obj->Diffuse[1], obj->Diffuse[2], obj->Diffuse[3]);
	printf("Ambient Color: <%f, %f, %f, %f>\n", obj->Ambient[0], obj->Ambient[1], obj->Ambient[2], obj->Ambient[3]);
	printf("Specular Color: <%f, %f, %f, %f>\n", obj->Specular[0], obj->Specular[1], obj->Specular[2], obj->Specular[3]);
	printf("%d faces\n\n", obj->nFaces);

	for(nf=0; nf < obj->nFaces; nf++){
		face = &obj->Faces[nf];
		printf("Face number %d\n", nf);
		printf("Plane equation: %f*x + %f*y + %f*z + %f\n", face->pEq.a, face->pEq.b, face->pEq.c, face->pEq.d);
		for(i=0; i<3; i++){
			n = &obj->Normals[face->NorIdx[i]];
			t = &obj->TexCoords[face->TexIdx[i]];
			v = &obj->Vertices[face->VertIdx[i]];
			
			printf("V: <%f, %f, %f>\n", v->x, v->y, v->z);
			printf("N: <%f, %f, %f>\n", n->x, n->y, n->z);
			printf("T: <%f, %f, %f>\n", t->x, t->y, t->z);
			printf("Neighbour Index: %d\n", face->NeighIdx[i]);
		}
		printf("\n");
	}

	FreeObject(obj);
	return 0;
}
