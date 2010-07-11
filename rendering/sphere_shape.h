#include "../game_object.h"

class sphere_shape : public shape
{
	public:
		sphere_shape(float size);
		sphere_shape(float size, float red, float green, float blue);
		virtual void draw();
		virtual ~sphere_shape();
	private:
		float size, red, green, blue;
};
