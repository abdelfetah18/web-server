#include "WebServer.h"
#pragma comment(lib, "Ws2_32.lib")

#include "base/HashTable.h"
#include "base/String.h"


auto const home_page = "<html><head><title>Web Server In CPP</title><style>* { padding: 0; margin: 0; box-sizing: border-box; }\n#app {width: 100%; height: 100vh; background-color: #2a2a2a; color: #ffffff; font-family: monospace; }\n#page_name { font-weight: 700; font-size: 16px; padding: 20px; }</style></head><body><div id='app'><div id='page_name'>Home</div></div></body>";
auto const about_page = "<html><head><title>Web Server In CPP</title><style>* { padding: 0; margin: 0; box-sizing: border-box; }\n#app {width: 100%; height: 100vh; background-color: #2a2a2a; color: #ffffff; font-family: monospace; }\n#page_name { font-weight: 700; font-size: 16px; padding: 20px; }</style></head><body><div id='app'><div id='page_name'>About</div></div></body>";

void Home(HttpRequest* req,HttpResponse* res){
    res->status(200);
    res->send(home_page);
}

void About(HttpRequest* req,HttpResponse* res){
    res->status(200);
    res->send("<h1>About!</h1>");
}

void DynamicPathHandler(HttpRequest* req, HttpResponse* res){
    String data("<html><head><title>Web Server In CPP</title><style>* { padding: 0; margin: 0; box-sizing: border-box; }\n#app {width: 100%; height: 100vh; background-color: #2a2a2a; color: #ffffff; font-family: monospace; }\n#page_name { font-weight: 700; font-size: 16px; padding: 20px; }</style></head><body><div id='app'><div id='page_name'>Username: ");

    auto username = req->params.get("username");
    if(!username.is_error())
        data.push(username.value());

    data.push("<br>id: ");

    auto id = req->params.get("id");
    if(!id.is_error())
        data.push(id.value());

    data.push("</div></div></body>");
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
