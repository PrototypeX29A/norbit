
#include <assert.h>
#include "physics.h"


/*----------------------------------------------------------------------------

Globals

*/

// weird CodeWarrior bug with global initialization forces me to do this
#define WIDTH 400
#define HEIGHT 400

int WorldWidth = WIDTH;
int WorldHeight = HEIGHT;

simulation_world World(r(WIDTH),r(HEIGHT));

float GetTime() { static float t = 0.0f ; return t++; } 



/*----------------------------------------------------------------------------

Run

*/

void Run( void )
{
	static real LastTime = GetTime();

	// use a fixed timestep until we implement a better integrator
	// real Time = GetTime();
	real Time = LastTime + r(0.02);

	World.Simulate(Time - LastTime);

	World.Render();

	LastTime = Time;
}

/*----------------------------------------------------------------------------

Toggles

*/

void ToggleWorldSpring( void )
{
	World.WorldSpringActive = World.WorldSpringActive ? 0 : 1;
}

void ToggleBodySpring( void )
{
	World.BodySpringActive = World.BodySpringActive ? 0 : 1;
}

void ToggleGravity( void )
{
	World.GravityActive = World.GravityActive ? 0 : 1;
}

void ToggleDamping( void )
{
	World.DampingActive = World.DampingActive ? 0 : 1;
}


/*----------------------------------------------------------------------------

utilities

*/


int main(int argc, char **argv)
{

for(int i=0; i<1000;i++){

 Run();
}

	return 0;
}
