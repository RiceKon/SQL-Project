#ifndef BTREE_H
#define BTREE_H
#include <iostream>
#include <iomanip>
#include "../btree_array_functions/btree_array_functions.h"
using namespace std;
//asdadadsa//


template <class T>
class BTree
{
public:
    BTree(bool dups = false): dups_ok(dups), data_count(0), child_count(0){}

    BTree(T *a, int size, bool dups = false): dups_ok(dups), data_count(0), child_count(0){
        for(int i = 0; i < size; i++){
            insert(a[i]);
        }  
    }

    //big three:
    BTree(const BTree<T>& other){
        copy_tree(other);
    }

    ~BTree(){
        clear_tree();
    }
    
    BTree<T>& operator =(const BTree<T>& RHS){
        if(this == &RHS){
            return RHS;
        }
        clear_tree();
        copy_tree(RHS);
        return *this;
    }

    void make_tree(){
        data[0] = 100;
        data[1] = 500;
        data_count = 2;
        child_count = 3;
        // left most subtree

        subset[0] = new BTree<T>();
        subset[0]->data[0] = 50;
        subset[0]->data[1] = 75;
        subset[0]->data_count = 2;
        subset[0]->child_count = 3;

        // leaf
        subset[0]->subset[0] = new BTree<T>();
        subset[0]->subset[0]->data[0] = 25;
        subset[0]->subset[0]->data_count = 1;
        subset[0]->subset[0]->child_count = 0;
        // leaf
        subset[0]->subset[1] = new BTree<T>();
        subset[0]->subset[1]->data[0] = 60;
        subset[0]->subset[1]->data_count = 1;
        subset[0]->subset[1]->child_count = 0;

        // leaf

        subset[0]->subset[2] = new BTree<T>();
        subset[0]->subset[2]->data[0] = 90;
        subset[0]->subset[2]->data_count = 1;
        subset[0]->subset[2]->child_count = 0;

        // middle subtree
        subset[1] = new BTree<T>();
        subset[1]->data[0] = 200;
        subset[1]->data_count = 1;
        subset[1]->child_count = 2;


        subset[1]->subset[0] = new BTree<T>();
        subset[1]->subset[0]->data[0] = 150;
        subset[1]->subset[0]->child_count = 0;
        subset[1]->subset[0]->data_count = 1;

        subset[1]->subset[1] = new BTree<T>();
        subset[1]->subset[1]->data[0] = 250;
        subset[1]->subset[1]->child_count = 0;

        subset[1]->subset[1]->data_count = 1;

        subset[2] = new BTree<T>();
        subset[2]->data[0] = 700;
        subset[2]->data[1] = 900;
        subset[2]->data_count = 2;
        subset[2]->child_count = 3;

        subset[2]->subset[0] = new BTree<T>();
        subset[2]->subset[0]->data[0] = 600;
        subset[2]->subset[0]->data_count = 1;
        subset[2]->subset[0]->child_count = 0;

        subset[2]->subset[1] = new BTree<T>();
        subset[2]->subset[1]->data[0] = 800;
        subset[2]->subset[1]->data_count = 1;
        subset[2]->subset[1]->child_count = 0;


        subset[2]->subset[2] = new BTree<T>();
        subset[2]->subset[2]->data[0] = 1000;
        subset[2]->subset[2]->data_count = 1;      
        subset[2]->subset[2]->child_count = 0;
 
    }
void make_tree2(){
        data[0] = 200;
        data_count = 1;
        child_count = 2;
        subset[0] = new BTree<T>();
        subset[0]->data[0] = 150;
        subset[0]->data_count = 1;
        subset[0]->child_count = 0;

        subset[1] = new BTree<T>();
        subset[1]->data[0] = 250;
        subset[1]->data_count = 1;
        subset[1]->child_count = 0;

    }

    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree

    void clear_tree();                          //clear this object
                                                //  (delete all nodes etc.)
    void copy_tree(const BTree<T>& other);      //copy other into this object

    bool contains(const T& entry);              //true if entry can be found in
                                                //                  the array
    T& get(const T& entry);                     //return a reference to entry
                                                //                  in the tree
    T* find(const T& entry);                    //return a pointer to this key.
                                                //          NULL if not there.

