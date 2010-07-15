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
#include <unistd.h>
#include <zmq.hpp>
#include"net/norbit.pb.h"

#include "control.h"
#include "net/netcontrol.h"
#include "main.h"

extern "C"
{
#include <stdlib.h>
#include <math.h>
//#include "extra.h"
#include "physics.h"
}

using namespace std;


void init_scene ();

list < game_object * >game_objects;
list < Controller * >game_controllers;
game_object *ship1, *ship2;
simulation_world *world;
SpaceShipController *control1, *control2;

zmq::context_t ctx (1);
zmq::socket_t publish_socket (ctx, ZMQ_PUB);	// publishes physics updates, gaming data
zmq::socket_t control_socket (ctx, ZMQ_REP);	// send input to srver
zmq::socket_t control_socket2 (ctx, ZMQ_REP);	// send input to srver

int
main (int argc, char **argv)
{
  publish_socket.bind ("tcp://lo:5555");
  control_socket.bind ("tcp://lo:5556");
  control_socket2.bind ("tcp://lo:5557");

  init_scene ();

  int running = 1;
  do
    {
      for (list < Controller * >::const_iterator it =
	   game_controllers.begin (); it != game_controllers.end (); ++it)
	{
	  (*it)->apply ();
	}
      static real LastTime = 0;
      //      real Time = LastTime  + 0.0001f;
      real Time = LastTime + 0.01f;
	usleep(5000);//microseconds
      //              printf("time  %0.4f %0.4f\n", Time, LastTime);
      world->Simulate (Time - LastTime);
      //              world->Render();  // some debug output
      LastTime = Time;

      for (list < game_object * >::iterator it =
	   game_objects.begin (); it != game_objects.end (); ++it)
	{
	  norbitnet_GOPHY gophy;


	  game_object *g = *it;
	  rigid_body *r = g->physics ();
	  gophy.set_id (g->get_id ());
	  gophy.set_posx (r->aConfigurations[r->SourceConfigurationIndex].
			  CMPosition.X);
	  gophy.set_posy (r->aConfigurations[r->SourceConfigurationIndex].
			  CMPosition.Y);
	  gophy.set_posz (0.0f);
	  gophy.set_orientation (r->
				 aConfigurations[r->SourceConfigurationIndex].
				 Orientation);
	  gophy.set_velx (r->aConfigurations[r->SourceConfigurationIndex].
			  CMVelocity.X);
	  gophy.set_vely (r->aConfigurations[r->SourceConfigurationIndex].
			  CMVelocity.Y);
	  gophy.set_velorientation (r->
				    aConfigurations[r->
						    SourceConfigurationIndex].
				    AngularVelocity);

	  std::string str;
	  gophy.SerializeToString (&str);
	  zmq::message_t q (str.size () + 1);
	  ::memcpy (q.data (), str.c_str (), str.size () + 1);
	  publish_socket.send (q);

	}


    }
  while (running);
}

void
init_scene ()
{
  /* ----- Init scene --------------- */

  world = new simulation_world ();
  game_object::set_simulation_world (world);
  for (int i = 0; i < 10; i++)
    {
      game_object *sun = new game_object ();
      rigid_body *r = world->add_body (100.0f);
      sun->set_rigid_body (r);
      sun->set_id (i);

      float rx = -50.0 + (rand () % 100);
      float ry = -50.0 + (rand () % 100);
      //float rz = -50.0 + (rand() % 100);
      sun->set_position (rx, ry, 0.0f);
      game_objects.push_front (sun);
    }

  ship1 = new game_object ();
  rigid_body *r = world->add_body (1.0f);
  ship1->set_rigid_body (r);
  ship1->set_position (1.0f, 1.0f, 0.0f);
  ship1->set_id (20);

  game_objects.push_front (ship1);

  GravityController *gravcontrol1 =
    new GravityController (ship1, &game_objects);
  control1 = new ServerSpaceShipController (ship1, &control_socket);
  game_controllers.push_front (gravcontrol1);
  game_controllers.push_front (control1);

  ship2 = new game_object ();
  r = world->add_body (1.0f);
  ship2->set_rigid_body (r);
  ship2->set_position (-1.0f, 1.5f, -0.0f);
  ship2->set_id (30);

  game_objects.push_front (ship2);
  GravityController *gravcontrol2 =
    new GravityController (ship2, &game_objects);
  control2 = new ServerSpaceShipController (ship2, &control_socket2);
  game_controllers.push_front (gravcontrol2);
  game_controllers.push_front (control2);
}
