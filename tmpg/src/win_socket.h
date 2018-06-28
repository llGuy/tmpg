#ifndef _WIN_SOCKET_H_
#define _WIN_SOCKET_H_

#include <WS2tcpip.h>
#include <iostream>
#include <utility>

#include "client_addr.h"

#pragma comment (lib, "ws2_32.lib")

namespace net {

	struct SocketInit
	{
		int32_t family;
		int32_t socktype;
		int32_t protocol;
	};

	using Byte = uint8_t;

	class WinSocket
	{
	public:
		WinSocket(void) = default;
		WinSocket(SOCKET fd);

		void InitSocket(const SocketInit& data);

		void Bind(void);
		void Connect(void);
		void Listen(int32_t max);
		WinSocket Accept(void) const;
		
		void Send(const Byte* data, uint32_t dataSize);
		int32_t Receive(Byte* buffer, uint32_t maxSize, int32_t flags = 0);

		std::pair<ClientAddr, int32_t> ReceiveFrom(Byte* data, int32_t maxSize);

		addrinfo FillCriteria(const SocketInit& data, int32_t flags);
		void Init(const SocketInit& data, int32_t, const char*, const char*);

		void Free(void);

		void Sendto(const Byte* data, uint32_t dataSize, addrinfo* address = nullptr);
		void Sendto(const Byte* data, uint32_t dataSize, const sockaddr_in& address);

	public:
		static void WSAPIInit(void);
		static void WSAPICleanup(void);
	private:
		SOCKET m_socket;
		addrinfo* m_ainfo;
		union
		{
			SOCKADDR m_address;
			SOCKADDR_IN m_addressIn;
		};
	};

}

#endif