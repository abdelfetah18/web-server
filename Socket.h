#ifndef SOCKET_HEADER //Include Guard
#define SOCKET_HEADER


#include <stdio.h>
#include <iostream>

#ifdef _WIN32    
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif

    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <iphlpapi.h>

#else
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <string.h>
    #include <stdlib.h>
    #include <unistd.h>
#endif

class Socket {
public:
    Socket(const char*);
    Socket(int s);
    void init(); // For windows only
    void close();
	int socket();
	int bind();
	int listen();
    int accept();
    int recv(char* buffer,unsigned int len);
    int send(char* data,int len);
private:
    char port[7];
    #ifdef _WIN32
        SOCKET socket_id;
        struct addrinfo* addr_info;
        struct addrinfo hints;
    #else
        int socket_id;
        struct sockaddr_in serv_addr, cli_addr;
    #endif
};

#endif
