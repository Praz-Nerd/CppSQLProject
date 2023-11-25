#pragma once
#include <iostream>
#include <string>
#include "Regex.h"
#include "commandParser.h"
using namespace std;
//a static class with functions for checking and extracting data from a SQL statement
class Parser {
public:
	//function for recognizing commands at a superficial level
	static int superficialParser(string statement)
	{
		string err = "";
		try {
				int i;
				string commandType = "";

				for (i = 0; (statement[i] != ' ') && (i < statement.length()); i++)
					commandType.push_back(statement[i]);
				commandParser cp(statement, commandType);

				//function calls for each type of command go here
				if (i == statement.length()) {
					err = "Invalid command type";
					throw(err);
				}
				else if (cp.getCommandType() == "CREATE") {
					//create parser
					cout << "this is a create command\n";
				}
				else if (cp.getCommandType() == "DROP") {
					//drop parser
					cout << "this is a drop command\n";
					if (regexStatements::checkRegex(statement, regexStatements::getDropStatement())) {
						cp.dropParser();
					}
					else {
						err = "Drop command not properly formatted\nDROP [TABLE|INDEX] entity_name";
						throw(err);
					}
				}
				else if (cp.getCommandType() == "DISPLAY") {
					//display parser
					cout << "this is a display command\n";
					if (regexStatements::checkRegex(statement, regexStatements::getDisplayStatement())) {
						cp.displayParser();
					}
					else {
						err = "Display command not properly formatted\nDISPLAY TABLE table_name;";
						throw(err);
					}
				}
				else if (cp.getCommandType() == "INSERT") {
					//insert parser
					cout << "this is an insert command\n";
					if (regexStatements::checkRegex(statement, regexStatements::getInsertStatement())) {
						if (!cp.insertParser(err))
							throw(err);
					}
					else {
						err = "Insert command not properly formatted\nINSERT INTO table_name VALUES(...,...,...,...)";
						throw(err);
					}
				}
				else if (cp.getCommandType() == "SELECT") {
					//select parser
					cout << "this is a select command\n";
					if (regexStatements::checkRegex(statement, regexStatements::getSelectStatement()))
					{
						if (!cp.selectParser(err))
							throw(err);
					}
					else
					{
						err = "Select command not properly formatted\nSELECT (...,...) FROM table_name [WHERE]";
						throw(err);
					}


				}
				else if (cp.getCommandType() == "UPDATE") {
					//update parser
					cout << "this is an update command\n";
				}
				else if (cp.getCommandType() == "DELETE") {
					//delete parser
					cout << "this is a delete command\n";
					if (regexStatements::checkRegex(statement, regexStatements::getDeleteStatement())) {
						if (!cp.deleteParser(err))
							throw(err);
					}
					else {
						err = "Delete command not properly formatted\nDELETE FROM table_name WHERE condition";
						throw(err);
					}
				}
				else {
					err = "Invalid command type";
					throw(err);
				}
		}
		catch (string err) {
			cout << err << endl;
			return 0;
		}

		return 1;
	}
};