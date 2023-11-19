#pragma once
#include <iostream>
#include <string>
#include "Regex.h";
using namespace std;

//a class which retains one SQL statement at a time
class Interpreter {
	//supported commands: CREATE/DROP TABLE/INDEX, DISPLAY TABLE
	//INSERT, SELECT, UPDATE, DELETE
	string statement = "";
	string err = "";
public:

	Interpreter(string statement) {
		setStatement(statement);
	}

	void setStatement(string s) {
		try {
			if (s.length()) {
				regexStatements::removeSpaces(s);
				this->statement = s;
			}
			else {
				err = "Statement does not exist";
				throw(err);
			}
		}
		catch (string err){
			cout << endl << err << endl;
		}
	}

	string getStatement(){
		return this->statement;
	}

};
