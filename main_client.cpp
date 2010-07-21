/* Norbit - Fridge in space simulation
 
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <iostream>
#include <string>

#include <zmq.hpp>
#include "net/norbit.pb.h"
#include "net/netcontrol.h"

#include "rendering/gl_renderer.h"
#include "rendering/obj_shape.h"
#include "rendering/sphere_shape.h"
#include "control.h"
#include "main.h"

extern "C"
{
#include <stdlib.h>
#include <math.h>
#include "physics.h"
}

using namespace std;


void init_sdl (SDL_Surface ** screen, int bpp, int flags, int width,
	       int height);
void init_scene (gl_renderer * renderer);

list < game_object * >game_objects;
list < Controller * >game_controllers;
game_object *ship1, *ship2;
simulation_world *world;
SpaceShipController *control1, *control2;
zmq::context_t ctx (1);
zmq::socket_t control_socket (ctx, ZMQ_REQ);	// send input to srver
zmq::socket_t control_socket2 (ctx, ZMQ_REQ);	// send input to srver

int
main (int argc, char **argv)
{
   zmq::socket_t publish_socket (ctx, ZMQ_SUB);	// subscribes physics updates, gaming data
  publish_socket.setsockopt (ZMQ_SUBSCRIBE, "",0);

  publish_socket.connect ("tcp://127.0.0.1:5555");

  control_socket.connect ("tcp://127.0.0.1:5556");

  control_socket2.connect ("tcp://127.0.0.1:5557");


  gl_renderer *renderer = new gl_renderer ();
  renderer->init ();
  init_scene (renderer);
  renderer->add_ship (ship1,1);
  renderer->add_ship (ship2,2);
  int running = 1;
  do
    {
      running = renderer->render ();
      for (list < Controller * >::const_iterator it =
	   game_controllers.begin (); it != game_controllers.end (); ++it)
	{
	                (*it)->apply();
	}
      static real LastTime = 0;
      //real Time = LastTime  + 0.0001f;
      real Time = LastTime + 0.001f;
      //              printf("time  %0.4f %0.4f\n", Time, LastTime);
      //      world->Simulate(Time - LastTime);
      //              world->Render();  // some debug output
      LastTime = Time;
	// Receive and display the result
       zmq::message_t resultset;
       while (publish_socket.recv (&resultset,ZMQ_NOBLOCK)) {
                  const char *resultset_string = (const char *)resultset.data ();
                  printf ("Received response: '%s'\n", resultset_string);
      } 
      for (list < game_object* >::const_iterator it =
           game_objects.begin (); it != game_objects.end (); ++it)
      {
        //  (*it)->;
      }

    }
  while (running);
  renderer->stop ();
}

void
init_scene (gl_renderer * renderer)
{
  /* ----- Init scene --------------- */

  world = new simulation_world ();
  game_object::set_simulation_world (world);
  for (int i = 0; i < 10; i++)
    {
      game_object *sun = new game_object ();
      rigid_body *r = world->add_body (100.0f);
      sun->set_rigid_body (r);
      float rx = -50.0 + (rand () % 100);
      float ry = -50.0 + (rand () % 100);
      //float rz = -50.0 + (rand() % 100);
      sun->set_position (rx, ry, 0.0f);
      game_objects.push_front (sun);

      renderer->add_star (sun);
      //GravityController * sungrav = new GravityController(sun,  &game_objects);
      //game_controllers.push_front(sungrav);
    }

  ship1 = new game_object ();
  rigid_body *r = world->add_body (1.0f);
  ship1->set_rigid_body (r);
  ship1->set_position (1.0f, 1.0f, 0.0f);
  game_objects.push_front (ship1);



  GravityController *gravcontrol1 =
    new GravityController (ship1, &game_objects);
  control1 = new ClientSpaceShipController (ship1,&control_socket);
  game_controllers.push_front (gravcontrol1);
  game_controllers.push_front (control1);

  ship2 = new game_object ();
  r = world->add_body (1.0f);
  ship2->set_rigid_body (r);
  ship2->set_position (-1.0f, 1.5f, -0.0f);
  game_objects.push_front (ship2);
  GravityController *gravcontrol2 =
    new GravityController (ship2, &game_objects);
  control2 = new ClientSpaceShipController (ship2, &control_socket2);
  game_controllers.push_front (gravcontrol2);
  game_controllers.push_front (control2);
}
