#ifndef BPLUS_TREE_H
#define BPLUS_TREE_H
#include <iostream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include "../btree_array_functions/btree_array_functions.h"
using namespace std;

template <class T>
class BPlusTree
{
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it= NULL, int _key_ptr = 0): it(_it), key_ptr(_key_ptr){}

        T operator *(){
            T temp = it->data[key_ptr];
            return temp;
        }

        Iterator operator++(int un_used){
            Iterator temp(it, key_ptr);
            ++(*this);
            return temp;
        }
        Iterator operator++(){
            if(it == NULL){
                return *this;
            }
            key_ptr++;
            if(key_ptr == it->data_count){
                it = it->next;
                key_ptr = 0;
            }
            
            return *this;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return lhs.it != rhs.it || (lhs.it == rhs.it && lhs.key_ptr != rhs.key_ptr);
        }
        void print_Iterator(){
            if (it){
                cout<<"iterator: ";
                print_array(it->data, it->data_count, key_ptr);
            }
            else{
                cout<<"iterator: NULL, key_ptr: "<<key_ptr<<endl;
            }
        }
        bool is_null(){return !it;}
        void info(){
            cout<<endl<<"Iterator info:"<<endl;
            cout<<"key_ptr: "<<key_ptr<<endl;
            cout<<"it: "<<*it<<endl;
        }

    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups = false): dups_ok(dups), data_count(0), child_count(0), next(NULL){};
    BPlusTree(T *a, int size, bool dups = false);

    //big three:
    BPlusTree(const BPlusTree<T>& other);
    ~BPlusTree();
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS);

    void copy_tree(const BPlusTree<T>& other);  //copy other into this object
    void copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node);

    void insert(const T& entry);   //insert entry into the tree
    void remove(const T& entry);   //remove entry from the tree
    void clear_tree();             //clear this object (delete all nodes etc.)

    bool contains(const T& entry) const; //true if entry can be found
    T& get(const T& entry);              //return a reference to entry
    const T& get(const T& entry)const;   //return a reference to entry
    T& get_existing(const T& entry);     //return a reference to entry
    Iterator find(const T& key);         //return an iterator to this key.
                                         //     NULL if not there.
    Iterator lower_bound(const T& key);  //return first that goes NOT BEFORE
                                         // key entry or next if does not
                                         // exist: >= entry
    Iterator upper_bound(const T& key);  //return first that goes AFTER key
                                         //exist or not, the next entry  >entry

    int size() const;                    //count the number of elements

    bool empty() const;                  //true if the tree is empty

    void print_tree(int level = 0,ostream &outs=cout) const;


    friend ostream& operator<<(ostream& outs,
                               const BPlusTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid();
    string in_order();
    string array_elements();
    string pre_order();
    string post_order();

    Iterator begin();
    Iterator end();
    ostream& list_keys(Iterator from = NULL, Iterator to = NULL){
        if (from == NULL) from = begin();
        if (to == NULL) to = end();
        for (Iterator it = from; it != to; it++)
            cout<<*it<<" ";
        return cout;

    }

private:
    //MEMBER VARIABLES
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                        //true if duplicate keys are allowed
    int data_count;                      //number of data elements
    T data[MAXIMUM + 1];                 //holds the keys
    int child_count;                     //number of children
    BPlusTree* subset[MAXIMUM + 2];      //subtrees
    BPlusTree* next;



    bool is_leaf() const{return child_count==0;}             //true if this is a leaf node

    T* find_ptr(const T& entry);         //return a pointer to this key.
                                         // NULL if not there.

    //insert element functions
    void loose_insert(const T& entry);   //allows MAXIMUM+1 data elements in
                                         //   the root
    void fix_excess(int i);              //fix excess in child i

    //remove element functions:
    void loose_remove(const T& entry);  //allows MINIMUM-1 data elements
                                        //  in the root

    BPlusTree<T>* fix_shortage(int i);  //fix shortage in child i
    // and return the smallest key in this subtree
    BPlusTree<T>* get_smallest_node();
    BPlusTree<T>* get_biggest_node();
    void get_smallest(T& entry);      //entry := leftmost leaf
    void get_biggest(T& entry);       //entry := rightmost leaf
    void remove_biggest(T& entry);    //remove the biggest child of tree->entry
    void transfer_left(int i);        //transfer one element LEFT from child i
    void transfer_right(int i);       //transfer one element RIGHT from child i
    BPlusTree<T> *merge_with_next_subset(int i); //merge subset i with  i+1

};
template<typename T>
BPlusTree<T>::BPlusTree(T *a, int size, bool dups): dups_ok(dups), data_count(0), child_count(0), next(NULL){
    /*
    docstring:
    the construct will take in an array of values, and push it into the datastructure, it will need an size, 
    however will not need to declare dups(it is automatically false)

    */
    for(int i = 0; i < size; i++){
        insert(a[i]);
    }  
}



