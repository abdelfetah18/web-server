# CrossPlatForm Web Server in C++ (Windows, Linux)

This project built from scratch without any third party libraries. It has
its own data structures including `String`, `SinglyLinkedList`, `HashTable`, `ByteBuffer` and more.

**PREVIEW:** https://web-server-mkgs.onrender.com

This preview is running from a docker container check the `Dockerfile` for more information.


### Main Classes:
1. Socket.
2. Server.
3. Client.
4. HttpRequest.
5. HttpResponse.
6. WebServer

### Some Details About These Classes :
- a Server class has full access to all the clients sockets.

- a Client Class can control only his own socket from a Server class.

- a HttpRequest class can access a socket from Client class.

- a Response class can access a socket from Client class.

---

# Features

1. the ability to set a static path (directory) where files will be served if matches the available mimeTypes.
2. Handle a request for a specific path by using a callback that give you control over request and response objects.
3. dynamic path, which mean you can pass params through your path.
and more features will be comming soon. like a json library that will help you generate a custom response for an api business logic.
4. cross platform support currently (windows, linux).

# Code Example

```c++

#include "WebServer.h"

#include "base/HashTable.h"
#include "base/String.h"


void Home(HttpRequest* req,HttpResponse* res){
    res->status(200);
    res->send("<h1>Home!</h1>");
}

void About(HttpRequest* req,HttpResponse* res){
    res->status(200);
    res->send("<h1>About!</h1>");
}

void DynamicPathHandler(HttpRequest* req, HttpResponse* res){
    res->status(200);
    res->send(req->params.get("username"));
}

int main()
{
    WebServer server;
    server.use_static_path("public");
    server.get("/", Home);
    server.get("/About", About);
    // you can have multiple params through url as you can see
    // in this example we have two params 'username' and 'id'.
    // you can access the params from req class.
    // like this: req->params.get(<param>)
    server.get("/users/:username/dashboard/:id", DynamicPathHandler);
    server.listen("8080");
    return 0;
}


```

as you can see in the code, first create an Object with type of `WebServer` and register the paths you wonna server.

```c++
    // Callback function must be in type void (HttpRequest*, HttpResponse*)
    void callback(HttpRequest* req,HttpResponse* res){
        res->send("<h1>HelloWorld!</h1>");
    }

    // Path must equal the request if it uppercase the request will
    // only apply to uppercase and the same for lower case and must
    // start with '/' character.
    server.get(<path>, callback);
```


currently we support only get method, and it will be extended to other methods.


---
# Build

```bash
    cd <project-directory>
```

then

```bash
    cmake -S . -B C:/Users/amazon/Desktop/WebServer/output 
        "-G<generator>" "-DCMAKE_BUILD_TYPE:STRING=Debug"
        "-DCMAKE_C_COMPILER:STRING=<c-compile-path>" 
        "-DCMAKE_CXX_COMPILER:STRING=<cpp-compile-path>"
```
and build :
```bash
    cmake --build output --target all
```

### Example how to use these commands, like in my case i use MinGw MakeFiles generator :

```bash
    cd C:/Users/abdelfetah-dev/Desktop/WebServer
```

```bash
    cmake -S . -B C:/Users/abdelfetah-dev/Desktop/WebServer/output 
        "-GMinGW Makefiles" "-DCMAKE_BUILD_TYPE:STRING=Debug" 
        "-DCMAKE_C_COMPILER:STRING=C:/Program Files/CodeBlocks/MinGW/bin/gcc.exe" 
        "-DCMAKE_CXX_COMPILER:STRING=C:/Program Files/CodeBlocks/MinGW/bin/g++.exe"
```

```bash
    cmake --build output --target all
```

```bash
    "./output/WebServer.exe"
```


