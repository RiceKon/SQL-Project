#ifndef SQL_H
#define SQL_H
#include "../table/table.h"
#include "../parser/parser.h"
using namespace std;


class SQL{

public:
    SQL(): batch_name(""){}
    SQL(const string& batch);
    void set_batch(const string& batch);
    void run_batch();
    Table command(const string& command);
    vector<long> select_recnos(){return recnos;};
    bool is_valid() const {return valid;}
private:
    bool valid = false;

    Table translate(MMap<string, string> valid_commands);
    Table excute_command(const string& command);
    
    string batch_name;
    fstream BATCH_FILE;

    vector<long> recnos;
    
    string _tname;
    string _command;
    string _where;
    vector<string> _conditions;
    vector<string> _fields;
    vector<string> _values;

};



#endif // SQL_H