#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include "base/LinkedList.h"
#include "base/String.h"
#include "Client.h"

class HttpResponse {
public:
    HttpResponse(Client);
    void status(int);
    char* get_raw_response();
    void setHeader(const char*,const char*);
    void send(const char*);


private:
    struct ControlData {
        String protocol_version;
        String reason_phrase;
        unsigned int status_code;
    };

    struct Field {
       String name;
       String token;
    };

    Client client;
    String raw_response;
    ControlData control_data;
    LinkedList<Field> header_fields;
    String body;
};

#endif // HTTPRESPONSE_H
