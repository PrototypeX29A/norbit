#ifndef MAIN_H_
#define MAIN_H_

#include <list>
#include "control.h"

extern list<game_object*> game_objects;
//extern list<Controller*> game_controllers;
extern game_object *ship1, *ship2;
extern simulation_world *world;
extern SpaceShipController *control1, *control2;
extern int running;

#endif /* MAIN_H_ */
