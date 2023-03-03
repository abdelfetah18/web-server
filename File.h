#ifndef FILE_HEADER
#define FILE_HEADER

#include "base/String.h"
#include "stdio.h"

#ifdef _WIN32

#else
    #include "fcntl.h"
    #include "unistd.h"
#endif

class File {
    public:
    File(char* path);
    int open();
    unsigned int getSize();
    bool read(char* buffer);
    void close();
    int getFileHandle();
    
    private:
    int file_handle;
    unsigned int file_size = 0;
    String file_path;
};

#endif