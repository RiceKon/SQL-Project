#include "stokenize.h"

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

STokenizer::STokenizer(){
    make_table(_table);
    _pos = 0;
    _done = false;
    TypeArray();

}

STokenizer::STokenizer(char str[]){
    make_table(_table);
    _pos = 0;
    _done = false;
    strcpy(_buffer, str);
    TypeArray();
}

bool STokenizer::done(){        //true: there are no more tokens
    return _done;
}            

bool STokenizer::more(){        //true: there are more tokens
    return !_done;
}            
//

//---------------
//extract one token (very similar to the way cin >> works)


//set a new string as the input string
void STokenizer::set_string(char str[]){
    strcpy(_buffer, str);
}

void STokenizer::TypeArray(){
 // alpha
    _success_table[1] = TOKEN_ALPHA;
    _success_table[3] = TOKEN_ALPHA;
// number
    _success_table[5] = TOKEN_NUMBER;
    _success_table[7] = TOKEN_NUMBER;
//space
    _success_table[10] = TOKEN_SPACE;

    _success_table[12] = TOKEN_PUNC;
}


//create table for all the tokens we will recognize
//                      (e.g. doubles, words, etc.)
void STokenizer::make_table(int _table[][MAX_COLUMNS]){

    init_table(_table);
    // alpha

    mark_success(_table, 1); // words
    mark_fail(_table, 2);   // words with (') they're
    mark_success(_table,3); // continue with re

    mark_cells(0, _table, ALFA, 1);
    mark_cells(1, _table, ALFA, 1);
    mark_cell(1,_table, '\'', 2);
    mark_cells(2,_table, ALFA, 3);
    mark_cells(3,_table, ALFA, 3);
 // 0 - 3

// digit
    mark_success(_table, 5);
    mark_fail(_table, 6);
    mark_success(_table, 7);
    
    mark_cells(0, _table, DIGITS, 5);
    mark_cells(5, _table, DIGITS, 5);
    mark_cell(5, _table, '.', 6);
    mark_cells(6, _table, DIGITS, 7);
    mark_cells(7, _table, DIGITS, 7);
// 5 - 7

//space

    mark_success(_table, 10);

    mark_cells(0, _table, SPACES, 10);
    mark_cells(10, _table, SPACES, 10);

// 10 - 10

// punct

    mark_success(_table, 12);

    mark_cells(0, _table, PUNC, 12);
    mark_cells(12, _table, PUNC, 12);




    // init_table(_table);
    // mark_fail(_table, 0);
    // mark_success(_table, 1);
    // mark_success(_table, 3);
    // mark_success(_table, 4);


    // mark_fail(_table, 2);

    // mark_cell(0,_table,'a', 1); // 'a'
    // mark_cell(0,_table,'c', 4); // 'a'


    // mark_cell(1,_table,'b', 2); // 'b'
    // mark_cell(1,_table,'a', 1); // 'b'

    // mark_cell(2,_table,'b', 2); // 'a'
    // mark_cell(2, _table, 'a', 3);
    
    // mark_cell(3, _table,'a', 3);



// use state machine functions
}

//extract the longest string that match
//     one of the acceptable token types

//cases, UNKNOWN, END, 

bool STokenizer::get_token(int& start_state, string& token){
    int success, state = 0, potential_size = 0, size = 0;

    string temp(_buffer);
    for(int i = _pos; i < temp.length() + 1; i++){
        // get the asccii value
        int check = static_cast<int>(_buffer[i]);
        //check the state (136 to 145)
            // get the state
            state = check > 0 ? _table[state][check] : -1;  

            
            // check if its a valid string or pos the string is at the end
            if(state == -1){
                // add the string
                if(size == 0){
                    start_state = state;
                    size++;
                }
                token = temp.substr(_pos, size);
                // update the postition
                _pos += size;
                return true;
                // if the ascii vaule unknown the state is -1
            }
            // if not a success state update the potential size
            if(_table[state][0] == 1 ){
                start_state = state;
                potential_size++;
                size = potential_size;
            // if success state update my state and potential size
            }else if (_table[state][0] == 0){
                potential_size++;
            }
    }
    return false;
}



// bool STokenizer::get_token(int& start_state, string& token){
// // string is "abaca"
// // output is aba, c, a
//     int start = _pos, state = 0, length = 0, length_success = 0;
//     int success;
//     string temp(_buffer);
//     if(start == temp.length()){
//         _pos++;
//     }
//     for(int i = start; i < temp.length(); i++){
//         // get the ascii value
//         int check = static_cast<int>(_buffer[i]);
//         // check the state from the state machine
//         if(check < 0 && start_state != -1){
//             if(length_success > 0){
//                 for(int i = start; i < (start + length_success); i++){
//                     token = token + temp.at(i);
//                 }
//                 return true;

//             }
//             start_state = TOKEN_UNKNOWN;
//             // move to the next token
//             _pos++;
//             // grab the token
//             token = token + temp.at(start);
//             //
//             return true;
//         }
//         // if im not out of bounds
//         state = _table[state][check];
//         if(state != -1){
//             // update success
//             length++;
//             success = _table[state][0];
//         }else if(state == -1 && start == _pos){
//             // if the first charater is unknown
//             length++;
//             length_success = length;
//         }else{
//             // if im out of bounds make it negative one
//             success = -1;
//         }
//         // add lenght + 1 everytime so i can updare success lenght
      
//         switch (success)
//         {
//         case 1:
//         // if success state update to the current location with the current charater being checked
//             length_success = length;
//             _pos = start + length_success;
//             start_state = state;
//             // update success location
//             break;
//         case 0:
//                 // if not success state continue until we see a success state
//             break;
//         case -1:
//             _pos = start + length_success;

//         // if i see someting ugly update postion and go next
//             break;
//         default:
//             break;
//         }

//         if(state <= -1 || _pos == temp.length() || start + length == temp.length()){
//             if(length_success == 1){
//                 token = token + temp.at(start);
//             }else{
//                 for(int i = start; i < (start + length_success); i++){
//                     token = token + temp.at(i);
//                 }
//             }
//             return true;
//             }
//     }
//     return true;
// }

