#pragma once
#include <iostream>
#include <string>
using namespace std;
//a static class with functions for checking and extracting data from a SQL statement
class Parser {
public:
	static int commandParser(string statement)
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
				if (i == commandType.length()) {
					err = "Invalid command type";
					throw(err);
				}
				else if (commandType == "CREATE") {
					//create parser
				}
				else if (commandType == "DROP") {
					//drop parser
				}
				else if (commandType == "DISPLAY") {
					//display parser
				}
				else if (commandType == "INSERT") {
					//insert parser
				}
				else if (commandType == "SELECT") {
					//select parser
				}
				else if (commandType == "UPDATE") {
					//update parser
				}
				else if (commandType == "DELETE") {
					//delete parser
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