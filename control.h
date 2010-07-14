/* spaceship & other controllers */


#if !defined(CONTROL_H)
#define CONTROL_H
#include<list>
#include "game_object.h"


// each class describes the different possibilities of each of the 


class Controller
{
public:
  virtual void apply () = 0;
  game_object * get_go()const { return go; }
protected:
    game_object * go;


};


class SpaceShipController:public Controller
{
public:
  SpaceShipController (game_object * go_);
  void toggleEngineL ();
  void toggleEngineR ();
  void setEngineL(bool b);
  void setEngineR (bool b);
  bool getEngineL ()const;
  bool getEngineR ()const;
  virtual void apply ();
protected:
    bool engine_L, engine_R;

};


class GravityController:public Controller
{
public:
  GravityController (game_object * go_, std::list < game_object * >*go_list);

  virtual void apply ();
private:

    std::list < game_object * >*go_list;
};





#endif
