#include "WebServer.h"
#include "Client.h"

#include "Socket.h"
#include "File.h"

typedef struct {
    WebServer* web_server;
    int client;
} Thread_Params;

#ifdef _WIN32

#define THREAD_FUNC static DWORD WINAPI

#else

#include "pthread.h"
#define THREAD_FUNC static void*

#endif

THREAD_FUNC Worker(void* args);

WebServer::WebServer(){
    // Init Supported Mime Types HashTable.
    supported_mime_types.set(".png","image/png");
    supported_mime_types.set(".jpg","image/jpeg");
    supported_mime_types.set(".jpeg","image/jpeg");
    supported_mime_types.set(".jfif","image/jpeg");
    supported_mime_types.set(".pjpeg","image/jpeg");
    supported_mime_types.set(".pjp","image/jpeg");
    supported_mime_types.set(".html","text/html");
    supported_mime_types.set(".js","text/javascript");
    supported_mime_types.set(".json","application/json");
    supported_mime_types.set(".mp4","video/mp4");
}

void WebServer::listen(const char port[]){

    Socket my_socket(port);

    // Create a SOCKET for the server to listen for client connections
    my_socket.socket();
    
    // Setup the TCP listening socket
    my_socket.bind();

    // Listen
    my_socket.listen();

    while(true){
        // Accept connection
        int _client = my_socket.accept();

        // Create thread for each connection
        Thread_Params* th_pr = new Thread_Params;
        th_pr->web_server = this;
        th_pr->client = _client;

        #ifdef _WIN32
        CreateThread( NULL, 0, Worker, th_pr,0, &WORKER_ID);
        #else
        pthread_create(&WORKER_ID, nullptr, Worker, th_pr);
        #endif
    }
    return;
}

WebServer::~WebServer(){
    return;
}

void WebServer::use_static_path(const char* path){
    static_paths.append(String(path));
}



void WebServer::register_callback(const char* path,const char* method,callback call){
    handlers.append(new PathHandler(String(path), String(method), call));
}

void WebServer::get(const char* path,callback call){
    register_callback(path, "GET", call);
}

void WebServer::head(const char *path, callback call){
    register_callback(path, "HEAD", call);
}

void WebServer::post(const char *path, callback call){
    register_callback(path, "POST", call);
}

void WebServer::put(const char *path, callback call){
    register_callback(path, "PUT", call);
}

void WebServer::del(const char *path, callback call){
    register_callback(path, "DELETE", call);
}

bool WebServer::is_static_path(String path){
    bool found = false;

    LinkedListIterator<LinkedList<String>::Bucket> iterator(static_paths.get_head());
    while(!iterator.is_end()){
        if(path.startWith(iterator.node()->value)){
            found = true;
            break;
        }
        iterator.increment();
    }

    return found;
}

void WebServer::handle_static_path(String path, HttpRequest* req, HttpResponse* res){
    // readfiles and send response
    // Open a handle to the file
    File my_file(path.get());
    
    #ifdef _WIN32
    if(my_file.getFileHandle() == INVALID_HANDLE_VALUE) {
        printf("\nINVALID_HANDLE_VALUE\n");
        // An error occurred
        res->status(404);
        res->setHeader("Server","abdelfetah-dev");
        res->send("<h1>404 Not Found!</h1>");
        return;
    }
    #else
    if(my_file.getFileHandle() < 0) {
        printf("\nINVALID_HANDLE_VALUE\n");
        // An error occurred
        res->status(404);
        res->setHeader("Server","abdelfetah-dev");
        res->send("<h1>404 Not Found!</h1>");
        return;
    }
    #endif

    // Determine the size of the file
    uint fileSize = my_file.getSize();

    // Allocate a buffer to hold the file contents
    char* buffer = new char[fileSize+1];

    // Read the contents of the file
    bool success = my_file.read(buffer);

    if(!success){
        printf("\nReadFile failed!\n");
        // An error occurred
        res->status(404);
        res->setHeader("Server","abdelfetah-dev");
        res->send("<h1>404 Not Found!</h1>");
        return;
    }

    // determine a mimeType by its extension.
    String ext(path.get() + path.indexOf('.'));
    ext.to_lower_case();

    auto mime_type = supported_mime_types.get(ext);
    
    if(!mime_type.is_error()){
        res->setHeader("Content-Type", mime_type.value());
    }else{ // if files exist with unkonws mediaType.
        res->setHeader("Content-Type","application/octet-stream");
    }

    res->status(200);
    res->send(buffer, fileSize);

    // Free the buffer
    delete[] buffer;

    // Close the handle to the file
    my_file.close();
}

bool WebServer::handle_path(HttpRequest* req, HttpResponse* res){
    LinkedListIterator<LinkedList<PathHandler*>::Bucket> iterator(handlers.get_head());
    while(!iterator.is_end()){
        if(iterator.node()->value->handlePath(req, res)){
            return true;
        }
        iterator.increment();
    }
    return false;
}

void WebServer::handle(HttpRequest* req, HttpResponse* res){
    if(req->getMethod().startWith("GET")){
        // 1. Checking the static_paths.
        String path(req->getPath());
        if(is_static_path(path)){
            handle_static_path(path, req, res);
            return;
        }
    }
   
    bool did_handle = handle_path(req, res);
    if(!did_handle){
        res->status(404);
        res->setHeader("Server","abdelfetah-dev");
        res->send("<h1>404 Not Found!</h1>");
    }
}

THREAD_FUNC Worker(void* args){
    Thread_Params* params = (Thread_Params*) args;
    WebServer* Server = params->web_server;
    Socket m_client(params->client);
    
    char* buffer[DEFAULT_BUFLEN];
    
    // FIXME: implement such a utility to clean the buffer, or add a method to ByteBuffer class
    #ifdef _WIN32
    ZeroMemory(buffer, DEFAULT_BUFLEN);
    #else
    bzero(buffer, DEFAULT_BUFLEN);
    #endif
    Client client(&m_client);

    client.recv((char*) buffer, DEFAULT_BUFLEN);

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
        Server->handle(&req, &res);
        printf("\n");
    }

    delete params;

    return 0;
}
