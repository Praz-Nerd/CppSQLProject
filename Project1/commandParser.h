#pragma once
#include <iostream>
#include "Regex.h"
#include "Entities.h"
using namespace std;
class commandParser {
    const static int INSERT_TABLE_LOCATION = 12;
    const static int FROM_SIZE = 4;
    //function for counting the number of appearances of a character from a string, to be used only with other commandParser functions
    static int countChars(string s, char c) {
        int k = 0;
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == c)
                k++;
        }
        return k;
    }
public:
    //functions for individually parsing a command
    static int displayParser(string& command) {
        string entityName = "";
        for (int i = command.find_last_of(' ') + 1; i < command.length(); i++)
            entityName.push_back(command[i]);

        cout << "Table to display: " << entityName << endl;
        return 1;
    }

    static int dropParser(string& command) {
        string entityName = "";
        for (int i = command.find_last_of(' ') + 1; i < command.length(); i++)
            entityName.push_back(command[i]);

        if (command.find("TABLE")!=string::npos)
            cout << "Table to drop: ";
        else
            cout << "Index to drop: ";
         cout << entityName << endl;
        return 1;
    }

    static int selectParser(string& command, string& err)
    {
        //length of the parameters substring from an select command
        int length = command.find_last_of(')') - command.find_first_of('(') - 1;
        string params = command.substr(command.find_first_of('(') + 1, length);
        cout << params << endl;
       

        int selectTableLocation = command.find("FROM") + FROM_SIZE;
        cout << selectTableLocation << endl;

        string tableName;
        for (int i = selectTableLocation;(i < command.length()); i++)
            tableName.push_back(command[i]);

        cout << tableName;
        return 1;


    }

    static int insertParser(string& command, string& err) {
        //length of the parameters substring from an insert command
        int length = command.find_last_of(')') - command.find_first_of('(') -1;
        string params = command.substr(command.find_first_of('(')+1, length);

        //extract table name
        string tableName;
        for (int i = INSERT_TABLE_LOCATION; command[i] != ' '; i++)
            tableName.push_back(command[i]);

        //array of strings, with enough space for the number of values in the insert command
        string* values = new string[countChars(params, ',') + 1];
        int currentValue = 0;
        for (int i = 0; i < params.length(); i++) {
            if (params[i] == ' ') {
                err = "Invalid parameter list";
                delete[] values;
                return 0;
            }
            else if (params[i] == ',')
                currentValue++;
            else
            values[currentValue].push_back(params[i]);
        }


        //instantiate a record (line of a table) and print to screen
        cout << "\nTable: " << tableName;
        Record r1(values, currentValue + 1);
        //r.displayRecord();
        cout << r1 << endl;

        //for testing operators and such
        /*Record r2 = r1;
        //cin >> r1;
        if (r1 == r2) cout << "good\n";
        //cout << r1;
        r1 + "baba";
        cout << r1;
        r2++;
        ++r2;
        cout << r2;
        if (r1 < 10) cout << "interesting\n";
        cout << !r2;*/
        
        delete[] values;
        return 1;
    }
};