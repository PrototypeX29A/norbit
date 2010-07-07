/* GL_shadow: an implementation of the stencil shadow volume technique
 * Copyright (C) 2005 Angelo "Encelo" Theodorou
 *
 * shadow.c: shadow and stencil related functions
 */


#include <GL/gl.h>
#include "object.h"
#include "shadow.h"


/* Draw the shadow of the object relative to a light in the stencil buffer */
void CastShadow(Object *obj, GLfloat lightpos[4])
{
	int nf;
	Plane *plane;
	GLfloat side;

	
/* ----- Caculating face visibility ----- */
	for(nf=0; nf < obj->nFaces; nf++){
		plane = &obj->Faces[nf].pEq;

		/* Calculating plane/light  distance*/
		side = plane->a * lightpos[0] +
			plane->b * lightpos[1] +
			plane->c * lightpos[2] +
			plane->d;

		if (side > 0)
			obj->Faces[nf].Visible = 1; /* True */
		else
			obj->Faces[nf].Visible = 0; /* False */
	}


/* ----- Performing stencil shadow volumes algorithm  ----- */
	glPushAttrib( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT
		| GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT );

	glDisable(GL_LIGHTING);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); /* No output to the color buffer */

	glDepthMask(GL_FALSE); /* Prevent writing in the depth buffer */
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_STENCIL_TEST); /* Enabling stencil test */
	glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFFL);

	glEnable(GL_CULL_FACE);
	/* First Pass - Increase stencil value in the shadow */
	glCullFace(GL_BACK); /* or glFrontFace(GL_CCW) */
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	ShadowVolume(obj, lightpos);
	/* Second Pass. Decrease stencil value outside the shadow */
	glCullFace(GL_FRONT); /* or glFrontFace(GL_CW) */
	glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
	ShadowVolume(obj, lightpos);

	glPopAttrib();
}


/* Detects the silhouette edge of an object and project it towards infinity 
 * in the direction of object-light vector */
void ShadowVolume(Object *obj, GLfloat lightpos[4])
{
	int nf, i;
	Face *face;
	int neighbour;
	Point *v1, *v2, v3, v4; /* The four vertices of the extruded edge */

	
/* ----- Silhouette Edge ----- */	
	for (nf=0; nf < obj->nFaces; nf++ ){
		face = &obj->Faces[nf];
		if(face->Visible){
			/* Extruding the silhouette */
			for (i=0; i<3; i++){
				neighbour = face->NeighIdx[i];
				/* If there's no neighbour or its neighbouring face is invisible,
				then this edge is part of the silhouette edge */
				if (neighbour == -1 || !obj->Faces[neighbour].Visible){
					v1 = &obj->Vertices[face->VertIdx[i]];
					v2 = &obj->Vertices[face->VertIdx[(i+1)%3]];
					/* Projecting this edge to "infinity" */
					v3.x = (v1->x - lightpos[0]) * INF;
					v3.y = (v1->y - lightpos[1]) * INF;
					v3.z = (v1->z - lightpos[2]) * INF;
	 				v4.x = (v2->x - lightpos[0]) * INF;
					v4.y = (v2->y - lightpos[1]) * INF;
					v4.z = (v2->z - lightpos[2]) * INF;

					glBegin(GL_TRIANGLE_STRIP);
					glVertex3f(v1->x, v1->y, v1->z);
					glVertex3f(v1->x + v3.x, v1->y + v3.y, v1->z + v3.z);
					/* Uncomment it for "real" infinity projection */
					/* glVertex4f(v1->x + v3.x, v1->y + v3.y, v1->z + v3.z, 0.0f); */
					glVertex3f(v2->x, v2->y, v2->z);
					glVertex3f(v2->x + v4.x, v2->y + v4.y, v2->z + v4.z);
					/* Uncomment it for "real" infinity projection */
					/* glVertex4f(v2->x + v4.x, v2->y + v4.y, v2->z + v4.z, 0.0f); */
					glEnd();
				}
			}
		}
	}
}


/* Draws a shadowing rectangle covering the entire screen */	
void DrawShadow(void)
{
	glPushAttrib( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT
		| GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT );

	glDisable(GL_LIGHTING);

	glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 0, 0xFFFFFFFFL);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(-0.1f, 0.1f,-0.1f);
		glVertex3f(-0.1f,-0.1f,-0.1f);
		glVertex3f( 0.1f, 0.1f,-0.1f);
		glVertex3f( 0.1f,-0.1f,-0.1f);
	glEnd();
	glPopMatrix();
	
	glPopAttrib();
}
