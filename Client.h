#ifndef CLIENT_HEADER //Include Guard
#define CLIENT_HEADER

#include "Socket.h"
#include <stdio.h>

class Client{
public:
    Client(Socket*);
    void send(char*,int);
    void close_connection();
    // NOTE: I thinking about making the buffer as property in this class.
    int recv(char* buffer,unsigned int len);
    char* get_ip();
private:
    Socket* m_socket = nullptr;
    double total_bytes_send = 0;
    double total_bytes_recv = 0;
    bool is_connected = false;
};

#endif
