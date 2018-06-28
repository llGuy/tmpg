#ifdef _WIN32

#include "win_socket.h"

namespace net {

	WinSocket::WinSocket(SOCKET fd)
		: m_socket(fd)
	{
	}

	void WinSocket::InitSocket(const SocketInit& data)
	{
		m_socket = socket(data.family, data.socktype, data.protocol);
		if (m_socket < 0) std::cerr << "ERROR : unable to initialize socket" << std::endl;
		else std::cout << "SUCCESS : initialized socket properly" << std::endl;
	}

	void WinSocket::Bind(void)
	{
		int32_t err = bind(m_socket, m_ainfo->ai_addr, m_ainfo->ai_addrlen);
		if (err < 0) std::cerr << "ERROR : unable to bind to local address" << std::endl;
		else std::cout << "SUCCESS : bound to local address" << std::endl;
	}

	void WinSocket::Connect(void)
	{
		int32_t err = connect(m_socket, m_ainfo->ai_addr, m_ainfo->ai_addrlen);
		if (err < 0) std::cerr << "ERROR : unable to connect()" << std::endl;
		else std::cout << "SUCCESS : connected socket to address" << std::endl;
	}
	void WinSocket::Listen(int32_t max)
	{
		int32_t err = listen(m_socket, max);
	}
	WinSocket WinSocket::Accept(void) const
	{
		SOCKADDR_STORAGE addr;
		socklen_t addressSize = sizeof(addr);
		SOCKET socket = accept(m_socket, (SOCKADDR*)&addr, &addressSize);
		return WinSocket(socket);
	}

	void WinSocket::Send(const Byte* data, uint32_t dataSize)
	{
		int32_t bytes = send(m_socket, (const char*)data, dataSize, 0);
		if (bytes < 0) std::cerr << "ERROR : unable to send packet" << std::endl;
	}
	int32_t WinSocket::Receive(Byte* buffer, uint32_t maxSize, int32_t flags)
	{
		return recv(m_socket, (char*)buffer, maxSize, flags);
	}

	addrinfo WinSocket::FillCriteria(const SocketInit& data, int32_t flags)
	{
		addrinfo criteria;
		memset(&criteria, 0, sizeof(criteria));
		criteria.ai_family = data.family;
		criteria.ai_socktype = data.socktype;
		criteria.ai_protocol = data.protocol;
		criteria.ai_flags = flags;

		return criteria;
	}
	void WinSocket::Init(const SocketInit& data, int32_t flags, const char* address, const char* port)
	{
		InitSocket(data);
		auto criteria = FillCriteria(data, flags);
		addrinfo* addr;
		int err = getaddrinfo(address, port, &criteria, &addr);
		if (err != 0) std::cerr << "ERROR : unable to get address info" << std::endl;
		else
		{
			std::cout << "SUCCESS : got address info" << std::endl;
			m_ainfo = addr;
		}
	}

	void WinSocket::Free(void)
	{
		freeaddrinfo(m_ainfo);
	}

	std::pair<ClientAddr, int32_t> WinSocket::ReceiveFrom(Byte* data, int32_t maxSize)
	{
		ClientAddr client;
		socklen_t clientSize = sizeof(client.addr);
		int32_t bytes = recvfrom(m_socket, (char*)data, maxSize, 0, (SOCKADDR*)&client.addr, &clientSize);
		if (bytes < 0) std::cerr << "ERROR : recvfrom() failed" << std::endl;
		return { client, bytes };
	}

	void WinSocket::WSAPIInit(void)
	{
		WSADATA wsData;
		WORD ver = MAKEWORD(2, 2);
		auto ok = WSAStartup(ver, &wsData);
		if (ok != 0)
		{
			std::cerr << "ERROR : unable to initialize winsock API" << std::endl;
		}
	}

	void WinSocket::WSAPICleanup(void)
	{
		WSACleanup();
	}

	void WinSocket::Sendto(const Byte* data, uint32_t dataSize, addrinfo* address)
	{
		addrinfo* ainfo = (!address) ? m_ainfo : address;
		auto bytes = sendto(m_socket, (const char*)data, dataSize, 0, ainfo->ai_addr, ainfo->ai_addrlen);
	}

	void WinSocket::Sendto(const Byte* data, uint32_t dataSize, const sockaddr_in& address)
	{
		auto bytes = sendto(m_socket, (const char*)data, dataSize, 0, (SOCKADDR*)&address, sizeof(address));
	}

}

#endif