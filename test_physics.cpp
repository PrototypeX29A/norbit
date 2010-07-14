
#include <assert.h>
#include <stdio.h>

#include "physics.h"


simulation_world World;

float
GetTime ()
{
  static float t = 0.0f;
  printf ("%0.4f ", t);
  return t++;
}



void
RunS (void)
{
  static real LastTime = GetTime ();
  // use a fixed timestep until we implement a better integrator
  // real Time = GetTime();
  real Time = LastTime + r (0.02);
  printf ("time  %0.4f %0.4f\n", Time, LastTime);
  World.Simulate (Time - LastTime);
  printf ("after simulate:\n");
  World.Render ();
  printf ("----------------------------------\n");
  LastTime = Time;
  return;
}


int
main (int argc, char **argv)
{
  // initialize bodies
  real const Density = r (0.01);
  World.add_body (100.0f);
  World.add_body (100.0f);
  World.add_body (100.0f);

  World.aBodies.at (0)->aConfigurations[0].CMVelocity =
    vector_2 (r (0.40), r (0.10));
  World.aBodies.at (2)->aConfigurations[0].AngularVelocity = r (PI);


  for (int i = 0; i < 1000; i++)
    {
      printf ("run %d\n", i);
      ::RunS ();
    }

  return 0;
}