//big three:
template <typename T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other){
    /*
    docstring: this ctor blackboxes the copy ctor
    */
    copy_tree(other);
}

template <typename T>
BPlusTree<T>:: ~BPlusTree(){
    clear_tree();
}

template <typename T>
BPlusTree<T>& BPlusTree<T>::operator =(const BPlusTree<T>& RHS){
        /*
    docstring: this ctor blackboxes the equal ctor
    */
    if(this == &RHS){
        return *this;
    }
    clear_tree();
    copy_tree(RHS);
    return *this;
}

template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other){

    /*
    docstring
    this function makes it hides the passing of null pointer into the copy_tree function
    */
    BPlusTree<T>* Last = nullptr;
    copy_tree(other, Last);
}  //copy other into this object

template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node){
    /*
    docstring
    this function copies the tree and link up the bottom, as a "linked list" making it easy for traversal
    */

    copy_array(data, other.data, data_count ,other.data_count);
    child_count = other.child_count;
    dups_ok = other.dups_ok;

    // while if it not a leaf allocate space for for a subtree and pass it in
    if(!is_leaf()){
        for(int i = 0; i < other.child_count; i++){
                subset[i] = new BPlusTree<T>();
                subset[i]->copy_tree(*other.subset[i], last_node);
            }
        return;
    }else if(last_node){
        // link up the list at the bottom
        this->next = last_node->next;
        last_node->next = this;   
    }
        last_node = this;
        
}

template <typename T>
void BPlusTree<T>::insert(const T& entry){
    /*
    docstring
    insert the nodes in the apporiate space and take care of any inbalance
    */
    loose_insert(entry);

    // fix fat parent
    if(data_count > MAXIMUM){
        BPlusTree<T>* LEFT = new BPlusTree<T>();
        // everything and the assing it to the left later becasue of merge subset
        copy_array(LEFT->data, data, LEFT->data_count, data_count);
        copy_array(LEFT->subset, subset, LEFT->child_count, child_count);
        // clear the current
        clear_list(data, data_count);
        clear_list(subset, child_count);
        // insert at the left
        insert_item(subset, 0 , child_count, LEFT);

        fix_excess(0);      
    }
}   //insert entry into the tree

template <typename T>
void BPlusTree<T>::remove(const T& entry){
      /*
    docstring
    remove the nodes in the apporiate space and take care of any inbalance
    */
    loose_remove(entry);


    if(!is_leaf() && data_count < MINIMUM){
        int i = subset[0] ? 0 : 1;

        BPlusTree* temp = new BPlusTree<T>();
        
        copy_array(data, subset[i]->data, data_count , subset[i]->data_count);
        delete_item(subset, i, child_count, temp);
        copy_array(subset, temp->subset, child_count, temp->child_count);

        temp->child_count = 0;
        delete temp;            
    }
}   //remove entry from the tree

template <typename T>
void BPlusTree<T>::clear_tree(){


    for(int i = 0; i < data_count; i++){
        data[i] = T();
    }
    
    for(int i = 0; i < child_count; i++){
        subset[i]->clear_tree();
        delete subset[i];
    }
    child_count = 0;
    data_count = 0;
}             //clear this object (delete all nodes etc.)

