#include "WebServer.h"
#include "Client.h"

typedef struct {
    WebServer* web_server;
    SOCKET client;
} Thread_Params;

static DWORD WINAPI Worker(void* args);

WebServer::WebServer(){ }

void WebServer::listen(const char port[]){
    int iResult;
    WSADATA wsaData;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return;
    }

    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    iResult = getaddrinfo(NULL, port, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return;
    }

    SOCKET ListenSocket = INVALID_SOCKET;
    // Create a SOCKET for the server to listen for client connections

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    freeaddrinfo(result);

    if (::listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
        printf( "Listen failed with error: %ld\n", WSAGetLastError() );
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    printf("Server is up running on port: %s\n", port);

    while(TRUE){
        SOCKET ClientSocket = INVALID_SOCKET;

        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return;
        }

        Thread_Params* th_pr = new Thread_Params;
        th_pr->web_server = this;
        th_pr->client = ClientSocket;

        CreateThread(NULL,0,Worker, th_pr,0, &WORKER_ID);
    }
    return;
}

WebServer::~WebServer(){
    return;
}

void WebServer::use_static_path(const char* path){
    static_paths.append(String(path));
}

// a dynamic path is a path that start with column character ':' to indicate its can be changable.
bool is_dynamic_path(const char* path){
    uint index = 0;
    while(path[index] != '\0'){
        if(path[index] == '/' && path[index+1] == ':'){
            return true;
        }
        index += 1;
    }
    return false;
}

void WebServer::get(const char* path, callback call){
    String key("GET:");
    key.push(path);
    // register a callback for that path
    if(is_dynamic_path(path)){
        DynamicHandler handler;
        handler.call = call;
        handler.dynamic_path = key;
        dynamic_handlers.append(handler);
    }else{
        handlers.set( key, call);
    }
}

void WebServer::head(const char *path, callback call){
    String key("HEAD:");
    key.push(path);
    // register a callback for that path
    if(is_dynamic_path(path)){
        DynamicHandler handler;
        handler.call = call;
        handler.dynamic_path = key;
        dynamic_handlers.append(handler);
    }else{
        handlers.set( key, call);
    }
}

void WebServer::post(const char *path, callback call){
    String key("POST:");
    key.push(path);
    // register a callback for that path
    if(is_dynamic_path(path)){
        DynamicHandler handler;
        handler.call = call;
        handler.dynamic_path = key;
        dynamic_handlers.append(handler);
    }else{
        handlers.set( key, call);
    }
}

void WebServer::put(const char *path, callback call){
    String key("PUT:");
    key.push(path);
    // register a callback for that path
    if(is_dynamic_path(path)){
        DynamicHandler handler;
        handler.call = call;
        handler.dynamic_path = key;
        dynamic_handlers.append(handler);
    }else{
        handlers.set( key, call);
    }
}

void WebServer::del(const char *path, callback call){
    String key("DELETE:");
    key.push(path);
    // register a callback for that path
    if(is_dynamic_path(path)){
        DynamicHandler handler;
        handler.call = call;
        handler.dynamic_path = key;
        dynamic_handlers.append(handler);
    }else{
        handlers.set( key, call);
    }
}

// TODO: improve the algorithm
bool match_dynamic_path(String static_path,String dynamic_path,HashTable<String,String> &params){
    uint i = 0;
    uint j = 0;
    uint static_path_len = static_path.length();
    uint dynamic_path_len = dynamic_path.length();
    char* a = static_path.get();
    char* b = dynamic_path.get();
    // we match the req method
    while(a[i] != ':'){
        if(a[i] != b[j])
            return false;
        i += 1;
        j += 1;
    }

    i += 1;
    j += 1;

    // algorithm to math the dynamic paths with the requested paths.
    while(i < static_path_len && j < dynamic_path_len){
        String a_1;
        String b_1;
        if(a[i] == '/'){
            i += 1;
            while(i < static_path_len && a[i] != '/'){
                a_1.push(a[i]);
                i += 1;
            }
        }

        if(b[j] == '/'){
            j += 1;
            while(j < dynamic_path_len && b[j] != '/'){
                b_1.push(b[j]);
                j += 1;
            }
        }

        if(!b_1.startWith(":")){
            if(!(a_1 == b_1)){
                return false;
            }
        }else{
            params.set(String(b_1.get() + 1),a_1);
        }
    }
    if((static_path_len - i) == 0 && (dynamic_path_len - j) == 0 ){
        return true;
    }else{
        if(static_path_len - i == 1 && a[i] == '/'){
            return true;
        }
        return false;
    }
}

