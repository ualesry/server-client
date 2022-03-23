/*
 * Socket.cpp
 *
 *  Created on: 17 Mar 2022
 *      Author: andreas
 */

#include "Socket.h"

Socket::Socket()
	: socketHandler(INVALID_SOCKET_HANDLER)
{

}

Socket::~Socket()
{

}

class wrapperSocket: public Socket
{
	public:
	bool Connect(const char *ipAddr, short ipPort)
	{
		return false;
	}

	bool Bind(const char *ipAddr, short ipPort)
	{
		return false;
	}

	Socket* Accept()
	{
		return 0;
	}

	int Read(char *rdBuffer, int rdBufferSize)
	{
		return 0;
	}

	int Write(const char *wrBuffer, int numBytesToSend)
	{
		return 0;
	}

};
