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

#include "../game_object.h"
#include "skybox.h"
#include "gl_drawable.h"
#include <list>


class camera {
public:
//	camera();
//	virtual ~camera();
	virtual void apply_rotation() = 0;
	virtual void apply_translation() = 0;
	virtual void active(int i) = 0;
};



class gl_renderer {
public:
	void init();
	void add_ship(game_object *go);
	void add_star(game_object *go);
	int render();
	void stop();
	gl_renderer();
	void switch_camera(int cam);
	virtual ~gl_renderer();
private:
	void draw_skybox(double size);
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
	camera *current_camera;
	camera *(game_camera[5]);
	skybox *background_stars;
};

#endif /* GL_RENDERER_H_ */
