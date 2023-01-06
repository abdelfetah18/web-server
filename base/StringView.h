#ifndef STRING_VIEW_HEADER //Include Guard
#define STRING_VIEW_HEADER

typedef unsigned int uint;

class StringView {
public:
    StringView(char* str, uint size):m_str(str), m_size(size){ }
    StringView(char* str);
    uint length();

    char operator [](uint index){
        return m_str[index];
    }

private:
    char* m_str { nullptr };
    uint size = 0;
};

#endif
