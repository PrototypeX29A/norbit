/*
 * desktop_camera.h
 *
 *  Created on: Jul 11, 2010
 *      Author: peter
 */

#include "gl_renderer.h"

#ifndef DESKTOP_CAMERA_H_
#define DESKTOP_CAMERA_H_

class desktop_camera : public camera {
public:
	virtual void apply_rotation();
	virtual void apply_translation();
	virtual void active(int i);
};

#endif /* DESKTOP_CAMERA_H_ */
