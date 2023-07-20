#ifndef ERROR_OR_HEADER
#define ERROR_OR_HEADER

#include <stdio.h>
#include "Variant.h"
#include <variant>


typedef unsigned char Byte;
typedef unsigned int uint;


template<typename T,typename E>
class ErrorOr
{
public:
    ErrorOr(ErrorOr&& copy){
        m_is_error = copy.m_is_error;
        m_value_or_error = copy.m_value_or_error;
    };
    
    ErrorOr(T value):m_is_error(false),m_value_or_error(value){ };
    
    ErrorOr(E value):m_is_error(true),m_value_or_error(value){ };

    ~ErrorOr(){ };
    
    bool is_error(){ return m_is_error; };
    
    T& value(){ 
        return std::get<T>(m_value_or_error);
        // TODO: Fix Variant<T,E> class issues. 
        // return m_value_or_error.template get<T>();
    }

    E& error(){ 
        return std::get<E>(m_value_or_error);
        // TODO: Fix Variant<T,E> class issues. 
        // return m_value_or_error.template get<E>();
    }

private:
    bool m_is_error;
    // TODO: Fix Variant<T,E> class issues. 
    std::variant<T, E> m_value_or_error;
};

#endif
