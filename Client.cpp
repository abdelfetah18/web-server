#include "Client.h"

// FIXME: here i assume that the socket already connected, and i think this is the wrong way to do it.
Client::Client(Socket* s):m_socket(s),is_connected(true){ };

void Client::close_connection(){
    is_connected = false;
    
    // FIXME: implement a close_socket in the Socket class
}

void Client::send(char* data,int len){
    m_socket->send(data, len);
    close_connection();
}

int Client::recv(char* buffer,unsigned int len){
    m_socket->recv(buffer, len);
    return 0;
}

char* Client::get_ip(){
    // TODO: create a ip_address property which will initiated in th constructor
    /*sockaddr name;
    ZeroMemory(&name, sizeof(sockaddr));
    int namelen = sizeof(sockaddr);
    int is_valid = getpeername(m_socket, &name,&namelen);
    if(is_valid == SOCKET_ERROR)
        printf("error_code: %d\n", WSAGetLastError());
    */

    // FIXME: find a way to get a port from struct sockaddr .sa_data member
    /*
    short int port = 0;
    char* p = (char*) &port;

    p[0] = name.sa_data[1];
    p[1] = name.sa_data[0];

    printf("port: %hu\n", port);


    for(int i=0; i < 14; i++){
        printf("%x ", *(name.sa_data+i));
    }
*/
    return nullptr;
}
