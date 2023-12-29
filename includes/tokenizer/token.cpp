#include "token.h"
#include "../constants/constants.h"

SToken::SToken(){
    _token = "";
    _type = -2;
}
SToken::SToken(const string& str, const int& type){
    _token = str;
    _type = type;
}

int SToken::type() const{
    return _type;
}

string SToken::type_string() const{
    switch (_type)
    {
    case TOKEN_NUMBER:
        return  "NUMBER";
        break;
    case TOKEN_ALPHA:
        return "ALPHA";
        break;
    case TOKEN_SPACE:
        return "SPACE";
        break;
    case TOKEN_OPERATOR:
        return "OPERATOR"; 
        break;
    case TOKEN_PUNC:
        return "PUNC";
        break;
    case TOKEN_UNKNOWN:
        return "UNKNOWN";
        break;
    default:
        return "what ";
        break;
    }
}
string SToken::token_str() const{
    return _token;
}

CommandToken::CommandToken(){
    _token = "";
}
CommandToken::CommandToken(const string& str): SToken(str, COMMAND){
    
    if (str == "make" || str == "create") {
        _type = CREATE;
    }
    else if(str == "insert"){
        _type = INSERT;
    }
    else if(str == "select"){
        _type = SELECT;
    }else if(str == "drop"){
        _type = DELETE;
    }
}
string CommandToken::token_str() const{
    return _token;
}
int CommandToken::get_type() const{
    return _type;
}

SYMToken::SYMToken(){
    _token = "";
}
SYMToken::SYMToken(const string& str): SToken(str, SYM){
    if(str == "and" || str == "or"){
        _type = LOGIC;
    }else if(str == ">=" || str == "<=" || str == "!=" || str == "=" || str == ">" || str == "<"){
        _type = REL;
    }else if(str == ","){
        _type = COMMA;
    }else if(str == "*"){
        _type = ALL;
    }else if(str == "("){
        _type = LPAREN;
    }else if(str == ")"){
        _type = RPAREN;
    }
    else{
        _type = SYM;
    }
    _token = str;

}
string SYMToken::token_str() const{
    return _token;
}

int SYMToken::get_type() const{
    return _type;
}


ConditionToken::ConditionToken(){
    _token = "";
}
ConditionToken::ConditionToken(const string& str): SToken(str, CONDITION){
    _token = str;

    if(str == "into"){
        _type = INTO;
    }else if(str == "values"){
        _type = VALUE;
    }if(str == "from"){
        _type = FROM;
    }else if(str == "table"){
        _type = TABLE;
    }else if(str == "where"){
        _type = WHERE;
    }else if(str == "fields"){
        _type = FIELD;
    }
}

int ConditionToken::get_type() const{
    return _type;
}