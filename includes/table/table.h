#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <vector>


//functions for table
#include "typedefs.h"
#include "../binary_files/file_record.h"
#include "../bplustree/multimap.h"
#include "../bplustree/map.h"

// functions for rpn sy
#include "../stack/MyStack.h"
#include "../queue/MyQueue.h"
// #include "../shunting_yard/shunting_yard.h"

#include "../constants/constants.h"

class Token;

using namespace std;
class Table{
    public: 
    // ctors
    Table():trecno(0), _table_name(""){}
    Table(string tname);


    Table(string fname, vectorstr ftype);

    void Iterate(MMapIt start, MMapIt end, Table& t, int fn, vectorstr columns);

    // member functions
    
    long insert_into(vectorstr values);
    Table select_all();
    Table select(const vectorstr& columns, const string& table_name, const string& op, string const& table_item);
    Table select(const vectorstr& columns, const Queue<Token *>& tokens);
    Table select(const vectorstr& columns, const vectorstr& tokens);

    void select_records(const vectorstr& columns, const string& table_name, const string& op, string const& table_item);
    vector<long> select_recnos();                                       // returns a vector of record numbers   
    vector<string> get_field_names() const;                             // returns a vector of field names
    string get_table_name()const {return _table_name;}                  // returns the table name
    int size() const {return trecno;}

    friend ostream& operator << (ostream& outs, const Table& t){
        fstream fieldnames, table;
        FileRecord fr;
        int i = 0;
        string holder;

        open_fileRW(table, (t._table_name + ".bin").c_str());
        fieldnames.open(t._table_name + ".txt", ios::in);
        if(fieldnames.fail()){
            cout << "ERROR: could not open file" << endl;
        }

        outs << "Table name: " << t._table_name << " record number: " << t.trecno << endl;        
        cout << "record ";
        while(getline(fieldnames, holder)){
            outs << setw(20) << holder << "";
        }
        cout << endl;


        long bytes = fr.read(table, i);
        while (bytes > 0){
            // cout << "debug: checking how many elements i am printing of the buffer" << t.buffer.size_of() << endl;
            outs << setw(5) << i << ": " << fr << endl;
            i++;
            bytes = fr.read(table, i);
        }        
        fieldnames.close();
        table.close();

        return outs;
    }
    
    private:

    void getbounds(MMapIt& start, MMapIt& end, const string& op, const string& table_item, const int& fn);

    void open_files(string fname){
        _field_file.open(fname + ".txt", ios::in);  
        open_fileRW(_file, (fname + ".bin").c_str());
    }

    void close_files(){
        _field_file.close();
        _file.close();
    }

    const static int MAX = 100;
    static int serial;
    // the table
    Map<string, int> _field_names;
    MMap<string, long> table[MAX];
    
    // names
    string _table_name;
    int trecno;
    // return when search
    vector<long> recnos;

    fstream _field_file;
    fstream _file;    
};

#endif // TABLE_H   

#ifndef TOKEN_H
#define TOKEN_H

class Token {
public:
    Token(const string& token, TokenTypes type);   // constructor;

    virtual void Print(ostream& outs=cout){}
    TokenTypes get_type(){return token_type;}
    string token();

    friend ostream& operator << (ostream& outs, const Token* t){
        outs << "[" << t->_token << "]";
        return outs;
    }

    friend ostream& operator << (ostream& outs, const Token t){
        outs << "Token: " << t._token << " TokenType:" << t.token_type << endl;
        return outs;
    }

    
private:
    TokenTypes token_type;
    string _token;
};

#endif // Token_H


#ifndef RESULTSET_H
#define RESULTSET_H

class ResultSet: public Token{
public: 
    ResultSet(const vector<long> result);

    virtual void Print(ostream& outs = cout);

    Token* intersection(ResultSet* other);
    Token* union_set(ResultSet* other);
    void set_result(vector<long> result);
    vector<long> get_result();

private:
    vector<long> _result;
};

#endif // RESULTSET_H


#ifndef LPAREN_H
#define LPAREN_H

class LParen : public Token {
public:
    LParen(const string& token = "("): Token(token, LPAREN), lparen(token){}
    virtual void Print(ostream& outs=cout);

private:
    string lparen;
};

#endif // LPAREN_H

#ifndef RPAREN_H
#define RPAREN_H

class RParen : public Token {
public:
    RParen(const string& token = ")"): Token(token, RPAREN), rparen(token){}
    virtual void Print(ostream& outs=cout);

private:
    string rparen;
};

#endif // RPAREN_H


#ifndef TOKENSTR_H
#define TOKENSTR_H

class TokenStr : public Token {
public:
    TokenStr(const string& token = "");
    virtual void Print(ostream& outs=cout);

private:
    string tokenstr;
};

#endif // TokenStr_H

#ifndef RELATIONAL_H
#define RELATIONAL_H

