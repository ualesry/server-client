#include <iostream>
#include <sstream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "Socket.h"
 
static void usage();
int createSocket();
int connectToServer(int socket, struct sockaddr_in serverAddrIn);
int readFromServer(int socket);
int writeToServer(int socket);

int res;
int main(int argc, const char *argv[])
{
    if (argc > 1 && *(argv[1]) == '-')
    {
        usage();
        return EXIT_FAILURE;
    }
 
    const char* serverAddr = "127.0.0.1";
    if (argc > 1)
        serverAddr = argv[1];

    short serverPort = 1234;
    if (argc >= 3)
        serverPort = (short) atoi(argv[2]);

    // Create socket

    int s0 = createSocket();
 
    // Fill in server IP address
    struct sockaddr_in serverAddrIn;
    serverAddrIn.sin_family = AF_INET;
    inet_pton(AF_INET, serverAddr, &serverAddrIn.sin_addr);
    serverAddrIn.sin_port = htons(serverPort);
 
    // Print a resolved address of server (the first IP of the host)
    std::cout << "server address = "
    		  << serverAddr
			  << ", port "
			  << static_cast<int>(serverPort)
			  << std::endl;

    // Connect to the remote server
    connectToServer(s0, serverAddrIn);

    //reading from server
    readFromServer(s0);

    //Writing to server
    writeToServer(s0);
}
 
static void usage()
{
    std::cout << "A simple Internet client application.\n"
              << "Usage:\n"
              << "         client [IP_address_of_server [port_of_server]]\n"
              << "     where IP_address_of_server is the IP number of server\n"
              << "     port_of_server is a port number, default is 1234.\n"
              << "The client connects to a server which address is given in a\n"
              << "command line, receives a message from a server, sends the message\n"
              << "\"Thanks! Bye-bye...\", and terminates.\n";
}

int createSocket() {
    int s0 = socket(AF_INET, SOCK_STREAM, 0);
    if (s0 < 0)
    {
        std::cerr << "Error creating socket - " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }
    return s0;
}

int connectToServer(int socket, struct sockaddr_in serverAddrIn) {
    int res = connect(socket, (struct sockaddr*) &serverAddrIn, sizeof(serverAddrIn));
    if (res < 0)
    {
    	close(socket);
        std::cerr << "Error connecting to server - " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }
 
    std::cout << "Connected. Reading a server message" << std::endl;

    return 0;
}

int readFromServer(int socket) {
    const int bufferLen = 1023;
    char buffer[bufferLen + 1];
    res = read(socket, buffer, bufferLen);
    if (res < 0)
    {
    	close(socket);
        std::cerr << "Error could not read from server - " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }
 
    buffer[res] = 0;
    std::cout << "Received:" << std::endl << buffer << std::endl;

    return 0;
}

int writeToServer(int socket) {
    const char reply[] = "Thanks! Bye-bye...\r\n";
    int replyLen = (int)strlen(reply);
    res = write(socket, reply, replyLen);
    if (res != replyLen)
    {
    	close(socket);
        std::cerr << "Error writing to server - " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }
 
    close(socket);

    return EXIT_SUCCESS;
}