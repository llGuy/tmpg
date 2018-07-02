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
		tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph, tmpg::Platform& platform)
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

		m_UDPThread = std::make_unique<std::thread>([&] { UDPThread(eh, ih, ph, platform); });
	}

	void Server::ParseUDPMessage(Byte* data, uint32_t size, float gravity, tmpg::EntitiesHandler& eh, tmpg::Platform& platform) const
	{
		PacketParser parser(data, size);
		uint8_t type = parser.ReadNext<packetType_t>(CHAR_DELIMITER);

		switch (type)
		{
		case CLIENT_UPDATE: ParseClientUpdate(parser, gravity, eh, platform);
		};
	}

	// need to update the state of the client
	void Server::ParseClientUpdate(PacketParser& parser, float gravity, tmpg::EntitiesHandler& eh, tmpg::Platform& platform) const
	{
		uint16_t playerID = parser.ReadNext<uint16_t>(CHAR_DELIMITER);
		tmpg::Player& player = eh[playerID];
		uint16_t flags = parser.ReadNext<uint16_t>(CHAR_DELIMITER);
		player.Direction() = parser.ReadNext<glm::vec3>(CHAR_DELIMITER);
		bool keyFlags[16];
		// print the key flags
		for (uint32_t i = 0; i < 16; ++i)
		{
			keyFlags[i] = (flags >> i) & 0b00000001;
		}

		int32_t movement = 4;

		if (keyFlags[0]) { player.Move(tmpg::FORWARD, 0.03f, gravity); --movement; }
		if (keyFlags[1]) { player.Move(tmpg::LEFT, 0.03f, gravity); --movement; }
		if (keyFlags[2]) { player.Move(tmpg::BACKWARD, 0.03f, gravity); --movement; }
		if (keyFlags[3]) { player.Move(tmpg::RIGHT, 0.03f, gravity); --movement; }

		if (movement == 4 ) player.NeutralizeMomentum();

		if (keyFlags[4])  player.Move(tmpg::JUMP, 0.03f, gravity);
		if (keyFlags[5])  player.Move(tmpg::FALL, 0.03f, gravity);
		if (keyFlags[6])  platform.HandleAction(tmpg::SHIELD, player);

		if (keyFlags[7])  eh.PushBullet();
		if (keyFlags[8])  platform.HandleAction(tmpg::START_TERRAFORMING, player);
		else			  platform.HandleAction(tmpg::END_TERRAFORMING, player);
	}

	void Server::UDPThread(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph, tmpg::Platform& platform)
	{
		static constexpr uint32_t BUFFER_MAX_SIZE = 512;
		for (;;)
		{
			Byte messageBuffer[BUFFER_MAX_SIZE];
			auto pair = m_UDPSocket.ReceiveFrom(messageBuffer, BUFFER_MAX_SIZE);
			// parse message
			ParseUDPMessage(messageBuffer, pair.second, ph.Gravity(), eh, platform);
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