template <typename T>
bool BPlusTree<T>::contains(const T& entry) const{
    int index = first_ge(data, data_count, entry) ;
    bool found = (index < data_count && data[index] == entry);               //flag to check if the entry is found

    // if(found && is_leaf()){
    //     return true;
    // }else
    if(found && !is_leaf()){
        return subset[index + 1]->contains(entry);
    }else if(!found && !is_leaf()){
        return subset[index]->contains(entry);
    }
    else if(!found && is_leaf()){
        return false;
    }        

    return true;

} //true if entry can be found

template <typename T>
T& BPlusTree<T>::get(const T& entry){
    /*
    docstring:
    this function "binary searches for entry, if it does not exist insert it into the tree"

    */
    if(contains(entry)){
        int index = first_ge(data,data_count, entry);
        bool found = (index < data_count && data[index] == entry);               //flag to check if the entry is found
        if(found && is_leaf()){
            return data[index];
        }else if(found && !is_leaf()){
            // if it is found go into the right subtree of the the found index
            return subset[index + 1]->get(entry);
        }else if(!found && !is_leaf()){
            // look for it
            return subset[index]->get(entry);
        }
    }

    insert(entry);
    return get_existing(entry);
    

}              //return a reference to entry

template <typename T>
const T& BPlusTree<T>::get(const T& entry)const{
    int index = first_ge(data, data_count, entry);
    bool found = (index < data_count && data[index] == entry);               //flag to check if the entry is found
    // if(found && is_leaf()){
    //     return data[index];
    // }else
    if(found && !is_leaf()){
        return subset[index + 1]->get(entry);
    }else if(!found && !is_leaf()){
        return subset[index]->get(entry);
    }
    else if(!found && is_leaf()){
        throw runtime_error("get: entry not found");
    }   
    
    return data[index];

        
}   //return a reference to entry

template <typename T>
T& BPlusTree<T>::get_existing(const T& entry){
    int index = first_ge(data, data_count, entry) ;
    bool found = (index < data_count && data[index] == entry);               //flag to check if the entry is found
    // if(found && is_leaf()){
    //     return data[index];
    // }
    if(found && !is_leaf()){
        return subset[index + 1]->get_existing(entry);
    }else if(!found && !is_leaf()){
        return subset[index]->get_existing(entry);
    }
    else if(!found && is_leaf()){
        throw runtime_error("get: entry not found");
    }   
    
    return data[index];

}     //return a reference to entry

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T& key){
    int index = first_ge(data, data_count, key);
       
    bool found = (index < data_count && data[index] == key);               //flag to check if the entry is found
    // if(found && is_leaf()){
    //     return Iterator(this, index);
    // }else

    if(found && !is_leaf()){
        return subset[index + 1]->find(key);
    }else if(!found && !is_leaf()){
        return subset[index]->find(key);
    }
    else if(!found && is_leaf()){
        return Iterator(NULL, 0);
    }   

    return Iterator(this, index);
}         //return an iterator to this key.
                                        //     NULL if not there.
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::lower_bound(const T& key){
    /*
    docstring
    this funciton upper_bound would will return the index/itterator, of the first value that is greater then or equal to the key
    */
    int index = first_ge(data, data_count, key);

    bool found = (index < data_count && data[index] == key);               //flag to check if the entry is found
    if(!is_leaf()){
        if (found){
            return subset[index + 1]->lower_bound(key);
        }else{
            return subset[index]->lower_bound(key);
        }
    }else{
        // index = index == 0 ? 0 : index - 1;
        bool nonneg = index == 0 ? 0 : 1;
        Iterator it(this, index - nonneg);
        while(it.it && *it < key){
            it++;
        }
        return it;
    }

    

    return Iterator(NULL, 0);
}  //return first that goes NOT BEFORE

                                        // key entry or next if does not
                                        // exist: >= entry
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::upper_bound(const T& key){
    /*
    docstring

    this funciton upper_bound would will return the index + 1 (basically return an iterator of the current index),
    of the first value that is greater then  to the key
    */
   
    Iterator it = lower_bound(key);
    if(it.it && *it <= key){
        ++it;
    }
    return it;

}  //return first that goes AFTER key
                                        //exist or not, the next entry  >entry

template <typename T>
int BPlusTree<T>::size() const{
    return data_count;
}                    //count the number of elements

