/* spaceship & other controllers */


#if !defined(CONTROL_H)
#define CONTROL_H
#include<list>
#include "game_object.h"


// each class describes the different possibilities of each of the 


class Controller {
public:
virtual void apply() = 0;

};


class SpaceShipController: public Controller
{
public:
	SpaceShipController(game_object * go_);
	void toggleEngineL();

	void toggleEngineR();

	virtual void apply();
private:
	bool engine_L, engine_R;
	game_object * go;

};


class GravityController: public Controller
{
public:
	GravityController(game_object * go_, std::list<game_object*>* go_list );
	
	virtual void apply();
private:
	
	game_object * go;
	 std::list<game_object*>* go_list;

};





#endif


