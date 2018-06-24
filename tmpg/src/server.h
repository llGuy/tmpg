#ifndef _SERVER_H_
#define _SERVER_H_

#include <array>
#include <vector>

#include "net_handler.h"

namespace net {

    class Server
	: public NetworkHandler
    {
    public:
	Server(void) = default;

	void Launch(const std::string& address, const std::string& port,
	    	    tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih) override;
    private:
	void UDPThread(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih);
	// thread for communicating with each client
	void TCPThread(UnixSocket socket, tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih); // take by copy
	void TCPAcceptThread(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih);
    private:
	static constexpr uint32_t MAX_PENDING = 10;
	
	tmpg::EntitiesHandler* m_ehandler;
	std::unique_ptr<std::thread> m_TCPAcceptThread;
	std::vector<std::thread> m_TCPCommunicationThreads;
    };

}

#endif /* _SERVER_H_ */
