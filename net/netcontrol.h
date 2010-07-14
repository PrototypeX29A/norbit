


#if !defined(NETCONTROL_H)
#define NETCONTROL_H

#include <zmq.hpp>
#include "../control.h"


class ClientSpaceShipController: public SpaceShipController 
{
public:
        ClientSpaceShipController(game_object * go_,zmq::socket_t*);
        virtual void apply();
private:
	zmq::socket_t * control_socket;

};

class ServerSpaceShipController: public SpaceShipController 
{
public:
        ServerSpaceShipController(game_object * go_,zmq::socket_t*);
	void setEngineL(bool);
	void setEngineR(bool);
        virtual void apply();

private:
	zmq::socket_t * control_socket;
};


#endif

