#ifndef VARIANT_HEADER
#define VARIANT_HEADER

#include <stdio.h>
#include<stdlib.h>


typedef unsigned char Byte;
typedef unsigned int uint;

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

template<typename T,int index>
constexpr int IndexOf(){
    return -1;
};

template<typename T,int index,typename U,typename ...TS>
constexpr int IndexOf(){
    if(is_same<T,U>)
        return index;
    return IndexOf<T,index+1,TS...>();    
};

template<typename T,typename ...TS>
constexpr int DefaultConstructorSize(){ return sizeof(T); }

// FIXME: I need to Implement a Copy Constructor.
template<typename... TS>
class Variant {
    private:
        int m_selected_type;
        char* m_storage;

    public:
    Variant():m_selected_type(0){ 
        m_storage = new char[DefaultConstructorSize<TS...>()];
    }

    template<typename T>
    constexpr Variant& operator =(T v){
        m_selected_type = IndexOf<T,0,TS...>();
        // printf("contain_type: %s\n", contain_type<T>() ? "true" : "false");
        static_assert(contain_type<T,TS...>(), "Type Not Found");
        
        // Free the old data
        delete[] m_storage;

        // Allocate memory with the size of target variant
        m_storage = new char[sizeof(T)+1];
        // Copy T object to m_storage
        for(int i = 0; i < sizeof(T); i++)
            m_storage[i] = ((char*) &v)[i];
        
        return *this;
    }

    template<typename T>
    Variant(T v):m_selected_type(IndexOf<T,0,TS...>()){
        // printf("contain_type: %s\n", contain_type<T>() ? "true" : "false");
        static_assert(contain_type<T,TS...>(), "Type Not Found");
        // Allocate memory with the size of target variant
        m_storage = new char[sizeof(T)+1];
        // Copy T object to m_storage
        for(int i = 0; i < sizeof(T); i++)
            m_storage[i] = ((char*) &v)[i];
    }

    template<typename T>
    T& get(){
        static_assert(contain_type<T,TS...>(), "Type Not Found");
        if(m_selected_type != IndexOf<T,0,TS...>()){
            // FIXME: Provide more information.
            printf("Type not Selected");
            // FIXME: make a wrapper for exit function and other syscalls.
            exit(1);
        }
        // TODO: I may use const selected_type for impilict constructors and mutable selected_type for non implicit ones.
        // static_assert(m_selected_type == IndexOf<T,0,TS...>(),"Type not Selected");
        return (T&)*((T*)(m_storage));
    }

    int getSelectedType(){ return m_selected_type; }
};


#endif
