#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include "bplustree.h"
using namespace std;

template <typename K, typename V>
struct Pair{
    K key;
    V value;

    Pair(const K& k=K(), const V& v=V()):key(k), value(v){}

    friend ostream& operator <<(ostream& outs, const Pair<K, V>& print_me){
        outs << print_me.key << ":" << print_me.value;
        return outs;
    }
    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key == rhs.key;
    }
    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key < rhs.key;
    }
    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key > rhs.key;
    }
    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key <= rhs.key;
    }
    friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return rhs;
    }
};

template <typename K, typename V>
class Map
{
public:
    typedef BPlusTree<Pair <K, V> > map_base;
    class Iterator{
    public:
        friend class Map;
        Iterator(typename map_base::Iterator it = NULL){
            _it = it;
        }
        Iterator operator ++(int unused){
            return _it++;
        }
        Iterator operator ++(){
            return ++_it;
        }
        Pair<K, V> operator *(){
            return *_it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return lhs._it == rhs._it;    
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return lhs._it != rhs._it;
        }
    private:
        typename map_base::Iterator _it;

    };
    Map();
//  Iterators
    Iterator begin();
    Iterator end();

//  Capacity
    int size() const;
    bool empty() const;

//  Element Access
    V& operator[](const K& key);
    V& at(const K& key);
    const V& at(const K& key) const;


//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();
    V get(const K& key);

//  Operations:
    Iterator find(const K& key);
    bool contains(const Pair<K, V>& target) const;

    Iterator count (const K& key);
    Iterator lower_bound(const K& key);
    Iterator upper_bound(const K& key);
    Iterator equal_range(const K& key);

    bool is_valid(){return map.is_valid();}

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
        outs<< print_me.map << endl;
        return outs;
    }
private:
    int key_count;
    BPlusTree<Pair<K, V> > map;
};

template <typename K, typename V>
Map<K,V>::Map(){
    key_count = 0;
}

//  Iterators
template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::begin(){
    return Map<K, V>::Iterator(map.begin());
}

template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::end(){
    return Iterator(nullptr);
}

//  Capacity
template <typename K, typename V>
int Map<K, V>::size() const{
    return key_count;
}

template <typename K, typename V>
bool Map<K, V>::empty() const{
    return key_count == 0;
}

//  Element Access
template <typename K, typename V>
V& Map<K, V>::operator[](const K& key){
    return map.get(Pair<K,V>(key, V())).value;
}

template <typename K, typename V>
V& Map<K, V>::at(const K& key){
    return map.get(Pair<K,V>(key, V())).value;

}

template <typename K, typename V>
const V& Map<K,V>::at(const K& key) const{
    return map.get(Pair<K,V>(key, V())).value;
}


//  Modifiers

template <typename K, typename V>
void Map<K,V>::insert(const K& k, const V& v){
    map.insert(Pair<K,V> (k,v));
    key_count++;
}

template <typename K, typename V>
void Map<K, V>::erase(const K& key){
    return;
}

template <typename K, typename V>
void Map<K, V>::clear(){
    map.clear_tree();
}

template <typename K, typename V>
V Map<K, V>::get(const K& key){
    return map.get(key).value;
}

//  Operations:
template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::find(const K& key){
    return map.find(key);
}
template <typename K, typename V>
bool Map<K, V>::contains(const Pair<K, V>& target) const{
    return map.contains(target);
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K, V>::count (const K& key){
    return Map<K, V>::Iterator();
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K, V>::lower_bound(const K& key){
    return map.lower_bound(key);
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K, V>::upper_bound(const K& key){
    return map.upper_bound(key);
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K, V>::equal_range(const K& key){
    return Map<K, V>::Iterator();
}


#endif // MAP_H