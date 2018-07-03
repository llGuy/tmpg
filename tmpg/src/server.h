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
	uint64_t packet;

	bool actions[9];
    };

    class PacketParser;

    class Server
	: public NetworkHandler
    {
    public:
	Server(void) = default;

	void Launch(const std::string& address, const std::string& port,
		    tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph, tmpg::Platform& platform) override;
    private:
	void UDPThread(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph, tmpg::Platform& platform);
	// thread for communicating with each client
	void TCPThread(Socket socket, tmpg::EntitiesHandler& eh,
		       tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph); // take socket by copy
	void TCPAcceptThread(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph);
	uint16_t ParseUDPMessage(Byte* data, uint32_t size, float gravity, tmpg::EntitiesHandler& eh, tmpg::Platform& platform);
	uint16_t ParseClientUpdate(PacketParser& parser, float gravity, tmpg::EntitiesHandler& eh, tmpg::Platform& platform);

	void UpdateClient(const ClientAddr& address, tmpg::EntitiesHandler& eh);

	template<typename _Ty>
	_Ty Flags(bool* bytes, uint32_t size)
	{
	    _Ty flags = (_Ty)0;
	    for (uint32_t i = 0; i < sizeof(bytes) / sizeof(int8_t); ++i)
	    {
		bool flag = bytes[i];
		flags += flag << i;
	    }
	    return flags;
	}
    private:
	static constexpr uint32_t MAX_PENDING = 10;

	tmpg::EntitiesHandler* m_ehandler;
	std::unique_ptr<std::thread> m_TCPAcceptThread;
	std::vector<std::thread> m_TCPCommunicationThreads;
	std::unordered_map<uint16_t, ClientData> m_addresses;
    };

}

#endif /* _SERVER_H_ */
