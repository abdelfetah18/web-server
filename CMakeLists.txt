cmake_minimum_required(VERSION 3.5)

project(WebServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    Client.cpp
    WebServer.cpp
    HttpRequest.cpp
    HttpResponse.cpp
    PathHandler.cpp
    Socket.cpp
    File.cpp
    base/ByteBuffer.cpp
    base/GenericParser.cpp
    base/Json.cpp
    base/String.cpp
    base/StringView.cpp)

set(HEADERS
    Client.h
    WebServer.h
    HttpRequest.h
    HttpResponse.h
    PathHandler.h
    Socket.h
    File.h
    base/ByteBuffer.h
    base/ErrorOr.h
    base/GenericParser.h
    base/HashTable.h
    base/Json.h
    base/LinkedList.h
    base/RefPointer.h
    base/String.h
    base/StringView.h
    base/Vector.h)

add_executable(WebServer ${SOURCES} ${HEADERS})

if(WIN32)
    target_link_libraries(WebServer wsock32 ws2_32)
endif()