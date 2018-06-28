#ifndef _SOCKET_H_
#define _SOCKET_H_

#ifdef _WIN32

#include "win_socket.h"

namespace net {

	using Socket = WinSocket;

}

#else

#include "unix_socket.h"

namespace net {

	using Socket = UnixSocket;

}

#endif

#endif