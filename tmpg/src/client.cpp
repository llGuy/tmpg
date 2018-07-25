#include "client.h"
#include "timer.h"
#include "entities_handler.h"
#include "input_handler.h"
#include "physics_handler.h"
#include "packet_parser.h"
#include "packet_encoder.h"
#include "packet_type.h"

#include <chrono>
#include <glm/gtx/string_cast.hpp>

template <typename ... T>
auto print(T && ... items) -> void
{
	int8_t impls [] { ((std::cout << items), static_cast<int8_t>(0))... };
}

namespace net {

    void Client::Launch(const std::string& addr, const std::string& port,
			tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph, tmpg::Platform& platform)
    {
		m_packetID = 0;

		SocketInit initTCP{ AF_INET, SOCK_STREAM, IPPROTO_TCP };
		m_TCPSocket.Init(initTCP, 0, addr.c_str(), port.c_str());
		m_TCPSocket.Connect();
		
		auto username = ReceiveUsername();
		eh.PlayerBoundByCamera().Username() = username;
		std::cout << "client username is " << username << std::endl;
		
		SocketInit initUDP{ AF_INET, SOCK_DGRAM, IPPROTO_UDP };
		m_UDPSocket.Init(initUDP, 0, addr.c_str(), port.c_str());
		m_UDPSocket.Connect();
		
		m_UDPThread = std::make_unique<std::thread>([&] { UDPThread(eh, ih, ph); });
	}
	
	std::string Client::ReceiveUsername(void)
	{
		Byte bytes[512];
		auto size = m_TCPSocket.Receive(bytes, 512);
		PacketParser parser(bytes, size);
		m_clientID = parser.ReadNext<uint16_t>(CHAR_DELIMITER);
		return std::string("player") + std::to_string(m_clientID);
	}
	
	void Client::TCPThread(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph)
	{
		// may receive stuff like new players or username changes
	}
	
	void Client::UDPThread(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih, tmpg::physics::PhysicsHandler& ph)
	{
		tmpg::Timer tickRateTracker;
		tickRateTracker.Start();
		
		for (;;)
		{
			tickRateTracker.Reset();
			bool keys[]
			{
			ih.Key(GLFW_KEY_W),
			ih.Key(GLFW_KEY_A),
ih.Key(GLFW_KEY_S),
ih.Key(GLFW_KEY_D),
ih.Key(GLFW_KEY_SPACE),
ih.Key(GLFW_KEY_C),
ih.Key(GLFW_KEY_LEFT_SHIFT),

			ih.MouseButton(GLFW_MOUSE_BUTTON_1),
			ih.MouseButton(GLFW_MOUSE_BUTTON_2)
			};

	    auto flags = Flags<uint16_t>(keys, sizeof(keys) / sizeof(bool));

	    auto& player = eh.PlayerBoundByCamera();
	    auto& direction = player.Direction();
	    auto& username = player.Username();

	    // first put username, then key flags, then direction
	    PacketEncoder encoder;
	    // need to send the type of packet
//	    encoder.PushBytes(CLIENT_UPDATE, m_clientID, flags, direction);
		encoder.PushBytes(CLIENT_UPDATE, m_clientID, m_packetID++, flags, direction);

		print(static_cast<uint32_t>(CLIENT_UPDATE), " ", m_clientID, " ", static_cast<uint32_t>(flags),
			glm::to_string(direction), username);

	    m_UDPSocket.Send(encoder.Data(), encoder.Size());

	    // one tick is 30 seconds
	    // only sleep if time elapsed is less than 30 milliseconds
	    if (float elapsed = tickRateTracker.Elapsed(); elapsed < 0.03f)
	    {
		std::this_thread::sleep_for(std::chrono::duration<float, std::milli>((0.03f - elapsed) * 1000.0f));
	    }
	}
    }

}
