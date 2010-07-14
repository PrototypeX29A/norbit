/* spaceship & other controllers */

#include <assert.h>


#include "control.h"
#include "stdio.h"

SpaceShipController::SpaceShipController (game_object * go_): engine_L (false),
engine_R (false)
{
  go = go_;
  assert(go);
}

void
SpaceShipController::toggleEngineL ()
{
  engine_L = !engine_L;
  printf ("engine_L = %d\n", engine_L);
}

void
SpaceShipController::toggleEngineR ()
{
  engine_R = !engine_R;
  printf ("engine_R = %d\n", engine_R);
}



void SpaceShipController::setEngineL(bool b) {engine_L = b;}
void SpaceShipController::setEngineR (bool b) {engine_R = b; }
bool SpaceShipController::getEngineL ()const{return engine_L;};
bool SpaceShipController::getEngineR ()const {return engine_R;};


void SpaceShipController::apply(){
	if (engine_R) {
		go->apply_force( vector_2(-0.1f,-1.0f), vector_2(-100.0f,-0.10f)); // pushes upward
	}
	if(engine_L) {
		go->apply_force( vector_2(-0.1f,-1.0f), vector_2(100.0f,-0.10f)); // pushes upward
	}
}



GravityController::GravityController (game_object * go_, std::list < game_object * >*go_list_): go_list (go_list_)
{
  go = go_;
  assert (go);
  assert (go_list);
}

void
GravityController::apply ()
{
  std::list < game_object * >&ref_go_list = *go_list;

  for (std::list < game_object * >::iterator it = ref_go_list.begin ();
       it != ref_go_list.end (); it++)
    {
      //if (  (*it)->mass() < 10 * go->mass()) continue;
      float DistSq =
	pow (go->posx () - (*it)->posx (),
	     2) + pow (go->posy () - (*it)->posy (),
		       2) + pow (go->posz () - (*it)->posz (), 2);
      if (DistSq < 2.0)
	continue;
      if (DistSq > 5000.0)
	continue;
      float G = (go->mass () * (*it)->mass ()) / DistSq;
      if (G > 10.0f)
	G = 10.0f;
      vector_2 DirF =
	-1.0f * vector_2 (go->posx () - (*it)->posx (),
			  go->posy () - (*it)->posy ()) / sqrt (DistSq);
      go->apply_force_G (G * DirF, vector_2 (go->posx (), go->posy ()));	// pushes upward
      //printf("applying grav Dsq:%0.4f G: %0.4f\n", DistSq, G);

    }


}
