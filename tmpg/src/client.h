#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <thread>
#include <memory>
#include "physics_handler.h"
#include "net_handler.h"

namespace net {

    class Client
	: public NetworkHandler
    {
    public:
	Client(void) = default;

	mode Mode(void) override { return CLIENT; }
	
	void Launch(const std::string& address, const std::string& port,
		    tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph, tmpg::Platform& platform) override;
    private:
	void UDPThread(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph);
	void TCPThread(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph);
	void ReceiveServerUpdate(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph);

	std::string ReceiveUsername(void);

	template<typename _Ty>
	_Ty Flags(bool* bytes, uint32_t size)
	    {
		_Ty flags = (_Ty)0;
		for (uint32_t i = 0; i < size; ++i)
		{
		    bool flag = bytes[i];
		    flags += flag << i;
		}
		return flags;
	    }
    private:
	std::unique_ptr<std::thread> m_TCPThread;
	std::unique_ptr<std::thread> m_UDPThread;

	// client id that the server uses to map clients' addresses
	uint16_t m_clientID;
	uint64_t m_packetID;
    };

}

#endif /* _CLIENT_H_ */
