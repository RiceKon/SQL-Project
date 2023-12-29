#include <iostream>
#include <iomanip>
#include "includes/sql/sql.h"


using namespace std;

int main(int argv, char** argc) {
    SQL sql; Table t; char x = ' '; string batch, command;
    cout << "Welcome to SQL database: " << endl;
    cout << "Please enter the name of a Batch file, or type 'none' to enter commands manually: " << endl;
    getline(cin, batch);
    if(batch != "none"){
        sql.set_batch(batch);
        if(sql.is_valid()){
            sql.run_batch();
        }
        cout << "Contiune? Press [X] to exit" << endl;
        cin.get(x); tolower(x);
    }
    cout << "=============================================================================================" << endl;

    while (x != 'x'){
        cout << "Please enter a SQL command, the commands are" << endl;
        cout << " \t commands: (create), (make), (insert) (select), " << endl;
        getline(cin, command); 
        cout << "\tcommand: " << command << endl;

        t = sql.command(command);
        // valid is a public variable to check if the code is valid or not
        (sql.is_valid() == true) ? cout << t << endl  << endl : cout << "\t please try again " << endl << endl;

        cout << "Do you with to continue with commands? (Enter X to exit)" << endl;
        cin.get(x); tolower(x); cin.ignore();// get the line check if its an x, and then stops cin from reading the next line
    }

    cout << "Exiting, Press [X] to clean up files" << endl;
    cin >> x;
    tolower(x);
    if(x == 'x'){
        cout << "cleaning up files" << endl;
        system("del -rf *.bin *.txt");
    }
    

    cout << "\n\n\n=====================" << endl;
    return 0;
}
