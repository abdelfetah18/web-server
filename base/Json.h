#ifndef JSON_H
#define JSON_H

#include "ByteBuffer.h"
#include "HashTable.h"
#include "LinkedList.h"
#include "String.h"
#include "StringView.h"
#include "RefPointer.h"
#include "Vector.h"
#include "GenericParser.h"

namespace Json {
    
class Number {
    public:
    Number();
    Number(int num):value(num){ }
    int value;
    void show(){
        printf("%d", value);
    }
};

class Array;
class JsonObject;

class JsonValue {
public:
    JsonValue();
    JsonValue(String);
    JsonValue(Number);
    JsonValue(Array);
    JsonValue(JsonObject);

    String* asString();
    Number* asNumber();
    Array* asArray();
    JsonObject* asObject();

    void operator =(JsonValue& other);
    void operator =(String);
    void operator =(Number);
    void operator =(Array);
    void operator =(JsonObject);
    void show();

    union JsonV {
        String* str;
        Number* number;
        Array* array;
        JsonObject* object;
        bool boolean;
        void* null = nullptr;
    };
    JsonV m_value;
    String m_type;
};


class Array {
public:
    Array();
    void append(JsonValue value);
    JsonValue operator [](uint index);
    void show();
private:
    Vector<JsonValue> m_array;
};

class JsonObject {
public:
    JsonValue operator [](char key[]);
    void set(String,String);
    void set(String,Number);
    void set(String,Array);
    void set(String,JsonObject);
    void set(String,JsonValue);

    void show();
private:
    HashTable<String,JsonValue> objects;

};


// TODO: add a support for unicode strings.
class Parser {
public:
    Parser(StringView);
    ~Parser();
    
    
    uint increment();
    char getCur();
    char lookAhead();
    void skipWhiteSpace();

    uint parse();
    uint parseObject(JsonObject&);
    uint parseValue(JsonValue&);
    uint parseString(String&);
    uint parseEscape(String&);
    uint parseNumber(Number&);
    uint parseArray(Array&);
    uint parseBoolean(bool&);
    uint parseNull(JsonValue&);
    
    JsonObject getRootObject(){
        return m_object;
    }

private:
    StringView m_input;
    uint pos = 0;
    JsonObject m_object;
};

};
#endif