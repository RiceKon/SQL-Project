#ifndef STOKEN_H
#define STOKEN_H
#include <iostream>
#include "constants.h"
using namespace std;

class SToken
{
public:
    SToken();
    SToken(const string& str, const int& type);
    friend ostream& operator <<(ostream& outs, const SToken& t){
        outs << "In ostream of Stoken" <<"|" << t._token << "|" << t._type << "|";
        return outs;
    }
    friend ostream& operator <<(ostream& outs, const SToken* t){
        outs << endl;
        outs <<"|" << t->_token << "|" << t->_type << "|" << endl;
        return outs;
    }

    virtual int get_type() const{
        return _type;
    }
    int type() const;
    string type_string() const;
    string token_str() const;
private:
    string _token;
    int _type;
};

class CommandToken : public SToken
{
public:
    CommandToken();
    CommandToken(const string& str);
    friend ostream& operator <<(ostream& outs, const CommandToken& t){
        outs << "|" << t._token << "|";
        return outs;
    }

    virtual int get_type() const;
    string token_str() const;
private:
    string _token;
    int _type;
};

class SYMToken : public SToken
{
public:
    SYMToken();
    SYMToken(const string& str);
    friend ostream& operator <<(ostream& outs, const SYMToken& t){
        outs << "|" << t._token << "|";
        return outs;
    }
    virtual int get_type() const;

    string token_str() const;
private:
    string _token;
    int _type;
};

class ConditionToken : public SToken
{
public:
    ConditionToken();
    ConditionToken(const string& str);
    friend ostream& operator <<(ostream& outs, const ConditionToken& t){
        outs << "|" << t._token << "|";
        return outs;
    }
    virtual int get_type() const;
    string token_str() const;

private:
    string _token;
    int _type;
};


    
#endif