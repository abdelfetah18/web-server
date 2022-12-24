#include "Client.h"

// FIXME: here i assume that the socket already connected, and i think this is the wrong way to do it.
Client::Client(SOCKET s):_socket(s),is_connected(true){ };

void Client::close_connection(){
    is_connected = false;
    closesocket(_socket);
}

void Client::send(char* data,int len){
    int iSendResult = ::send(_socket, data, len, 0);
    close_connection();
}

char* Client::get_ip(){
    // TODO: create a ip_address property which will initiated in th constructor
    sockaddr name;
    ZeroMemory(&name, sizeof(sockaddr));
    int namelen = sizeof(sockaddr);
    int is_valid = getpeername(_socket, &name,&namelen);
    if(is_valid == SOCKET_ERROR)
        printf("error_code: %d\n", WSAGetLastError());


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
