#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <thread>
#include <memory>
#include "net_handler.h"
#include "entities_handler.h"

namespace net {

    class Client
	: public NetworkHandler
    {
    public:
	Client(tmpg::EntitiesHandler*);

	void Launch(const std::string& address, const std::string& port) override;
    private:
	void UDPThread(void);
	void TCPThread(void);

	std::string ReceiveUsername(void);
    private:
	tmpg::EntitiesHandler* m_ehandler;
	std::unique_ptr<std::thread> m_TCPThread;
	std::unique_ptr<std::thread> m_UDPThread;
    };

}

#endif /* _CLIENT_H_ */
