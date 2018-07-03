#ifndef _UNIX_SOCKET_H_
#define _UNIX_SOCKET_H_

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <utility>
#include <netdb.h>
#include <stdint.h>
#include <string.h>
#include <string>

#include "client_addr.h"

namespace net {

    struct SocketInit
    {
	int32_t family;
	int32_t socktype;
	int32_t protocol;
    };

    using Byte = uint8_t;

    class UnixSocket
    {
    public:
	UnixSocket(void) = default;
	UnixSocket(int32_t socket);

	void InitSocket(const SocketInit& data);

	void Bind(void);
	void Connect(void);
	void Listen(int32_t max);
	UnixSocket Accept(void) const;

	void Send(const Byte* data, uint32_t dataSize);
	int32_t Receive(Byte* data, uint32_t maxSize, int32_t flags = 0);

	void Sendto(const Byte* data, uint32_t dataSize, addrinfo* address = nullptr);
	void Sendto(const Byte* data, uint32_t dataSize, const sockaddr_in& address);

	std::pair<ClientAddr, int32_t> ReceiveFrom(Byte* data, int32_t maxSize);
    public:
	addrinfo FillCriteria(const SocketInit& data, int32_t flags);
	void Init(const SocketInit& data, int32_t, const char* address, const char* port);

	void Free(void);
    private:
	int32_t m_socket;
	addrinfo* m_ainfo;
	union
	{
	    sockaddr m_address;
	    sockaddr_in m_addressIn;
	};
    };

}

#endif /* _SOCKET_H_ */
