#ifndef PATHHANDLER_H
#define PATHHANDLER_H

#include "base/String.h"
#include "base/Vector.h"
#include "base/HashTable.h"

#include "HttpRequest.h"
#include "HttpResponse.h"

typedef void (*callback)(HttpRequest*,HttpResponse*);

class PathHandler {
    public:
    PathHandler(String path,String method,callback call);
    
    bool isDynamicPath();
    bool handleDynamicPath(HttpRequest* req,HttpResponse* res);
    bool handlePath(HttpRequest*,HttpResponse*);

    private:
       String m_path;
       String m_method;
       callback m_callback;
       bool is_dynamic_path { false };
};

#endif // PATHHANDLER_H