#include <iostream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>

#include "Socket.h"
 
static void usage();
int createSocket();
int bindSocket(int listenPort, int socket);
int listenForConnection(int socket);
int acceptConnection(int socket);
int writeMessage(int socket);
int readMessage(int socket);

int res;

int main(int argc, char *argv[])
{
    

    if (argc > 1 && *(argv[1]) == '-')
    {
        usage();
        return EXIT_FAILURE;
        
    }
 
    int listenPort = 1234;
    if (argc > 1)
        listenPort = atoi(argv[1]);
 
   
    int s0 = createSocket();     // Create a socket
 
    bindSocket(listenPort, s0);  // Port to listen and bind socket
 
    // Set the "LINGER" timeout to zero, to close the listen socket
    // immediately at program termination.
    struct linger linger_opt = { 1, 0 }; // Linger active, timeout 0
    setsockopt(s0, SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt));
 
    listenForConnection(s0);    // Now, listen for a connection
 
    // Accept a connection (the "accept" command waits for a connection with
    // no timeout limit...)
    int s1 = acceptConnection(s0);
 
    // A connection is accepted. The new socket "s1" is created
    // for data input/output. The clientAddr structure is filled in with
    // the address of connected entity, print it.
    // Close the listen socket
 
    writeMessage(s1);

    readMessage(s1);
    // return successful message or failure message
}
 
static void usage() {
    std::cout << "A simple Internet server application.\n"
              << "It listens to the port written in command line (default 1234),\n"
              << "accepts a connection, and sends the \"Hello!\" message to a client.\n"
              << "Then it receives the answer from a client and terminates.\n\n"
              << "Usage:\n"
              << "     server [port_to_listen]\n"
              << "Default is the port 1234.\n";
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

int bindSocket(int listenPort, int socket) {
    struct sockaddr_in listenAddrIn;
    memset(&listenAddrIn, 0, sizeof(struct sockaddr_in));
    listenAddrIn.sin_family = AF_INET;
    listenAddrIn.sin_addr.s_addr = htonl(INADDR_ANY);
    listenAddrIn.sin_port = htons(listenPort);

    int res = bind(socket, (struct sockaddr*) &listenAddrIn, sizeof(listenAddrIn));
    if (res < 0)
    {
        std::cerr << "Error binding socket to server address - " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}

int listenForConnection(int socket) {
    int res = listen(socket, 1);    // "1" is the maximal length of the queue
    if (res < 0)
    {
        std::cerr << "Error listening for clients -  " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}

int acceptConnection(int socket) {
    struct sockaddr_in clientAddr;
    socklen_t clientAddr_len = sizeof(clientAddr);
    int s1 = accept(socket, (struct sockaddr*) &clientAddr, &clientAddr_len);
    if (s1 < 0)
    {
    	close(socket);
        std::cerr << "Error while accepting clients - " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, ip, INET_ADDRSTRLEN);
    std::cout << "Connection from IP "
              << ip
              << " Port "
              << ntohs(clientAddr.sin_port)
			  << std::endl;
 
    res = close(socket);

    return s1;
}

int writeMessage(int socket) {
    const char request[] = "Hello!\r\n";
    int requestLen = (int)strlen(request);
    res = write(socket, request, requestLen);
    if (res != requestLen)
    {
    	close(socket);
        std::cerr << "Error writing to client - " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}

int readMessage(int socket) {
    const int bufferLen = 1023;
    char buffer[bufferLen + 1];
    res = read(socket, buffer, bufferLen);
    if (res < 0)
    {
    	close(socket);
        std::cerr << "Error reading from client - " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    buffer[res] = 0;
    std::cout << "Received " << res << " bytes:" << std::endl << buffer << std::endl;
    close(socket);
    return EXIT_SUCCESS;
}