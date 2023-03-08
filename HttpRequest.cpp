#include "HttpRequest.h"

bool is_equal(char str[],const char method[],int m_size,int pos){
    bool is_equal = true;
    for(int i = pos; i < pos+m_size; i++){
        is_equal = str[i] == method[i-pos];
        if(!is_equal){
            break;
        }
    }
    return is_equal;
}

unsigned int parseInt(String num){
    unsigned int n = 0;
    unsigned int len = num.length();
    unsigned int f = 1;
    char* nm = num.get();
    for(int i = len-1; i >= 0; i--){
        n += (nm[i] - '0') * f;
        f *= 10;
    }
    return n;
}

HttpRequest::HttpRequest(char* req,Client c):raw_request(req),client(c) { }

String HttpRequest::getMethod(){
    String method;
    switch(control_data.method){
        case HTTP_METHOD::GET:{
            method.push("GET");
            break;
        }
        default:{
            method.push("GET");
        }
    }

    return method;
}

char* HttpRequest::getPath(){
    return control_data.request_target.get();
}

enum PARSING_STATE { METHOD, RESOURCE, HTTP_VERSION, FIELD_NAME, FIELD_TOKEN, SPACE, CRLF, BODY };
int HttpRequest::parse(){
    PARSING_STATE p_state = PARSING_STATE::METHOD;

    char* str = raw_request.get();
    int len = raw_request.length();
    int pos = 0;

    if(p_state == PARSING_STATE::METHOD){
        if(is_equal(str,"GET",3,0)){
            this->control_data.method = HTTP_METHOD::GET;
            p_state = PARSING_STATE::SPACE;
            pos += 3;
        }

        if(is_equal(str,"HEAD",4,0)){
            this->control_data.method = HTTP_METHOD::HEAD;
            p_state = PARSING_STATE::SPACE;
            pos += 4;
        }

        if(is_equal(str,"POST",4,0)){
            this->control_data.method = HTTP_METHOD::POST;
            p_state = PARSING_STATE::SPACE;
            pos += 4;
        }

        if(is_equal(str,"PUT",3,0)){
            this->control_data.method = HTTP_METHOD::PUT;
            p_state = PARSING_STATE::SPACE;
            pos += 3;
        }

        if(is_equal(str,"DELETE",6,0)){
            this->control_data.method = HTTP_METHOD::DELETE_;
            p_state = PARSING_STATE::SPACE;
            pos += 6;
        }

        if(is_equal(str,"CONNECT",7,0)){
            this->control_data.method = HTTP_METHOD::CONNECT;
            p_state = PARSING_STATE::SPACE;
            pos += 7;
        }

        if(is_equal(str,"OPTIONS",7,0)){
            this->control_data.method = HTTP_METHOD::OPTIONS;
            p_state = PARSING_STATE::SPACE;
            pos += 7;
        }

        if(is_equal(str,"TRACE",5,0)){
            this->control_data.method = HTTP_METHOD::TRACE;
            p_state = PARSING_STATE::SPACE;
            pos += 5;
        }
    }

    if(p_state == PARSING_STATE::SPACE){
        if(str[pos] == ' '){
            p_state = PARSING_STATE::RESOURCE;
            pos += 1;
        }
    }

    if(p_state == PARSING_STATE::RESOURCE){
        for(int i = pos; str[i] != ' '; i++){
            control_data.request_target.push(str[i]);
        }
        p_state = PARSING_STATE::SPACE;
        pos += control_data.request_target.length();
    }

    if(p_state == PARSING_STATE::SPACE){
        if(str[pos] == ' '){
            p_state = PARSING_STATE::HTTP_VERSION;
            pos += 1;
        }
    }

    if(p_state == PARSING_STATE::HTTP_VERSION){
        for(int i = pos; str[i] != '\r'; i++){
            control_data.protocol_version.push(str[i]);
        }
        p_state = PARSING_STATE::CRLF;
        pos += control_data.protocol_version.length();
    }

    if(p_state == PARSING_STATE::CRLF){
        if(is_equal(str,"\r\n",2,pos)){
            p_state = PARSING_STATE::FIELD_NAME;
            pos += 2;
        }
    }

    while(p_state != PARSING_STATE::BODY){
        if(p_state == PARSING_STATE::FIELD_NAME){
            String key, value;
            for(int i = pos; str[i] != ':'; i++){
                key.push(str[i]);
            }
            pos += key.length();
            pos += 2; // skip semi_column and white-space
            p_state = PARSING_STATE::FIELD_TOKEN;
            for(int i = pos; str[i] != '\r'; i++){
                value.push(str[i]);
            }
            pos += value.length();
            header_fields.set( key, value);
            p_state = PARSING_STATE::CRLF;
        }
        if(p_state == PARSING_STATE::CRLF){
            if(is_equal(str,"\r\n",2,pos)){
                pos += 2;
                if(is_equal(str,"\r\n",2,pos)){
                    p_state = PARSING_STATE::BODY;
                    pos += 2;
                }else{
                    p_state = PARSING_STATE::FIELD_NAME;
                }
            }
        }
    }

    if(p_state == PARSING_STATE::BODY){
        if(control_data.method == HTTP_METHOD::POST || control_data.method == HTTP_METHOD::PUT){
            auto content_length = header_fields.get("Content-Length");
            if(!content_length.is_error()){
                unsigned int len = parseInt(content_length.value());
                for(unsigned int i = pos; i < pos+len; i++){
                    body.append((Byte) str[i]);
                }
                // handle body
                // Content-Type: application/json
                // Content-Type: application/json
                return 0;
            }
            return 1;
        }
        return 0;
    }else{
        return 1;
    }
}
