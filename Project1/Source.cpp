#include <iostream>
#include <string>
#include "statementInput.h"
#include "Parser.h"
using namespace std;

int main() {
	Interpreter interpreter;
	string s;
	while (s != "0")
	{
		cout << "\nCommand (or 0 to exit)> ";
		getline(cin, s);
		interpreter.setStatement(s);
		if(Parser::commandParser(interpreter.getStatement()))
			cout << interpreter.getStatement() << endl;
	}
	cout << "\nProgram exited...";
	return 0;
}