#pragma once
#include <iostream>
#include <string>
#include "Regex.h"
#include "commandParser.h"
using namespace std;
//a static class with functions for checking a SQL statement
class Parser {
public:
	//function for recognizing commands at a superficial level
	//TO BE REMADE LATER
	static int superficialParser(string statement)
	{
		string err = "";
		try {
				int i;
				string commandType = "";

				for (i = 0; (statement[i] != ' ') && (i < statement.length()); i++)
					commandType.push_back(statement[i]);

				//command parser instantiation
				commandParser cp(statement, commandType);

				//function calls for each type of command go here
				if (i == statement.length()) {
					err = "Invalid command type";
					throw(err);
				}
				else if (cp.getCommandType() == "CREATE") {
					//create parser
					cout << "this is a create command\n";
					if (regexStatements::checkRegex(statement, regexStatements::getCreateIndexStatement())) {
						if (!cp.createIndexParser(err))
							throw(err);
					}
					else if (regexStatements::checkRegex(statement, regexStatements::getCreateTableStatement())) {
						if (!cp.createTableParser(err))
							throw(err);
					}
					else {
						err = "Create command not properly formatted\nCREATE TABLE table_name [IF NOT EXISTS] ((col_name,col_type,col_size,col_default),...)\nCREATE INDEX [IF NOT EXISTS] index_name ON table_name (column_name)";
						throw(err);
					}
				}
				else if (cp.getCommandType() == "DROP") {
					//drop parser
					cout << "this is a drop command\n";
					if (regexStatements::checkRegex(statement, regexStatements::getDropStatement())) {
						if (!cp.dropParser(err))
							throw(err);
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
					if (regexStatements::checkRegex(statement, regexStatements::getUpdateStatement())) {
						if (!cp.updateParser(err))
							throw(err);
					}
					else {
						err = "Update command not properly formatted\nUPDATE table_name SET column_name = value WHERE condition";
						throw(err);
					}
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
		catch (exception e) {
			cout << e.what() << endl;
			return 0;
		}
		/*catch (...) {

		}*/
		return 1;
	}
};