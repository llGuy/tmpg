#include <iostream>

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
	
	uint16_t Server::ParseUDPMessage(Byte* data, uint32_t size, float gravity, 
		tmpg::EntitiesHandler& eh, tmpg::Platform& platform, tmpg::InputHandler& inputHandler)
	{
		PacketParser parser(data, size);
		uint8_t type = parser.ReadNext<packetType_t>(CHAR_DELIMITER);
		
		switch (type)
		{
		case CLIENT_UPDATE: return ParseClientUpdate(parser, gravity, eh, platform, inputHandler);
		};
	}

    // need to update the state of the client
    uint16_t Server::ParseClientUpdate(PacketParser& parser, float gravity, tmpg::EntitiesHandler& eh, 
		tmpg::Platform& platform, tmpg::InputHandler& ih) 
    {
		uint16_t playerID = parser.ReadNext<uint16_t>(CHAR_DELIMITER);
		uint64_t packet = parser.ReadNext<uint64_t>(CHAR_DELIMITER);
		auto& client = m_addresses[playerID];
		client.packet = packet;
		tmpg::Player& player = eh[playerID];
		uint16_t flags = parser.ReadNext<uint16_t>(CHAR_DELIMITER);
		player.Direction() = parser.ReadNext<glm::vec3>(CHAR_DELIMITER);

		if (ih.Key(GLFW_KEY_P))
		{
			std::cout << "direction : " << glm::to_string(player.Direction()) << std::endl;
			std::cout << "flags     : " << flags << std::endl;
			std::cout << "client id : " << playerID << std::endl;
		}

		// print the key flags
		for (uint32_t i = 0; i < 9; ++i)
		{
			client.actions[i] = (flags >> i) & 0b00000001;
		}
		
		int32_t movement = 4;
	
		if (client.actions[0]) { player.Move(tmpg::FORWARD, 0.03f, gravity); --movement; }
		if (client.actions[1]) { player.Move(tmpg::LEFT, 0.03f, gravity); --movement; }
		if (client.actions[2]) { player.Move(tmpg::BACKWARD, 0.03f, gravity); --movement; }
		if (client.actions[3]) { player.Move(tmpg::RIGHT, 0.03f, gravity); --movement; }
		
		if (movement == 4) player.NeutralizeMomentum();
		
		if (client.actions[4])  player.Move(tmpg::JUMP, 0.03f, gravity);
		if (client.actions[5])  platform.HandleAction(tmpg::SHIELD, player);
		if (client.actions[6])  player.Move(tmpg::FALL, 0.03f, gravity);
		
		if (client.actions[7])  eh.PushBullet(playerID);
		if (client.actions[8])  platform.HandleAction(tmpg::START_TERRAFORMING, player);
		else		  platform.HandleAction(tmpg::END_TERRAFORMING, player);
		
		return playerID;
	}
	
	void Server::UDPThread(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph, tmpg::Platform& platform)
	{
		static constexpr uint32_t BUFFER_MAX_SIZE = 512;
		for (;;)
		{
			Byte messageBuffer[BUFFER_MAX_SIZE];
			auto pair = m_UDPSocket.ReceiveFrom(messageBuffer, BUFFER_MAX_SIZE);
			// parse message
			auto player = ParseUDPMessage(messageBuffer, pair.second, ph.Gravity(), eh, platform, ih);
			auto clientData = m_addresses[player];
			clientData.address = pair.first;
			clientData.entityIndex = player;
			
			UpdateClient(clientData.address, eh);
		}
	}
	
	void Server::UpdateClient(const ClientAddr& address, tmpg::EntitiesHandler& eh)
	{
		PacketEncoder encoder;
		
		for (auto client = m_addresses.begin(); client != m_addresses.end(); ++client)
		{
			tmpg::Player& player = eh[client->second.entityIndex];
			uint16_t flags = Flags<uint16_t>(client->second.actions, 9u);
			encoder.PushBytes(client->first, client->second.packet, player.Position(), player.Direction(), flags);
		}
		
		m_UDPSocket.Sendto(encoder.Data(), encoder.Size(), (const sockaddr_in&)address.addr);
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
