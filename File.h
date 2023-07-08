#ifndef FILE_HEADER
#define FILE_HEADER

#include "base/String.h"
#include "stdio.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include "fcntl.h"
    #include "unistd.h"
#endif

class File {
    public:
    File(char* path);
    unsigned int getSize();
    bool read(char* buffer);
    void close();

    #ifdef _WIN32
    HANDLE open();
    HANDLE getFileHandle();
    #else
    int open();
    int getFileHandle();
    #endif

    private:
    #ifdef _WIN32
    HANDLE file_handle;
    #else
    int file_handle;
    #endif
    unsigned int file_size = 0;
    String file_path;
};

#endif