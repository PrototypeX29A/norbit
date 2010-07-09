/* spaceship & other controllers */


#include "control.h"
#include "stdio.h"

SpaceShipController::SpaceShipController(game_object * go_ ):go(go_), engine_L(false), engine_R(false) {}

void SpaceShipController::toggleEngineL(){
	engine_L = !engine_L;
	printf ("engine_L = %d\n",engine_L);
}

void SpaceShipController::toggleEngineR(){
	engine_R = !engine_R;
	printf ("engine_R = %d\n",engine_R);
}

void SpaceShipController::apply(){
	if (engine_L) {
		go->apply_force( vector_2(-0.1f,-1.0f), vector_2(-100.0f,-0.10f)); // pushes upward
	}
	if(engine_R) {
		go->apply_force( vector_2(-0.1f,-1.0f), vector_2(100.0f,-0.10f)); // pushes upward
	}
}
