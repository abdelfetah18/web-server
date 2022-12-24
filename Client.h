#ifndef CLIENT_HEADER //Include Guard
#define CLIENT_HEADER

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

class Client{
public:
    Client(SOCKET);
    void send(char*,int);
    void close_connection();
    int recv();
    char* get_ip();
private:
    SOCKET _socket;
    DWORD total_bytes_send = 0;
    DWORD total_bytes_recv = 0;
    bool is_connected = false;
};

#endif
