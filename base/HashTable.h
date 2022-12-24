#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "LinkedList.h"
#include "stdio.h"

template <typename KEY,typename VALUE>
class HashTable {

public:

    void set(KEY key,VALUE value){
        size += 1;
        unsigned int index = hash_function(key) % 64;
        table[index].append(createBucket(key,value));
    }

    bool get(KEY key,VALUE& value){
        int index = hash_function(key) % 64;
        Bucket test;
        if(table[index].find(createBucket(key,{}), test)){
            value = test.value;
            return true;
        }
        return false;
    }

    void show(){
        for(int i=0; i < capacity; i++){
            printf("\n[ %d ] =======================================\n", i);
            table[i].show();
            printf("==============================================\n");
        }
    }

    unsigned int hash_function(KEY key){
        unsigned char* str = (unsigned char*) key.get();
        unsigned int len = key.length();
        unsigned int hash = 7;
        for(unsigned int i=0; i < len; i++){
            hash = hash*31 + str[i];
        }
        return hash;
    }

private:
    struct Bucket {
        KEY key;
        VALUE value;
        bool isSet = false;

        Bucket(){}
        bool operator ==(Bucket& rhs){
            unsigned char* str_1 = (unsigned char*) key.get();
            unsigned char* str_2 = (unsigned char*) rhs.key.get();

            bool is_equal = true;
            int l_len = key.length();
            int r_len = rhs.key.length();
            if(l_len == r_len){
                for(int i = 0; i < l_len; i++){
                    is_equal = str_1[i] == str_2[i];
                    if(!is_equal)
                        break;
                }
            }else{
                return false;
            }
            return is_equal;
        }

        void show(){
            printf("\t");
            key.show();
            printf(" : ");
            value.show();
            printf("\n");
        }
    };

    int capacity = 64;
    int size = 0;

    LinkedList<Bucket> table[64];

    Bucket createBucket(KEY key,VALUE value){
        Bucket b;
        b.key = key;
        b.value = value;
        b.isSet = true;
        return b;
    }
};

#endif // HASHTABLE_H
