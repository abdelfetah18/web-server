cmake_minimum_required(VERSION 3.5)

project(WebServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    Json.cpp
    Client.cpp
    WebServer.cpp
    HttpRequest.cpp
    HttpResponse.cpp
    base/String.cpp
    base/StringView.cpp
    base/GenericParser.cpp
    base/ByteBuffer.cpp)

set(HEADERS
    Json.h
    Client.h
    WebServer.h
    HttpRequest.h
    HttpResponse.h
    base/String.h
    base/StringView.h
    base/GenericParser.h
    base/ByteBuffer.h
    base/HashTable.h
    base/LinkedList.h)

add_executable(WebServer ${SOURCES} ${HEADERS})
target_link_libraries(WebServer wsock32 ws2_32)
