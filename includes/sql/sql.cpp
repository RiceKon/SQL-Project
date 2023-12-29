#include "sql.h"


SQL::SQL(const string& batch){
    set_batch(batch);
}
void SQL::set_batch(const string& batch){
    batch_name = batch;
    BATCH_FILE.open(batch_name, ios::in);
    if(BATCH_FILE.fail()){
        cout << "batch file does not exist" << endl;
        valid = false;
        BATCH_FILE.close();
    }else{
        valid = true;
    }
    BATCH_FILE.close();
}

void SQL::run_batch(){
    /*
    docstring:
    this function runs the batch file and prints out the table
    */
    string com;  
    Table t;   
    bool print = true;
    BATCH_FILE.open(batch_name, ios::in | ios::app);

    if(getline(BATCH_FILE, com) && com == "@echo off"){ 
        print = false;
    }else{
        t = command(com);
        if(valid && print){
            cout << t << endl;
        }
    }

    while(getline(BATCH_FILE, com)){
        t = command(com);
        if(valid && print){
            cout << t << endl;
        }
    }

    BATCH_FILE.close();
}


Table SQL::command(const string& command){
    /*
    docstring: 
    this function is the black box of parser and table, jumbling it all together creating a SQL data base
    */
    Table t;
    char s[300];
    strcpy(s, command.c_str());
    Parser parser(s);
    mmap_ss ptree = parser.parse_tree();
    // do not go futher parse tree is empty
    if(ptree.empty()){
        valid = false;
        return t;
    }
    valid = true;
    //execute the command and put it into a table
    t = translate(ptree);
    if(valid && batch_name.empty()){
        BATCH_FILE.open(_tname + "_batch", ios::out | ios::app);
        BATCH_FILE << command << endl;
        BATCH_FILE.close();
    }
    return t;
}



Table SQL::translate(MMap<string, string> valid_commands){
    /*
    docstring:
    this function translates the parse tree into a table using iterators
    */
    for(MMap<string, string>::Iterator it = valid_commands.begin(); it != valid_commands.end(); it++){
        // cout << "printing out the key" << (*it).key << endl;


        if((*it).key == "command"){
            _command = (*it).value_list.at(0);
        }else if((*it).key == "col" || (*it).key == "fields"){
            _fields = (*it).value_list;
        }else if((*it).key == "table_name"){
            _tname = (*it).value_list.at(0);
        }else if((*it).key == "conditions"){
            _conditions = (*it).value_list;
        }else if((*it).key == "values"){
            _values = (*it).value_list;
        }else if((*it).key == "where"){
            _where = (*it).value_list.at(0);
        }else{
            cout << "something is wrong with parser T^T" << endl;
        }
    }

    // empty table if anything goes wrong return an empty table
    Table t;
    t = excute_command(_command);

    return t;
}

Table SQL::excute_command(const string& command){
    /*
    docstring:
    this function executes the command and returns a table
    */
    // creates a falg that check if the file exists already will be used in the commnads make/create, select and insert
    fstream checker(_tname + ".txt", ios::in); bool check = checker.fail(); checker.close();
    if(command == "make" || command == "create"){
        if(checker.is_open()){
            cout << "Trying to create/make: file already exists" << endl;
            valid = false;
            return Table();
        }
        Table t(_tname, _fields);
        return t;
    }else if(command == "select"){
        if(check){
            cout << "Trying to select: cannot, file doesnot exist" << endl;
            valid = false;
            return Table();
        }
        Table t(_tname), _return;
        if(_fields.at(0) == "*"){
            _fields = t.get_field_names();                 // restrict the fields
        }
        if(_where == "yes"){
            _return = t.select(_fields, _conditions);
            _conditions.clear();
        }else if (_where.empty() && _fields != t.get_field_names()){
            _return = t.select(_fields, "", "", "");       // apply the restricted fields
        }else{
            _return = t.select_all();
        }

        recnos = t.select_recnos();

        return _return;
    }else if (command == "insert"){
        if(check){
            cout << "Trying to insert: cannot, file doesnot exist" << endl;
            valid = false;
            return Table();
        }
        Table t(_tname);
        t.insert_into(_values);
        return t;
    }else if((command == "drop")){
        // ask about drop
    }else {
        std::cout << "??? stooid, ur parser no work" << endl;
    }
    
    // if anything goes wrong return an empty table
    Table t;
    return t;
}