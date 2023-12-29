#include "../table.h"
#include <algorithm>

// token
Token::Token(const string& token, TokenTypes type){
    _token = token;
    token_type = type;
}   // constructor;

string Token::token() {
    return _token;
}
// end of token

// token string
TokenStr::TokenStr(const string& token): Token(token, TOKEN_STR), tokenstr(token){} 

void TokenStr::Print(ostream& outs){
    outs << "In TokenStr, Printing out the string" << tokenstr << endl;
}

// end of token string

// relational
Relational::Relational(const string& token): Token(token, RELATIONAL), relational(token){};

Token* Relational::eval(Token* left, Token* right, const Table& t){
    /*
    docstring
    this function evaluates the relational operator and returns a result set
    */
    Table temp(t.get_table_name());
    // restrict the fields to the ones in the table
    vectorstr columns = t.get_field_names();
    temp.select_records(columns, left->token(), relational, right->token());
        // cout << temp.select_recnos() << endl;
    return new ResultSet(temp.select_recnos());
}

void Relational::Print(ostream& outs){
    outs << "In relational, Printing out the string" << relational << endl;
}


// end of relational

// logical

Logical::Logical(const string& token): Token(token, LOGICAL), logical(token){}   // constructor

Token* Logical::eval(Token* left, Token* right){    
    /*
    docstring
    this function evaluates the logical operator and returns a result set
    */
    Token* _return;
    
    if(logical == "and"){
        _return =  static_cast<ResultSet *>(left)->intersection(static_cast<ResultSet *>(right));
    }
    else if(logical == "or"){
        _return =  static_cast<ResultSet *>(left)->union_set(static_cast<ResultSet *>(right));
    }
    else{
        cout << "invalid operator" << endl;
        return nullptr;
    }
    return _return;
}

void Logical::Print(ostream& outs){
    outs << "In Logical, Printing out the string: " << logical << endl;
}


int Logical::get_pemdas(){
    if(logical == "and"){
        return 2;
    }
    else if(logical == "or"){
        return 1;
    }
    else{
        cout << "invalid operator" << endl;
    }
    return -1;
}
// end of logical



// result set
ResultSet::ResultSet(const vector<long> result): Token("", RESULT_SET), _result(result){}


void ResultSet::Print(ostream& outs){
    outs << "In ResultSet, Printing out the vector: " << endl;
    for(int i = 0; i < _result.size(); i++){
        outs << _result[i];
    }
}


Token* ResultSet::intersection(ResultSet* other){
    vector<long> temp = other->get_result();
    vector<long> result;
    sort(_result.begin(), _result.end());
    sort(temp.begin(), temp.end());
    set_intersection(_result.begin(), _result.end(), temp.begin(), temp.end(), back_inserter(result));
    return new ResultSet(result);

}

Token* ResultSet::union_set(ResultSet* other){
    vector<long> temp = other->get_result();
    vector<long> result;
    sort(_result.begin(), _result.end());
    sort(temp.begin(), temp.end());
    set_union(_result.begin(), _result.end(), temp.begin(), temp.end(), back_inserter(result));
    return new ResultSet(result);
}

void ResultSet::set_result(vector<long> result){
    _result = result;
}

vector<long> ResultSet::get_result(){
    return _result;
}


// Lparen

void LParen::Print(ostream& outs){
    outs << "In LParen, Printing out the string" << lparen << endl;
}


// enf of Lparen
// Rparen


void RParen::Print(ostream& outs){
    outs << "In RParen, Printing out the string" << rparen << endl;
}


// end of Rparen