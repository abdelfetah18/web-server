#include "Json.h"

void skipWhiteSpace(char* str,uint& index){
    while(str[index] == ' ')
        index += 1;
}

int Json::parseJson(){
    StringView m_input(raw_json);
    GenericParser m_parser(m_input);

    while(!m_parser.is_eof()){
        // TODO: parse json data
    }
}
