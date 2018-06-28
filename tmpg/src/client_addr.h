#ifndef _CLIENT_ADDR_H_
#define _CLIENT_ADDR_H_

#ifdef _WIN32

#include <WS2tcpip.h>

namespace net {

	struct ClientAddr
	{
		SOCKADDR_STORAGE addr;
	};

}

#else

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdint.h>

namespace net {

    struct ClientAddr
    {
	sockaddr_storage addr;
    };

}

#endif

#endif /* _CLIENT_H_ */
