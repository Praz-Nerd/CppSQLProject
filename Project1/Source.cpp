#include "statementInput.h"

int main() {
	Interpreter interpreter;
	string s;
	while (s != "0")
	{
		cout << "\nCommand (or 0 to exit)> ";
		getline(cin, s);
		interpreter.setStatement(s);
		if(interpreter.commandParser())
			cout << interpreter.getStatement() << endl;
	}
	cout << "\nProgram exited...";
	return 0;
}