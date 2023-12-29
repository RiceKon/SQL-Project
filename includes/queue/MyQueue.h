#ifndef QUEUE_H
#define QUEUE_H

#include "../node/node.h"
#include "../linked_list_functions/linkedlistfunctionsTemplated.h"
#include <iostream>

using namespace std;

template <typename T>
class Queue
{
public:
    class Iterator{
    public:
        friend class Queue;                               //give access to list to access _ptr
        Iterator(){
            _ptr = nullptr;
        }                          //default ctor
        Iterator(node<T>* p){
            _ptr = p;
        }                            //Point Iterator to where
                                                          //  p is pointing to
        T operator *(){
            return _ptr->_item;
        }
       //dereference operator
        bool is_null(){
            return _ptr == nullptr;
        }            //true if _ptr is NULL
        friend bool operator !=(const Iterator& left, const Iterator& right){  //true if left != right
            return left._ptr != right._ptr;

        }

        friend bool operator ==(const Iterator& left, const Iterator& right){  //true if left == right
            return left._ptr == right.ptr;
        }


        Iterator& operator++(){                         //member operator:++it
                                                        // or ++it = new_value
            // 
            _ptr =_ptr->_next;
            return *this;
        }

        friend Iterator operator++(Iterator& it, int unused){         //friend operator: it++
            ++it;
            return it;
        }

    private:
        node<T>* _ptr;                          //pointer being encapsulated
    };

    Queue(){
        // assign values into all of the variables
        _front = nullptr;
        _rear = nullptr;
        _size = 0;
    }

    Queue(const Queue<T>& copyMe){
        // copy the list then the size
        _rear = _copy_list(_front,copyMe._front);
        _size = copyMe._size;
    }
    ~Queue(){
        // calling clear list
        _clear_list(_front);
    }
    Queue& operator=(const Queue<T>& RHS){
        // if the address are the same return the same Queue back to the user
        if(this == &RHS){
            return *this;
        }
        _clear_list(_front);
        _rear = _copy_list(_front, RHS._front);
        _size = RHS._size;
        return *this;
    }

    bool empty(){
        return _front == nullptr;
    }
    T front(){
        return _front->_item;
    }
    T back(){
        return _rear->_item;
    }
    // insert before at the _rear
    // head -> 1 2 <- insert 3
    // _insert after return the node that is inserted
    void push(T item){
        _rear = _insert_after(_front, _rear, item);
        _size++;
    }
    // first in first out (delete the front)
    // head ->  1 2 3 4 5 6 7 8 9 10
    T pop(){
        if (_size > 0 ){
            _size--;
            return _delete_node(_front, _front);
        }
        return T();
    }


    Iterator begin() const{
        return _front;
    }                                     //Iterator to the head node
    Iterator end() const{
        return nullptr;
    }                                       //Iterator to NULL
    void print_pointers();
    int size() const {
        return _size;
    }
    template<typename TT>
    friend ostream& operator << (ostream& outs, const Queue<TT>& printMe);
private:
    node<T>* _front;
    node<T>* _rear;
    int _size;
};

    template<typename U>
    ostream& operator<<(ostream& outs, const Queue<U>& printMe){
        outs << "Queue: Head -> ";
        for(node<U>* walker = printMe._front; walker != nullptr; walker = walker->_next){
            outs << walker->_item << "  ";
        }
        outs << endl;
        return outs;
    }



#endif // QUEUE_H