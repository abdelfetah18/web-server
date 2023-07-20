#ifndef STRING_HEADER //Include Guard
#define STRING_HEADER

typedef unsigned int uint;

#include "Vector.h"

class String{
public:
    String(const char[]);
    String();
    String(const String&);
    ~String();

    void push(const char[]);
    void push(char[]);
    void push(char[],int);
    void push(char);
    void push(String);
    void set(const char[]);
    char* get();
    uint length();
    void clear();
    bool lookup(const char[]);
    void show();
    int indexOf(char);
    void to_lower_case();
    void to_upper_case();
    bool startWith(String);
    Vector<String>* split(char);
    void operator =(String&);


    bool operator ==(String other){
        if(size != other.length())
            return false;
        char* str_1 = other.get();
        for(uint i=0; i < size; i++){
            if(str[i] != str_1[i]){
                return false;
            }
        }
        return true;
    }

    int parseInt();

    // TODO: implement a grabage collector algorithm for the String Class.
    void operator delete(void* ptr){ delete[] ((String*) ptr)->str; }
private:
    uint size = 0;
    uint capacity = 8;
    char* str = new char[capacity];
};

#endif
