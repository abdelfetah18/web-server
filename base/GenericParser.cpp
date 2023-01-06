#include "GenericParser.h"

bool GenericParser::consume(char *input){
    uint index = 0;
    while(input[index] != '\0'){
        if(input[index] != m_input[m_index + index])
            return false;
        index += 1;
    }
    m_index += index;
    return true;
}

bool GenericParser::next_is(char c){
    return m_input[m_index + 1] == 'c';
}

char GenericParser::peek(unsigned int offset){
    return m_input[m_index + offset];
}

bool GenericParser::is_eof(){
    return m_index >= m_input.length();
}
