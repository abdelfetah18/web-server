#include "WebServer.h"
#pragma comment(lib, "Ws2_32.lib")

#include "base/HashTable.h"
#include "base/String.h"

struct Field {
    String field_name;
    String token;
};

int hash(String key){
    int hash = 7;
    char* str = key.get();
    int str_len = key.length();
    for (int i = 0; i < str_len; i++) {
        hash = hash*31 + str[i];
    }
    return hash;
}

class INTEGER_TEST {
public:
    int value = 0x41;

    INTEGER_TEST(int v):value(v){};
    INTEGER_TEST(){};

    void* get(){
        return &value;
    }

    void show(){
        printf("%d", value);
    }

    int length(){
        return sizeof(int);
    }
};

int test_hash(INTEGER_TEST value){
    unsigned char* str = (unsigned char*) &(value.value);
    int index = 0;
    while(str[index] != '\0')
        index++;
    printf("index: %d\n", index);
    int hash = 7;
    for(int i=0; i < 4; i++){
        hash = hash*31 + str[i];
    }
    return hash;
}


int main()
{
    /*HashTable<String,String> fields;
    fields.set("Host","google.com");
    fields.set("Authentication","access_token");
    fields.set("username","Abdelfetah");
   *//*String str;
    if(fields.get("username", str))
        printf("value: %s\n", str.get());
    else
        printf("not Found!\n");

    if(fields.get("Host", str))
        printf("value: %s\n", str.get());
    else
        printf("not Found!\n");
    */
    //fields.show();
    
    WebServer server("8080");
    return 0;
}
