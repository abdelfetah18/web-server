#include "ByteBuffer.h"

ByteBuffer::ByteBuffer(){ }
ByteBuffer::ByteBuffer(const ByteBuffer& copy){
    delete[] buffer;
    capacity = copy.capacity;
    buffer = new Byte[capacity];
    size = copy.size;
    for(uint i = 0; i < capacity; i++)
        buffer[i] = copy.buffer[i];
}

ByteBuffer::~ByteBuffer(){
    delete[] buffer;
}

void clean_buffer(Byte* buffer, uint size){
    for(uint i = 0; i < size; i++)
        buffer[i] = '\0';
}

ByteBuffer::ByteBuffer(Byte byte){
    if(size >= capacity){
        capacity += 8;
        Byte* new_buffer = new Byte[capacity];

        clean_buffer(new_buffer, capacity);

        for(uint i = 0; i < size; i++)
            new_buffer[i] = buffer[i];
        delete[] buffer;
        buffer = new_buffer;
    }
    buffer[size] = byte;
    size += 1;
}

ByteBuffer::ByteBuffer(Byte* bytes,uint bytes_size){
    if(bytes_size >= capacity){
        capacity = (((size + bytes_size) / 8) + 1) * 8;
        Byte* new_buffer = new Byte[capacity];

        clean_buffer(new_buffer, capacity);

        for(uint i = 0; i < size; i++)
            new_buffer[i] = buffer[i];
        delete[] buffer;
        buffer = new_buffer;
    }
    for(int i = 0; i < bytes_size; i++)
        buffer[i + size] = bytes[i];
    size += bytes_size;
}

void ByteBuffer::append(Byte byte){
    buffer[size] = byte;
    size += 1;
}

void ByteBuffer::append(Byte* bytes,uint bytes_size){
    if((size + bytes_size) >= capacity){
        capacity = (((size + bytes_size) / 8) + 1) * 8;
        Byte* new_buffer = new Byte[capacity];

        clean_buffer(new_buffer, capacity);

        for(uint i = 0; i < size; i++)
            new_buffer[i] = buffer[i];
        delete[] buffer;
        buffer = new_buffer;
    }
    for(int i = 0; i < bytes_size; i++)
        buffer[i + size] = bytes[i];
    size += bytes_size;
}

void ByteBuffer::append(ByteBuffer byte_buffer){
    if((size + byte_buffer.get_size()) >= capacity){
        capacity = (((size + byte_buffer.get_size()) / 8) + 1) * 8;
        Byte* new_buffer = new Byte[capacity];

        clean_buffer(new_buffer, capacity);

        for(uint i = 0; i < size; i++)
            new_buffer[i] = buffer[i];
        delete[] buffer;
        buffer = new_buffer;
    }
    for(int i = 0; i < byte_buffer.get_size(); i++)
        buffer[i + size] = byte_buffer.get_buffer()[i];
    size += byte_buffer.get_size();
}

void ByteBuffer::show(){
    printf("size: %u\n", size);
    printf("[ ByteBuffer ] ==============================\n");
    for(uint i = 0; i < capacity; i++){
        if(i % 8 == 0)
            printf("\n");
        printf("%.2x ", buffer[i]);
    }
    printf("\n=============================================");
}

void ByteBuffer::operator=(ByteBuffer& copy){
    delete[] buffer;
    capacity = copy.capacity;
    buffer = new Byte[capacity];
    size = copy.size;
    for(uint i = 0; i < capacity; i++)
        buffer[i] = copy.buffer[i];
}

Byte* ByteBuffer::get_buffer(){ return buffer;}
uint ByteBuffer::get_capacity(){ return capacity; }
uint ByteBuffer::get_size(){ return size; }
