#ifndef _PACKET_TYPE_H_
#define _PACKET_TYPE_H_

namespace net {

    enum packetType_t
    {
	CLIENT_UPDATE = 0x21,
	CLIENT_USERNAME_UPDATE = 0x22,
	CLIENT_DISCONNECT_UPDATE = 0x23,

	SERVER_GAME_STATE = 0x30,
	SERVER_NEW_CONNECTED_PLAYER = 0x31,
    };

}

#endif /* _PACKET_TYPE_H_ */
