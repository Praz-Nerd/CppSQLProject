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
string regexStatements::displayStatement = "(DISPLAY) (TABLE) [_a-zA-Z]+";
string regexStatements::dropStatement = "(DROP) (?:TABLE|INDEX) [_a-zA-Z]+";
string regexStatements::insertStatement = "(INSERT) (INTO) [_a-zA-Z]+ (VALUES)\\s*\\((.*?)\\)";
string regexStatements::selectStatement = "(SELECT)\\s*(?:\\((.*?)\\)|ALL) (FROM) [_a-zA-Z]+\\s*(?:WHERE (.*?)|)";
string regexStatements::deleteStatement = "(DELETE) (FROM) [_a-zA-Z]+ (WHERE) (.*?)";
string regexStatements::updateStatement = "(UPDATE) [_a-zA-Z]+ (SET) [_a-zA-Z]+\\s*=\\s*(.*?) (WHERE) (.*?)";
string regexStatements::createTableStatement = "(CREATE) (TABLE) [_a-zA-Z]+\\s*(?:IF NOT EXISTS|)\\s*\\((.*?)\\)";
string regexStatements::createIndexStatement = "(CREATE) (INDEX)\\s*(?:IF NOT EXISTS|)\\s*[_a-zA-Z]+ (ON) [_a-zA-Z]+\\s*\\([_a-zA-Z]+\\)";
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
    

    string s;
    
    while (s != "0")
    {
        cout << "\nCommand (or 0 to exit)> ";
        getline(cin, s);
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