#include "state_machine_functions.h"

//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS]){
    for(int i = 0; i <= 40; i++){
        for(int j = 0; j <= MAX_COLUMNS; j++){
            _table[i][j] = -1;
        }
    }
}

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state){
    _table[state][0] = 1;
}

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state){
    _table[state][0] = 0;
}

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state){
    return _table[state][0] == 1;
}

//Mark a range of cells in the array. 
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state){
    for (int i = from; i < to; i++){
        // the row stays static, go down left changing all of the value of the state to the state intlaized
        _table[row][i] = state;
    }
}

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state){
    for(int i = 0; i < strlen(columns); i++){
        // get the asscii representation for the column to change
        int change = columns[i];
        _table[row][change] = state;
    }
}

//Mark this row and column
void mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state){
        _table[row][column] = state;
}

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS]){


    for(int i = 0; i <= 40;i ++){
        
        cout << "row[" << i << "]:  ";
        cout << _table[i][0] << setw(10);
        for(int j = 0; j <=  40; j++){
            cout <<_table[i][j] << setw(3);
        }
        cout << endl;
    }
}

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(char s[], int _pos);


