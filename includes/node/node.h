#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

template <class T>
struct node{
    T _item;
    node<T>* _next;
    node<T>* _prev;
    node(const T& item = T(), node<T>* next = nullptr, node<T>* prev = nullptr): _item(item), _next(next), _prev(prev){}
    
    friend ostream& operator << (ostream& outs, const node<T>& print_me){
        outs<<"["<<print_me._item<<"]->";
        return outs;
    }
};

#endif //NODE_H
