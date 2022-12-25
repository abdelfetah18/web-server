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

uint HttpResponse::get_control_data_size(){
    uint total = 3; // status code size already is 3
    total += control_data.protocol_version.length();
    total += control_data.reason_phrase.length();
    total += 2; // two spaces
    return total;
}

uint HttpResponse::get_header_fields_size(){
    uint total = 0;
    LinkedListIterator<LinkedList<Field>::Bucket> iterator(header_fields.get_head());
    // header fields
    while(!iterator.is_end()){
        total += iterator.node()->value.name.length();
        total += 3; // space + seperator ':' + space
        total += iterator.node()->value.token.length();
        total += 2; // CRLF "\r\n"
        iterator.increment();
    }
    total += 2; // CRLF "\r\n"
    return total;
}

void HttpResponse::setHeader(const char* name,const char* token){
    Field* field = new Field;
    field->name.push(name);
    field->token.push(token);
    header_fields.append(*field);
}

void HttpResponse::send(const char* data){
    setHeader("Content-Type", "text/html");
    int len = strlen(data);
    setHeader("Content-Length", deciaml_to_text(len).get());
    body.append((unsigned char*) data, len);
    ByteBuffer buf = get_raw_response();
    char* buffer = (char*) buf.get_buffer();
    uint total_size = buf.get_size();
    client.send( buffer, total_size);
}

void HttpResponse::send(char* buffer, int size){
    // TODO: implement such a method to support popular media type
    setHeader("content-disposition", "inline");
    setHeader("content-length", deciaml_to_text(size).get());
    body.append((unsigned char*) buffer, size);
    ByteBuffer buf = get_raw_response();
    char* data = (char*) buf.get_buffer();
    uint total_size = buf.get_size();
    client.send( data, total_size);
}

ByteBuffer HttpResponse::get_raw_response(){
    // the control data
    raw_response.append((Byte*) control_data.protocol_version.get(), control_data.protocol_version.length());
    raw_response.append(' ');
    raw_response.append((Byte*) deciaml_to_text(control_data.status_code).get(), 3);
    raw_response.append(' ');
    raw_response.append((Byte*) control_data.reason_phrase.get(), control_data.reason_phrase.length());
    raw_response.append((Byte*) "\r\n", 2);

    LinkedListIterator<LinkedList<Field>::Bucket> iterator(header_fields.get_head());
    // header fields
    while(!iterator.is_end()){
        raw_response.append((Byte*) iterator.node()->value.name.get(), iterator.node()->value.name.length());
        raw_response.append((Byte*) " : ", 3);
        raw_response.append((Byte*) iterator.node()->value.token.get(), iterator.node()->value.token.length());
        raw_response.append((Byte*) "\r\n", 2);
        iterator.increment();
    }
    raw_response.append((Byte*) "\r\n", 2);

    // body
    raw_response.append(body);

    return raw_response;
}

HttpResponse::HttpResponse(Client c):client(c){
    control_data.protocol_version.push("HTTP/1.1");
}

void HttpResponse::status(int code){
    control_data.status_code = code;
    switch(code){
        case 200: {
            control_data.reason_phrase.set("OK");
            break;
        }
        case 404:{
            control_data.reason_phrase.set("Not Found");
            break;
        }
        default:
            control_data.reason_phrase.set("OK");
    }
}
