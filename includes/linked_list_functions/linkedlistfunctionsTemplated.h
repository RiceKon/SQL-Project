#ifndef LINKEDLISTFUNCTIONSTEMPLATED_H
#define LINKEDLISTFUNCTIONSTEMPLATED_H
#include "../node/node.h"
#include <iostream>
#include <iomanip>

using namespace std;
//Linked List General Functions:
template <typename T>
void _print_list(node<T>* head);
//recursive fun! :)
template <typename T>
void _print_list_backwards(node<T> *head);
//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head,
                            T key);
template <typename T>
node<T>* _insert_head(node<T> *&head,
                            T insert_this);
//insert after ptr: insert head if marker null
template <typename T>
node<T>* _insert_after(node<T>*& head,
                                node<T> *after_this,
                                T insert_this);
//insert before ptr: insert head if marker null
template <typename T>
node<T>* _insert_before(node<T>*& head,
                                node<T>* before_this,
                                T insert_this);
//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this);
//delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this);
//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head);
//duplicate the list, return pointer to last node in dest... 
//     use this function in your queue big three 
template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src);
//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head);
//_item at this position
template <typename T>
T& _at(node<T>* head, int pos);



template <typename T>
void _print_list(node<T>* head){
    while(head != nullptr){
        cout << head->_item << "   ";
        head = head->_next;
    }
    cout << endl;
}


//recursive fun! :)
template <typename T>
void _print_list_backwards(node<T> *head){
    if (head == nullptr){
        cout << "[]" << endl;
    }else{
        node<T>* walker = head;
        while(walker->_next != nullptr){
            walker = walker->_next;
        }
        for(;walker != nullptr; walker = walker->_prev){
            cout << "[" << walker->_item << "]" << setw(10);
        }
        cout << endl;
    }


}


//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head, T key){
    if (head == nullptr){
        return nullptr;
    }
    node<T>* temp = head;
    while (temp != nullptr){
        if(temp->_item == key){
            return temp;
        }
        temp = temp->_next;
    }
    return nullptr;
}


template <typename T>
node<T>* _insert_head(node<T> *&head, T insert_this){
    // make  a new node with head as it next and
    node<T>* temp = new node<T>(insert_this, head, nullptr);
    if(head != nullptr){
        head->_prev = temp;
    }

    head = temp;

    return head;
}


//insert after ptr: insert head if marker null
template <typename T>
node<T>* _insert_after(node<T>*& head, node<T> *after_this, T insert_this){
// case 1: when called insert after when empty
    if(head == nullptr){
        return _insert_head(head, insert_this);;
    }
//case 2: when called in the middle
    node<T>* temp = new node<T>(insert_this, after_this->_next, after_this);
//case 3: at the end
    if(after_this->_next != nullptr){
        after_this->_next->_prev = temp;
    }
    after_this->_next = temp;

    

    return temp;
}


//insert before ptr: insert head if marker null
template <typename T>
node<T>* _insert_before(node<T>*& head, node<T>* before_this, T insert_this){
    if(head == before_this){
        head = _insert_head(head, insert_this);
        return head;
    }

    node<T>* temp = new node<T>(insert_this, before_this, before_this->_prev);

    if(before_this->_prev != nullptr){
        before_this->_prev->_next = temp;
    }

    before_this->_prev = temp;

    return temp;
}


//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this){
    if(prev_to_this == nullptr){
        return nullptr;
    }
    return prev_to_this->_prev;
}


//delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this){
    if(head == nullptr){
        return T();
    }
    node<T>* temp = delete_this;
    T item = delete_this->_item;
    // if deleting the first and last node
    if(delete_this == head && delete_this->_next == nullptr){
        // have head point to nothing
        head = nullptr;
        // delete item
        delete delete_this;
        // return item
        return item;
    }// if deleting the first node of the linked lsit  
    else if(delete_this == head){
        // move the head to the next
        head = head->_next;
        // have the prev pointer point to nothing
        head->_prev = nullptr;
        // delete old node
        delete delete_this;
        // return item
        return item;
    } // if deleating the laste node of the linked list
    else if(delete_this->_next == nullptr){
        temp = delete_this->_prev;
        temp->_next = nullptr;
        delete delete_this;
        return item;
    }

    node<T>* before = delete_this->_prev;
    node<T>* after = delete_this->_next;

    before->_next = after;
    after->_prev = before;

    delete delete_this;

    return item;
}


//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head){ //return the head of the fucntion
    if (head == nullptr){
        return head;
    }
    node<T>* new_head = nullptr;

    node<T>* walker = head;

    while (head != nullptr){
        walker = _insert_after(new_head, walker, head->_item);
        head = head->_next;
    }
    return new_head;
}


//duplicate the list, return pointer to last node in dest... 
//     use this function in your queue big three 
template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src){ // return the rear of the function
    dest = _copy_list(src);
    node<T>* walker = dest;
    while (walker != nullptr){
        if(walker->_next == nullptr){
            return walker;
        }
        walker = walker->_next;
    }
    return walker;
}


//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head){
      // head / current node
    node<T> *walker_node = head;

    // next node
    node<T> *next_walker;

    // while the next node is not null ptr
    while (walker_node != nullptr)
    {
        // next node is pointing to the node after the walker_node
        next_walker = walker_node->_next;

        // delete the walker
        delete walker_node;

        // walker is now the next node
        walker_node = next_walker;
    }

    // set head to null
    head = nullptr;
}


//_item at this position
template <typename T>
T& _at(node<T>* head, int pos){
    int i = 0;
    //walker from the head
    node<T>* walker = head;
    while (i < pos){
        walker = walker->_next;
    }
    return walker;
}


#endif //LINKEDLISTFUNCTIONSTEMPLATED_H