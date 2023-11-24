#pragma once
#include <iostream>
#include "Regex.h"
#include "Entities.h"
using namespace std;
class commandParser {
    const static int INSERT_TABLE_LOCATION = 12;
    const static int FROM_SIZE = 4;
    const static int WHERE_SIZE = 5;
    //function for counting the number of appearances of a character from a string, to be used only with other commandParser functions
    static int countChars(string s, char c) {
        int k = 0;
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == c)
                k++;
        }
        return k;
    }
    //extract strings in a range by the separator, by default is comma, requires string with no spaces
    static string* extractParameters(string& s, int start, int end, char separator = ',') {
        string* values = new string[countChars(s, separator) + 1];
        int currentValue = 0;
        for (int i = start; i < end; i++) {
            if (s[i] == ' ') {
                delete[] values;
                return nullptr;
            }
            else if (s[i] == separator)
                currentValue++;
            else
                values[currentValue].push_back(s[i]);
        }
        return values;
    }
    //extract the string between 2 positions
    static string extractString(string& s, int start, int end) {
        string string = "";

        for (int i = start; i < end; i++)
            string.push_back(s[i]);
        return string;
        /*if (s.find("WHERE") != string::npos) {
            int filterLocation = s.find("WHERE") + WHERE_SIZE;
            for (int i = filterLocation + 1; i < s.length(); i++)
                filter.push_back(s[i]);
        }
        return filter;*/
    }
public:
    //functions for individually parsing a command
    static int displayParser(string& command) {
        string entityName = extractString(command, command.find_last_of(' ') + 1, command.length());

        cout << "Table to display: " << entityName << endl;
        return 1;
    }

    static int dropParser(string& command) {
        string entityName  = extractString(command, command.find_last_of(' ') + 1, command.length());
        
        if (command.find("TABLE")!=string::npos)
            cout << "Table to drop: ";
        else
            cout << "Index to drop: ";
         cout << entityName << endl;
        return 1;
    }

    static int selectParser(string& command, string& err)
    {
        string params;
        bool hasFilter = false;
        if (command.find("ALL") != string::npos)
            params = "all";
        else {
            //params = extractString(command, command.find_first_of('(') + 1, command.find_last_of(')'));
            int length = command.find_last_of(')') - command.find_first_of('(') - 1;
            params = command.substr(command.find_first_of('(') + 1, length);
            if (params.find(' ') != string::npos || length <= 0) {
                err = "Invalid parameter list";
                return 0;
            }
        }

        //length of the parameters substring from an select command
        int selectTableLocation = command.find("FROM") + FROM_SIZE;
        cout << endl;
        string tableName = "";
        for (int i = selectTableLocation+1; i < command.length(); i++) {
            if (command[i] == ' ')
                break;
            tableName.push_back(command[i]);
        }

        int filterLocation;
        string filter = "";
        if (command.find("WHERE") != string::npos) {
            filterLocation = command.find("WHERE") + WHERE_SIZE;
            filter = extractString(command, filterLocation + 1, command.length());
            if (countChars(filter, ' ') != 2 || countChars(filter, ',')) {
                err = "Invalid filter";
                return 0;
            }
        }
            
        if (filter != "")
            hasFilter = true;
        
        cout << "Table: " << tableName << endl;
        if (params != "all")
            cout << "Columns: " << countChars(params, ',') + 1 << endl;
        cout << "Columns: " << params << endl;
        cout << "Filter: " << hasFilter << endl;
        if (hasFilter)
            cout << "Filter column: " << filter << endl;

        //string* values = extractParameters(params, 0, params.length());

        //if (!values) {
        //    err = "Invalid parameter list";
        //    //delete[] values;
        //    return 0;
        //}
        //delete[] values;
        return 1;
    }

    static int insertParser(string& command, string& err) {
        //length of the parameters substring from an insert command
        int length = command.find_last_of(')') - command.find_first_of('(') -1;
        string params = command.substr(command.find_first_of('(')+1, length);
        //cout << length << endl;
        //extract table name
        string tableName;
        for (int i = INSERT_TABLE_LOCATION; command[i] != ' '; i++)
            tableName.push_back(command[i]);

        //array of strings, with enough space for the number of values in the insert command
        /*string* values = new string[countChars(params, ',') + 1];
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
        }*/
        string* values = extractParameters(params, 0, params.length());

        if (!values) {
            err = "Invalid parameter list";
            delete[] values;
            return 0;
        }

        //instantiate a record (line of a table) and print to screen
        cout << "\nTable: " << tableName;
        Record r1(values, countChars(params, ',') + 1);
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

    static int deleteParser(string& command, string& err) {
        string tableName = extractString(command, command.find("FROM")+ FROM_SIZE + 1, command.find("WHERE") - 1);

        int filterLocation;
        string filter = "";
        
        filterLocation = command.find("WHERE") + WHERE_SIZE;
        filter = extractString(command, filterLocation + 1, command.length());
        if (countChars(filter, ' ') != 2 || countChars(filter, ',')) {
            err = "Invalid filter";
            return 0;
        }
        
        cout << "Table: " << tableName << endl;
        cout << "Filter column: " << filter << endl;

        return 1;
    }
};