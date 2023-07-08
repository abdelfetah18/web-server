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
            auto call = handlers.get(key);
            if(!call.is_error())
                call.value()(req,res);
            else{
                res->status(404);
                res->setHeader("Server","abdelfetah-dev");
                res->send("<h1>404 Not Found!</h1>");
            }
        }
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
        Server->handle(key, &req, &res);
        printf("\n");
    }

    delete params;

    return 0;
}
