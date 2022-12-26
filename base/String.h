#ifndef STRING_HEADER //Include Guard
#define STRING_HEADER

typedef unsigned int uint;

class String{
public:
    String(char[]);
    String();
    String(const String&);
    ~String();

    void push(const char[]);
    void push(char[]);
    void push(char[],int);
    void push(char);
    void push(String);
    void set(char[]);
    char* get();
    uint length();
    void clear();
    bool lookup(const char[]);
    void show();
    int indexOf(char);
    void to_lower_case();
    void to_upper_case();
    bool startWith(String);
    void operator =(String&);
    bool operator ==(String& other){
        char* str_1 = other.get();
        for(uint i=0; i < size; i++){
            if(str[i] != str_1[i]){
                return false;
            }
        }
        return true;
    }

    // TODO: implement a grabage collector algorithm for the String Class.
    void operator delete(void* ptr){ delete[] ((String*) ptr)->str; }
private:
    uint size = 0;
    uint capacity = 8;
    char* str = new char[capacity];
};

#endif
