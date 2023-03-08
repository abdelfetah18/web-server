#include <stdio.h>

// Type Treat is_same which runs at compile time
template<typename T,typename U>
constexpr static bool is_same = false;

template<typename T>
constexpr static bool is_same<T,T> = true;

// Base case for recursive template parameter finder
template<typename T>
constexpr bool contain_type(){
    return false;
}

// Template parameter finder at compile time
template<typename T,typename Current,typename... Args>
constexpr  bool contain_type(){
    if(is_same<T,Current>)
        return true;
    else{
        if(sizeof...(Args) == 0)
            return false;
        return contain_type<T,Args...>();
    }
}


template<typename... TS>
class Variant {
    private:
        int m_selected_type;
        char* m_storage;
    public:

    Variant(){ }

    template<typename T>
    Variant& operator =(T v){
        // printf("contain_type: %s\n", contain_type<T>() ? "true" : "false");
        static_assert(contain_type<T,TS...>(), "Type Not Found");
        
        // Allocate memory with the size of target variant
        m_storage = new char[sizeof(T)+1];
        // Copy T object to m_storage
        for(int i = 0; i < sizeof(T); i++)
            m_storage[i] = ((char*) &v)[i];
        
        return *this;
    }

    template<typename T>
    Variant(T v){
        // printf("contain_type: %s\n", contain_type<T>() ? "true" : "false");
        static_assert(contain_type<T,TS...>(), "Type Not Found");
        // Allocate memory with the size of target variant
        m_storage = new char[sizeof(T)+1];
        // Copy T object to m_storage
        for(int i = 0; i < sizeof(T); i++)
            m_storage[i] = ((char*) &v)[i];
    }

    template<typename T>
    T* get(){
        static_assert(contain_type<T,TS...>(), "Type Not Found");
        return (T*)(m_storage);
    }
};

#include <variant>

class Node {
    public:
    Node():m_str(nullptr){ }
    Node(const char* str){
        unsigned int size = str_len((char*) str);
        m_str = new char[size];
        for(unsigned int i = 0; i < size; i++)
            m_str[i] = str[i];
    }

    void show(){ printf("Node: %s\n", m_str); }

    private:
    unsigned int str_len(char* str){
        unsigned int size = 0;
        while(str[size] != '\0')
            size++;
        return size;
    }
    char* m_str;
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

    // Third a;
    // Test<Third> test(a);
    // test.show();
    Variant<Node,Object> test;
    test = Node("HelloWorld");

    Object* node = test.get<Object>();
    node->show();

    // std::variant<Node,Object> test;
    // std::get<Node>(test).show();  

    return 0;
}
