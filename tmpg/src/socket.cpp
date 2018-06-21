#include "socket.h"

namespace net {

    UnixSocket::UnixSocket(int32_t socket)
	: m_socket(socket)
    {
    }

    void UnixSocket::InitSocket(const SocketInit& data)
    {
	m_socket = socket(data.family, data.socktype, data.protocol);
	if(m_socket < 0) std::cerr << "ERROR : unable to initialize socket" << std::endl;
	else std::cout << "SUCCESS : initialized socket properly" << std::endl;
    }

    void UnixSocket::Bind(void)
    {
	int32_t err = bind(m_socket, m_ainfo->ai_addr, m_ainfo->ai_addrlen);
	if(err < 0) std::cerr << "ERROR : unable to bind to local address" << std::endl;
	else std::cout << "SUCCESS : bound to local address" << std::endl;
    }

    void UnixSocket::Connect(void)
    {
	int32_t err = connect(m_socket, m_ainfo->ai_addr, m_ainfo->ai_addrlen);
	if(err < 0) std::cerr << "ERROR : unable to connect()" << std::endl;
	else std::cout << "SUCCESS : connected socket to address";
    }
    
    void UnixSocket::Listen(int32_t max)
    {
	int32_t err = listen(m_socket, max);
    }
    
    UnixSocket UnixSocket::Accept(void) const
    {
	sockaddr_storage addr;
	socklen_t addressSize = sizeof(addr);
	int32_t socket = accept(m_socket, (sockaddr*)&addr, &addressSize);
	return UnixSocket(socket);
    }

    void UnixSocket::Send(const Byte* data, uint32_t dataSize)
    {
	int32_t bytes = send(m_socket, data, dataSize, 0);
	if(bytes < 0) std::cerr << "ERROR : unable to send packet" << std::endl;
    }
    
    int32_t UnixSocket::Receive(Byte* data, uint32_t maxSize, int32_t flags)
    {
	return recv(m_socket, data, maxSize, flags);
    }

    void UnixSocket::Sendto(const Byte* data, uint32_t dataSize, addrinfo* address)
    {
	
    }
    
    void UnixSocket::Sendto(const Byte* data, uint32_t dataSize, const sockaddr_in& address)
    {

    }

    addrinfo UnixSocket::FillCriteria(const SocketInit& data, int32_t flags)
    {
	addrinfo criteria;
	memset(&criteria, 0, sizeof(criteria));
	criteria.ai_family = data.family;
	criteria.ai_socktype = data.socktype;
	criteria.ai_protocol = data.protocol;
	criteria.ai_flags = flags;

	return criteria;
    }
    
    void UnixSocket::Init(const SocketInit& data, int32_t flags, const char* address, const char* port)
    {
	InitSocket(data);
	addrinfo criteria = FillCriteria(data, flags);
	addrinfo* addr;
	int err = getaddrinfo(address, port, &criteria, &addr);
	if(err != 0) std::cerr << "ERROR : unable to get address info" << std::endl;
	else
	{
	    std::cout << "SUCCESS : got address info";
	    m_ainfo = addr;
	}
    }

    void UnixSocket::Free(void)
    {
	freeaddrinfo(m_ainfo);
    }

    std::pair<ClientAddr, int32_t> UnixSocket::ReceiveFrom(Byte* data, int32_t maxSize)
    {
	ClientAddr client;
	socklen_t clientSize = sizeof(client.addr);
	int32_t bytes = recvfrom(m_socket, data, maxSize, 0, (sockaddr*)&client.addr, &clientSize);
	if(bytes < 0) std::cerr << "ERROR : recvfrom() failed" << std::endl;
	return { client, bytes };
    }

}
