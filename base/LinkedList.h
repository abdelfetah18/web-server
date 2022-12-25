#ifndef LINKEDLIST_H
#define LINKEDLIST_H


// FIXME: Bad implementation for SinglyLinkedList Iterator.

template <typename TYPE>
class LinkedListIterator {
public:
    LinkedListIterator(TYPE* node): m_node(node), m_next(node != nullptr ? node->next : nullptr){ }

    TYPE* prev(){ return m_prev; }
    TYPE* node(){ return m_node; }
    TYPE* next(){ return m_next; }

    void increment(){
        if(m_node != nullptr)
            m_prev = m_node;
        m_node = m_next;
        if(m_next != nullptr)
            m_next = m_next->next;
    }

    bool is_end(){
        if(m_node != nullptr)
            return false;
        return true;
    }

private:
    TYPE* m_prev { nullptr };
    TYPE* m_node { nullptr };
    TYPE* m_next { nullptr };
};

template <typename  TYPE>
class LinkedList {
public:
    struct Bucket {
        TYPE value;
        Bucket* next;

        void show(){
            value.show();
        }
    };

    void append(TYPE value){
        if(head != nullptr){
            Bucket* cur = head;
            while(cur->next != nullptr){
                cur = cur->next;
            }
            cur->next = createBucket(value);
        }else{
            head = createBucket(value);
        }
    } // add new bucket to the end of the list

    void push(TYPE value){
        if(head != nullptr){
            Bucket* new_bucket = createBucket(value);
            new_bucket->next = head;
            head = new_bucket;
        }else{
            head = createBucket(value);
        }
    } // push new bucket to the start of the list

    bool find(TYPE value, TYPE& exist){
        Bucket* cur = head;
        while(cur != nullptr){
            if(cur->value == value){
                exist = cur->value;
                return true;
            }

           cur = cur->next;
        }
        return false;
    }

    void show(){
        for(Bucket* cur = head; cur != nullptr; cur = cur->next){
            cur->value.show();
        }
    }

    bool includes(TYPE value){
        Bucket* cur = head;
        while(cur != nullptr){
            if(cur->value == value)
                return true;
            cur = cur->next;
        }
        return false;
    }

    TYPE getHead(){
        if(head != nullptr)
           return head->value;
        else
            return {};
    }

    Bucket* get_head(){ return head; }


private:
    Bucket* head = nullptr;
    Bucket* createBucket(TYPE value){
        Bucket* new_bucket = new Bucket;
        new_bucket->value = value;
        new_bucket->next = nullptr;
        return new_bucket;
    }
};

#endif // LINKEDLIST_H
