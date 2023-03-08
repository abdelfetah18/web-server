#include <stdio.h>

// Type Treat is_same
template<typename T,typename U>
static bool is_same = false;

template<typename T>
static bool is_same<T,T> = true;


template<typename... TS>
class Variant {
    private:
        int max;
        char* storage;
    public:

    template<typename T>
    Variant(T v){
        printf("contain_type: %s\n", contain_type<T>() ? "true" : "false");
        // Allocate memory with the size of target variant
        storage = new char[sizeof(T)+1];
        // Copy T object to storage
        for(int i = 0; i < sizeof(T); i++)
            storage[i] = ((char*) &v)[i];
    }

    template<typename T>
    T* get(){
        return (T*)(storage);
    }

    // base case for recursion
    void traverse_pack() {}
    
    // recursive function to traverse the pack
    template<typename T, typename... Args>
    void traverse_pack(T arg, Args... args) {
        // process current argument
        // ...
        int t = sizeof(T);
        printf("sizeof: %d\n", t);
        if(t > max)
            max = t;
        // call recursive function for remaining arguments
        traverse_pack(args...);
    }
    
    template<typename T>
    bool contain_type(){ 
        return contain_type<T,TS...>();
    }

    template<typename T,typename Current,typename... Args>
    bool contain_type(){
        if(is_same<T,Current>)
            return true;
        else{
            if(sizeof...(Args) == 0)
                return false;
            return contain_type<T,Args...>();
        }
    }

    int getMax(){
        traverse_pack<TS...>(sizeof(TS)...);
        return max;
    }
};

#include <variant>

class Node {
    public:
    void show(){ printf("Node\n"); }
};

class Object {
    public:
    void show(){ printf("Object\n"); }
};

class Third {
    public:
    void show(){ printf("Third\n"); }
};

template<typename T>
class Test {
    public:
    Test(T v):test(v){ }
    void show(){ 
        // std::get<T>(test).show();
        T* obj = test.get<T>();
        obj->show();
    }

    private:
    // std::variant<Node,Object> test;
    Variant<Node,Object> test;
};


int main()
{
/*
    Node a;
    Test<Node> test(a);
    test.show();
*/

    std::variant<Node,Object> test;
    std::get<Node>(test).show();  

    return 0;
}
