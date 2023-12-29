#ifndef BTREE_ARRAY_FUNCTIONS_H
#define BTREE_ARRAY_FUNCTIONS_H
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

template <class T>
void print_array(const T data[], int n, int pos = -1){
    cout << "Items: ";
    for (int i = 0; i < n; i++){
        cout << "[" << data[i] << "]";
    }
    cout << endl;
}  //print array data
template <class T>
T maximal(const T& a, const T& b){
    return max(a, b);
}                      //return the larger of the two items

template <class T>
void swap(T& a, T& b){
    T temp = a;
    a = b;
    b = temp;
}  //swap the two items

template <class T>
int index_of_maximal(T data[ ], int n){
    int max_index = 0;
    for (int i = 0; i < n; i++){
        if (data[i] > data[max_index]){
            max_index = i;
        }
    }
    return max_index;
}                 //return index of the largest item in data

template <class T>
void insert_item(T data[ ], int i, int& n, T entry){
// shift right
    if(i > n){
        cout << "\tout of index. DID NOT INSERT" << endl;
        return;
    }

    for(int j = n; j > i; j--){
        data[j] = data[j - 1];
        data[j - 1] = T();
    }
    data[i] = entry;
    n++;

}    //insert entry at index i in data


template <class T>
void ordered_insert(T data[ ], int& n, T entry){
    for(int i = 0; i < n; i++){
        if(data[i] > entry){
            insert_item(data, i, n, entry);
            return;
        }
    }
    insert_item(data, n, n, entry);

}        //insert entry into the sorted array
                                                        //data with length n

template <class T>
int first_ge(const T data[ ], int n, const T& entry){
    int index = 0;
    for(int i = 0; i < n; i++, index++){
        if(!(data[i] < entry)){
            return i;
        }
    }
    return n;
}   //return the first element in data that is
                                                        //not less than entry

template <class T>
void attach_item(T data[ ], int& n, const T& entry){
    data[n] = entry;
    n++;
}    //append entry to the right of data


template <class T>
void detach_item(T data[ ], int& n, T& entry){
    entry = data[n];
    data[n] = T();
    n--;
}          //remove the last element in data and place
                                                        //it in entry

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry){
    entry = data[i];
    if(i < n){
        n--;
        for(int j = i; j < n; j++){
            data[j] = data[j + 1];
        }
    }
}   //delete item at index i and place it in entry

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2){
    // n1 is the size dont neeed to do n1 - 1
    int first_indexes = n1;
    for(int second_indexes = 0; second_indexes < n2; second_indexes++, first_indexes++){
        data1[first_indexes] = data2[second_indexes];
    }
    n1 += n2;
    n2 = 0;
}   //append data2 to the right of data1

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2){
    int j = 0;

    for(int i = ceil(n1 / 2.0); i < n1; i++, j++){
        data2[j] = data1[i];
        data1[i] = T();
    }
    // update the size
    n2 = j;
    n1 -= n1 / 2;


}   //move n/2 elements from the right of data1
                                                        //and move to data2
template <class T>
void clear_list(T data[ ], int& n){
    for(int i = 0; i < n; i++){
        data[i] = T();
    }
    n = 0;
}          //reset the list to an empty state
template <class T>
void copy_array(T dest[], const T src[], int& dest_size, int src_size){
    for(int i = 0; i < src_size; i++){
        dest[i] = src[i];
    }
    dest_size = src_size;
}              //copy src[] into dest[]

template <class T>
bool is_gt(const T data[], int n, const T& item){
    for(int i = 0; i < n; i++){
        if(data[i] > item){
            return false;
        }
    }
    return true;
}       //item > all data[i]

template <class T>
bool is_le(const T data[], int n, const T& item){
    for(int i = 0; i < n; i++){
        if(data[i] <= item){
            return false;
        }
    }
    return true;
}       //item <= all data[i]

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list){
    cout << "[";
    for(int i = 0; i < list.size(); i++){
        cout << list.at(i) << " ";
    }
    cout << "]" << endl;
    return outs;
} //print vector list

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme){
    list.push_back(addme);
    return list;
} //list.push_back addme




#endif //BTREE_ARRAY_FUNCTIONS_H
 
