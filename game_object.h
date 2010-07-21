#include <string>

#include "physics.h"

using namespace std;

#if !defined(GO_H)
#define GO_H

class shape
{
public:
  shape ();
  virtual ~ shape ();
  virtual void draw () = 0;
};

class game_object
{
public:
  game_object();
  void set_rigid_body (rigid_body *);
  void set_position (float x, float y, float z);

  static void set_simulation_world (simulation_world * w);
  void apply_force (vector_2 const &F, vector_2 const &Pl);	// TODO: Make it fit for 3D
  void apply_force_G (vector_2 const &F, vector_2 const &Pl);	// TODO: Make it fit for 3D
  float posx ();
  float posy ();
  float posz ();
  float orientation ();		// TODO: Make it fit for 3D
  float mass ();

  int get_id()const  {return id;}
  void set_id(int id_){ id = id_;}
   
  rigid_body* physics() {return body; } ;


private:
    int id;

    shape * object_shape;
  static simulation_world *world;
  rigid_body *body;
};



#endif
