#ifndef _NET_HANDLER_H_
#define _NET_HANDLER_H_

#include <memory>
#include <thread>
#include <vector>
#include "socket.h"
#include "client_addr.h"
#include "platform.h"
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

	enum mode { CLIENT, SERVER, INVALID };
    
    class NetworkHandler
    {
    public:
	NetworkHandler(void) = default;
	virtual ~NetworkHandler(void) {}
	virtual mode Mode(void) { return INVALID; }

	virtual void Launch(const std::string& address, const std::string& port,
		tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph, tmpg::Platform& platform) = 0;
    protected:
	// tmpg only supports unix for now
	Socket m_UDPSocket;
	Socket m_TCPSocket;

	std::unique_ptr<std::thread> m_UDPThread;
    };

}

#endif /* _NET_HANDLER_H_ */
