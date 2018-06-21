#ifndef _SERVER_H_
#define _SERVER_H_

#include <array>
#include <vector>

#include "entities_handler.h"
#include "input_handler.h"
#include "net_handler.h"

namespace net {

    class Server
	: public NetworkHandler
    {
    public:
	Server(tmpg::EntitiesHandler*);

	void Launch(const std::string& address, const std::string& port) override;
    private:
	void UDPThread(void);
	// thread for communicating with each client
	void TCPThread(UnixSocket socket); // take by copy
	void TCPAcceptThread(void);
    private:
	static constexpr uint32_t MAX_PENDING = 10;
	
	tmpg::EntitiesHandler* m_ehandler;
	std::unique_ptr<std::thread> m_TCPAcceptThread;
	std::vector<std::thread> m_TCPCommunicationThreads;
    };

}

#endif /* _SERVER_H_ */
