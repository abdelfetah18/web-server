#include "File.h"

File::File(char* path){
    file_path.set(path);
    file_handle = open();

    #ifdef _WIN32
    DWORD fileSize = GetFileSize(file_handle, NULL);
    file_size = fileSize;
    #else
    file_size = lseek(file_handle, 0, SEEK_END);
    lseek(file_handle, 0, SEEK_SET);
    #endif

}

#ifdef _WIN32
HANDLE File::open(){
    HANDLE hFile = CreateFile(file_path.get(), GENERIC_READ,0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    return hFile;
}
#else
int File::open(){
    return ::open(file_path.get(), O_RDONLY);   
}
#endif

unsigned int File::getSize(){ return file_size; }

bool File::read(char* buffer){
    #ifdef _WIN32
    DWORD bytesRead;
    bool success = ReadFile(file_handle, buffer, file_size, &bytesRead, nullptr);
    return success;
    #else
    int bytes_read = ::read(file_handle, buffer, file_size);
    return bytes_read;
    #endif
}

void File::close(){
    #ifdef _WIN32
    CloseHandle(file_handle);
    #else
    ::close(file_handle);
    #endif
}

#ifdef _WIN32
HANDLE File::getFileHandle(){ return file_handle; }
#else
int File::getFileHandle(){ return file_handle; }
#endif