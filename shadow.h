/* GL_shadow: an implementation of the stencil shadow volume technique
 * Copyright (C) 2005 Angelo "Encelo" Theodorou
 */


#include "extra.h"

#define INF 10 /* What to consider "infinity" for silhouette edge projection */


void CastShadow(Object *obj, GLfloat lightpos[4]);
void DrawShadow(void);
void ShadowVolume(Object *obj, GLfloat lightpos[4]);