template <typename T>
bool BPlusTree<T>::empty() const{
    return data_count == 0;
}                  //true if the tree is empty

template <typename T>
void BPlusTree<T>::print_tree(int level, ostream &outs ) const{
    if(!is_leaf()){
            subset[child_count - 1]->print_tree(level + 1, outs);
        }
        // then print the right most sub trees (right to left)
        for(int i = data_count - 1; i >= 0; i--){
            cout << setw(10 * level) << data[i] << endl << endl;
            if(!is_leaf()){
                subset[i]->print_tree(level + 1, outs);
            }
        }
}


template <typename T>
bool BPlusTree<T>::is_valid(){
    if(data_count > MAXIMUM){
        return false;
    }else if(!is_leaf() && child_count != data_count + 1){
        return false;
    }else if(!is_leaf() && child_count < MINIMUM){
        return false;
    }else if(!is_leaf() && child_count > MAXIMUM + 1){
        return false;
    }else if(!is_leaf() && !subset[0]->is_valid()){
        return false;
    }else if(!is_leaf() && !subset[child_count - 1]->is_valid()){
        return false;
    }else if(!is_leaf()){
        for(int i = 0; i < data_count; i++){
            if(data[i] < subset[i]->get_smallest_node()->data[0]){
                return false;
            }
        }
    }else if(!is_leaf()){
        for(int i = 0; i < data_count; i++){
            if(!subset[i]->is_valid()){
                return false;
            }
        }
    }

    for(int i = 0; i < data_count - 1; i++){
        if(data[i] > data[i + 1]){
            return false;
        }
    }
    return true;
}
template <typename T>
string BPlusTree<T>::in_order(){
    /*
    docstring
    this return the whole traversal of inorder, repeating the dupilcates
    */
    string result = "";
    int j = child_count - 1;
    if(!is_leaf() && child_count > 1){
        result.insert(0, subset[j]->in_order());
        j--;
    }

    // then print the right most sub trees (right to left)
    for(int i = data_count - 1; i >= 0; i--){
        result.insert(0, to_string(data[i]) + "|");
        if(!is_leaf()){
            result.insert(0, subset[j]->in_order());
            j--;
        }
    }
    return result;
}

