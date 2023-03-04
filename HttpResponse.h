#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include "base/ByteBuffer.h"
#include "base/LinkedList.h"
#include "base/String.h"
#include "Client.h"

class HttpResponse {
public:
    HttpResponse(Client);
    void status(int);
    ByteBuffer get_raw_response();
    void setHeader(const char*,const char*);
    void setHeader(const char* name,String token);
    void send(const char*);
    void send(char*,int);
    uint get_control_data_size();
    uint get_header_fields_size();

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
    ByteBuffer raw_response;
    ControlData control_data;
    LinkedList<Field> header_fields;
    ByteBuffer body;
};

#endif // HTTPRESPONSE_H
