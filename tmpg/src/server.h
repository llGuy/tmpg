#ifndef _SERVER_H_
#define _SERVER_H_

#include <array>
#include <vector>
#include <unordered_map>

#include "net_handler.h"

namespace tmpg {

    class Entity;

}

namespace net {

    struct ClientData
    {
	ClientAddr address;
	uint32_t entityIndex;
    };

    class PacketParser;
    
    class Server
	: public NetworkHandler
    {
    public:
	Server(void) = default;

	void Launch(const std::string& address, const std::string& port,
	    	    tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph) override;
    private:
	void UDPThread(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph);
	// thread for communicating with each client
	void TCPThread(UnixSocket socket, tmpg::EntitiesHandler& eh,
		       tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph); // take socket by copy
	void TCPAcceptThread(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph);
	void ParseUDPMessage(Byte* data, uint32_t size, float gravity, tmpg::EntitiesHandler& eh) const;
	void ParseClientUpdate(PacketParser& parser, float gravity, tmpg::EntitiesHandler& eh) const;
    private:
	static constexpr uint32_t MAX_PENDING = 10;
	
	tmpg::EntitiesHandler* m_ehandler;
	std::unique_ptr<std::thread> m_TCPAcceptThread;
	std::vector<std::thread> m_TCPCommunicationThreads;
	std::unordered_map<uint16_t, ClientData> m_addresses;
    };

}

#endif /* _SERVER_H_ */
