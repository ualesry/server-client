/*
 * Socket.h
 *
 *  Created on: 17 Mar 2022
 *      Author: andreas
 */

#ifndef SOCKET_H_
#define SOCKET_H_

class Socket
{
public:
	Socket();
	~Socket();

	bool Connect(const char *ipAddr, short ipPort);
	bool Bind(const char *ipAddr, short ipPort);
	Socket* Accept();

	int Read(char *rdBuffer, int rdBufferSize);
	int Write(const char *wrBuffer, int numBytesToSend);

private:
static const int INVALID_SOCKET_HANDLER = -1;

	int socketHandler;
};

#endif /* SOCKET_H_ */