void WebServer::handle(String key, HttpRequest* req, HttpResponse* res){
    // NOTE: this is not a good way to extract a path from a String
    char* str = key.get();
    while(*(str++) != ':'){ }
    str++;
    String path(str);

    bool found = false;

    LinkedListIterator<LinkedList<String>::Bucket> iterator(static_paths.get_head());
    while(!iterator.is_end()){
        if(path.startWith(iterator.node()->value)){
            found = true;
            break;
        }
        iterator.increment();
    }

    if(found && key.startWith("GET")){
        // readfiles and send response
        // Open a handle to the file
         HANDLE hFile = CreateFile(path.get(), GENERIC_READ,0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

         if (hFile == INVALID_HANDLE_VALUE) {
             printf("\nINVALID_HANDLE_VALUE %d\n", GetLastError());
           // An error occurred
             res->status(404);
             res->setHeader("Server","abdelfetah-dev");
             res->send("<h1>404 Not Found!</h1>");
             return;
         }

         // Determine the size of the file
         DWORD fileSize = GetFileSize(hFile, NULL);

         // Allocate a buffer to hold the file contents
         char* buffer = new char[fileSize+1];

         // Read the contents of the file
         DWORD bytesRead;
         bool success = ReadFile(hFile, buffer, fileSize, &bytesRead, nullptr);

         if (!success) {
             printf("\nReadFile failed! %d\n", GetLastError());
           // An error occurred
             res->status(404);
             res->setHeader("Server","abdelfetah-dev");
             res->send("<h1>404 Not Found!</h1>");
             return;
         }

         // TODO: implement a way to map file extensions on its mimeType.

         // determine a mimeType by its extension.
        String ext(path.get() + path.indexOf('.'));
        ext.to_lower_case();
        // .png
        bool is_set = false;
        if(ext.startWith(String(".png"))){
            res->setHeader("Content-Type","image/png");
            is_set = true;
        }

        // .jpg, .jpeg, .jfif, .pjpeg, .pjp
        if(ext.startWith(String(".jpg")) || ext.startWith(String(".jpeg")) || ext.startWith(String(".jfif")) || ext.startWith(String(".pjpeg")) || ext.startWith(String(".pjp"))){
            res->setHeader("Content-Type","image/jpeg");
            is_set = true;
        }

        if(ext.startWith(String(".html"))){
            res->setHeader("Content-Type", "text/html");
            is_set = true;
        }

        if(ext.startWith(String(".js"))){
            res->setHeader("Content-Type", "text/javascript");
            is_set = true;
        }

        if(ext.startWith(String(".json"))){
            res->setHeader("Content-Type", "application/json");
            is_set = true;
        }

        if(ext.startWith(String(".mp4"))){
            res->setHeader("Content-Type","video/mp4");
            is_set = true;
        }

        // if files exist with unkonws mediaType.
        if(!is_set){
            res->setHeader("Content-Type","application/octet-stream");
            is_set = true;
        }

        res->status(200);
        res->send(buffer, fileSize);

        // Free the buffer
        delete[] buffer;

        // Close the handle to the file
        CloseHandle(hFile);
    }else{
        // TODO: improve the algorithm
        bool found = false;
        // first we check in dynamic handlers
        LinkedListIterator<LinkedList<DynamicHandler>::Bucket> iterator(dynamic_handlers.get_head());
        while(!iterator.is_end()){
            if(match_dynamic_path(key, iterator.node()->value.dynamic_path, req->params)){
                iterator.node()->value.call(req,res);
                found = true;
                break;
            }
            iterator.increment();
        }

        // and if we didnt find anything then we check in static handlers
        if(!found){
            callback call;
            bool exist = handlers.get(key, call);
            if(exist)
                call(req,res);
            else{
                res->status(404);
                res->setHeader("Server","abdelfetah-dev");
                res->send("<h1>404 Not Found!</h1>");
            }
        }
    }
}

DWORD WINAPI Worker(void* args){
    Thread_Params* params = (Thread_Params*) args;
    WebServer* Server = params->web_server;
    SOCKET m_client = params->client;
    printf("\nsocket_id: %d\n", m_client);
    char* buffer[DEFAULT_BUFLEN];
    ZeroMemory( buffer, DEFAULT_BUFLEN);

    DWORD BytesRECV = 0;

    if((BytesRECV = ::recv(m_client, (char*) buffer, DEFAULT_BUFLEN, 0)) == SOCKET_ERROR){
        if(WSAGetLastError() != WSA_IO_PENDING){
            printf("recv() failed with error %d\n", WSAGetLastError());
            return 1;
        }
    }

    Client client(m_client);

    HttpRequest req((char*) buffer, client);
    HttpResponse res(client);

    int is_valid = req.parse();

    if(is_valid != 0){
        printf("parsing error: %d\n", is_valid);
        res.status(400);
        res.send("Bad Request!");
    }else{


        String key(req.getMethod());
        key.push(":");
        key.push(req.getPath());
        key.show();
        Server->handle(key, &req, &res);
        printf("\n");
    }
    return 0;
}
