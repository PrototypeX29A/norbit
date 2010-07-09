#include "physics.h"

class game_object
{
public:
	void draw();
	void set_object(Object *object);
	void set_rigid_body(rigid_body*);

	void set_position(float x, float y, float z);
	
	static void set_simulation_world(simulation_world *w);


private:
	static simulation_world * world;
	Object *object;
	rigid_body *body;
	//float posx, posy, posz;
	float posx();
	float posy();
	float posz();
};



