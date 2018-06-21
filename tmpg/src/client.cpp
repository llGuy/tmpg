#include "client.h"
#include "packet_parser.h"

namespace net {

    Client::Client(tmpg::EntitiesHandler* ehandler)
	: m_ehandler(ehandler)
    {
    }

    void Client::Launch(const std::string& addr, const std::string& port)
    {
	SocketInit initTCP { AF_INET, SOCK_STREAM, IPPROTO_TCP };
	m_TCPSocket.Init(initTCP, 0, addr.c_str(), port.c_str());
	m_TCPSocket.Connect();
//	m_TCPThread = std::make_unique<std::thread>([this] { TCPThread(); });
	
	auto username = ReceiveUsername();
	std::cout << "client username is " << username << std::endl;

	SocketInit initUDP { AF_INET, SOCK_DGRAM, IPPROTO_UDP };
	m_UDPSocket.Init(initUDP, 0, addr.c_str(), port.c_str());
	m_UDPSocket.Connect();
    }

    std::string Client::ReceiveUsername(void)
    {
	Byte bytes[512];
	auto size = m_TCPSocket.Receive(bytes, 512);
	PacketParser parser(bytes, size);
	std::string username;
	parser.ReadNext(CHAR_DELIMITER, username);
	return username;
    }

    void Client::TCPThread(void)
    {

    }

    void Client::UDPThread(void)
    {
	
    }

}