    int size() const{return data_count;}                           //count the number of elements
                                                //              in the tree
    bool empty() const{return data_count == 0;}                         //true if the tree is empty

    bool is_valid(){return true;}
                                                //print a readable version of
                                                //                  the tree
    void print_tree(int level = 0, ostream &outs=cout) const{
        // traverse the tree go down to to the right
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

    void print_debug(int level = 0, ostream& outs = cout) const{
        if(!is_leaf() && child_count > 1){
            subset[child_count - 1]->print_debug(level + 1, outs);
        }
        // then print the right most sub trees (right to left)
        for(int i = data_count - 1; i >= 0; i--){
            cout << setw(10 * level) << "[D: " << data_count << ":: " << data[i] << " :: C: " << child_count << "]" << endl << endl;
            if(!is_leaf()){
                subset[i]->print_debug(level + 1, outs);
            }
        }
    }
    friend ostream& operator<<(ostream& outs, const BTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }

    string in_order();                          // traverse the tree in an
                                                // inorder fashion, return a 
                                                // string of all the data items
                                                // with vertical delimiters

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                               //true if duplicate keys may be
                                                //      inserted
    int data_count;                             //number of data elements
    T data[MAXIMUM + 1];                        //holds the keys
    int child_count;                            //number of children
    BTree* subset[MAXIMUM + 2];                 //subtrees

    bool is_leaf() const {return child_count == 0;} //true if this is a leaf node

    //insert element functions
    void loose_insert(const T& entry);          //allows MAXIMUM+1 data
                                                //      elements in the root
    void fix_excess(int i);                     //fix excess of data elements
                                                //                  in child i

    //remove element functions:
    void loose_remove(const T& entry);          //allows MINIMUM-1 data
                                                //      elements in the root
    void fix_shortage(int i);                   //fix shortage of data elements
                                                //                  in child i

    void remove_biggest(T& entry);              //remove the biggest child of
                                                //          this tree->entry
    void rotate_left(int i);                    //transfer one element LEFT
                                                //              from child i
    void rotate_right(int i);                   //transfer one element RIGHT
                                                //              from child i
    void merge_with_next_subset(int i);         //merge subset i with subset
                                                //                      i+1

};



template<class T>   
    void BTree<T>::insert(const T& entry){      //insert entry into the tree
        loose_insert(entry);

        // fix fat parent
        if(data_count > MAXIMUM){
            BTree<T>* LEFT = new BTree<T>();
            
            copy_array(LEFT->data, data, LEFT->data_count, data_count);
            copy_array(LEFT->subset, subset, LEFT->child_count, child_count);

            clear_list(data, data_count);
            clear_list(subset, child_count);

            insert_item(subset, 0 , child_count, LEFT);
 
            fix_excess(0);      
        }
    }                

template<class T>   
    void BTree<T>::remove(const T& entry){      //remove entry from the tree

        loose_remove(entry);

        if(!is_leaf() && data_count < MINIMUM){
            int i = subset[0] ? 0 : 1;

            BTree* temp = new BTree<T>();
            
            copy_array(data, subset[i]->data, data_count , subset[i]->data_count);
            delete_item(subset, i, child_count, temp);
            copy_array(subset, temp->subset, child_count, temp->child_count);

            temp->child_count = 0;
            delete temp;            
        }
    }               
template<class T>   
    void BTree<T>::  clear_tree(){                //    clear this object
                                                //  (delete all nodes etc.)
        for(int i = 0; i < data_count; i++){
            data[i] = 0;
        }
        
        for(int i = 0; i < child_count; i++){
            subset[i]->clear_tree();
            delete subset[i];
        }
        child_count = 0;
        data_count = 0;
    }                          

template<class T>   
    void BTree<T>::copy_tree(const BTree<T>& other){
        copy_array(data, other.data, data_count ,other.data_count);
        child_count = other.child_count;
        dups_ok = other.dups_ok;
        for(int i = 0; i < other.child_count; i++){
            subset[i] = new BTree<T>();
            subset[i]->copy_tree(*other.subset[i]);
        }
    }      //copy other into this object

template<class T>   
    bool BTree<T>::contains(const T& entry){    //        true if entry can be found in
                                                //                  the array
        // get the index
        // check if the index is greater than size

        int index = first_ge(data, data_count, entry) > data_count ? first_ge(data, data_count, entry) - 1 : first_ge(data, data_count, entry) ;
 
        if(data[index] == entry){
            return true;
        }else if(is_leaf()){
            return false;
        }else{
            return subset[index]->contains(entry);
        }
    }                                           
template<class T>   
    T& BTree<T>::get(const T& entry){           //           return a reference to entry
                                                //                  in the tree
        int index = first_ge(data, data_count, entry) > data_count ? first_ge(data, data_count, entry) - 1 : first_ge(data, data_count, entry) ;

        if(data[index] == entry){
            return data[index];
        } else if (!is_leaf()){
            return subset[index]->get(entry);
        }else{
            throw runtime_error("get: entry not found");
        }
        
    }                     
template<class T>   
    T* BTree<T>::find(const T& entry){
        // get the index
        // check if the index is greater than size

        int index = first_ge(data, data_count, entry) > data_count ? first_ge(data, data_count, entry) - 1 : first_ge(data, data_count, entry) ;

 
        if(data[index] == entry){
            return &data[index];
        }else if(is_leaf()){
            return nullptr;
        }else{
            return subset[index]->find(entry);
        }

    }                                           //return a pointer to this key.
                                                //          NULL if not there.


template<class T>   
    string BTree<T>::in_order(){
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
    }                                           // traverse the tree in an
                                                // inorder fashion, return a 
                                                // string of all the data items
                                                // with vertical delimiters
template<class T>
    void BTree<T>::loose_insert(const T& entry){          //allows MAXIMUM+1 data
        int index = first_ge(data, data_count, entry) > data_count ? first_ge(data, data_count, entry) - 1 : first_ge(data, data_count, entry) ;
        
        if(data[index] == entry){
            if(!dups_ok)                                    // continue if we allow dups
                data[index] = entry;                        // over ride
            return;
        }else if(is_leaf()){
            insert_item(data, index, data_count, entry);
            return;
        }else{                              // keep looking
            subset[index]->loose_insert(entry);
        }

        
        if(!is_leaf() && subset[index]->data_count > MAXIMUM){                          // check if each data count exceedes the max
            fix_excess(index);                                                              // fix acces
        }

    }
template<class T>
                                                //      elements in the root
    void BTree<T>::fix_excess(int i){                     //fix excess of data elements
                                                //                  in child i

            BTree<T>* temp = new BTree();
            // get half of the data
            T midItem;                        
            delete_item(subset[i]->data, MAXIMUM/2, subset[i]->data_count, midItem);
                
            split(subset[i]->data, subset[i]->data_count, temp->data, temp->data_count);        // split the data
            split(subset[i]->subset, subset[i]->child_count, temp->subset, temp->child_count);
            
            ordered_insert(data, data_count, midItem);              
            int index = first_ge(data, data_count, temp->data[0]);
            insert_item(subset, index, child_count, temp);
    }

