#ifndef PARSER_H
#define PARSER_H

#include "../table/table.h"
#include "../tokenizer/stokenize.h"
#include "../tokenizer/token.h"

#include "../tokenizer/constants.h"
#include "../tokenizer/state_machine_functions.h"
#include "../constants/constants.h"

#include <iostream>
#include <cstring>
#include <string>
using namespace std;

class Parser{
public:
    Parser():_input(""){make_table();}

    Parser(char* input);

    MMap<string, string> parse_tree();
    void set_string(char* input);

    void print_tokens(){
        for(int i = 0; i < _tokens.size(); i++){
            cout << *_tokens[i] << endl;
        }
    }
private:
    void make_table();
    vector<SToken *> tokenize();
    static const int MAX = 300;
    char _input[MAX];
    int SM[MAX_ROWS][MAX_COLUMNS];

    MMap<string, string> _ptree;

    Table _table;
    STokenizer stk;
    vector<SToken *> _tokens;
};



#endif  // PARSER_H