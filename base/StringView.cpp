#include "StringView.h"

uint str_len(char* str){
    uint size = 0;
    while(str[size] != '\0')
        size += 1;
    return size;
}

StringView::StringView(char* str) : m_str(str) {
    m_size = str_len(str);
}

uint StringView::length(){
    return m_size;
}
