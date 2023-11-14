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
public:

	void setStatement(string s) {
		regexStatements::removeSpaces(s);
		this->statement = s;
	}
	string getStatement(){
		return this->statement;
	}

};
