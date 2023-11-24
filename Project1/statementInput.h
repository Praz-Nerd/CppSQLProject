#pragma once
#include <iostream>
#include <string>
#include "Regex.h";
using namespace std;

//a class which retains one SQL statement at a time
class Interpreter {
	//supported commands: CREATE/DROP TABLE/INDEX, DISPLAY TABLE
	//INSERT, SELECT, UPDATE, DELETE
	//string statement = "";
	string* statement = nullptr;
	const int statementNumber = 0;
	//string err = "";
public:

	//consttructors
	Interpreter(string statement) {
		setStatement(statement);
	}

	Interpreter(string statement, int& statementNumber) : statementNumber(statementNumber) {
		setStatement(statement);
	}

	Interpreter(const Interpreter& i) : statementNumber(i.statementNumber+1) {
		if (i.statement) {
			this->setStatement(*(i.statement));
		}
		else {
			this->statement = nullptr;
		}
	}

	Interpreter& operator=(const Interpreter& i) {
		if (this != &i) {
			if (this->statement)
				delete this->statement;
			
			if (i.statement) {
				this->statement = new string(*(i.statement));
			}
			else
				this->statement = nullptr;
		}
	}

	void setStatement(string& s) {
			if (!s.empty()) {

				regexStatements::removeSpaces(s);
				statement = new string(s);
			}
			else {
				string err = "Statement does not exist";
				throw (err);
			}
	}

	string getStatement() {
		if (statement)
			return *statement;
		else
			return "";
	}

	int getStatementNumber() {
		return this->statementNumber;
	}

	~Interpreter() {
		if (statement)
			delete statement;
		statement = nullptr;
	}

	bool compareStatements(Interpreter& i) {
		if ((*statement) == i.getStatement())
			return true;
		return false;
	}

	void displayStatement() {
		cout << endl << "Statement no." << this->statementNumber << " : " << getStatement() << endl;
	}

	explicit operator int() {
		return this->statementNumber;
	}

	char operator[](int index) {
		if (statement != nullptr && index >= 0 && index < statement->length()) {
			return (*statement)[index];
		}
	}

	bool operator==(Interpreter i) {
		return (*statement) == (*(i.statement));
	}

	bool operator<(int i) {
		return (this->statementNumber < i);
	}

	bool operator!() {
		if (!statement)
			return true;
		return false;
	}

	int operator+(int x) {
		return this->statementNumber + x;
	}

	Interpreter& operator++() {
		(*statement)[statement->length() - 1]++;
		return *this;
	}

	Interpreter operator++(int i) {
		Interpreter copy = *this;
		(*statement)[statement->length() - 1]++;
		return copy;
	}

	friend ostream& operator<<(ostream&, Interpreter);
	friend istream& operator>>(istream&, Interpreter&);
};

ostream& operator<<(ostream& out, Interpreter i) {
	out << endl << "Statement no." << i.statementNumber << " : " << i.getStatement() << endl;
	return out;
}

istream& operator>>(istream& in, Interpreter& i) {
	string s;
	cout << endl << "Change statement: ";
	in >> s;
	i.setStatement(s);
	return in;
}
