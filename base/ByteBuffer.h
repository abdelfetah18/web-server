#ifndef BYTEBUFFER_HEADER //Include Guard
#define BYTEBUFFER_HEADER

#include <stdio.h>

typedef unsigned char Byte;
typedef unsigned int uint;

class ByteBuffer {
public:
    ByteBuffer();
    ByteBuffer(Byte);
    ByteBuffer(Byte*,uint);
    ~ByteBuffer();

    void append(Byte);
    void append(Byte*,uint);
    void append(ByteBuffer);
    void show();
    uint get_size();
    uint get_capacity();
    Byte* get_buffer();

private:
    Byte* buffer = new Byte[8];
    uint size = 0;
    uint capacity = 8;
};

#endif
