#include "table.h"

int Table::serial = 0;

Table::Table(string tname): _table_name(tname){                                //gets all of the record number from that file name
    /*
    docstring
    this ctor, it not really a ctor where it create a new object with new information
    however it creates a new object that has exisiting information and fill up the object 
    with the information from the file
    */
    open_files(tname);

    FileRecord buffer;
    string holder;
    vectorstr values;
    int i = 0;
    // get the field names 
    while(getline(_field_file, holder)){       // get the text from for field names
        _field_names.insert(holder, i);
        i++;
    }

    i = 0;
    // fill in the Mulitmap
    while(buffer.read(_file, i) > 0){          // get the record number from the file
        values = buffer.get_record();
        recnos.push_back(i);
        for(int j = 0; j < _field_names.size(); j++){
            table[j].get(values.at(j)).push_back(i);
        }
        i++;
    }
    trecno = i - 1;

    close_files();
}

Table::Table(string fname, vectorstr ftype): trecno(0), _table_name(fname){
    /*
    docstring
    this fucntion create
    */

    open_fileW(_file, (fname  + ".bin").c_str());       // creates a new file with the name of fname'
    _field_file.open(fname + ".txt", ios::out);

    assert(!_field_file.fail());
    
    for(int i = 0; i < ftype.size(); i++)   // creates a new field name
    {
        _field_names.insert(ftype.at(i), i);
        _field_file << ftype.at(i) << endl;
    }

    close_files();
}

long Table::insert_into(vectorstr values){
    open_files(_table_name);
    FileRecord buffer = FileRecord(values);
    trecno = buffer.write(_file);
    
    // cout << "_field_names.size(): " << _field_names.size() << endl;
    // cout << values << endl;

    for(int i = 0; i < _field_names.size(); i++){ // _field_names.size() == values.size()
        table[i].get(values.at(i)).push_back(trecno);
    }
    
    close_files();

    return trecno;
           
}

Table Table::select_all(){
    /*
    docstring

    this fucniton selects everything in a exisiting file and create a new table that has serail appened onto it
    */
    recnos.clear();
    Table t(_table_name + to_string(++serial), get_field_names());
    open_files(_table_name);
    FileRecord buffer;
    vectorstr items;
    int i = 0;

    // get the item from the file
    while(buffer.read(_file, i) > 0){
        items = buffer.get_record();
        t.insert_into(items);
        recnos.push_back(i);
        i++;
    }

    return t;
}


void Table::Iterate(MMapIt start, MMapIt end, Table& t, int fn, vectorstr columns){
    /*
    docstring
    This function takes 2 itterators as paramaters, and it itterates throguh them, from start to end
    adding the record numbers in to the private variable recnos
    and adding everything to a table
    this function can also filter the columns
    */
    // have to open bc only in read and write
    open_fileRW(_file, (_table_name + ".bin").c_str());


    vectorstr temp, items;
    FileRecord buffer;

    // cout << "start: " << (*start).value_list << endl;

    for(; start != end; start++){
        // iiterate through the size of the record
        for(int i = 0; i < (*start).value_list.size(); i++){
            // get the record number
            recnos.push_back((*start).value_list.at(i));
            // read from the file
            buffer.read(_file, (*start).value_list.at(i));
            //get the values from the file
            temp = buffer.get_record();
            // filter / push to the respect of columns
            for(int j = 0; j < columns.size(); j++){    //and then get the location of tha field name / position at the record, and insert it into temp
                items.push_back(temp.at(_field_names.get(columns.at(j))));          
            }
            // cout << items << endl;
            // insert it into a new table
            long record = t.insert_into(items);
            items.clear();
        }
    }
    _file.close();

}

Table Table::select(const vectorstr& columns, const string& col, const string& op, string const& table_item){
    /*
    docstring
    this fucntion is the basic form of select, where it will take in 1 column, 1 restriction, and 1 value
    This function will create a table and returns it
    */
    recnos.clear();

    Table t(_table_name + to_string(++serial), columns);
    int fn = _field_names.get(col);    
    MMapIt start = table[fn].begin(), end = table[fn].end();
    getbounds(start, end, op, table_item, fn);
    Iterate(start, end, t, fn, columns);
    return t;
}

Table Table::select(const vectorstr& columns, const Queue<Token *>& tokens){
    /*
    docstring
    this fucntion take in a infix of token pointers and caculates it, this will use the small select
    with the help of RPN
    It will return a table that contains all the informaiton
    we are also able to filter out the columns
    */


    // empty table
    open_files(_table_name);
    if(_field_file.fail()){
        return Table();
    }

    // create a new table object
    Table t(_table_name + to_string(++serial), columns);
    vectorstr items, temp;
    RPN rpn(tokens, _table_name);
    FileRecord buffer;
    // dont run rpn if its empty
    if(tokens.size() != 0){
        recnos = rpn.eval();        
    }

    // this will not run if rpn is empty
    for(int i = 0; i < recnos.size(); i++){
        buffer.read(_file, recnos.at(i));
        temp = buffer.get_record();

        // rearange the order of the items with the correct amount of items
        for(int j = 0; j < columns.size(); j++){
            items.push_back(temp.at(_field_names.get(columns.at(j))));
        }
        t.insert_into(items);
        // cout << items << endl;
        items.clear();
    }

    close_files();
    return t;
}

Table Table::select(const vectorstr& columns, const vectorstr& tokens){
    /*
    this function take a infix of string and we will change it into a queue of token pointers
    Then we will pass it into the select that has 2 params that takes token pointers and eval
    */
    SY sy(tokens);
    cout << sy.get_postfix() << endl;
    return select(columns, sy.get_postfix());
}


void Table::select_records(const vectorstr& columns, const string& col, const string& op, string const& table_item){
    /*
    docstring
    this function is the small selese however, it does not create a new table object but get the information
    (record number) and returns it
    */
    recnos.clear();
    int fn = _field_names.get(col);
    MMapIt start = table[fn].begin(), end = table[fn].end();
    getbounds(start, end, op, table_item, fn);
    for(; start != end; start++){
        for(int i = 0; i < (*start).value_list.size(); i++){
            recnos.push_back((*start).value_list.at(i));
        }
    }

}



vector<long> Table::select_recnos(){
    return recnos;
}

void Table::getbounds(MMapIt& start, MMapIt& end, const string& op, const string& table_item, const int& fn){
        if(op == "="){
            start = table[fn].find(table_item);
            end = ++(table[fn].find(table_item));
        }else if (op == "<"){
            start = table[fn].begin();
            end = table[fn].lower_bound(table_item);
        }else if (op == ">"){
            start = table[fn].upper_bound(table_item);
            end = table[fn].end();
        }else if (op == "<="){
            start = table[fn].begin();
            end = table[fn].upper_bound(table_item);
        }else if (op == ">="){
            start = table[fn].lower_bound(table_item);
            end = table[fn].end();
        }
}


vectorstr Table::get_field_names() const {
    fstream ff;
    ff.open((_table_name + ".txt").c_str(), ios::in);
    vectorstr field_names;
    string holder;

    while(getline(ff, holder)){
        field_names.push_back(holder);
    }

    return field_names;
}
