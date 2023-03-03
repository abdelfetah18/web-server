#include "Socket.h"

Socket::Socket(const char* p){
    for(int i = 0; i < 6; i++)
        port[i] = p[i];
    port[6] = '\0';
    init();
}

Socket::Socket(int s):socket_id(s) { }

// only windows
void Socket::init(){
    #ifdef _WIN32
    WSADATA wsaData;

    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return;
    }

    addr_info = nullptr;

    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    iResult = getaddrinfo(NULL, port, &hints, &addr_info);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return;
    }

    return;
    #endif
}

// FIXME: move this error function to another place
void error(const char *msg)
{
    perror(msg);
    exit(1);
}


int Socket::socket(){
    #ifdef _WIN32
    
    socket_id = INVALID_SOCKET;
    // Create a SOCKET for the server to listen for client connections

    socket_id = ::socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol);

    if (socket_id == INVALID_SOCKET) {
        printf("Error at socket(): %d\n", WSAGetLastError());
        freeaddrinfo(addr_info);
        WSACleanup();
        return (int) INVALID_SOCKET;
    }

    return socket_id;
    #else
    
    socket_id = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket_id < 0) 
        error("ERROR opening socket");
    
    #endif
}

int Socket::bind(){
    #ifdef _WIN32
    
    // Setup the TCP listening socket
    int iResult = ::bind( socket_id, addr_info->ai_addr, (int)addr_info->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(addr_info);
        closesocket(socket_id);
        WSACleanup();
        return SOCKET_ERROR;
    }

    freeaddrinfo(addr_info);

    return socket_id;
    
    #else

    bzero((char *) &serv_addr, sizeof(serv_addr));
    int portno = atoi(port);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if(::bind(socket_id, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding"); 

    #endif
}

int Socket::listen(){
    #ifdef _WIN32

    if (::listen( socket_id, SOMAXCONN ) == SOCKET_ERROR ) {
        printf( "Listen failed with error: %d\n", WSAGetLastError() );
        closesocket(socket_id);
        WSACleanup();
        return -1;
    }
    
    return 0;

    #else

    // FIXME: set the backlogs to its maximun value.
    ::listen(socket_id, 0xffff);

    #endif

    printf("Server is up running on port: %s\n", port);
}

int Socket::accept(){
    #ifdef _WIN32

    SOCKET client_socket = INVALID_SOCKET;

    // Accept a client socket
    client_socket = ::accept(socket_id, NULL, NULL);
    if (client_socket == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        closesocket(socket_id);
        WSACleanup();
        return (int) INVALID_SOCKET;
    }

    return client_socket;

    #else

    socklen_t clilen = sizeof(cli_addr);
    int client_socket = ::accept(socket_id, (struct sockaddr *) &cli_addr, &clilen);
    if (client_socket < 0) 
        error("ERROR on accept");

    return client_socket;

    #endif
}

int Socket::send(char* data,int len){
    #ifdef _WIN32
     
        int iSendResult = ::send(socket_id, data, len, 0);
        return iSendResult;
    
    #else

    int bytes_sent = ::send(socket_id, data, len, 0);
    return bytes_sent;

    #endif
}
int Socket::recv(char* buffer,unsigned int len){
    printf("client_socket: %d\n", socket_id);
    #ifdef _WIN32
    int total_bytes_recv;
    if((total_bytes_recv = ::recv(socket_id, (char*) buffer, len, 0)) == SOCKET_ERROR){
        printf("recv() failed with error %d\n", WSAGetLastError());
        return SOCKET_ERROR;
    }
    return total_bytes_recv;

    #else

    int bytes_recv = ::recv(socket_id, buffer, len, 0);
    return bytes_recv;
    
    #endif
}