    //remove element functions:
template<class T>
    void BTree<T>::loose_remove(const T& entry){            //allows MINIMUM-1 data
                                                            //elements in the root
        int index = first_ge(data, data_count, entry) > data_count ? first_ge(data, data_count, entry) - 1 : first_ge(data, data_count, entry) ;
        T temp = entry;                                     // dont change the value of entry 
        if(data[index] == entry){
            if(is_leaf()){                                  // if its a leaf
                delete_item(data, index, data_count, temp);  // delete the item
                return;
            }else{
                remove_biggest(temp);                       // remove the biggest child
            }

        }else if (!is_leaf()){                                  // keep looking
            subset[index]->loose_remove(entry);
        }

        if(!is_leaf() && subset[index]->data_count < MINIMUM){                          // check if each data count exceedes the max
            fix_shortage(index);                                                              // fix acces
        }
    }
template<class T>
    void BTree<T>::fix_shortage(int i){                   //fix shortage of data elements
                                                //                  in child i
        // last indext merge left
        // any other index merge right
        int index = i == data_count && !empty() ? i - 1 : i;

        if(i + 1 < child_count && subset[i + 1]->data_count > MINIMUM){           // check if left sibling can share
            rotate_left(i);        
        }else if(i - 1 >= 0 && subset[i - 1]->data_count > MINIMUM){                          //check if right sibling can share
            rotate_right(i);
        }
        else if(data_count == child_count - 1){                 // if data_count equals to child_count                 
            merge_with_next_subset(index);                                                  
        }

    }   

template<class T>
    void BTree<T>::remove_biggest(T& entry){                //remove the biggest child of
                                                            //          this tree->entry
        cout << "in remove biggest" << endl;
        int index = first_ge(data, data_count, entry) > data_count ? first_ge(data, data_count, entry) - 1 : first_ge(data, data_count, entry) ;
        T key = entry;        
        if(is_leaf()){
            delete_item(data, data_count - 1, data_count, entry);
            cout << "item we are deleting: " << entry << endl;
            return;
        }else{
            subset[index]->remove_biggest(entry);
            if(data[index] == key){
                delete_item(data, index, data_count, key);  // delete the item
                insert_item(data, index, data_count, entry);  // insert the biggest child

            }
        }


        if(!is_leaf() && data_count != 0 && subset[index]->data_count < MINIMUM){             // check if each data count exceedes the max
            fix_shortage(index);                                                              // fix access
        }

    }
template<class T>
    void BTree<T>::rotate_left(int i){                                                                         //transfer one element LEFT     
        cout << "in rotate left" << endl;
        T demotion, promotion;
        BTree<T>* promotionBTree = new BTree<T>();
        delete_item(data, i, data_count, demotion);                                                            // detach the last element of the parent
        delete_item(subset[i + 1]->data, 0, subset[i + 1]->data_count, promotion);                             // detach the first element of the right child

        ordered_insert(data, data_count, promotion);                                                           // inserting items to their respective locations
        ordered_insert(subset[i]->data, subset[i]->data_count, demotion);                                       

        if(!subset[i + 1]->is_leaf()){                                                                          // if the right child is not a leaf
            delete_item(subset[i + 1]->subset, 0, subset[i + 1]->child_count, promotionBTree);                      // detach the first child of the right child
            insert_item(subset[i]->subset, subset[i]->child_count, subset[i]->child_count, promotionBTree);         // insert the child into the left child
        }

    //inserting at the parent

    }
template<class T>
    void BTree<T>::rotate_right(int i){         //      transfer one element RIGHT
    cout << "in rotate right" << endl;
        T demotion, promotion;
        BTree<T>* promotionBTree = new BTree<T>();
        delete_item(data, i - 1, data_count, demotion);                                                            // detach the last element of the parent
        delete_item(subset[i - 1]->data, subset[i - 1]->data_count - 1, subset[i - 1]->data_count, promotion);                             // detach the first element of the right child

        ordered_insert(data, data_count, promotion);                                                           // inserting items to their respective locations
        ordered_insert(subset[i]->data, subset[i]->data_count, demotion);

        if(!subset[i - 1]->is_leaf()){                                                                          // if the right child is not a leaf
            delete_item(subset[i - 1]->subset, subset[i - 1]->child_count - 1, subset[i - 1]->child_count, promotionBTree);                      // detach the first child of the right child
            insert_item(subset[i]->subset, 0, subset[i]->child_count, promotionBTree);         // insert the child into the left child
        }
    }
template<class T>
                                                //          merge subset i with subset
                                                //                      i+1
    void BTree<T>::merge_with_next_subset(int i){      
        cout << "in merge" << endl;
        T demotion;
        BTree<T>* deleteTree;
        delete_item(data, i, data_count, demotion);                                                            // detach the last element of the parent
        //inserting at the parent
        insert_item(subset[i]->data, subset[i]->data_count, subset[i]->data_count, demotion);                                               // insert the parent into the right child
        
        merge(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);
        merge(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset, subset[i+1]->child_count);

        delete_item(subset, i + 1, child_count, deleteTree);
        delete deleteTree;
    }

#endif //BTREE_H