template <typename T>
string BPlusTree<T>::array_elements(){
    string inorder = "";
    for(Iterator it = this->begin(); it != this->end(); it++){
        inorder += to_string(*it) + "|";
    }
    return inorder;
}
template <typename T>
string BPlusTree<T>::pre_order(){
    string pre_order = "";
    for(int i = 0; i < data_count; i++){
        pre_order += to_string(data[i]) + "|";
        if(!is_leaf()){
            if(i == 0)
                pre_order += subset[i]->pre_order();
            pre_order += subset[i + 1]->pre_order();
        }

    }
    return pre_order;

}
template <typename T>
string BPlusTree<T>::post_order(){
    string post_order = "";
    for (int i = 0; i < data_count; i++){
        if(!is_leaf()){
            if(i == 0)
                post_order += subset[i]->post_order();
            post_order += subset[i + 1]->post_order();
        }
        post_order += to_string(data[i]) + "|";
    
    }
    return post_order;
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::begin(){
    if(subset[0] && !is_leaf()){
        return subset[0]->begin();
    }else if (!empty() && is_leaf()){
        return Iterator (this, 0);
    }else{
        return Iterator(NULL, 0);
    }
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::end(){
    return Iterator(NULL, 0);
}

template <typename T>
T* BPlusTree<T>::find_ptr(const T& entry){
    int index = first_ge(data, data_count, entry);
       
    bool found = (index < data_count && data[index] == entry);               //flag to check if the entry is found
    // if(found && is_leaf()){
    //     return this;
    // }else 
    if(found && !is_leaf()){
        return subset[index + 1]->find_ptr(entry);
    }else if(!found && !is_leaf()){
        return subset[index]->find_ptr(entry);
    }
    else if(!found && is_leaf()){
        return nullptr;
    }   

    return this;
}       //return a pointer to this key.
        // NULL if not there.

//insert element functions
template <typename T>
void BPlusTree<T>::loose_insert(const T& entry){
    int index = first_ge(data, data_count, entry);
        bool found = (index < data_count && data[index] == entry);               //flag to check if the entry is found

        if(is_leaf() && found && !dups_ok){                            // check wether its a duplicate
            data[index] = entry;
            return;
        }else if(is_leaf() && found && dups_ok){
            data[index] = data[index] + entry; 
            return;
        }
        else if(found && !is_leaf()){                                  // keep looking
            subset[index + 1]->loose_insert(entry);
        }
        else if(!found && !is_leaf()){
            subset[index]->loose_insert(entry);
        }
        else if(!found && is_leaf()){
            insert_item(data, index, data_count, entry);
            return;
        }


        if(!is_leaf() && subset[index]->data_count > MAXIMUM){                          // check if each data count exceedes the max
            fix_excess(index);                                                              // fix access
        }

}   //allows MAXIMUM+1 data elements in
    //   the root
template <typename T>
void BPlusTree<T>::fix_excess(int i){
        BPlusTree<T>* temp = new BPlusTree();
        // get half of the data
        T midItem;                        
        delete_item(subset[i]->data, MAXIMUM/2, subset[i]->data_count, midItem);
            
        split(subset[i]->data, subset[i]->data_count, temp->data, temp->data_count);        // split the data
        split(subset[i]->subset, subset[i]->child_count, temp->subset, temp->child_count);
        
        ordered_insert(data, data_count, midItem);              
        int index = first_ge(data, data_count, temp->data[0]);
        insert_item(subset, index, child_count, temp);

        if(temp->is_leaf()){
            ordered_insert(temp->data, temp->data_count, midItem);
            subset[i + 1]->next = subset[i]->next;
            subset[i]->next = subset[i + 1];
            
        }
}
                //fix excess in child i
                //remove element functions:
template <typename T>
void BPlusTree<T>::loose_remove(const T& entry){
    int index = first_ge(data, data_count, entry);

    bool found = (index < data_count && data[index] == entry);               //flag to check if the entry is found
    T temp = entry;                                                          // dont change the value of entry 

    if(!is_leaf() && found){
        remove_biggest(temp);
    }else if(is_leaf() && found){
        delete_item(data, index, data_count, temp);
        return;
    }else{
        subset[index]->loose_remove(entry);
    }



    if(!is_leaf() && subset[index + found]->data_count < MINIMUM){            // check if the if there is a shortage
        fix_shortage(index + found);                                         
    }
}               //allows MINIMUM-1 data elements
                //  in the root

template <typename T>
BPlusTree<T>* BPlusTree<T>::fix_shortage(int i){
    int index = i >= data_count ? i - 1 : i;

    if(i - 1 >= 0 && subset[i - 1]->data_count > MINIMUM){          
        // if the left sibling can lend a value
        transfer_right(i - 1);   
    }else if(i + 1 < child_count&& subset[i + 1]->data_count > MINIMUM){                          
        // if the right sibling can lend a value
        transfer_left(i + 1);
    }
    else if(data_count == child_count - 1){        
        // there is the same amount of data and the number or children or more                       
        merge_with_next_subset(index);                                                  
    }
    
    return this;
} 
                    //fix shortage in child i
                    // and return the smallest key in this subtree

template <typename T>
BPlusTree<T>* BPlusTree<T>::get_smallest_node(){
    if (!is_leaf()){
        return subset[0]->get_smallest_node();
    }else{
        return this;
    }
}

template <typename T>
BPlusTree<T>* BPlusTree<T>::get_biggest_node(){
    if(!is_leaf()){
        return subset[child_count - 1]->get_biggest_node();
    }
    else{
        return this;
    }
}


template <typename T>
void BPlusTree<T>::get_smallest(T& entry){
    if(!is_leaf()){
        subset[0]->get_smallest(entry);
    }else{
        entry = data[0];
        return;
    }
}      //entry := leftmost leaf

template <typename T>
void BPlusTree<T>::get_biggest(T& entry){  
    if(!is_leaf()){
        subset[child_count - 1]->get_biggest(entry);
    }else{
        entry = data[data_count - 1];
    }

}       //entry := rightmost leaf

template <typename T>
void BPlusTree<T>::remove_biggest(T& entry){
    int index = first_ge(data, data_count, entry);
    bool found = (index < data_count && data[index] == entry);               //flag to check if the entry is found

    if(!is_leaf()){
        subset[index + found]->remove_biggest(entry);
        if(found && data[index] != entry){
            data[index] = entry;
        }else if (found && data[index] == entry){
            subset[index + found]->get_smallest(data[index]);
        }
    }else{
        Iterator it = ++find(entry);
        T temp = (it != end()) ? *it : entry;
        delete_item(data, index, data_count, entry);
        entry = temp;
    }

    if(!is_leaf() && !found && subset[index + found]->data_count < MINIMUM){
        fix_shortage(index + found);
    }


}    //remove the biggest child of tree->entry

template <typename T>
void BPlusTree<T>::transfer_left(int i){
    cout << "transfer left" << endl;
    // moving the data
    T demotion, promotion;
    delete_item(data, i - 1, data_count, demotion);
    
    // when it is not the leaf case, transfer the subsets 
    if(!subset[i]->is_leaf()){
        BPlusTree<T>* temp;
        // transfer the breadcrumb
        insert_item(subset[i - 1]->data, subset[i - 1]->data_count, subset[i - 1]->data_count, demotion);
        delete_item(subset[i]->data, 0, subset[i]->data_count, promotion);
        insert_item(data, i - 1, data_count, promotion);

        delete_item(subset[i]->subset, 0, subset[i]->child_count, temp);
        insert_item(subset[i - 1]->subset, subset[i - 1]->child_count, subset[i - 1]->child_count, temp);
    }
    // when it is the leaf case, transfter the data down one
    else{
        // deleting the breadcrumb and the data
        delete_item(subset[i]->data, 0, subset[i]->data_count, demotion);
        // promoting the data giving it it's own breadcrumb
        insert_item(data, i - 1, data_count, subset[i]->data[0]);
        insert_item(subset[i - 1]->data, subset[i - 1]->data_count, subset[i - 1]->data_count, demotion);
    }

}        //transfer one element LEFT from child i

template <typename T>
void BPlusTree<T>::transfer_right(int i){
    cout << "transfer right" << endl;
    // moving the data
    T demotion, promotion;
    if(!subset[i]->is_leaf()){
        BPlusTree<T>* temp;
        // rotate down the breadcrumb
        delete_item(data, i, data_count, demotion);
        ordered_insert(subset[i + 1]->data, subset[i + 1]->data_count, demotion);

        // rotate up the data/breadcrumb
        delete_item(subset[i]->data, subset[i]->data_count - 1, subset[i]->data_count, promotion);
        ordered_insert(data, data_count, promotion);        

        // transfer the subsets
        delete_item(subset[i]->subset, subset[i]->child_count - 1, subset[i]->child_count, temp);
        insert_item(subset[i + 1]->subset, 0, subset[i + 1]->child_count, temp);

    }
    else{
        delete_item(data, i, data_count, demotion);
        delete_item(subset[i]->data, subset[i]-> data_count - 1, subset[i]->data_count, promotion);
        insert_item(data, i, data_count, promotion);
        insert_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, promotion);
    }

}       //transfer one element RIGHT from child i

template <typename T>
BPlusTree<T>* BPlusTree<T>::merge_with_next_subset(int i){
    cout << "merge with next subset" << endl;
    BPlusTree<T>* deleteTree, *nextTree = subset[i + 1]->next;
    T demotion;

    delete_item(data, i, data_count, demotion);
    if(!subset[i]->is_leaf()){
        insert_item(subset[i]->data, subset[i]->data_count, subset[i]->data_count, demotion);
    }

    merge(subset[i]->data, subset[i]->data_count, subset[i + 1]->data, subset[i + 1]->data_count);
    merge(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset, subset[i + 1]->child_count);

    delete_item(subset, i + 1, child_count, deleteTree);
    delete deleteTree;
    if(subset[i]->is_leaf()){
        subset[i]->next = nextTree;
    }
    
    return this;
} //merge subset i with  i+1


#endif // BPLUS_TREE_H