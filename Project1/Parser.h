#pragma once
#include <iostream>
#include <string>
#include "Regex.h"
#include "commandParser.h"
using namespace std;
//a static class with functions for checking and extracting data from a SQL statement
class Parser {
public:
	static int commandParser(string statement)
	{
		string err = "";
		try {
				int i;
				string commandType = "";

				for (i = 0; (statement[i] != ' ') && (i < statement.length()); i++)
					commandType.push_back(statement[i]);

				//function calls for each type of command go here
				if (i == statement.length()) {
					err = "Invalid command type";
					throw(err);
				}
				else if (regexStatements::checkRegex(commandType, "(CREATE)")) {
					//create parser
					cout << "this is a create command\n";
				}
				else if (regexStatements::checkRegex(commandType, "(DROP)")) {
					//drop parser
					cout << "this is a drop command\n";
					if (regexStatements::checkRegex(statement, regexStatements::dropStatement)) {
						commandParser::dropParser(statement);
					}
					else {
						err = "Drop command not properly formatted\nDROP TABLE table_name";
						throw(err);
					}
				}
				else if (regexStatements::checkRegex(commandType, "(DISPLAY)")) {
					//display parser
					cout << "this is a display command\n";
					if (regexStatements::checkRegex(statement, regexStatements::displayStatement)) {
						commandParser::displayParser(statement);
					}
					else {
						err = "Display command not properly formatted\nDISPLAY TABLE table_name;";
						throw(err);
					}
				}
				else if (regexStatements::checkRegex(commandType, "(INSERT)")) {
					//insert parser
					cout << "this is an insert command\n";
					if (regexStatements::checkRegex(statement, regexStatements::insertStatement)) {
						if (!commandParser::insertParser(statement, err))
							throw(err);
					}
					else {
						err = "Insert command not properly formatted\nINSERT INTO table_name VALUES(...,...,...,...)";
						throw(err);
					}
				}
				else if (regexStatements::checkRegex(commandType, "(SELECT)")) {
					//select parser
					cout << "this is a select command\n";
				}
				else if (regexStatements::checkRegex(commandType, "(UPDATE)")) {
					//update parser
					cout << "this is an update command\n";
				}
				else if (regexStatements::checkRegex(commandType, "(DELETE)")) {
					//delete parser
					cout << "this is a delete command\n";
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