#pragma once
#include <iostream>
#include "Regex.h"
#include "Entities.h"
using namespace std;
class commandParser {
    //table name in insert command starts here
    const static int insertTableLocation = 12;
    const static int fromSize = 4;
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
        string tableName = "";
        for (int i = command.find_last_of(' ') + 1; i < command.length(); i++)
            tableName.push_back(command[i]);

        cout << "Table to display: " << tableName << endl;
        return 1;
    }

    static int dropParser(string& command) {
        string tableName = "";
        for (int i = command.find_last_of(' ') + 1; i < command.length(); i++)
            tableName.push_back(command[i]);

        cout << "Table to drop: " << tableName << endl;
        return 1;
    }

    static int selectParser(string& command, string& err)
    {
        //length of the parameters substring from an select command
        int length = command.find_last_of(')') - command.find_first_of('(') - 1;
        string params = command.substr(command.find_first_of('(') + 1, length);
        cout << params << endl;
       

        int selectTableLocation = command.find("FROM") + fromSize;
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
        for (int i = insertTableLocation; command[i] != ' '; i++)
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
        Record r(values, currentValue + 1);
        r.displayRecord();

        delete[] values;
        return 1;
    }
};