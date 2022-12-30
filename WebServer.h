#ifndef WEBSERVER_H
#define WEBSERVER_H

#endif // WEBSERVER_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include "base/String.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

#define KBYTE 1024
#define DEFAULT_BUFLEN KBYTE*8

typedef struct _SOCKET_INFORMATION {
   CHAR Buffer[DEFAULT_BUFLEN];
   WSABUF DataBuf;
   SOCKET Socket;
   WSAOVERLAPPED Overlapped;
   DWORD BytesSEND;
   DWORD BytesRECV;
} SOCKET_INFORMATION, * LPSOCKET_INFORMATION;

class WebServer {
private:
    typedef void (*callback)(HttpRequest*,HttpResponse*);
    HashTable<String, callback> handlers;
    // Dynamic handler is a way that let you handle a params from the path.
    struct DynamicHandler {
        String dynamic_path;
        callback call;
    };
    LinkedList<DynamicHandler> dynamic_handlers;
    LinkedList<String> static_paths;
    DWORD WORKER_ID;
public:
    WebServer();
    ~WebServer();



    void use_static_path(char*);
    void listen(const char port[]);
    void get(char* path, callback);
    void handle(String, HttpRequest*, HttpResponse*);

    //static DWORD WINAPI Worker(LPVOID lpParam);
};
