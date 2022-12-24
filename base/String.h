#ifndef STRING_HEADER //Include Guard
#define STRING_HEADER

class String{
public:
    String(char[]);
    String();
    ~String();

    void push(const char[]);
    void push(char[]);
    void push(char[],int);
    void push(char);
    void push(String);
    char* get();
    int length();
    void clear();
    bool lookup(const char[]);
    void show();
    bool operator ==(String& other){
        char* str_1 = other.get();
        for(int i=0; i < size; i++){
            if(str[i] != str_1[i]){
                return false;
            }
        }
        return true;
    }
private:
    int size = 0;
    int capacity = 8;
    char* str = new char[capacity];
};

#endif
