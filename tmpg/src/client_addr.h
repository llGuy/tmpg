#ifndef _CLIENT_ADDR_H_
#define _CLIENT_ADDR_H_

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

#endif /* _CLIENT_H_ */
