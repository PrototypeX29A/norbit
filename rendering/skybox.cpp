/*
 * skybox.cpp
 *
 *  Created on: Jul 13, 2010
 *      Author: peter
 */

#include "skybox.h"
extern "C" {
#include <SDL/SDL.h>
}
#include <iostream>


GLuint skybox::load_texture(const char *filename)
{
	GLuint tex;
    SDL_Surface* bmpFile = SDL_LoadBMP(filename);
    if (bmpFile == NULL) std::cout << "BMP not found\n";
    glPixelStorei(GL_UNPACK_ALIGNMENT,4);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D,tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D,3,bmpFile->w,
	   bmpFile->h,GL_LUMINANCE, //GL_BGR_EXT,
	   GL_UNSIGNED_BYTE,bmpFile->pixels);
    //Free surface after using it
    SDL_FreeSurface(bmpFile);
    return tex;
}

void skybox::init(double size)
{
	this->boxsize = size;
    north = load_texture("rendering/textures/north.bmp");
    east = load_texture("rendering/textures/east.bmp");
    south = load_texture("rendering/textures/south.bmp");
    west = load_texture("rendering/textures/west.bmp");
    up = load_texture("rendering/textures/up.bmp");
    down = load_texture("rendering/textures/down.bmp");
}

void skybox::draw()
{

// replace the quad colours with the texture
/*	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

*/
	glColor3f(1.0f,0.5f,0.3f);
	// SKYBOX UP
	///textureSkyUP.enable();
	//textureSkyUP.bind();
	glEnable(GL_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, up);
	glColor3f(1.0f, 0.2f, 0.2f);
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	glTexCoord2f(1,1);
	glVertex3f(-boxsize, +boxsize, -boxsize);
	glTexCoord2f(1,0);
	glVertex3f(-boxsize, +boxsize, +boxsize);
	glTexCoord2f(0,0);
	glVertex3f(+boxsize, +boxsize, +boxsize);
	glTexCoord2f(0,1);
	glVertex3f(+boxsize, +boxsize, -boxsize);
	glEnd();
	/*textureSkyUP.disable();

	// SKYBOX WEST
	textureSkyWEST.enable();
	textureSkyWEST.bind(); */
	glBindTexture(GL_TEXTURE_2D, west);
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(1,1);
	glVertex3f(-boxsize, -boxsize, +boxsize);
	glTexCoord2f(1,0);
	glVertex3f(-boxsize, +boxsize, +boxsize);
	glTexCoord2f(0,0);
	glVertex3f(-boxsize, +boxsize, -boxsize);
	glTexCoord2f(0,1);
	glVertex3f(-boxsize, -boxsize, -boxsize);
	glEnd();
	/*textureSkyWEST.disable();

	// SKYBOX NORTH
	textureSkyNORTH.enable();
	textureSkyNORTH.bind();*/
	glBindTexture(GL_TEXTURE_2D, north);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1,1);
	glVertex3f(-boxsize, -boxsize, -boxsize);
	glTexCoord2f(1,0);
	glVertex3f(-boxsize, +boxsize, -boxsize);
	glTexCoord2f(0,0);
	glVertex3f(+boxsize, +boxsize, -boxsize);
	glTexCoord2f(0,1);
	glVertex3f(+boxsize, -boxsize, -boxsize);
	glEnd();
	//textureSkyNORTH.disable();

	/*
	// SKYBOX EAST
	textureSkyEAST.enable();
	textureSkyEAST.bind();*/
	glBindTexture(GL_TEXTURE_2D, east);
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	glTexCoord2f(1,1);
	glVertex3f(+boxsize, -boxsize, -boxsize);
	glTexCoord2f(1,0);
	glVertex3f(+boxsize, +boxsize, -boxsize);
	glTexCoord2f(0,0);
	glVertex3f(+boxsize, +boxsize, +boxsize);
	glTexCoord2f(0,1);
	glVertex3f(+boxsize, -boxsize, +boxsize);
	glEnd();
	/*textureSkyEAST.disable();

	// SKYBOX SOUTH
	textureSkySOUTH.enable();
	textureSkySOUTH.bind(); */
	glBindTexture(GL_TEXTURE_2D, south);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(1,1);
	glVertex3f(+boxsize, -boxsize, +boxsize);
	glTexCoord2f(1,0);
	glVertex3f(+boxsize, +boxsize, +boxsize);
	glTexCoord2f(0,0);
	glVertex3f(-boxsize, +boxsize, +boxsize);
	glTexCoord2f(0,1);
	glVertex3f(-boxsize, -boxsize, +boxsize);
	glEnd();
	/*textureSkySOUTH.disable();

	// SKYBOX DOWN
	textureSkyDOWN.enable();
	textureSkyDOWN.bind();*/
	glBindTexture(GL_TEXTURE_2D, down);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(1,1);
	glVertex3f(-boxsize, -boxsize, +boxsize);
	glTexCoord2f(1,0);
	glVertex3f(-boxsize, -boxsize, -boxsize);
	glTexCoord2f(0,0);
	glVertex3f(+boxsize, -boxsize, -boxsize);
	glTexCoord2f(0,1);
	glVertex3f(+boxsize, -boxsize, +boxsize);
	glEnd();
	/*textureSkyDOWN.disable();*/
}



