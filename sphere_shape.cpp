#include <string>
#include <cstring>

#include "malloc.h"
#include "sphere_shape.h"
#include <GL/glu.h>
#include <GL/gl.h>

sphere_shape::~sphere_shape()
{
}

sphere_shape::sphere_shape(float size) : shape()
{	
	this->size = size;
	this->red = 1.0f;
	this->green = 1.0f;	
	this->blue = 0.0f;
}

sphere_shape::sphere_shape(float size, float red, float green, float blue) : shape()
{	
	this->size = size;
	this->red = red;
	this->blue = blue;
	this->green = green;
}


void sphere_shape::draw()
{
	GLUquadricObj *sphere=gluNewQuadric();	
	glColor3f(red,green,blue);
	gluSphere(sphere, size, 10, 10);
}


