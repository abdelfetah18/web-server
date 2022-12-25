#include "WebServer.h"
#pragma comment(lib, "Ws2_32.lib")

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

int main()
{
    WebServer server;
    server.use_static_path("public");
    server.get("/home", Home);
    server.get("/About", About);
    server.listen("8080");
    return 0;
}
