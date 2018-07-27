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


	template <typename T>
	void ExtractFlags(T flags, T size, bool* dest)
	{
		for (T i = 0; i < size; ++i)
		{
			dest[i] = (flags >> i) & 0b00000001;
		}
	}


	// need to update the state of the client
	uint16_t Server::ParseClientUpdate(PacketParser& parser, float gravity, tmpg::EntitiesHandler& eh,
		tmpg::Platform& platform, tmpg::InputHandler& ih)
	{
		uint16_t playerID = parser.ReadNext<uint16_t>(CHAR_DELIMITER);
		uint64_t packet = parser.ReadNext<uint64_t>(CHAR_DELIMITER);
		auto& client = m_addresses[playerID];
		auto& history = client.history;
		client.entityIndex = playerID;
		client.packet = packet;
		tmpg::Player& player = eh[playerID];

		// input handling bich
		uint16_t sequenceSize = parser.ReadNext<uint16_t>(CHAR_DELIMITER);
		for (tmpg::Input i = parser.ReadNext<tmpg::Input>(CHAR_DELIMITER); !parser.Max(); i = parser.ReadNext<tmpg::Input>(CHAR_DELIMITER))
		{
			uint16_t compressed = i.flags;

			bool flags[10];
			ExtractFlags<uint16_t>(compressed, 10, flags);

			int32_t movement = 4;

			if (flags[0]) { player.Move(tmpg::FORWARD, i.time, gravity); --movement; }
			if (flags[1]) { player.Move(tmpg::LEFT, i.time, gravity); --movement; }
			if (flags[2]) { player.Move(tmpg::BACKWARD, i.time, gravity); --movement; }
			if (flags[3]) { player.Move(tmpg::RIGHT, i.time, gravity); --movement; }

			if (movement == 4) player.NeutralizeMomentum();

			if (flags[4]) { player.Move(tmpg::JUMP, i.time, gravity); }
			if (flags[5]) 
			{ 
				platform.HandleAction(tmpg::SHIELD, player); 
			}
			if (flags[6]) { player.Move(tmpg::FALL, i.time, gravity); }
			if (flags[7]) { eh.PushBullet(playerID); }
			if (flags[8]) 
			{ 
				platform.HandleAction(tmpg::START_TERRAFORMING, player); 
				platform.UpdateFP(player.Terraforming(), i.time);
//				platform.UpdateMesh();
				platform.UpdatedExternally() = true;
			}
			else platform.HandleAction(tmpg::END_TERRAFORMING, player);

			if (flags[9])
			{
				player.Direction() = tmpg::Camera::Look(player.Direction(), i.mouseMagnitude, 0.02f);
			}
		}

		history.Push({ packet, player.Position(), player.Direction() });

		return playerID;
	}

	void Server::UDPThread(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph, tmpg::Platform& platform)
	{
		static constexpr uint32_t BUFFER_MAX_SIZE = 2048;
		for (;;)
		{
			Byte messageBuffer[BUFFER_MAX_SIZE];
			auto pair = m_UDPSocket.ReceiveFrom(messageBuffer, BUFFER_MAX_SIZE);
			// parse message
			if (pair.second > 0)
			{
				auto player = ParseUDPMessage(messageBuffer, pair.second, ph.Gravity(), eh, platform, ih);
				auto clientData = m_addresses[player];
				clientData.address = pair.first;
				clientData.entityIndex = player; 
				UpdateClient(clientData.address, eh);
			}



			/*
			
			FUTURE LUC !!!! READ THIS   :

			MAKE SURE THAT THE SERVER DOESN'T ONLY UPDATE THE CLIENT WHEN A PACKET IS SENT
			BUT THAT IT DOES SO ALL THE TIME


			ALSO !!!!!!!! 
			MAKE SURE THAT THE SERVER KNOWS THE ADDRESS OF THE CLIENT BEFORE THE THE FIRST PACKET OF
			THE CLIENT IS SENT BECAUSE IT NEED TO UPDATE THE CLIENT AS SOON AS POSSIBLE !!!
			
			
			*/




		}
	}

	void Server::UpdateClient(const ClientAddr& address, tmpg::EntitiesHandler& eh)
	{
		PacketEncoder encoder;

		for (auto client = m_addresses.begin(); client != m_addresses.end(); ++client)
		{
			if (client->second.entityIndex != 0)
			{
				tmpg::Player& player = eh[client->second.entityIndex];
				uint16_t flags = Flags<uint16_t>(client->second.actions, 9u);
				std::cout << glm::to_string(player.Position()) << " ; " << glm::to_string(player.Direction()) << std::endl;
				encoder.PushBytes(client->first, client->second.packet, player.Position(), player.Direction(), flags);
			}
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