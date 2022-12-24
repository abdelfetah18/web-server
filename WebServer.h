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
    DWORD WORKER_ID;
public:
    WebServer(const char[]);
    ~WebServer();

    static DWORD WINAPI Worker(LPVOID lpParam);
};
