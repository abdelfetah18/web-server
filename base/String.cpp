#include "String.h"
#include <stdio.h>

int strlen(char* data){
    int i=0;
    while(data[i] != '\0'){
        i++;
    }
    return i;
}

int strlen(const char* data){
    uint i = 0;
    while(data[i] != '\0'){
        i++;
    }
    return i;
}

void copyFromTo(char* src,char* dst,int size){
    for(uint i=0; i < size; i++){
        dst[i] = src[i];
    }
    dst[size] = '\0';
}

String::String(char data[]){
    uint len = strlen(data);
    if(len >= capacity){
        capacity = ((len / 8) + 1) * 8;
        delete[] str;
        str = new char[capacity];
    }
    for(uint i = 0; i < capacity; i++){
        str[i] = '\0';
    }
    copyFromTo( data, str, len);
    size = len;
}

String::String(){
    for(uint i = 0; i < capacity; i++){
        str[i] = '\0';
    }
}

String::String(const String& copy){
    delete[] str;
    capacity = copy.capacity;
    size = copy.size;
    str = new char[capacity];
    copyFromTo(copy.str, str, size);
}

void String::operator=(String &copy){
    delete[] str;
    capacity = copy.capacity;
    size = copy.size;
    str = new char[capacity];
    copyFromTo(copy.str, str, size);
}

String::~String(){
    delete[] str;
}

int String::indexOf(char c){
    uint index = 0;
    for(;index < size; index++){
        if(str[index] == c)
            return index;
    }
    return -1;
}

void String::to_lower_case(){
    for(uint i = 0; i < size; i++){
        if(str[i] >= 'A' && str[i] <= 'Z'){
            str[i] = str[i] + ('a' - 'A');
        }else{
            str[i] = str[i];
        }
    }

}

void String::to_upper_case(){
    for(uint i = 0; i < size; i++){
        if(str[i] >= 'a' && str[i] <= 'z'){
            str[i] = str[i] - ('a' - 'A');
        }else{
            str[i] = str[i];
        }
    }
}

void String::push(const char data[]){
    uint len = strlen(data);
    push((char*)data, len);
}

void String::push(char data[]){
    uint len = strlen(data);
    push(data, len);
}

void String::push(char data[],int len){
    if(str != nullptr){
        if(capacity - size > len){
            copyFromTo( data, str + size, len);
        }else{
            capacity = (((size + len) / 8)+1) * 8;
            char* new_str = new char[capacity];
            copyFromTo(str,new_str,size);
            copyFromTo(data,new_str+size, len);
            delete[] str;
            str = new_str;
       }
    }else{
        if(capacity > len){
           copyFromTo(data, str, len);
        }else{
            capacity = (((size + len) / 8) + 1) * 8;
            str = new char[capacity];
            copyFromTo( data, str, len);
        }
    }
    size = size + len;
}

void String::push(char c){
    push(&c,1);
}

void String::push(String c){
    push(c.get(),c.length());
}

void String::set(char* data){
    uint len = strlen(data);
    if(len > capacity){
        capacity = ((len / 8) + 1) * 8;
        char* buffer = new char[capacity];
        delete[] str;
        str = buffer;
    }
    for(uint i = 0; i < capacity; i++)
        str[i] = '\0';

    for(uint i = 0; i < len; i++)
        str[i] = data[i];
    str[len] = '\0';
    size = len;
}

char* String::get(){
    return str;
}

uint String::length(){
    return size;
}

void String::clear(){
    for(uint i=0; i < capacity; i++){
        str[i] = '\0';
    }
    size = 0;
}

void String::show(){
    printf("%s", str);
}

bool String::startWith(String other){
    char* target = other.get();
    char* source = str;
    bool same = true;
    while(*source != '\0' && *target != '\0'){
        if(*target != *source){
            same = false;
            break;
        }
        target++;
        source++;
    }
    return same;
}

bool String::lookup(const char* data){
    bool found = false;
    for(uint i=0; i < size; i++){
        found = true;
        for(uint j=0; j < strlen(data); j++){
            if(str[i+j] != data[j]){
                found = false;
            }
        }
        if(found)
            return true;
    }
    return found;
}
