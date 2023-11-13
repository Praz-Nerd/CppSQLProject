#include <iostream>
#include <string>
#include "statementInput.h"
#include "Parser.h"
#include "Regex.h"
using namespace std;

//define regex patterns here
//DO NOT FORGET COLON AT THE END PLS THANK YOU
const string regexStatements::displayStatement = "(DISPLAY) (TABLE) [_a-z]+;";
const string regexStatements::dropStatement = "(DROP) (TABLE) [_a-z]+;";
const string regexStatements::insertStatement = "(INSERT) (INTO) [_a-z]+ (VALUES)\\((.*?)\\);";

int main() {
    Interpreter interpreter;
    string s;
    while (s != "0")
    {
        cout << "\nCommand (or 0 to exit)> ";
        getline(cin, s);
        interpreter.setStatement(s);
        if (Parser::commandParser(interpreter.getStatement()))
            cout << "Command ran successfully" << endl;
        else
            cout << "Command failed" << endl;
    }
    cout << "\nProgram exited...";
    return 0;
}