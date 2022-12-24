# Web Server in C++ for windows

Hi, this is me Abdelfetah Lachenani the creator of this project.
i do like to mention that this project was built from the scratch with no third party libraries.
i built my own data structure like:
1. String
2. HashTable
3. SinglyLinkedList

`The documentation for the previous Classes will be comming soon.`

### Main Classes:
1. Server.
2. Client.
3. HttpRequest.
4. HttpResponse.
5. WebServer

### Some Details About These Classes :
- a Server class has full access to all the clients sockets.

- a Client Class can control only his own socket from a Server class.

- a HttpRequest class can access a socket from Client class.

- a Response class can access a socket from Client class.

---

# Build

```bash
    cd <project-directory>
```

then

```bash
    $ cmake -S . -B C:/Users/amazon/Desktop/WebServer/output "-G<generator>" "-DCMAKE_BUILD_TYPE:STRING=Debug" "-DCMAKE_C_COMPILER:STRING=<c-compile-path>" "-DCMAKE_CXX_COMPILER:STRING=<cpp-compile-path>"
```
and build :
```bash
    $ cmake --build output --target all
```

### Example how to use these commands, like in my case i use MinGw MakeFiles generator :

```bash
    $ cd C:/Users/abdelfetah-dev/Desktop/WebServer
    
    $ cmake -S . -B C:/Users/abdelfetah-dev/Desktop/WebServer/output "-GMinGW Makefiles" "-DCMAKE_BUILD_TYPE:STRING=Debug" "-DCMAKE_C_COMPILER:STRING=C:/Program Files/CodeBlocks/MinGW/bin/gcc.exe" "-DCMAKE_CXX_COMPILER:STRING=C:/Program Files/CodeBlocks/MinGW/bin/g++.exe"

    $ cmake --build output --target all

    $ "./output/WebServer.exe"
```

