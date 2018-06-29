#include "server.h"
#include "entities_handler.h"
#include "input_handler.h"
#include "physics_handler.h"
#include "packet_encoder.h"
#include "packet_parser.h"
#include "entity.h"
#include "packet_type.h"

#include <glm/gtx/string_cast.hpp>

namespace net {

	void Server::Launch(const std::string& address, const std::string& port,
		tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph)
	{
		// initialize TCP thread and TCP socket
		SocketInit initTCP{ AF_INET, SOCK_STREAM, IPPROTO_TCP };
		m_TCPSocket.Init(initTCP, AI_PASSIVE, nullptr, port.c_str());
		m_TCPSocket.Bind();
		m_TCPSocket.Listen(MAX_PENDING);

		// tcp thread is the thread that accepts new connections
		m_TCPAcceptThread = std::make_unique<std::thread>([&] { TCPAcceptThread(eh, ih, ph); });

		// initialize UDP thread and UDP socket
		SocketInit initUDP{ AF_INET, SOCK_DGRAM, IPPROTO_UDP };
		m_UDPSocket.Init(initUDP, AI_PASSIVE, nullptr, port.c_str());
		m_UDPSocket.Bind();

		m_UDPThread = std::make_unique<std::thread>([&] { UDPThread(eh, ih, ph); });
	}

	void Server::ParseUDPMessage(Byte* data, uint32_t size, float gravity, tmpg::EntitiesHandler& eh) const
	{
		PacketParser parser(data, size);
		uint8_t type = parser.ReadNext<packetType_t>(CHAR_DELIMITER);

		switch (type)
		{
		case CLIENT_UPDATE: ParseClientUpdate(parser, gravity, eh);
		};
	}

	void Server::ParseClientUpdate(PacketParser& parser, float gravity, tmpg::EntitiesHandler& eh) const
	{
		uint16_t playerID = parser.ReadNext<uint16_t>(CHAR_DELIMITER);
		tmpg::Player& player = eh[playerID];
		uint8_t flags = parser.ReadNext<uint8_t>(CHAR_DELIMITER);
		player.Direction() = parser.ReadNext<glm::vec3>(CHAR_DELIMITER);
		bool keyFlags[5];
		// print the key flags
		for (uint32_t i = 0; i < 5; ++i)
		{
			keyFlags[i] = (flags >> i) & 0b00000001;
		}

		if (keyFlags[0])  player.Move(tmpg::FORWARD, 0.03f, gravity);
		if (keyFlags[1])  player.Move(tmpg::LEFT, 0.03f, gravity);
		if (keyFlags[2])  player.Move(tmpg::BACKWARD, 0.03f, gravity);
		if (keyFlags[3])  player.Move(tmpg::RIGHT, 0.03f, gravity);
	}

	void Server::UDPThread(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph)
	{
		static constexpr uint32_t BUFFER_MAX_SIZE = 512;
		for (;;)
		{
			Byte messageBuffer[BUFFER_MAX_SIZE];
			auto pair = m_UDPSocket.ReceiveFrom(messageBuffer, BUFFER_MAX_SIZE);
			// parse message
			ParseUDPMessage(messageBuffer, pair.second, ph.Gravity(), eh);
		}
	}

	void Server::TCPAcceptThread(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph)
	{
		for (;;)
		{
			auto socket = m_TCPSocket.Accept();
			std::cout << "new client has joined the game" << std::endl;
			eh.PushPlayer(glm::vec3(0.01f, 0.0f, 2.0f), glm::vec3(1.0f, 0.0f, 1.0f));
			m_TCPCommunicationThreads.emplace_back([&] { TCPThread(socket, eh, ih, ph); });
		}
	}

	void Server::TCPThread(Socket socket, tmpg::EntitiesHandler& eh,
		tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph)
	{
		PacketEncoder encoder;
		uint16_t id = eh.NumPlayers() - 1;
		encoder.PushBytes(id);
		socket.Send(encoder.Data(), encoder.Size());

		for (;;)
		{
			// receive and send
		}
	}

}
