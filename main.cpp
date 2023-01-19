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

void DynamicPathHandler(HttpRequest* req, HttpResponse* res){
    String data("username: ");

    String username;
    req->params.get("username", username);
    data.push(username);

    data.push("<br>id: ");

    String id;
    req->params.get("id", id);
    data.push(id);

    res->status(200);
    res->send(data.get());
}

int main()
{
    WebServer server;
    server.use_static_path("public");
    server.get("/home", Home);
    server.get("/About", About);
    server.get("/users/:username/dashboard/:id", DynamicPathHandler);
    server.listen("8080");
    return 0;
}



/*

#include <stdio.h>

#include "base/Json.h"


int main(){
    Json::Parser m_parser("{ \"user\" : \"Abdelfetah\" }");
    m_parser.parse();

    return 0;
}

*/
