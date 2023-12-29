#ifndef STOKENIZE_H
#define STOKENIZE_H
#include <iostream>
#include "token.h"
#include <cctype>
#include "state_machine_functions.h"
#include <cassert>
#include <string>
using namespace std;

class STokenizer
{
public:
    STokenizer();
    STokenizer(char str[]);
    bool done();            //true: there are no more tokens
    bool more();            //true: there are more tokens
    //
    void TypeArray();
    //---------------
    //extract one token (very similar to the way cin >> works)
    friend STokenizer& operator >> (STokenizer& s, SToken& t){
        s._done = false;
        string temp;
        int state = 0; 
        
        // mark doen if end of the string
        if(s._pos >= strlen(s._buffer)){
            s._done = true;
            s._pos = 0;
            return s;
        }
        s.get_token(state, temp);

        // moves the _pos
        // assign the state
        if(state != -1){
            state = s._success_table[state];
        }
        // make token
        t = SToken(temp, state);
        return s;
    }

    //set a new string as the input string
    void set_string(char str[]);
private:
    bool _done;
    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][MAX_COLUMNS]);

    //extract the longest string that match
    //     one of the acceptable token types
    bool get_token(int& start_state, string& token);
    //---------------------------------
    char _buffer[MAX_BUFFER + 1];       //input string
    int _pos;                       //current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];
    int _success_table[20];
};


#endif