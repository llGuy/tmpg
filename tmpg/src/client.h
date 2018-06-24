#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <thread>
#include <memory>
#include "net_handler.h"

namespace net {

    class Client
	: public NetworkHandler
    {
    public:
	Client(void) = default;

	void Launch(const std::string& address, const std::string& port,
	    tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih) override;
    private:
	void UDPThread(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih);
	void TCPThread(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih);

	std::string ReceiveUsername(void);

	template<typename _Ty>
	_Ty Flags(bool* bytes, uint32_t size)
	{
	    _Ty flags = (_Ty)0;
	    for(uint32_t i = 0; i < sizeof(bytes) / sizeof(int8_t); ++i)
		flags += bytes[i] << bytes[i];
	    return flags;
	}
    private:
	std::unique_ptr<std::thread> m_TCPThread;
	std::unique_ptr<std::thread> m_UDPThread;
    };

}

#endif /* _CLIENT_H_ */
