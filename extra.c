/* GL_shadow: an implementation of the stencil shadow volume technique
 * Copyright (C) 2005 Angelo "Encelo" Theodorou
 *
 * extra.c: general and light relative functions
 */    

#include <math.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include "extra.h"
#include "object.h"


/* Calculate frame interval and print FPS each 5s */
int FrameTiming(void)
{
	Uint32 interval;
	static Uint32 current, last = 0, five = 0, nframes = 0;

	current = SDL_GetTicks();
	nframes++;

	if (current - five > 5*1000) {
		printf("%u frames in 5 seconds = %.1f FPS\n", nframes, (float)nframes/5.0f);
		nframes = 0;
		five = current;
	}
	
	interval = current - last;
	last = current;
	
	return interval;
}


/* Replaces gluPerspective. Sets the frustum to perspective mode.
 * fovY     - Field of vision in degrees in the y direction
 * aspect   - Aspect ratio of the viewport
 * zNear    - The near clipping distance
 * zFar     - The far clipping distance
 */
void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	GLdouble fW, fH;

	fH = tan( (fovY / 2) / 180 * M_PIl ) * zNear;
	fW = fH * aspect;

	glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}


/* Draws the walls on which objects' shadow will be projected */
void DrawRoom(void)
{
	glPushAttrib(GL_TEXTURE_BIT);
	
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.6f, 0.6f, 0.6f);
	glBegin(GL_QUADS);
		/* Floor */
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f,-1.0f,-2.0f);
		glVertex3f(-1.0f,-1.0f, 2.0f);
		glVertex3f( 1.0f,-1.0f, 2.0f);
		glVertex3f( 1.0f,-1.0f,-2.0f);
		/* Ceiling */
		glNormal3f(0.0f,-1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 2.0f);
		glVertex3f(-1.0f, 1.0f,-2.0f);
		glVertex3f( 1.0f, 1.0f,-2.0f);
		glVertex3f( 1.0f, 1.0f, 2.0f);
		/* Back Wall */
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, -2.0f);
		glVertex3f(-1.0f,-1.0f, -2.0f);
		glVertex3f( 1.0f,-1.0f, -2.0f);
		glVertex3f( 1.0f, 1.0f, -2.0f);
		/* Left Wall */
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 2.0f);
		glVertex3f(-1.0f,-1.0f, 2.0f);
		glVertex3f(-1.0f,-1.0f,-2.0f);
		glVertex3f(-1.0f, 1.0f,-2.0f);
		/* Right Wall */
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f( 1.0f, 1.0f,-2.0f);
		glVertex3f( 1.0f,-1.0f,-2.0f);
		glVertex3f( 1.0f,-1.0f, 2.0f);
		glVertex3f( 1.0f, 1.0f, 2.0f);
	glEnd();
	
	glPopAttrib();
}


/* Updates light position in the OpenGL state machine and then draws the light */
void UpdateLight(Light *light, GLint gl_light, GLfloat size)
{
	GLfloat x, y, z;

	x = light->Position[0];
	y = light->Position[1];
	z = light->Position[2];
		
	glLightfv(gl_light, GL_POSITION, light->Position);
	glLightfv(gl_light, GL_AMBIENT, light->Ambient);
	glLightfv(gl_light, GL_DIFFUSE, light->Diffuse);
	glLightfv(gl_light, GL_SPECULAR, light->Specular);

	glPushAttrib(GL_ENABLE_BIT);
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glColor3f(light->Diffuse[0], light->Diffuse[1], light->Diffuse[2]);
	glBegin(GL_QUADS);
		glVertex3f(x+(0.5f*size), y+(0.5f*size), z);
		glVertex3f(x-(0.5f*size), y+(0.5f*size), z);
		glVertex3f(x-(0.5f*size), y-(0.5f*size), z);
		glVertex3f(x+(0.5f*size), y-(0.5f*size), z);
	glEnd();
	
	glPopAttrib();
}


/* Vector per matrix multiplication */
void VMatMult(GLfloat M[16], GLfloat v[4])
{
	GLfloat res[4];

	res[0] = M[0] * v[0] + M[4] * v[1] + M[8] *v[2] +M[12] * v[3];
	res[1] = M[1] * v[0] + M[5] * v[1] + M[9] *v[2] +M[13] * v[3];
	res[2] = M[2] * v[0] + M[6] * v[1] + M[10] *v[2] +M[14] * v[3];
	res[3] = M[3] * v[0] + M[7] * v[1] + M[11] *v[2] +M[15] * v[3];

	/* Results Are Stored Back In v[] */	
	v[0] = res[0];
	v[1] = res[1];
	v[2] = res[2];
	v[3] = res[3];
}


/* Vector copy function */
void CpVec(GLfloat *src, GLfloat *dst, int size)
{
	int i;
	
	for(i=0; i<size; i++)
		dst[i] = src[i];
}
