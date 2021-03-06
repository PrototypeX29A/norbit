/*
 * gl_renderer.cpp
 *
 *  Created on: Jul 11, 2010
 *      Author: peter
 */

#include "gl_renderer.h"

#include <iostream>
#include <list>
#include <string>

#include "desktop_camera.h"
#include "ego_camera.h"
#include "obj_shape.h"
#include "sphere_shape.h"
#include "../control.h"
#include "../main.h"
#include "../units.h"

extern "C" {
#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "extra.h"
#include "../physics.h"
}

#define SHIPSCALE 0.1 

gl_renderer::gl_renderer() {
	drawables = new list<gl_drawable*>();
}

void gl_renderer::add_ship(game_object *go, int cam)
{
	drawables->push_front(new gl_drawable(go, ship_shape));
	game_camera[cam] = new ego_camera(go, 0.0f, -0.4f * SHIPSCALE, -0.15f * SHIPSCALE);

}

void gl_renderer::add_star(game_object *go)
{
	drawables->push_front(new gl_drawable(go, star_shape));
}

void gl_renderer::switch_camera(int cam)
{
	current_camera = game_camera[cam];
}

void gl_renderer::init()
{
	const SDL_VideoInfo* info = NULL;
	bpp = 0;
	flags = SDL_OPENGL | SDL_RESIZABLE;
	//width = 640;
	//height = 480;
	width = 1024;
	height = 786;
	angle = 0;


	GLint stencil = 0;

	current_camera = game_camera[0] = new desktop_camera();


	/* ----- SDL init --------------- */
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
		exit(-1);
	}

	atexit(SDL_Quit);

	info = SDL_GetVideoInfo();
	bpp = info->vfmt->BitsPerPixel;


/* ----- OpenGL attribute setting via SDL --------------- */
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


/* ----- Setting up the screen surface --------------- */
	screen = SDL_SetVideoMode(width, height, bpp, flags);
	if (screen == 0) {
		fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
			exit(-1);
	}

	glGetIntegerv(GL_STENCIL_BITS, &stencil);
	if(stencil == 0){
		printf("Stencil buffer not available!\n");
		exit(-1);
	}

	SDL_WM_SetCaption("Norbit fridge in space simulator", NULL);

	light1 = (Light){{ 0.0f, 1.5f,-1.5f, 1.0f}, { 0.7f, 0.7f, 0.7f, 1.0f},
		   { 0.7f, 0.7f, 0.7f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}};
	light2 = (Light){{-1.5f,-1.8f, 0.0f, 1.0f}, { 0.4f, 0.4f, 0.4f, 1.0f},
			{ 0.4f, 0.4f, 0.4f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}};


/* ----- OpenGL init --------------- */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* The following function replaces gluPerspective */
	gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.001f, 100000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	ship_shape = new obj_shape(new string("ptr_mk1.obj"), SHIPSCALE); 
	star_shape = new sphere_shape(1392000.0 * KM);	
	background_stars = new skybox();
	background_stars->init(200.0f);

}


int gl_renderer::render() {

	int running = 1;


/* ----- Event cycle --------------- */

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				running = 0;
				break;
			case SDL_KEYDOWN:
				switch( event.key.keysym.sym ){
				case SDLK_ESCAPE:
				case SDLK_q:
					running = 0;
					break;
				case SDLK_F1:
					SDL_WM_ToggleFullScreen(screen);
					break;
				case SDLK_s:
					control1->toggleEngineL();
					break;
				case SDLK_d:
					control1->toggleEngineR();
					break;
				case SDLK_k:
					control2->toggleEngineL();
					break;
				case SDLK_l:
					control2->toggleEngineR();
					break;

				case SDLK_1:
					switch_camera(0);
					break;
				case SDLK_2:
					switch_camera(1);
					break;
				case SDLK_3:
					switch_camera(2);
					break;
				default:
					break;
				}
				break;
			case SDL_VIDEORESIZE:
				if((screen = SDL_SetVideoMode(event.resize.w, event.resize.h, bpp, flags)) == 0) {
					fprintf(stderr, "Video resize failed: %s\n", SDL_GetError());
					exit(-1);
				}
				glPushAttrib(GL_TRANSFORM_BIT);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				gluPerspective(90.0f, (GLfloat)event.resize.w/(GLfloat)event.resize.h, 0.1, 100000.0);
				glViewport(0.0f, 0.0f, event.resize.w, event.resize.h);
				glPopAttrib();
				break;
			case SDL_ACTIVEEVENT:
				if(event.active.state != SDL_APPMOUSEFOCUS && event.active.gain == 0)
					while(1){
						SDL_WaitEvent(&event);
						if(event.type == SDL_ACTIVEEVENT &&
							event.active.state != SDL_APPMOUSEFOCUS &&
							event.active.gain == 1);
							break;
					}
				break;
			default:
				break;
		}
	}

	interval = FrameTiming();
/* apply control movement */


/* ----- Blitting on the screen --------------- */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();

	current_camera->apply_rotation();
	//glutSolidCube(50.0);
	//draw_skybox(200.0);
	background_stars->draw();
	current_camera->apply_translation();



/* ----- Light ----- */
	angle += 0.001f * interval;

	light1.Position[0] = sinf(angle) * 1.5f;
	light1.Position[2] = cosf(angle) * 1.5f;

	light2.Position[1] = sinf(angle) * 1.5f;
	light2.Position[2] = cosf(angle) * 1.5f;

	UpdateLight(&light1, GL_LIGHT1, 0.1f);
	UpdateLight(&light2, GL_LIGHT2, 0.1f);

/* ----- Objects ----- */
	for(list<gl_drawable*>::const_iterator it = drawables->begin(); it != drawables->end(); ++it)
	{
		(*it)->draw();
	}

	SDL_GL_SwapBuffers();
	/*		SDL_Delay(25); */ /* Decomment this if you want 1/50th screen update */

	return running;
}



void gl_renderer::stop()
{
	SDL_Quit();
}


gl_renderer::~gl_renderer() {

}

/*
camera::camera() {}
camera::~camera() {}
*/
