/*
 * gl_drawable.h
 *
 *  Created on: Jul 11, 2010
 *      Author: peter
 */

#ifndef GL_DRAWABLE_H_
#define GL_DRAWABLE_H_

#include "../game_object.h"

class gl_drawable {
public:
	gl_drawable(game_object *go, shape *sh);
	void draw();
private:
	game_object *go;
	shape *sh;
};

#endif /* GL_DRAWABLE_H_ */
