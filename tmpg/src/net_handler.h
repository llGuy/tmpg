#ifndef _NET_HANDLER_H_
#define _NET_HANDLER_H_

#include <memory>
#include <thread>
#include <vector>
#include "socket.h"

namespace net {

    class NetworkHandler
    {
    public:
	NetworkHandler(void) = default;

	virtual void Launch(const std::string& address, const std::string& port) = 0;
    protected:
	// tmpg only supports unix for now
	UnixSocket m_UDPSocket;
	UnixSocket m_TCPSocket;

	std::unique_ptr<std::thread> m_UDPThread;
    };

}

#endif /* _NET_HANDLER_H_ */
