#include "server.h"
#include "packet_encoder.h"

namespace net {

    Server::Server(tmpg::EntitiesHandler* ehandler)
	: m_ehandler(ehandler)
    {
    }

    void Server::Launch(const std::string& address, const std::string& port)
    {
	// initialize TCP thread and TCP socket
	SocketInit initTCP { AF_INET, SOCK_STREAM, IPPROTO_TCP };
	m_TCPSocket.Init(initTCP, AI_PASSIVE, nullptr, port.c_str());
	m_TCPSocket.Bind();
	m_TCPSocket.Listen(MAX_PENDING);

	// tcp thread is the thread that accepts new connections
	m_TCPAcceptThread = std::make_unique<std::thread>([this] { TCPAcceptThread(); });

	// initialize UDP thread and UDP socket
	SocketInit initUDP { AF_INET, SOCK_DGRAM, IPPROTO_UDP };
	m_UDPSocket.Init(initUDP, AI_PASSIVE, nullptr, port.c_str());
	m_UDPSocket.Bind();

	m_UDPThread = std::make_unique<std::thread>([this] { UDPThread(); });
    }

    void Server::UDPThread(void)
    {
	
    }

    void Server::TCPAcceptThread(void)
    {
	for(;;)
	{
	    auto socket = m_TCPSocket.Accept();
	    std::cout << "new client has joined the game" << std::endl;

	    m_TCPCommunicationThreads.emplace_back([this, &socket] { TCPThread(socket); });
	}
    }

    void Server::TCPThread(UnixSocket socket)
    {
	PacketEncoder encoder;
	std::string username = std::string("player") + std::to_string(m_ehandler->NumPlayers());
	encoder.PushString(username);

	socket.Send(encoder.Data(), encoder.Size());

	for(;;)
	{
	    // receive and send
	}
    }

}
