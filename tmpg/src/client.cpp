#include "client.h"
#include "timer.h"
#include "entities_handler.h"
#include "input_handler.h"
#include "packet_parser.h"
#include "packet_encoder.h"

#include <chrono>

namespace net {

    void Client::Launch(const std::string& addr, const std::string& port,
			tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih)
    {
	SocketInit initTCP { AF_INET, SOCK_STREAM, IPPROTO_TCP };
	m_TCPSocket.Init(initTCP, 0, addr.c_str(), port.c_str());
	m_TCPSocket.Connect();
	
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

    void Client::TCPThread(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih)
    {
	
    }

    void Client::UDPThread(tmpg::EntitiesHandler& eh, tmpg::InputHandler& ih)
    {
	Timer tickRateTracker;
	tickRateTracker.Start();
	
	for(;;)
	{
	    tickRateTracker.Reset();
	    bool keys[]
	    {
		ih.Key(GLFW_KEY_W),
		ih.Key(GLFW_KEY_A),
		ih.Key(GLFW_KEY_S),
		ih.Key(GLFW_KEY_D),
		ih.Key(GLFW_KEY_SPACE)
	    };

	    auto flags = Flags<uint8_t>(keys, sizeof(keys) / sizeof(bool));

	    // encode direction
	    
	    PacketEncoder encoder;
	    
	    // one tick is 30 seconds
	    // only sleep if time elapsed is less than 30 milliseconds
	    if(float elapsed = tickRateTracker.Elapsed(); elapsed < 0.03.0f)
	    {
		std::this_thread::sleep_for(std::chrono::seconds(elapsed));
	    }
	}
    }

}
