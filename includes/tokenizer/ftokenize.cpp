#include "ftokenize.h"


    FTokenizer::FTokenizer(char* fname){
        _f.open(fname);
        if(_f.fail()){
            exit(1);
        }
        _pos = 0;
        _more = get_new_block();
    }
    SToken FTokenizer::next_token(){
        SToken t;
        // get tokens, will return done if at the end of the string
        _stk >> t;
        if(_stk.done()){
            cout << "New Block ------------------------------" << endl;
            // read more tokens and put them in buffer
            _more = get_new_block();
            // read the new buffer
            _stk >> t;
        }
        _blockPos =+ t.token_str().length();
        return t;
    }
    bool FTokenizer::more(){         //returns the current value of _more
        return _more;
    }       
    int FTokenizer::pos(){            //returns the value of _pos
        return _pos;
    }          //returns the value of _pos
    int FTokenizer::block_pos(){  //returns the value of _blockPos
        return _blockPos;
    }

    bool FTokenizer::get_new_block(){ //gets the new block from the file
        bool more = true;
        _blockPos = 0;
        // check if end of file
        if(_f.eof()){
            more = false;
            return more;
        }
        // to assign to stk when use f.read
        char buffer[MAX_BUFFER + 1];
        // create a block stk to be assinged in
        _f.read(buffer, MAX_BUFFER);
        int size = _f.gcount();
        _pos += size;
        // make null charater for the c string
        buffer[size] = '\0';
        // set the string
        _stk.set_string(buffer);
        return more;

    }
    // update the more function
