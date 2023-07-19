#include "WebServer.h"
#pragma comment(lib, "Ws2_32.lib")

#include "base/HashTable.h"
#include "base/String.h"


void Home(HttpRequest* req,HttpResponse* res){
    res->status(200);
    res->send("<h1>Home</h1>");
}

void About(HttpRequest* req,HttpResponse* res){
    res->status(200);
    res->send("<h1>About!</h1>");
}

void DynamicPathHandler(HttpRequest* req, HttpResponse* res){
    String data("username: ");

    auto username = req->params.get("username");
    if(!username.is_error())
        data.push(username.value());

    data.push("<br>id: ");

    auto id = req->params.get("id");
    if(!id.is_error())
        data.push(id.value());

    res->status(200);
    res->send(data.get());
}

int main()
{
    WebServer server;
    server.use_static_path("public");
    server.get("/", Home);
    server.get("/About", About);
    server.get("/users/:username/dashboard/:id", DynamicPathHandler);
    server.listen("8000");
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
