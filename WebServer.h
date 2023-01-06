#ifndef WEBSERVER_H
#define WEBSERVER_H

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

    // HttpRequest Methods
    void get(char* path, callback);
    void head(char* path, callback);
    void post(char* path, callback);
    void put(char* path, callback);
    void del(char* path, callback);
    /*
        Method Name  | Description Section
       ------------------------------------------------------------------------------------------------------------------
         GET           Transfer a current representation of the target resource.	9.3.1
         HEAD          Same as GET, but do not transfer the response content.	9.3.2
         POST          Perform resource-specific processing on the request content.	9.3.3
         PUT           Replace all current representations of the target resource with the request content.	9.3.4
         DELETE        Remove all current representations of the target resource.	9.3.5
         CONNECT       Establish a tunnel to the server identified by the target resource.	9.3.6
         OPTIONS       Describe the communication options for the target resource.	9.3.7
         TRACE         Perform a message loop-back test along the path to the target resource.
    */

    void handle(String, HttpRequest*, HttpResponse*);

    //static DWORD WINAPI Worker(LPVOID lpParam);
};


#endif // WEBSERVER_H
