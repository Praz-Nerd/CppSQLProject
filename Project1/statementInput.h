#pragma once
#include <iostream>
#include <string>
using namespace std;

class Interpreter {
	//supported commands: CREATE/DROP TABLE/INDEX, DISPLAY TABLE
	//INSERT, SELECT, UPDATE, DELETE
	string statement = "";
public:

	void setStatement(string s) {
		this->statement = s;
	}
	string getStatement(){
		return this->statement;
	}

	int commandParser()
	{
		string err = "";
		try {
			if (statement[statement.length() - 1] != ';')
			{
				err = "Invalid command terminator";
				throw (err);
			}
			else
			{
				int i;
				string commandType = "";
				
				for (i = 0; (statement[i] != ' ') && (i < statement.length()); i++)
					commandType.push_back(statement[i]);

				//function calls for each type of command go here
				if (i == statement.length()){
					err = "Invalid command type";
					throw(err);
				}
				else if (statement == "CREATE") {

				}
				else if (statement == "DROP") {

				}
				else if (statement == "DISPLAY") {

				}
				else if (statement == "INSERT") {

				}
				else if (statement == "SELECT") {

				}
				else if (statement == "UPDATE") {

				}
				else if (statement == "DELETE") {

				}
				else {
					err = "Invalid command type";
					throw(err);
				}
				
			}
		}
		catch (string err) {
			cout << err << endl;
			return 0;
		}
		
		return 1;
	}
};
