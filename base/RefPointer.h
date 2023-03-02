#ifndef REF_POINTER_HEADER
#define REF_POINTER_HEADER

typedef unsigned int uint;

template<typename T>
class RefPointer {
  public:
  RefPointer(T* object = nullptr){
    pointer = object;
    ref = new uint;
    *ref = 1;
  }

  RefPointer(RefPointer& object){
    *(object.ref) += 1;
    ref = object.ref;
  }

  T& operator *(){
    return *pointer;
  }

  T* operator ->(){
    return pointer;
  }

  // FIXME: Allow RefPointer to be initialized with nullptr.
  /*void operator = (T* p){
      pointer = p;
  }*/

  ~RefPointer(){
    *ref -= 1;
    if(*ref == 0)
      delete(pointer);
  }
  T* ptr(){ return pointer;}
  private:
  T* pointer;
  uint* ref = nullptr;
};

#endif
