/*
 * gl_renderer.h
 *
 *  Created on: Jul 11, 2010
 *      Author: peter
 */

#ifndef GL_RENDERER_H_
#define GL_RENDERER_H_

extern "C" {
#include <SDL/SDL.h>
#include <GL/gl.h>
#include "extra.h"
}

#include "game_object.h"
#include "gl_drawable.h"
#include <list>

class gl_renderer {
public:
	void init();
	void add_ship(game_object *go);
	void add_star(game_object *go);
	int render();
	void stop();
	gl_renderer();
	virtual ~gl_renderer();
private:
	int bpp;
	int flags;
	int width;
	int height;
	SDL_Surface *screen;
	Light light1;
	Light light2;
	SDL_Event event;
	Uint32 interval;
	GLfloat angle;
	int quit;
	shape *ship_shape, *star_shape;
	list<gl_drawable*> *drawables;
};

#endif /* GL_RENDERER_H_ */
