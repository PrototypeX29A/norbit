#include <zmq.hpp>
#include <assert.h>
#include <stdio.h>
#include "../control.h"
#include "../net/netcontrol.h"
#include "../net/norbit.pb.h"


void
adapt(const SpaceShipController * sc,
	    norbitnet_SSControllerState * sscs)
{
  assert (sc);
  assert (sscs);
  //assert(sc->get_go());
  //sscs->set_id (sc->get_go()->get_id());
  sscs->set_id (0);
  sscs->set_toggleenginel(sc->getEngineL ());
  sscs->set_toggleenginer(sc->getEngineR ());
}

void
adapt(const norbitnet_SSControllerState * sscs,
	      SpaceShipController * sc)
{
  assert (sc);
  assert (sscs);
//  assert (sscs->id() == sc->get_go()->get_id());
  sc->setEngineL(sscs->toggleenginel());
  sc->setEngineR(sscs->toggleenginer());
}


ClientSpaceShipController::ClientSpaceShipController (game_object * go_, zmq::socket_t * control_socket_):SpaceShipController (go_),
control_socket
(control_socket_)
{
  assert (control_socket);
}


void
ClientSpaceShipController::apply ()
{
  std::string msg_str;
  norbitnet_SSControllerState netSSCS;
  adapt(this, &netSSCS);
  netSSCS.SerializeToString (&msg_str);
  zmq::message_t msg (msg_str.size () + 1);
  ::memcpy (msg.data (), msg_str.c_str (), msg_str.size () + 1);
  control_socket->send (msg);

  zmq::message_t msg_reply;
  control_socket->recv(&msg_reply);
}

ServerSpaceShipController::ServerSpaceShipController (game_object * go_, zmq::socket_t * control_socket_):SpaceShipController
  (go),control_socket
(control_socket_)
{
   assert (control_socket);

}

void
ServerSpaceShipController::apply ()
{
// get msg and set l & r
// use separate socket for l&r
  zmq::message_t msg;
  if (control_socket->recv (&msg, ZMQ_NOBLOCK))
    {
      norbitnet_SSControllerState netSSCS;
      std::string msg_str((const char*)msg.data());
      netSSCS.ParseFromString(msg_str);
      adapt (&netSSCS,this);

      std::string str =  "OK";
      zmq::message_t q(str.size()+1);
      ::memcpy(q.data(), str.c_str(),str.size()+1);
      control_socket->send(q);
    }
  SpaceShipController::apply ();
}
