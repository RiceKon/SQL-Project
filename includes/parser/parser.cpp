#include "parser.h"



Parser::Parser(char* input){
    copy(input, input + strlen(input), _input);
    make_table();
    stk.set_string(input);
    // print_table(SM);
    tokenize();

}

MMap<string, string> Parser::parse_tree(){
    /*
    this function check if the input is valid for an SQL command
    */
    _ptree.clear();
    SToken* token;
    int i = 0, state = 0, previous_state, where_to_go = 0;
    bool valid = true;

    while(valid && _tokens.size() > i){
        token = _tokens[i];
            // cout << "token: " << token->token_str() << endl;
            // cout << "token-type: " << token->type() << endl;
            // cout << "token->get_type():" << token->get_type() << endl;
            // cout << "state: " << state << endl;
            // cout << "is valid:" << boolalpha << (SM[state][token->get_type()] != -1) << endl << endl << endl;
        
        // if it is a valid edge, get the state of the token such as if it is a command, symbol or a condition
        if(SM[state][token->get_type()] != -1){
            where_to_go = static_cast<SToken* >(token)->type();
        }else{
            cout << "bad arguemnt, exiting";
            _ptree.clear();
            break;
        }
        // update the edge
        state = SM[state][token->get_type()];
    
        switch (where_to_go)
        {
        case COMMAND:
            // push in command token and if it is select previous state to select to get the fieldnames
            previous_state = token->get_type() == SELECT ? SELECT : -1;
            _ptree["command"].push_back(token->token_str());
            break;
        case SYM:
            if(token->get_type() != COMMA)
                switch (previous_state)
                {
                    case VALUE:
                        _ptree["values"].push_back(token->token_str());
                        break;
                    case FIELD:
                        _ptree["col"].push_back(token->token_str());
                        break;
                    case FROM:   
                    case TABLE:
                    case INTO:
                        _ptree["table_name"].push_back(token->token_str());
                        break;        
                    case WHERE:
                        _ptree["conditions"].push_back(token->token_str());
                        break;
                    case SELECT:
                        _ptree["fields"].push_back(token->token_str());
                        break;
                    default:
                        break;
                }
                break;
        case CONDITION:
            if(token->get_type() == WHERE){
                _ptree["where"].push_back("yes");
            }
            // this will get types, FROM, INTO, VALUES, TABLE ect
            previous_state = token->get_type();
            break;
        default:
            valid = false;
            break;
        }
        i++;
    }
    // 1 final check if it is a valid or not
    if(!is_success(SM, state)){
        cout << "bad arugment";
        _ptree.clear();
    }

    return _ptree;
}
void Parser::set_string(char* input){
    copy(input, input + strlen(input), _input);
    _input[strlen(input)] = '\0'; 
    stk.set_string(_input);
    tokenize();         // get the tokens from the string
}


vector<SToken *> Parser::tokenize(){
    /*
    docstring:
    this function tokenizes the input into strings, and then tokenizes them into SQL commands
    */
    _tokens.clear();
    
    SToken temp;
    stk >> temp;
    
    string str = "";
    while(stk.more()){
        if(temp.type_string() != "SPACE"){
            str = temp.token_str();
            
            if(str == "\""){
                str = "";
                stk >> temp;
                while(temp.token_str() != "\""){
                    str += temp.token_str();
                    stk >> temp;
                    
                }
            }

            if(str == "make"   || str == "create" || str == "drop" ||
               str == "insert" || str == "select"
              ){
                _tokens.push_back(new CommandToken(str));
            }
            else if(str == "table" || str == "into"   || str == "from"   || 
                    str == "where" || str == "values" || str == "fields"
            ){
                _tokens.push_back(new ConditionToken(str));
            }else{
                _tokens.push_back(new SYMToken(str));
            }
        }
        str = "";
        stk >> temp;
    }
    // cout << _tokens << endl;
    return _tokens;
}

void Parser::make_table(){
    init_table(SM);
    //create table 
    // 1 - 5

    mark_success(SM, 5);    

    mark_cell(0, SM, CREATE, 1);
    mark_cell(1, SM, TABLE, 2);
    mark_cell(2, SM, SYM, 3);
    mark_cell(3, SM, FIELD, 4);
    mark_cell(4, SM, SYM, 5);
    mark_cell(5, SM, COMMA, 6);
    mark_cell(6, SM, SYM, 5);

    // select
    // 10 - 23

    mark_success(SM, 14);
    mark_success(SM, 18);

    mark_cell(0, SM, SELECT, 10);

    mark_cell(10, SM, SYM, 11);        mark_cell(10, SM, ALL, 11);
    mark_cell(11, SM, COMMA, 12);      mark_cell(11, SM, FROM, 13);
    mark_cell(12, SM, SYM, 11);
    mark_cell(11, SM, FROM, 13);

    mark_cell(13, SM, SYM, 14);

    mark_cell(14, SM, WHERE, 15);

    mark_cell(15, SM, LPAREN, 15);
    mark_cell(15, SM, SYM, 16);
    mark_cell(16, SM, REL, 17);
    mark_cell(17, SM, SYM, 18);

    mark_cell(18, SM, RPAREN, 18);
    mark_cell(18, SM, LOGIC, 15);
    // insert 30 - 34
    
    mark_success(SM, 34);

    mark_cell(0, SM, INSERT, 30);
    mark_cell(30, SM, INTO, 31);
    mark_cell(31, SM, SYM, 32);
    mark_cell(32, SM, VALUE, 33); // value
    mark_cell(33, SM, SYM, 34);
    mark_cell(34, SM, COMMA, 35);
    mark_cell(35, SM, SYM, 34);

    // delete 40 - 42
    mark_success(SM, 42);

    mark_cell(0, SM, DELETE, 40);
    mark_cell(40, SM, TABLE, 41);
    mark_cell(41, SM, SYM, 42);
}
