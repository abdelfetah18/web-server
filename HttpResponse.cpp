#include "HttpResponse.h"

String deciaml_to_text(unsigned int num){
    String text;
    int n = num;
    int i = 1;
    while(n != 0){
        n = n / 10;
        i *= 10;
    }

    i = i / 10;

    while(i != 0){
        unsigned char c = (num / i) % 10;
        c += '0';
        text.push(c);
        i = i / 10;
    }
    return text;
}

void HttpResponse::setHeader(const char* name,const char* token){
    Field* field = new Field;
    field->name.push(name);
    field->token.push(token);
    header_fields.append(*field);
}

void HttpResponse::send(const char* data){
    status(200);
    setHeader("Content-Type", "text/html");
    int len = strlen(data);
    setHeader("Content-Length", deciaml_to_text(len).get());
    body.push(data);
    char* buffer = get_raw_response();
    client.send( buffer, strlen(buffer));
}

char* HttpResponse::get_raw_response(){
    // the control data
    raw_response.push(control_data.protocol_version);
    raw_response.push(' ');
    raw_response.push(deciaml_to_text(control_data.status_code));
    raw_response.push(' ');
    raw_response.push(control_data.reason_phrase);
    raw_response.push("\n\r");

    LinkedListIterator<LinkedList<Field>::Bucket> iterator(header_fields.get_head());
    // header fields
    while(!iterator.is_end()){
        raw_response.push(iterator.node()->value.name);
        raw_response.push(" : ");
        raw_response.push(iterator.node()->value.token);
        raw_response.push("\n\r");
        iterator.increment();
    }
    raw_response.push("\n\r");

    // body
    raw_response.push(body);

    return raw_response.get();
}

HttpResponse::HttpResponse(Client c):client(c){
    control_data.protocol_version.push("HTTP/1.1");
}

void HttpResponse::status(int code){
    control_data.status_code = code;
    switch(code){
        case 200: {
            control_data.reason_phrase.push("OK");
            break;
        }
        default:
            control_data.reason_phrase.push("OK");
    }
}
