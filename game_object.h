#include "physics.h"

class game_object
{
public:
	void draw();
	void set_object(Object *object);
	void set_position(float x, float y, float z);
	
private:
	Object *object;
	rigid_body *body;
	float posx, posy, posz;
};
