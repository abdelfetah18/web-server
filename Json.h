#ifndef JSON_H
#define JSON_H

#include "base/ByteBuffer.h"
#include "base/LinkedList.h"
#include "base/HashTable.h"
#include "base/GenericParser.h"
#include "base/String.h"

/*
    object
    array
    string
    number
    "true"
    "false"
    "null"
*/
union Value {
    uint number;
    LinkedList<Value> * array;
    HashTable<String,Value> *object;
    bool true_false;
    String* string;
    //nullptr null;
};


class Json {
public:
    Json(char* str):raw_json(str){ }
    int parseJson();
    bool is_parsed(){ return parsed; }
private:
    char* raw_json = nullptr;
    bool parsed = false;
    HashTable<String,Value> root;
};

#endif // JSON_H