class Relational : public Token {
public:
    Relational(const string& token = "");

    virtual void Print(ostream& outs=cout);

    Token* eval(Token* left, Token* right, const Table& t);


    void get_pemdas();


private:
    string relational;
};

#endif // Relational_H



#ifndef LOGICAL_H
#define LOGICAL_H

class Logical : public Token {
public:

    Logical (const string& token = "");   
    virtual void Print(ostream& outs=cout);

    Token* eval(Token* left, Token* right);
    int get_pemdas();

private:
    string logical;
};

#endif // Logical_H

#ifndef RPN_H
#define RPN_H
class RPN{
public:
    // constructor
    RPN(const Queue<Token *> post_fix, const string& table_name = ""): _post_fix(post_fix), t(table_name){}   
    
    vector<long> eval();
    void set_post_fix(Queue<Token*> post_fix);


    friend ostream& operator << (ostream& outs, const Queue<Token *>& print_me){
        return outs << print_me;
    }
private:
    Queue<Token *> _post_fix;
    Table t;
};

#endif // RPN_H


#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H


class SY{
public:
    // default ctor
    SY(){}

    SY(const string& infix = ""){
        /*
        docstring
        this fucntion takes a string and converts it into a vector of strings
        then it converts the vector of strings into a queue of tokens
        */
    _infix = infix;
    //string to vector
    string holder;
    for(int i = 0; i < infix.size(); i++){
        if(infix.at(i) == ' ' || infix.at(i) == '(' || infix.at(i) == ')'){
                // cout << "holder: " << holder << endl;
            switch (infix.at(i))
            {
            case '(':
                // push in the paren
                _infix_vector.push_back("(");
                break;
            case ')':
                if(holder != "")
                // push in the exisiting holder and push the paren
                _infix_vector.push_back(holder);
                _infix_vector.push_back(")");
                break;
            default:
                if(holder != "")
                    _infix_vector.push_back(holder);
                break;
            }
            //
            holder = "";
        }else{
            holder += infix.at(i);
        }
    }
        infix_to_postfix();
    }

    SY(const vectorstr& infix_vector): _infix_vector(infix_vector){
        infix_to_postfix();
    }
    // member functions
    void infix_to_postfix(){
        /*
        docstring
        this function takes a vector of strings and converts it into a queue of tokens
        */
        Queue<Token *> output;
        Stack<Token *> operators;
        string holder;
        
        for(int i = 0; i < _infix_vector.size(); i++){
            holder = _infix_vector.at(i);
            if(holder == "and" || holder == "or"){
                logicalCheck(new Logical(holder), operators, output);
            }else if(holder == "(" || holder == ")"){
                parenCheck(holder, operators, output);
            }else if(holder == "=" || holder == "<" || holder == ">" || holder == "<=" || holder == ">="){
                operators.push(new Relational(holder));
            }else{
                output.push(new Token(holder, TOKEN_STR));
            }
        }

        while(!operators.empty()){
            output.push(operators.pop());
        }

        _postfix = output;
    }

    void logicalCheck(Logical* popped, Stack<Token*>& operators, Queue<Token*>& output){
        /*
        docstring
        this function checks the logical operators and pushes them into the output queue
        */
        while(!operators.empty() && operators.top()->get_type() != LPAREN){
            if(operators.top()->get_type() == LOGICAL){
                // while top is a logical operator check its precedence with the one that is popped
                if(static_cast<Logical *>(operators.top())->get_pemdas() > popped->get_pemdas()){
                    // if my top has a high value of precedance pop it out
                    output.push(operators.pop());                        
                }else{
                    // break the while loop
                    break;
                }   
            }
            else{
                // if my top is not a logical operator pop it out ONCE
                output.push(operators.pop());
            }
        }
        // push the popped operator into the stack
        operators.push(popped);
    }

    void parenCheck(string holder, Stack<Token*>& operators, Queue<Token*>& output){
        /*
        docstring
        this function checks the paren and pushes them into the output queue
        */
       
        if(holder == "("){
            operators.push(new LParen("("));
        }else{
            // pop out all the operators until you see a left paren
             while(!operators.empty() && operators.top()->get_type() != LPAREN){
                output.push(operators.pop());
            }
            // pop out the left paren if not empty
            if(!operators.empty())
                operators.pop();
        }
    }


    void set_infix(const string& infix){
        _infix = infix;
    }
    void set_infix_vector(const vector<string>& infix_vector){
        _infix_vector = infix_vector;
    }

    Queue<Token*> get_postfix(){
        return _postfix;
    }

    friend ostream& operator << (ostream& outs, const SY& sy){
        outs << "Infix: " << sy._infix << endl;
        outs << "Postfix: " << sy._postfix << endl;
        return outs;
    }    
private:
    string _infix;
    vector<string> _infix_vector;
    Queue<Token *> _postfix;
    Map<string, long> _field;

};

#endif
