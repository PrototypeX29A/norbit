/* spaceship & other controllers */


#if !defined(CONTROL_H)
#define CONTROL_H

#include "game_object.h"


// each class describes the different possibilities of each of the 

class SpaceShipController
{
public:
	SpaceShipController(game_object * go_ );
	void toggleEngineL();

	void toggleEngineR();

	void apply();
private:
	bool engine_L, engine_R;
	game_object * go;

};






#endif


