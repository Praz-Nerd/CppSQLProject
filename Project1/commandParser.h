#pragma once
#include <iostream>
#include "Regex.h"
#include "Entities.h"
using namespace std;
class commandParser {

    string* command = nullptr;
    string commandType = "";

    const static int INSERT_TABLE_LOCATION = 12;
    const static int FROM_SIZE = 4;
    const static int WHERE_SIZE = 5;
    const static int INDEX_SIZE = 5;
    const static int IF_NOT_EXISTS_SIZE = 13;
    const static int ON_SIZE = 2;
    const static int UPDATE_SIZE = 6;
    const static int SET_SIZE = 3;
    const static int TABLE_SIZE = 5;
    const static int COLUMN_ATTRIBUTES = 4;
public:
    void setCommand(string& s) {
        if (!s.empty()) {
           this->command = new string(s);
        }
        else {
            string err = "Command does not exist";
            throw (err);
        }
    }

    string getCommand() {
        if (this->command)
            return *command;
        else
            return "";
    }

    void setCommandType(string s) {
        if (s == "DISPLAY" || s == "DROP" || s == "INSERT" || s == "SELECT" || s == "DELETE" || s == "UPDATE" || s == "CREATE") {
            this->commandType = s;
        }
        else {
            string err = "Invalid command type";
            throw (err);
        }
    }

    string getCommandType() {
        return this->commandType;
    }

    commandParser(string command, string commandType) {
       setCommand(command);
       setCommandType(commandType);
    }

    commandParser(string* command, string commandType) {
        setCommand(*command);
        setCommandType(commandType);
    }

    commandParser(const commandParser& cp) {
        this->commandType = cp.commandType;
        if (cp.command) {
            this->setCommand(*(cp.command));
        }
        else
            this->command = nullptr;
    }

    commandParser& operator=(const commandParser& cp) {
        if (this != &cp) {
            this->commandType = cp.commandType;
            if (this->command)
                delete command;
            if (cp.command)
                this->command = new string(*(cp.command));
            else
                this->command = nullptr;
        }
    }

    explicit operator string() {
        return *command;
    }

    char operator[](int index) {
        if (command != nullptr && index >= 0 && index < command->length()) {
            return (*command)[index];
        }
    }

    bool operator==(commandParser cp) {
        return commandType == cp.commandType;
    }

    bool operator<(string s) {
        return commandType < s;
    }

    bool operator!() {
        if (!command)
            return true;
        return false;
    }

    string operator+(string s) {
        return (*command) + s;
    }

    commandParser& operator++() {
        (*command)[command->length() - 1]++;
        return *this;
    }

    commandParser operator++(int i) {
       commandParser copy = *this;
        (*command)[command->length() - 1]++;
        return copy;
    }

