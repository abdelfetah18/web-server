#ifndef WEBSERVER_H
#define WEBSERVER_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif


#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#endif


#include <stdio.h>
#include "base/String.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "base/Vector.h"
#include "PathHandler.h"

#define KBYTE 1024
#define DEFAULT_BUFLEN KBYTE*8


class WebServer {
public:
    WebServer();
    ~WebServer();

    void use_static_path(const char*);
    void listen(const char port[]);

    void register_callback(const char* path,const char* method,callback call);

    // HttpRequest Methods
    void get(const char* path, callback);
    void head(const char* path, callback);
    void post(const char* path, callback);
    void put(const char* path, callback);
    void del(const char* path, callback);
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

    void handle(HttpRequest*, HttpResponse*);
    void handle_static_path(String, HttpRequest*, HttpResponse*);
    bool handle_path(HttpRequest* req, HttpResponse* res);

    //static DWORD WINAPI Worker(LPVOID lpParam);
    bool is_static_path(String path);
private:
    HashTable<String, String> supported_mime_types;

    LinkedList<PathHandler*> handlers;
    LinkedList<String> static_paths;

    

    #ifdef _WIN32
    DWORD WORKER_ID;
    #else
    pthread_t WORKER_ID;
    #endif
};


#endif // WEBSERVER_H
