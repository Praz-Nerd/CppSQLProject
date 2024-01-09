#include <iostream>
#include <string>
#include <fstream>
#include "commandParser.h"
#include "statementInput.h"
#include "Parser.h"
#include "Regex.h"
#include "Files.h"
using namespace std;

//define regex patterns here
string regexStatements::displayStatement = "(DISPLAY) (TABLE) [_a-zA-Z]+\\s*";
string regexStatements::dropStatement = "(DROP) (?:TABLE|INDEX) [_a-zA-Z]+\\s*";
string regexStatements::insertStatement = "(INSERT) (INTO) [_a-zA-Z]+ (VALUES)\\s*\\((.*?)\\)\\s*";
string regexStatements::selectStatement = "(SELECT)\\s*(?:\\((.*?)\\)|ALL) (FROM) [_a-zA-Z]+\\s*(?:WHERE (.*?)|)\\s*";
string regexStatements::deleteStatement = "(DELETE) (FROM) [_a-zA-Z]+ (WHERE) (.*?)\\s*";
string regexStatements::updateStatement = "(UPDATE) [_a-zA-Z]+ (SET) [_a-zA-Z]+\\s*=\\s*(.*?) (WHERE) (.*?)\\s*";
string regexStatements::createTableStatement = "(CREATE) (TABLE) [_a-zA-Z]+\\s*(?:IF NOT EXISTS|)\\s*\\((.*?)\\)\\s*";
string regexStatements::createIndexStatement = "(CREATE) (INDEX)\\s*(?:IF NOT EXISTS|)\\s*[_a-zA-Z]+ (ON) [_a-zA-Z]+\\s*\\([_a-zA-Z]+\\)\\s*";
//IMPORT statement to be made
string regexStatements::importStatement = "(IMPORT) [_a-zA-Z]+ [_a-zA-Z]+(.csv)";
//keeping track of display and select statements to write to text files
int commandParser::displayCounter = 0;
int commandParser::selectCounter = 0;

int main(int argc, char* argv[]) {
    int k = 0;
    //reading data from parameters (text files)
    try {

        if (argc > 6)
            cout << "Too many given arguments\n";
        else {
            for (int i = 1; i < argc; i++) {
                TextFile argFile(argv[i]);
                //ifstream fout(argFile.getFileName());
                ifstream fin = argFile.openToRead();
                if (!fin) {
                    cout << "File " << argFile.getFileName() << " does not exist\n";
                }
                else {
                    cout << "From file " << argFile.getFileName() << ":\n";
                    string s;
                    while (getline(fin, s)) {
                 
                        Interpreter interpreter(s, ++k);
                        interpreter.displayStatement();
                        if (Parser::superficialParser(interpreter.getStatement())) {
                            cout << "Command ran successfully" << endl;
                        }
                        else
                            cout << "Command failed" << endl;
                    }
                    fin.close();
                }
            }
                
        }
    }
    catch (exception e) {
        cout << e.what() << endl;
    }

    //bere de la altex
   /* cout << "TESTING TO FLOAT!!!!!!!!!" << endl;
    string altex = "-2024.85";
    float x = commandParser::toFloat(altex);
    cout << x << endl;*/

    string s;
    cout << "---SQL Simulator---\n";
    cout << "Supported commands: CREATE/DROP TABLE/INDEX, DISPLAY TABLE, INSERT, SELECT, UPDATE, DELETE" << endl;
    cout << "*  Command formats: " << endl;
    cout << "\tCREATE TABLE table_name [IF NOT EXISTS] ((col_name,col_type,col_size,col_default),...)\n\tCREATE INDEX [IF NOT EXISTS] index_name ON table_name (column_name)" << endl;
    cout << "\tDROP [TABLE|INDEX] entity_name" << endl;
    cout << "\tDISPLAY TABLE table_name" << endl;
    cout << "\tINSERT INTO table_name VALUES(...,...,...,...)" << endl;
    cout << "\tSELECT (...,...)|ALL FROM table_name [WHERE] " << endl;
    cout << "\tUPDATE table_name SET column_name = value WHERE condition" << endl;
    cout << "\tDELETE FROM table_name WHERE condition" << endl;
    cout << "\tIMPORT tablename file.csv" << endl;
    cout << "Tip: All data to be inserted into table has to be written between ' ', including CSV files" << endl;
    cout << "Data in CSV files is separated by commas" << endl;

    while (s != "0")
    {
        cout << "\nCommand (or 0 to exit)> ";
        getline(cin, s);
        if (s[0]== '0')
            break;
        Interpreter interpreter(s, ++k);
        interpreter.displayStatement();
        //interpreter.setStatement(s);
        if (Parser::superficialParser(interpreter.getStatement())) {
            cout << "Command ran successfully" << endl;
            //cout << interpreter.getStatement() << endl;
        }
        else
            cout << "Command failed" << endl;
    }
    cout << "\nProgram exited...";
    return 0;
}