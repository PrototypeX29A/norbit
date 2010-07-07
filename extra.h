/* GL_shadow: an implementation of the stencil shadow volume technique
 * Copyright (C) 2005 Angelo "Encelo" Theodorou
 */

#ifndef _EXTRA_H_
#define _EXTRA_H_

#ifndef M_PIl
    #define M_PIl 3.1415926535897932384626433832795029L
#endif


typedef struct Light {
	GLfloat Position[4];
	GLfloat Ambient[4];
	GLfloat Diffuse[4];
	GLfloat Specular[4];
} Light;


int FrameTiming(void);
void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
void DrawRoom(void);
void UpdateLight(Light *light, GLint gl_light, GLfloat size);
void VMatMult(GLfloat M[16], GLfloat v[4]);
void CpVec(GLfloat *src, GLfloat *dst, int size);

#endif
