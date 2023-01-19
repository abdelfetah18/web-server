#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#include "ByteBuffer.h"

template<typename VALUE>
class Vector
{
private:
    uint m_capacity = 8;
    uint m_size = 0;
    VALUE* m_array = new VALUE[m_capacity];
public:
    Vector(){ };
    ~Vector(){ };

    uint size(){
        return m_size;
    }

    VALUE* operator[](uint index){
        if(index < m_size)
            return &m_array[index];
        return nullptr;
    }

    void append(VALUE value){
        if(m_array != nullptr && m_size < m_capacity){
            m_array[m_size] = value;
            m_size += 1;
        }else{
            m_capacity += 8;
            VALUE* new_array = new VALUE[m_capacity];
            for(uint i = 0; i < m_size; i++)
                new_array[i] = m_array[i];
            new_array[m_size] = value;
            m_size += 1;
            delete[] m_array;
            m_array = new_array;
        }
    }

    void show(){
        printf("[ ");
        for(uint i = 0; i < m_size; i++)
            printf("%d ", m_array[i]);
        printf("]");
    }
};

#endif