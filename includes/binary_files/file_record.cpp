#include "file_record.h"

long FileRecord::write(fstream &outs){
    //r.write(f); //take the Record r and write it into file f
    //  and return the record number of this Record

    //write to the end of the file.
    outs.seekg(0, outs.end);


    long pos = outs.tellp();    //retrieves the current position of the
                                //      file pointer

    //pay attention to this:
    //outs.write(&record[0], sizeof(record));

    outs.write(_record[0], sizeof(_record));
    // cout << "record number: " << pos/sizeof(_record) << endl;

    return pos/sizeof(_record);  //record number
}

long FileRecord::read(fstream &ins, long recno){
    //returns the number of bytes read.
    //    r.read(f, 6);
    //    cout<<r<<endl;
    long pos = recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);

    //ins.read(&__record[0], sizeof(__record))
    ins.read(_record[0], sizeof(_record));

    size = 0;
    for(; _record[size][0] != '\0'; ++size){
        // cout << _record[size] << endl;

      } // get the size of the record

    //don't you want to mark the end of  the cstring with null?
    //_record[] => [zero'\0'trash trash trash trash]
    //don't need the null character, but for those of us with OCD and PTSD:
    return ins.gcount();

}

vector<string> FileRecord::get_record() const{
    vector<string> result;

    for (int i = 0; i < size; ++i)
        result.push_back(_record[i]);

    return result;
}

ostream& operator<<(ostream& outs, const FileRecord& r){
    // vector<string> temp = r.get_record();
    // cout << temp;


    for(int i = 0; i < r.size; i++){
        outs<< setw(20) << r._record[i] << "";
    }
    return outs;
}

