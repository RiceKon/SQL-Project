#ifndef STACK_H
#define STACK_H

#include "../node/node.h"
#include "../linked_list_functions/linkedlistfunctionsTemplated.h"
#include <iostream>
#include <cassert>
using namespace std;

template <typename ITEM_TYPE>
class Stack{
public:
    class Iterator{
    public:
        friend class Stack;                 //give access to list to access _ptr
        Iterator(){
            _ptr = NULL;
            }            //default ctor
        Iterator(node<ITEM_TYPE>* p){
            _ptr = p;

        }      //Point Iterator to where
               //  p is pointing to
        ITEM_TYPE operator *(){ //dereference operator
            return _ptr->_item;

        }   
        bool is_null(){
            return _ptr == NULL;
        }            //true if _ptr is NULL
        friend bool operator !=(const Iterator& left, const Iterator& right)  //true if left != right
        {
            return left._ptr != right._ptr;
        }

        friend bool operator ==(const Iterator& left,const Iterator& right)  //true if left == right
        {
            return left._ptr == right._ptr;
        }


        Iterator& operator++(){                        //member operator:++it
                                                       // or ++it = new_value
            _ptr = _ptr->_next;
                                                       // return an iterator
            return *this;
        }

        friend Iterator operator++(Iterator& it, int unused){        //friend operator: it++
            ++it;
            return it;
        }

    private:
        node<ITEM_TYPE>* _ptr;    //pointer being encapsulated
    };

    Stack(){
        _top = nullptr;
        _size = 0;
    }
    Stack(const Stack<ITEM_TYPE>& copyMe){
        _top = _copy_list(copyMe._top);
        _size = copyMe._size;
    }
    ~Stack(){
        _clear_list(_top);
    }
    Stack<ITEM_TYPE>& operator=(const Stack<ITEM_TYPE>& RHS){
        if(this == &RHS){
            return *this;
        }
        _clear_list(_top);
        _top = _copy_list(RHS._top);
        _size = RHS._size;
        return *this;
    }
    ITEM_TYPE top(){
        return _top->_item;
    }
    bool empty(){
        return _top == nullptr;
    }
    // first in last out
    // 1 should leave last
    // if push, push after the 3;
    // head -> 3 2 1
    void push(ITEM_TYPE item){
        _top = _insert_head(_top, item);
        _size++;
    }
    ITEM_TYPE pop(){
        assert(_top != nullptr && "empty linked list touching nullptr");
        _size--;
        return _delete_node(_top, _top);
    }
    template<typename T>
    friend ostream& operator<<(ostream& outs, const Stack<T>& printMe);
    
    
    Iterator begin() const{
        return _top;
    }                   //Iterator to the head node
    Iterator end() const{
        return nullptr;
    }                     //Iterator to NULL
    
    
    int size() const {
        return _size;
        }

private:
    node<ITEM_TYPE>* _top;
    int _size;
};

    template<typename U>
    ostream& operator<<(ostream& outs, const Stack<U>& printMe){
        outs << "Stack: Head ->";
        for(node<U>* walker = printMe._top; walker != nullptr; walker = walker->_next){
            outs << walker->_item << setw(7);
        }
        outs << endl;
        return outs;
    }

#endif //STACK_H