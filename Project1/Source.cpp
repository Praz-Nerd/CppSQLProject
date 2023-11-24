#include <iostream>
#include <string>
#include "statementInput.h"
#include "Parser.h"
#include "Regex.h"
using namespace std;

//define regex patterns here
string regexStatements::displayStatement = "(DISPLAY) (TABLE) [_a-zA-Z]+";
string regexStatements::dropStatement = "(DROP) (?:TABLE|INDEX) [_a-zA-Z]+";
string regexStatements::insertStatement = "(INSERT) (INTO) [_a-zA-Z]+ (VALUES)\\s*\\((.*?)\\)";
string regexStatements::selectStatement = "(SELECT)\\s*(?:\\((.*?)\\)|ALL) (FROM) [_a-zA-Z]+\\s*(?:WHERE (.*?)|)";
string regexStatements::deleteStatement = "(DELETE) (FROM) [_a-zA-Z]+ (WHERE) (.*?)";
string regexStatements::updateStatement = "(UPDATE) [_a-zA-Z]+ (SET) (.*?) (WHERE) (.*?)";
string regexStatements::createTableStatement = "(CREATE) (TABLE) [_a-zA-Z]+ (?:IF NOT EXISTS|)\\s*\\((.*?)\\)";
string regexStatements::createIndexStatement = "(CREATE) (INDEX) [_a-zA-Z]+ (ON) [_a-zA-Z]+\\s*\\([_a-zA-Z]+\\)";

int main() {
    string s;
    int k = 0;
    while (s != "0")
    {
        cout << "\nCommand (or 0 to exit)> ";
        getline(cin, s);
        Interpreter interpreter(s, ++k);
        interpreter.displayStatement();
        //interpreter.setStatement(s);
        if (Parser::commandParser(interpreter.getStatement())) {
            cout << "Command ran successfully" << endl;
            //cout << interpreter.getStatement() << endl;
        }
        else
            cout << "Command failed" << endl;
    }
    cout << "\nProgram exited...";
    return 0;
}