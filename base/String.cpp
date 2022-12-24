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
    int i=0;
    while(data[i] != '\0'){
        i++;
    }
    return i;
}

void copyFromTo(char* src,char* dst,int size){
    for(int i=0; i < size; i++){
        dst[i] = src[i];
    }
    dst[size] = '\0';
}

String::String(char data[]){
    if(data != nullptr){
        str = new char[strlen(data) + 1];
        copyFromTo( data, str, strlen(data));
        size = strlen(str);
    }
}

String::String(){
    for(int i = 0; i < capacity; i++){
        str[i] = '\0';
    }
}
String::~String(){}

void String::push(const char data[]){
    int len = strlen(data);
    push((char*)data, len);
}

void String::push(char data[]){
    int len = strlen(data);
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
            capacity = (((size + len) / 8)+1) * 8;
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

char* String::get(){
    return str;
}

int String::length(){
    return size;
}

void String::clear(){
    for(int i=0; i < capacity; i++){
        str[i] = '\0';
    }
    size = 0;
}

void String::show(){
    printf("%s", str);
}

bool String::lookup(const char* data){
    bool found = false;
    for(int i=0; i < size; i++){
        found = true;
        for(int j=0; j < strlen(data); j++){
            if(str[i+j] != data[j]){
                found = false;
            }
        }
        if(found)
            return true;
    }
    return found;
}
