#include "PathHandler.h"

PathHandler::PathHandler(String path,String method,callback call) : m_path(path), m_method(method), m_callback(call) {
    is_dynamic_path = isDynamicPath();
}

// A dynamic path is a path that start with column character ':' to indicate that its can be changable.
bool PathHandler::isDynamicPath(){
    char* path = m_path.get();
    uint index = 0;
    while(path[index] != '\0'){
        if(path[index] == '/' && path[index+1] == ':'){
            return true;
        }
        index += 1;
    }
    return false;
}

bool PathHandler::handleDynamicPath(HttpRequest* req,HttpResponse* res){
    HashTable<String,String> map;
    String path = String(req->getPath()); 
    
    if(m_method != req->getMethod())
        false;
    
    Vector<String>* m_array = m_path.split('/');
    Vector<String>* array = path.split('/');
    if(m_array->size() != array->size())
        return false;

    for(uint i = 0; i < m_array->size(); i++){
        if(m_array->operator[](i)->startWith(":")){
            map.set(*(m_array->operator[](i)->split(':')->operator[](0)), *array->operator[](i));
        }else{
            if(*m_array->operator[](i) != *array->operator[](i))
                return false;
        }
    }

    req->params = map;
    map.show();
    m_callback(req, res);
    return true;
}

bool PathHandler::handlePath(HttpRequest* req,HttpResponse* res){
    if(is_dynamic_path){
        return handleDynamicPath(req, res);
    }else{
        if(m_path == req->getPath() && m_method == req->getMethod()){
            m_callback(req, res);
            return true;
        }
        return false;
    }
}