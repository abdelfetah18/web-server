#ifndef REF_POINTER_HEADER
#define REF_POINTER_HEADER

template<typename T>
class RefPointer {
  public:
  RefPointer(T* object = nullptr){
    pointer = object;
    ref = new uint;
    *ref = 1;
  }

  RefPointer(RefPointer& object){
    printf("Copy\n");
    *(object.ref) += 1;
    ref = object.ref;
  }

  T& operator *(){
    return *pointer;
  }

  T* operator ->(){
    return pointer;
  }

  ~RefPointer(){
    *ref -= 1;
    if(*ref == 0)
      delete(pointer);
  }
  private:
  T* pointer;
  uint* ref = nullptr;
};

#endif