    friend ostream& operator<<(ostream&, commandParser);
    friend istream& operator>>(istream&, commandParser&);

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
    static string* extractParameters(string s, int start, int end, char separator = ',') {
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
    static string extractString(string s, int start, int end) {
        string string = "";

        for (int i = start; i < end; i++)
            string.push_back(s[i]);
        return string;
    }

    //functions for individually parsing a command
    int displayParser() {
        string entityName = extractString(this->getCommand(), this->getCommand().find_last_of(' ') + 1, this->getCommand().length());

        cout << "Table to display: " << entityName << endl;
        return 1;
    }

    int dropParser() {

        string entityName  = extractString(this->getCommand(), this->getCommand().find_last_of(' ') + 1, this->getCommand().length());
        
        if (this->getCommand().find("TABLE") != string::npos)
            cout << "Table to drop: ";
        else
            cout << "Index to drop: ";
         cout << entityName << endl;
        return 1;
    }

    int selectParser(string& err)
    {
        string params;
        bool hasFilter = false;
        if (this->getCommand().find("ALL") != string::npos)
            params = "all";
        else {
            //params = extractString(command, command.find_first_of('(') + 1, command.find_last_of(')'));
            int length = this->getCommand().find_last_of(')') - this->getCommand().find_first_of('(') - 1;
            params = this->getCommand().substr(this->getCommand().find_first_of('(') + 1, length);
            if (params.find(' ') != string::npos || length <= 0) {
                err = "Invalid parameter list";
                return 0;
            }
        }

        //length of the parameters substring from an select command
        int selectTableLocation = this->getCommand().find("FROM") + FROM_SIZE;
        cout << endl;
        string tableName = "";
        for (int i = selectTableLocation+1; i < this->getCommand().length(); i++) {
            if (this->getCommand()[i] == ' ')
                break;
            tableName.push_back(this->getCommand()[i]);
        }

        int filterLocation;
        string filter = "";
        if (this->getCommand().find("WHERE") != string::npos) {
            filterLocation = this->getCommand().find("WHERE") + WHERE_SIZE;
            filter = extractString(this->getCommand(), filterLocation + 1, this->getCommand().length());
            if (countChars(filter, ' ') > 2 || countChars(filter, ',')) {
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

    int insertParser(string& err) {
        //length of the parameters substring from an insert command
        int length = this->getCommand().find_last_of(')') - this->getCommand().find_first_of('(') -1;
        string params = this->getCommand().substr(this->getCommand().find_first_of('(')+1, length);
        //extract table name
        string tableName;
        for (int i = INSERT_TABLE_LOCATION; this->getCommand()[i] != ' '; i++)
            tableName.push_back(this->getCommand()[i]);

        string* values = extractParameters(params, 0, params.length());

        if (!values) {
            err = "Invalid parameter list";
            delete[] values;
            return 0;
        }

        //instantiate a record (line of a table) and print to screen
        cout << "\nTable: " << tableName;
        Record r1(values, countChars(params, ',') + 1);
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

    int updateParser(string& err) {
        string tableName = extractString(this->getCommand(), this->getCommand().find("UPDATE") + UPDATE_SIZE + 1, this->getCommand().find("SET") - 1);
        string setValue = extractString(this->getCommand(), this->getCommand().find("SET") +SET_SIZE+ 1, this->getCommand().find("WHERE") - 1);
        string filter = extractString(this->getCommand(), this->getCommand().find("WHERE") + WHERE_SIZE + 1, this->getCommand().length());
        string columnName, value;


        if (countChars(setValue, ' ') > 2 || countChars(setValue, ',')) {
            err = "Invalid SET clause";
            return 0;
        }
        else {
            regexStatements::removeSpaces(setValue, "");
            columnName = extractString(setValue, 0, setValue.find('='));
            value = extractString(setValue, setValue.find('=') + 1, setValue.length());
        }

        if (countChars(filter, ' ') > 2 || countChars(filter, ',')) {
            err = "Invalid WHERE clause";
            return 0;
        }

        cout << "Table: " << tableName << endl;
        cout << "Column to change: " << columnName << endl;
        cout << "Value: " << value << endl;
        cout << "Condition: " << filter << endl;


        return 1;
    }

    int deleteParser(string& err) {
        string tableName = extractString(this->getCommand(), this->getCommand().find("FROM")+ FROM_SIZE + 1, this->getCommand().find("WHERE") - 1);

        int filterLocation;
        string filter = "";
        
        filterLocation = this->getCommand().find("WHERE") + WHERE_SIZE;
        filter = extractString(this->getCommand(), filterLocation + 1, this->getCommand().length());
        if (countChars(filter, ' ') > 2 || countChars(filter, ',')) {
            err = "Invalid filter";
            return 0;
        }
        
        cout << "Table: " << tableName << endl;
        cout << "Filter column: " << filter << endl;

        return 1;
    }

    int createIndexParser(string& err) {
        string indexName = "";
        string tableName = extractString(this->getCommand(), this->getCommand().find("ON")+ON_SIZE+1, this->getCommand().find('(')-1);
        string columnName = extractString(this->getCommand(), this->getCommand().find('(')+1, this->getCommand().find(')'));

        if (this->getCommand().find("IF NOT EXISTS") != string::npos)
            indexName = extractString(this->getCommand(), this->getCommand().find("IF NOT EXISTS") + IF_NOT_EXISTS_SIZE + 1, this->getCommand().find("ON") - 1);
        else
            indexName = extractString(this->getCommand(), this->getCommand().find("INDEX") + INDEX_SIZE + 1, this->getCommand().find("ON") - 1);

        //this condition is checked in regex pattern
        /*if (countChars(columnName, ' ') || countChars(columnName, ',')) {
            err = "Invalid column name";
            return 0;
        }*/

        cout << "Index: " << indexName << endl;
        cout << "On table: " << tableName << endl;
        cout << "Column: " << columnName << endl;

        //instantiate index, continue processing

        return 1;

    }

    int createTableParser(string& err) {
        string tableName;
        string columns = extractString(this->getCommand(), this->getCommand().find_first_of('(')+1, this->getCommand().find_last_of(')'));
        //cout << countChars(columns, '(') << endl;
        if (this->getCommand().find("IF NOT EXISTS") != string::npos)
            tableName = extractString(this->getCommand(), this->getCommand().find("TABLE") + TABLE_SIZE + 1, this->getCommand().find("IF") - 1);
        else
            tableName = extractString(this->getCommand(), this->getCommand().find("TABLE") + TABLE_SIZE + 1, this->getCommand().find_first_of('(') - 1);

        regexStatements::removeSpaces(columns, "");
        if ((countChars(columns, '(') != countChars(columns, ')'))||columns[0]!='(') {
            err = "Invalid column list";
            return 0;
        }

        /*string** params = new string*[countChars(columns, '(')];
        for (int i = 0; i < countChars(columns, '('); i++)
            params[i] = new string[COLUMN_ATTRIBUTES];*/
        //string* params = new string[COLUMN_ATTRIBUTES * countChars(columns, '(')];
        //cout << COLUMN_ATTRIBUTES * countChars(columns, '(');
        int i = columns.find_first_of('('), k = 0;
        int j = columns.find_first_of(')');
        string params;
        for (k = 0; k < countChars(columns, '('); k++) {
            while (columns[i] != '(')
                i++;
            while (columns[j] != ')')
                j++;
            string oneColumn = extractString(columns, i + 1, j);
            if (countChars(oneColumn, ',') != 3) {
                err = "Invalid column";
                return 0;
            }
            else {
                params += oneColumn;
                params += ',';
                i++;
                j++;
            }
            
        }
        params.pop_back();
        string* values = extractParameters(params, 0, params.length());
        //cout << params << endl;
        cout << "Table: " << tableName << endl;
        k = 1;
        for (int i = 0; i < COLUMN_ATTRIBUTES*countChars(columns, '('); i+=4) {
            cout<< endl << "Column " << k << endl;
            cout << "Table name: " << values[i] << endl;
            cout << "Type: " << values[i+1] << endl;
            cout << "Size: " << values[i+2] << endl;
            cout << "Default values: " << values[i+3] << endl;
            //Column c(values[i], values[i + 1], (int)values[i + 2], values[i + 3]);
            k++;
        }
           

        //cout << tableName << endl;
        //cout << columns << endl;

        delete[] values;
        return 1;
    }

    ~commandParser() {
        if (command)
            delete command;
        command = nullptr;
    }

};

ostream& operator<<(ostream& out, commandParser cp) {
    out << endl << "Command type: " << cp.commandType;
    out << endl << "Command: " << *(cp.command);
    return out;
}

istream& operator>>(istream& in, commandParser& cp) {
    string buffer;
    cout << endl << "Change command";
    in >> buffer;
    cp.setCommand(buffer);
    return in;
}