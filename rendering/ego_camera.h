/*
 * ego_camera.h
 *
 *  Created on: Jul 12, 2010
 *      Author: peter
 */

#ifndef EGO_CAMERA_H_
#define EGO_CAMERA_H_

#include "gl_renderer.h"

class ego_camera: public camera {
	virtual void apply_rotation();
	virtual void apply_translation();
	virtual void active(int i);
};

#endif /* EGO_CAMERA_H_ */
