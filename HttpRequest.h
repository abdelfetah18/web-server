#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "base/String.h"
#include "base/HashTable.h"
#include "Client.h"

/*
    Control Data: request-method request-target protocol-version

*/

/*enum HTTP_METHOD {
    GET,        //Transfer a current representation of the target resource.	9.3.1
    HEAD,       //Same as GET, but do not transfer the response content.	9.3.2
    POST,       //Perform resource-specific processing on the request content.	9.3.3
    PUT       //Replace all current representations of the target resource with the request content.	9.3.4
    DELETE, 	//Remove all current representations of the target resource.	9.3.5
    CONNECT,	//Establish a tunnel to the server identified by the target resource.	9.3.6
    OPTIONS,	//Describe the communication options for the target resource.	9.3.7
    TRACE       //Perform a message loop-back test along the path to the target resource.
};*/

enum HTTP_METHOD { GET, HEAD, POST, PUT, DELETE_, CONNECT, OPTIONS, TRACE };

class HttpRequest {
public:
    HttpRequest(char*,Client);
    int parse();

private:
    struct ControlData {
        HTTP_METHOD method;
        String request_target;
        String protocol_version;
    };

    Client client;
    String raw_request;
    ControlData control_data;
    HashTable<String,String> header_fields;
    String body;
};

#endif // HTTPREQUEST_H
