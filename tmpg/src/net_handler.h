#ifndef _NET_HANDLER_H_
#define _NET_HANDLER_H_

#include <memory>
#include <thread>
#include <vector>
#include "socket.h"
//#include "input_handler.h"
//#include "entities_handler.h"

namespace tmpg {

    class EntitiesHandler;
    class InputHandler;
    
    namespace physics
    {
	class PhysicsHandler;
    }

}

namespace net {    
    
    class NetworkHandler
    {
    public:
	NetworkHandler(void) = default;
	virtual ~NetworkHandler(void) {}

	virtual void Launch(const std::string& address, const std::string& port,
			    tmpg::EntitiesHandler&, tmpg::InputHandler&, tmpg::physics::PhysicsHandler& ph) = 0;
    protected:
	// tmpg only supports unix for now
	UnixSocket m_UDPSocket;
	UnixSocket m_TCPSocket;

	std::unique_ptr<std::thread> m_UDPThread;
    };

}

#endif /* _NET_HANDLER_H_ */
