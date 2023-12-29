#ifndef FILE_RECORD_H
#define FILE_RECORD_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string.h>
#include "utilities.h"
#include "../table/typedefs.h"

using namespace std;

class FileRecord{
public:
    //when you construct a FileRecord, it's either empty or it
    //  contains a word
    FileRecord(){
        _record[0][0] = '\0';
        size = -1;
        //['\0'|  | G | a | r | b | a   |  g  |  e |    ]
    }

    FileRecord(vectorstr v): size(-1){
        for(int i = 0; i < v.size(); i++){
            strncpy(_record[i], v.at(i).c_str(), MAX);
            _record[i][v.at(i).size()] = '\0';
            size++;
        }
        _record[size + 1][0] = '\0';
    }
    int size_of() const{
        return size;
    }

    long write(fstream& outs);              //returns the record number
    long read(fstream& ins, long recno);    //returns the number of bytes
                                            //      read = MAX, or zero if
                                            //      read passed the end of file
    vectorstr get_record() const;


    friend ostream& operator<<(ostream& outs, const FileRecord& r);
    

private:
    static const int MAX = 100;
    char _record[MAX + 1][MAX + 1]; //NULL char
    int size;
};

#endif