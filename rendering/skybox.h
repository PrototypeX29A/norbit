/*
 * skybox.h
 *
 *  Created on: Jul 13, 2010
 *      Author: peter
 */

#ifndef SKYBOX_H_
#define SKYBOX_H_

//#include <string>
#include <GL/glu.h>

class skybox {
public:
		void init(double boxsize);
		void draw();
private:
		double boxsize;
		GLuint up,down,north,east, south, west;
		GLuint load_texture(const char *filename);
};

#endif /* SKYBOX_H_ */
