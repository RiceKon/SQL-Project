#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <vector>
#include "../bplustree/multimap.h"
#include <iostream>
using namespace std;

typedef vector<string> vectorstr;
typedef MMap<string, long>::Iterator MMapIt;
typedef MMap<string, string> mmap_ss;
#endif // TYPEDEF_H