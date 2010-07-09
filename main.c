/* GL_shadow: an implementation of the stencil shadow volume technique
   Copyright (C) 2005 Angelo "Encelo" Theodorou
 
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <iostream>
#include <list>
#include <string>

#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include "extra.h"
#include "game_object.h"
#include "physics.h"

using namespace std;

void init_sdl(SDL_Surface **screen, int bpp, int flags, int width, int height);
void init_scene(list<game_object*> *go_list, simulation_world *world);

int main(int argc, char **argv)
{
	SDL_Surface *screen;
	SDL_Event event;
	
	int bpp = 0;
	int flags = 0;
	int width = 640;
	int height = 480;
	
	Uint32 interval;
	Light light1 = {{ 0.0f, 1.5f,-1.5f, 1.0f}, { 0.7f, 0.7f, 0.7f, 1.0f},
                   { 0.7f, 0.7f, 0.7f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}};
	Light light2 = {{-1.5f,-1.8f, 0.0f, 1.0f}, { 0.4f, 0.4f, 0.4f, 1.0f},
                    { 0.4f, 0.4f, 0.4f, 1.0f}, { 1.0f, 1.0f, 1.0f, 1.0f}};
	
	
	GLfloat angle = 0;
	

//	flags = SDL_OPENGL | SDL_FULLSCREEN; 
	flags = SDL_OPENGL | SDL_RESIZABLE;
	init_sdl(&screen, bpp, flags, width, height);

/* ----- OpenGL init --------------- */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* The following function replaces gluPerspective */
	perspectiveGL(45.0f, (GLfloat)width/(GLfloat)height, 0.001f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);


/* ----- Init scene --------------- */
	list<game_object*> *game_objects = new list<game_object*>();
	simulation_world *world = new simulation_world();
	init_scene(game_objects, world);

/* ----- Event cycle --------------- */
	int quit = 0;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_KEYDOWN:
					switch( event.key.keysym.sym ){
					case SDLK_ESCAPE:
					case SDLK_q:
						quit = 1;
						break;
					case SDLK_F1:
						SDL_WM_ToggleFullScreen(screen);
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
					perspectiveGL(45.0f, (GLfloat)event.resize.w/(GLfloat)event.resize.h, 0.001f, 100.0f);
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

/* ----- simulation time ticks ----*/

		static real LastTime = 0;
		real Time = LastTime  + 0.0001f;
		printf("time  %0.4f %0.4f\n", Time, LastTime);
		world->Simulate(Time - LastTime);
		world->Render();  // some debug output
		LastTime = Time;


/* ----- Blitting on the screen --------------- */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		angle += 0.001f * interval;

		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, -7.5f); /* Negative Zoom */


/* ----- Light ----- */
		light1.Position[0] = sinf(angle) * 1.5f;
		light1.Position[2] = cosf(angle) * 1.5f;
		
		light2.Position[1] = sinf(angle) * 1.5f;
		light2.Position[2] = cosf(angle) * 1.5f;
		
		UpdateLight(&light1, GL_LIGHT1, 0.1f);
		UpdateLight(&light2, GL_LIGHT2, 0.1f);

/* ----- Objects ----- */
		for(list<game_object*>::const_iterator it = game_objects->begin(); it != game_objects->end(); ++it)
		{
			(*it)->draw();
		}
	
		SDL_GL_SwapBuffers();
/*		SDL_Delay(25); */ /* Decomment this if you want 1/50th screen update */
	}


/* ----- Quitting --------------- */
	
	SDL_Quit();
	return 0;	
}

void init_scene(list<game_object*> *go_list, 	
	simulation_world *world)
{
	game_object::set_simulation_world(world);	
	real const Density = r(1.0);
	shape* sh = new shape(new string("ptr_mk1.obj"));

	game_object* ship1 = new game_object();
	rigid_body * r = world->add_body( 1.0f );
	ship1->set_rigid_body(r);
	ship1->set_shape(sh);
	ship1->set_position(2.0f, 1.0f, 0.0f);
	go_list->push_front(ship1);

	game_object* ship2 = new game_object();
	r = world->add_body( 1.0f );
	ship2->set_rigid_body(r);
	ship2->set_shape(sh);
	ship2->set_position(-1.0f, -20.0f, -0.0f);
	go_list->push_front(ship2);
//	world->aBodies.at(0)->aConfigurations[0].CMVelocity = vector_2(0.10f,0.10f);
//	world->aBodies.at(0)->aConfigurations[0].AngularVelocity = PI;
}

void init_sdl(SDL_Surface **screen, int bpp, int flags, int width, int height)
{
	const SDL_VideoInfo* info = NULL;
	
	
	GLint stencil = 0;


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
	if((*screen = SDL_SetVideoMode(width, height, bpp, flags)) == 0) {
		fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
        	exit(-1);
	}

	glGetIntegerv(GL_STENCIL_BITS, &stencil);
	if(stencil == 0){
		printf("Stencil buffer not available!\n");
		exit(-1);
	}

	SDL_WM_SetCaption("Norbit fridge in space simulator", NULL);

}
