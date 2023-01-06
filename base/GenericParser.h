#ifndef GENERIC_PARSER_HEADER //Include Guard
#define GENERIC_PARSER_HEADER

#include "StringView.h"

class GenericParser {
public:
    GenericParser(StringView input): m_input(input) { }
    bool consume(char input[]);
    bool next_is(char c);
    char peek(unsigned int offset);
    bool is_eof();
private:
    StringView m_input;
    unsigned int m_index = 0;
};

#endif
