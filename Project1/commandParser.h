#pragma once
#include <iostream>
#include <fstream>
#include "Regex.h"
#include "Entities.h"
#include "Files.h"
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
    static int displayCounter;
    static int selectCounter;
public:
    void setCommand(string& s) {
        if (!s.empty()) {
            s = regexStatements::trimString(s);
            this->command = new string(s);
        }
        else {
            throw exception("Command does not exist");
        }
    }

    string getCommand() {
        if (this->command)
            return *command;
        else
            return "";
    }

    void setCommandType(string s) {
        if (s == "DISPLAY" || s == "DROP" || s == "INSERT" || s == "SELECT" || s == "DELETE" || s == "UPDATE" || s == "CREATE" || s == "IMPORT") {
            this->commandType = s;
        }
        else {
            throw exception("Invalid command type");
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
        return *this;
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

    //function for counting the number of appearances of a character from a string
    static int countChars(string s, char c) {
        int k = 0;
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == c)
                k++;
        }
        return k;
    }
    //extract strings in a range by the separator, by default is comma
    static string* extractParameters(string s, int start, int end, char separator = ',') {
        int size = countChars(s, separator) + 1;
        string* values = new string[size];
        int currentValue = 0;
        for (int i = start; i < end; i++) {
           /* if (s[i] == ' ') {
                delete[] values;
                return nullptr;
            }*/
            if (s[i] == separator)
                currentValue++;
            else
                values[currentValue].push_back(s[i]);
        }

        for (int i = 0; i < size; i++) {
            //remove spaces from beginning and end of strings
      
            values[i] = regexStatements::trimString(values[i]);
        }

        return values;
    }
    //extract the string between 2 positions
    static string extractString(string s, int start, int end) {
        string string = "";
        string = s.substr(start, end - start);
      /*  for (int i = start; i < end; i++)
            string.push_back(s[i]);
        return string;*/
        return string;
    }
    

    //function that extracts WHERE clause elements
    void whereDecoder(string filter, string& filterColumn, string& filterOperator, string& filterValue) {
        filterColumn = extractString(filter, 0, filter.find_first_of("<>=! "));
        //extract and check operator
        filterOperator = extractString(filter, filter.find_first_of("<>=!"), filter.find_last_of("<>=!")+1);
        if (filterOperator != "<" && filterOperator != ">" && filterOperator != "=" && filterOperator != "!=" && filterOperator != "<=" && filterOperator != ">=")
            throw exception("Invalid operator in the WHERE clause");

        //extract and format value
        filterValue = extractString(filter, filter.find_last_of("<>=!") + 1, filter.length());
        filterValue = regexStatements::trimString(filterValue);
        //filterValue = regexStatements::removeQuote(filterValue, "");
        cout << filterColumn << " " << filterOperator << " " << filterValue << endl;
    }

    //functions for individually parsing a command
    int displayParser(string& err) {
        //extract the table name for displaying
        string entityName = extractString(this->getCommand(), this->getCommand().find_last_of(' ') + 1, this->getCommand().length());
        cout << "Table to display: " << entityName << endl;

        BinaryFile tableFile(entityName + ".tab");

        if (!tableFile.exists()) {
            err = "Table does not exist";
            return 0;
        }
        else {
            //display table information to a report file and to the console
            TextFile reportFile("DISPLAY_" + to_string(++displayCounter)+".txt");
            ofstream fout = reportFile.openToWrite();
            Table table(entityName);
            table.displayTableInfo();
            table.displayTableInfo(fout);
            fout.close();
            cout << "Report file " << reportFile.getFileName() << " created.\n";
        }

        return 1;
    }

    int dropParser(string & err) {
        //extract the name of the entity to be dropped
        string entityName  = extractString(this->getCommand(), this->getCommand().find_last_of(' ') + 1, this->getCommand().length());
        //if it's a table or an index
        if (this->getCommand().find("TABLE") != string::npos)
            cout << "Table to drop: ";
        else
            cout << "Index to drop: ";
         cout << entityName << endl;

         if (this->getCommand().find("TABLE") != string::npos) {
             BinaryFile tableFile(entityName + ".tab");
             BinaryFile dataFile(entityName + ".data");
             if (tableFile.exists()) {
                 tableFile.deleteFile();
                 dataFile.deleteFile();
                 cout << "Table " << entityName << " was dropped" << endl;
             }
             else {
                 err = "Table does not exist";
                 return 0;
             }
         }

        return 1;
    }

    int selectParser(string& err)
    {
        string params;
        bool hasFilter = false;
        string filterColumn, filterOperator, filterValue;
       /* string filter = extractString(this->getCommand(), this->getCommand().find("WHERE") + WHERE_SIZE + 1, this->getCommand().length());*/
        //extracting column names to be selected (or all)
        if (this->getCommand().find("ALL") != string::npos)
            params = "all";
        else {
            //params = extractString(command, command.find_first_of('(') + 1, command.find_last_of(')'));
            int length = this->getCommand().find_last_of(')') - this->getCommand().find_first_of('(') - 1;
            params = this->getCommand().substr(this->getCommand().find_first_of('(') + 1, length);
            //remove spaces from column list
            regexStatements::removeSpaces(params, "");
            if (params.find(' ') != string::npos || length <= 0) {
                err = "Invalid parameter list";
                return 0;
            }
        }

        int selectTableLocation = this->getCommand().find("FROM") + FROM_SIZE; //location of the table name in the select statement
        cout << endl;
        string tableName = "";
        //save table name in a variable
        for (int i = selectTableLocation+1; i < this->getCommand().length(); i++) {
            if (this->getCommand()[i] == ' ')
                break;
            tableName.push_back(this->getCommand()[i]);
        }

        //searches for a filter after the WHERE clause
        //eg WHERE id = 2 string filter  = "id = 2"
        //bool condition(char operator, int val1, int val2)
        //bool condition(char operator, float val1, float val2)
        //maybe
        // for int i = 0 ; i < table.numberOfRecords ; i++
        // for int j = 0; j  < table.numberOfColumns; j++
        // record[i][j]
        // if tyep = intgerte, float, taext
        // if(condition(operator, val1, val2 = record[i][j])
        int filterLocation;
        string filter = "";
        if (this->getCommand().find("WHERE") != string::npos) {
            filterLocation = this->getCommand().find("WHERE") + WHERE_SIZE; //location of filter statement in the command
            filter = extractString(this->getCommand(), filterLocation + 1, this->getCommand().length()); //extract filter
            if (countChars(filter, ' ') > 2 || countChars(filter, ',')) { //check if filter is valid
                err = "Invalid filter";
                return 0;
            }
        }
            
        if (filter != "")
            hasFilter = true;
        //display extracted parameters
        cout << "Table: " << tableName << endl;
        if (params != "all")
            cout << "Columns: " << countChars(params, ',') + 1 << endl;
        cout << "Columns: " << params << endl;
        cout << "Filter: " << hasFilter << endl;
        if (hasFilter) {
            cout << "Filter column: " << filter << endl;
            whereDecoder(filter, filterColumn, filterOperator, filterValue);
        }
            
        //string* values = extractParameters(params, 0, params.length());

        //if (!values) {
        //    err = "Invalid parameter list";
        //    //delete[] values;
        //    return 0;
        //}
        //delete[] values;
        BinaryFile tableFile(tableName + ".tab");
        //BinaryFile dataFile(entityName + ".data");

        //checking if the table exists
        if (tableFile.exists()) {
            Table usedTable(tableName);
            //extracting column names
            string* columns = extractParameters(params, 0, params.length());
            string* tableColumns = usedTable.getColumnNames();
            int noColumns = countChars(params, ',') + 1;
            //positions if Where clause valid records
            int* validRec = nullptr;
            int size = 0;
            //creating report file
            TextFile reportFile("SELECT_" + to_string(++selectCounter) + ".txt");
            //checking filter column
            if (hasFilter) {
                bool ok = 0;
                for (int i = 0; i < usedTable.getNumColumns(); i++)
                    if (filterColumn == tableColumns[i])
                        ok = 1;

                if (ok == 0) {
                    delete[] columns;
                    delete[] tableColumns;
                    throw exception("Filter column does not exist");
                }
                else {
                    //check valid records return their position
                    validRec = usedTable.validRecords(filterColumn, filterOperator, filterValue, size);
                    if (size == 0) {
                        ofstream fout = reportFile.openToWrite();
                        cout << "No records to show" << endl;
                        fout << "No records to show" << endl;
                        fout.close();
                        cout << "Report file " << reportFile.getFileName() << " created\n";
                        delete[] columns;
                        delete[] tableColumns;
                        delete[]validRec;
                        return 1;
                    }
                }

            }
            //displaying for the "all" case
            if (params == "all")
            {
                ofstream fout = reportFile.openToWrite();
                if (!hasFilter) {
                    //no WHERE clause
                    usedTable.selectAll(fout);
                }
                else {
                    //with WHERE clause
                    usedTable.selectAll(validRec, size, fout);
                }
                fout.close();
            }
            else
            {
                
                bool ok = 0;
                for (int i = 0; i < noColumns; i++)
                {
                    ok = 0;
                    for(int j=0; j< usedTable.getNumColumns(); j++)
                    if (columns[i] == tableColumns[j])
                    {
                        ok = 1;
                    }
                    if (ok == 0)
                    {
                        delete[] columns;
                        delete[] tableColumns;
                        throw exception("Column does not exist");
                        
                    }
                }
                //all columns exist, proceed to print them
                ofstream fout = reportFile.openToWrite();
                if (!hasFilter) {
                    usedTable.selectSome(columns, noColumns, fout);
                }
                else
                    usedTable.selectSome(columns, noColumns, validRec, size, fout);
                fout.close();
               
               
            }
            cout << "Report file " << reportFile.getFileName() << " created\n";
            //deallocate memory
            delete[] columns;
            delete[] tableColumns;
            delete[] validRec;
        }
        else {
            err = "Table does not exist";
            return 0;
        }
      
        return 1;
    }

    int insertParser(string& err) {
       
        //length of the parameters substring from an insert command
        int length = this->getCommand().find_last_of(')') - this->getCommand().find_first_of('(') - 1;
        //the substring between the to paranthesies from an insert statement
        string params = this->getCommand().substr(this->getCommand().find_first_of('(')+1, length);
        //extract table name
        string tableName;
        for (int i = INSERT_TABLE_LOCATION; this->getCommand()[i] != ' '; i++)
            tableName.push_back(this->getCommand()[i]);

        //get an array of strings from parameters
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
        delete[] values;
        //writing to files
        BinaryFile tableFile(tableName+".tab");
        //check if table structure exists
        if (!tableFile.exists()) {
            err = "Table does not exist";
            return 0;
        }
        else {
            //read table structure
            Table table(tableName);
            //check record if it works with table structure
            if (!table.checkRecord(r1, err))
                return 0;
            else {
                //removing apostrophes
                r1.removeApostrophes();
                //write record to file
                // .data file stores records
                BinaryFile dataFile(tableName + ".data");
                //increment number of records
                table.incrementNumRecords();
                //if exists, append to file, else create file
                if (!dataFile.exists()) {
                    ofstream fout = dataFile.openToWrite();
                    r1.writeRecord(fout);
                    fout.close();
                }
                else {
                    ofstream fout = dataFile.openToAppend();
                    r1.writeRecord(fout);
                    fout.close();
                }
                //rewrite table structure
                table.writeToBFile(tableFile);
            }
        }
        return 1;
    }

    int updateParser(string & err) {
            
        //extracting table name, the SET clause and the WHERE caluse
        string tableName = extractString(this->getCommand(), this->getCommand().find("UPDATE") + UPDATE_SIZE + 1, this->getCommand().find("SET") - 1);
        string setValue = extractString(this->getCommand(), this->getCommand().find("SET") +SET_SIZE+ 1, this->getCommand().find("WHERE") - 1);
        string filter = extractString(this->getCommand(), this->getCommand().find("WHERE") + WHERE_SIZE + 1, this->getCommand().length());
        // set clause variables
        string columnName, value;
        //where clause variables
        string filterCol, filterOperator, filterValue;

        //checks for good SET clause
        if (countChars(setValue, ',')) {
            err = "Invalid SET clause";
            return 0;
        }
        else {
            //get column and value that get set
            //regexStatements::removeSpaces(setValue, "");
            columnName = extractString(setValue, 0, setValue.find('='));
            regexStatements::removeSpaces(columnName, "");
            value = extractString(setValue, setValue.find('=') + 1, setValue.length());
            value = regexStatements::trimString(value);
            //format value
        }

        //checks for good WHERE clause
        if (countChars(filter, ',')) {
            err = "Invalid WHERE clause";
            return 0;
        }
        else {
            //extract column and values
            whereDecoder(filter, filterCol, filterOperator, filterValue);
        }

        //prints to screen
        cout << "Table: " << tableName << endl;
        cout << "Column to change: " << columnName << endl;
        cout << "Value: " << value << endl;
        cout << "Condition: " << filter << endl;

        BinaryFile tableFile(tableName + ".tab");
        if (!tableFile.exists())
            throw exception("Table does not exist");
        else {
            Table table(tableName);
            int colPosition = 0;
            int* validRec = nullptr;
            int size = 0;
            string* tableColumns = table.getColumnNames();
            bool ok = false;
            for (int i = 0; i < table.getNumColumns(); i++)
                if (columnName == tableColumns[i])
                {
                    ok = true;
                    colPosition = i;
                }
            if (ok) {
                if (table.checkValue(columnName, value))
                {
                  ok = false;
                    for (int i = 0; i < table.getNumColumns(); i++)
                        if (columnName == tableColumns[i])
                            ok = 1;

                    if (ok == 0) {
                        
                        delete[] tableColumns;
                        throw exception("Filter column does not exist");
                    }
                    else {
                        //check valid records return their position
                        validRec = table.validRecords(filterCol, filterOperator, filterValue, size);
                        if (size == 0) {
                            cout << "No records to update" << endl;
                            delete[] tableColumns;
                            delete[]validRec;
                            return 1;
                        }
                        else {
                            table.updateTable(value, colPosition, validRec, size);
                            table.rewriteData();
                        }
                    }
                }
            }
            else throw exception("Set column does not exist");

        }


        return 1;
    }

    int deleteParser(string& err) {
      
        //extract table name
        string tableName = extractString(this->getCommand(), this->getCommand().find("FROM")+ FROM_SIZE + 1, this->getCommand().find("WHERE") - 1);

        int filterLocation;
        string filter = "";
        string filterColumn, filterOperator, filterValue;
        //filter check
        filterLocation = this->getCommand().find("WHERE") + WHERE_SIZE;
        filter = extractString(this->getCommand(), filterLocation + 1, this->getCommand().length());
        if (countChars(filter, ',')) {
            err = "Invalid filter";
            return 0;
        }
        else {
            whereDecoder(filter, filterColumn, filterOperator, filterValue);
        }
        //display to screen
        cout << "Table: " << tableName << endl;
        cout << "Filter column: " << filter << endl;

        BinaryFile tableFile(tableName + ".tab");
        if (!tableFile.exists())
            throw exception("Table does not exist");
        else {
            Table table(tableName);
            int* validRec = nullptr;
            int size = 0;
            string* tableColumns = table.getColumnNames();
            bool ok = false;
            for (int i = 0; i < table.getNumColumns(); i++)
                if (filterColumn == tableColumns[i])
                    ok = 1;

            if (ok == 0) {

                delete[] tableColumns;
                throw exception("Filter column does not exist");
            }
            else {
                //check valid records return their position
                validRec = table.validRecords(filterColumn, filterOperator, filterValue, size);
                if (size == 0) {
                    cout << "No records to delete" << endl;
                    delete[] tableColumns;
                    delete[]validRec;
                    return 1;
                }
                else
                {
                    //deletes first record at position 0, then reassigns position array with new positions and continues until no more
                    while (size != 0)
                    {
                        table.deleteRecord(validRec[0]);
                        delete[]validRec;
                        validRec = table.validRecords(filterColumn, filterOperator, filterValue, size);
                    }
                    //rerwite table structure and data
                    table.writeToBFile(tableFile);
                    table.rewriteData();
                }
            }
            delete[] tableColumns;
            delete[]validRec;
        }

        return 1;
    }

    int createIndexParser(string& err) {
        //extract index name, table name and column name
        string indexName = "";
        string tableName = extractString(this->getCommand(), this->getCommand().find("ON")+ON_SIZE+1, this->getCommand().find('(')-1);
        string columnName = extractString(this->getCommand(), this->getCommand().find('(')+1, this->getCommand().find(')'));

        //taking into account the IF NOT EXISTS clause when extracting index name
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
        //extract table name and columns
        string tableName;
        string columns = extractString(this->getCommand(), this->getCommand().find_first_of('(')+1, this->getCommand().find_last_of(')'));

        //cout << countChars(columns, '(') << endl;
        //taking into account IF NOT EXISTS clause
        if (this->getCommand().find("IF NOT EXISTS") != string::npos)
            tableName = extractString(this->getCommand(), this->getCommand().find("TABLE") + TABLE_SIZE + 1, this->getCommand().find("IF") - 1);
        else
            tableName = extractString(this->getCommand(), this->getCommand().find("TABLE") + TABLE_SIZE + 1, this->getCommand().find_first_of('(') - 1);

        BinaryFile tableFile(tableName + ".tab");
        


        //remove all spaces from the column substring and check validity
        regexStatements::removeSpaces(columns, "");
        cout << columns << endl;
        if ((countChars(columns, '(') != countChars(columns, ')'))||columns[0]!='(') {
            err = "Invalid column list";
            return 0;
        }

        //remove all paranthesis, get a string with only column parameters and commas, each column gets 3 parameters: name, type, default value
        //eg: if the statement has 2 columns, then the string obtained in params is going to have 6 word, separated by commas, no spaces
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

        //check existance of table
        //if there is an IF NOT EXISTS clause, stop
        //else delete the existent table and record structure
        if (this->getCommand().find("IF NOT EXISTS") != string::npos && tableFile.exists()) {
            err = "Table already exists";
            return 0;
        }
        else {
            tableFile.deleteFile();
            BinaryFile dataFile(tableName + ".data");
            dataFile.deleteFile();
        }


        //take care of last paranthesis
        params.pop_back();
        //make an array from the string and a column array
        int paramNumber = COLUMN_ATTRIBUTES * countChars(columns, '(');
        int colNumber = paramNumber / 4; //each column has 4 parameters associated
        string* values = extractParameters(params, 0, params.length());
        Column* columnArray = new Column[colNumber];
        //cout << params << endl;
        //display all the information
        cout << "Table: " << tableName << endl;
        k = 1;
        for (int i = 0; i < paramNumber; i+=4) {
            cout<< endl << "Column " << k << endl;
            cout << "Table name: " << values[i] << endl;
            cout << "Type: " << values[i+1] << endl;
            cout << "Size: " << values[i+2] << endl;
            cout << "Default values: " << values[i+3] << endl;
            //as we only read strings, we need toInt to make the length string into a number
            Column c(values[i], values[i + 1], regexStatements::toInt(values[i+2]), values[i + 3]);
            columnArray[k - 1] = c;
            k++;
        }
        
        Table table(tableName, colNumber, columnArray);
        table.displayTableInfo();
        //write to file
        table.writeToBFile(tableFile);

        //cout << tableName << endl;
        //cout << columns << endl;
        //avoid memory leaks

        //execute command, create table file
        delete[] values;
        delete[] columnArray;
        return 1;
    }

    int importParser(string& err) {
        //IMPORT table file.csv
        //extract table and file name
        string tableName = extractString(this->getCommand(), this->getCommand().find_first_of(' ')+1, this->getCommand().find_last_of(' '));
        string fileName = extractString(this->getCommand(), this->getCommand().find_last_of(' ') + 1, this->getCommand().length());
        cout << "Table: " << tableName << endl;
        cout << "File: " << fileName << endl;
        //create object with filename
        BinaryFile tableFile(tableName + ".tab");
        CSVFile file(fileName);

        if (!tableFile.exists()) {
            err = "Table does not exist";
            return 0;
        }
        else {
            //read table structure from file
            Table table(tableName);
            if (!file.exists()) {
                err = "File does not exist";
                return 0;
            }
            else {
                int k = 0;
                ifstream fin = file.openToRead();
                string val;
                //first pass through file: count the number of values in file, separated by comma
                while (getline(fin, val, ',')) {
                    k++;
                }
                fin.close();

                //find number of records in the file
                float numRecords = (float)k / table.getNumColumns();
                cout << "Number of records in file is " << (int)numRecords  << endl;

                if (numRecords != (int)numRecords) {
                    err = "Invalid number of values in file or invalid separator (expected comma)";
                    return 0;
                }

                //second pass: check data integrity
                fin = file.openToRead();
                //dynamic array with the number of values in a record
                string* values;

                //read values from file, one by one and check integrity
                for (int i = 0; i < numRecords; i++) {
                    values = new string[table.getNumColumns()];
                    for (int j = 0; j < table.getNumColumns(); j++) {
                        getline(fin, values[j], ',');
                    }
                    Record r(values, table.getNumColumns());
                    delete[] values;
                    if (!table.checkRecord(r, err)) {
                        fin.close();
                        return 0;
                    }
                }
                fin.close();

                //third pass: saving values in database
                BinaryFile dataFile(tableName + ".data");
                fin = file.openToRead();

                for (int i = 0; i < numRecords; i++) {
                    values = new string[table.getNumColumns()];
                    for (int j = 0; j < table.getNumColumns(); j++) {
                        getline(fin, values[j], ',');
                    }
                    Record r(values, table.getNumColumns());
                    delete[] values;
                    //format data for writing to file
                    r.removeApostrophes();
                    table.incrementNumRecords();
                    //if exists, append to file, else create file
                    if (!dataFile.exists()) {
                        ofstream fout = dataFile.openToWrite();
                        r.writeRecord(fout);
                        fout.close();
                    }
                    else {
                        ofstream fout = dataFile.openToAppend();
                        r.writeRecord(fout);
                        fout.close();
                    }
                    //rewrite table structure
                    table.writeToBFile(tableFile);
                }
                fin.close();
            }
        }
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