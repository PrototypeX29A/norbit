/* GL_shadow: an implementation of the stencil shadow volume technique
 * Copyright (C) 2005 Angelo "Encelo" Theodorou
 */   

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <stdio.h>
#include <GL/gl.h>


typedef struct Point {
	GLfloat x, y, z;     /* Point coordinates */
} Point;

typedef struct Plane {
	GLfloat a, b, c, d;  /* ax + by + cz + d = 0 */
} Plane;

/* Triangular face */
typedef struct Face {
	int VertIdx[3];      /* Vertex indices */
	int TexIdx[3];       /* Texture coordinates indices */
	int NorIdx[3];       /* Normal vectors indices */
	Plane pEq;           /* Equation of the plane on which the triangle is laying */
	int NeighIdx[3];     /* Indices of the faces neighbouring this one */
	int Visible;         /* Is this face visible by the light? */
} Face;

typedef struct Object {
	char *Name;
	GLuint TexId;
	GLfloat Diffuse[4];
	GLfloat Ambient[4];
	GLfloat Specular[4];
	int nFaces;
	Face *Faces;          /* Array of faces */
	int nVertices;
	Point *Vertices;       /* Array of vertex coordinates */
	Point *TexCoords;     /* Array of texture coordinates */
	Point *Normals;       /* Array of normal vectors */
} Object;


Object *LoadObject(char *filename);
GLuint LoadTexture(char *texname);
void SetConnectivity(Object *obj);
void CalculatePlane(Object *obj, int nf);
Object *InitObject(char *objfile);
void FreeObject(Object *obj);
void DrawObject(Object *obj);

#endif
