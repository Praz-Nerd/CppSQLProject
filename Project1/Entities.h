#pragma once
#include <string>
using namespace std;

//a class which represents a column from a table
class Column {
	string name;
	string type;//integer, text, numeric
	int dimension;
	string defaultValue;
public:
	Column(string name, string type, int dimension, string defaultValue) {
		this->name = name;
		this->type = type;
		this->dimension = dimension;
		this->defaultValue = defaultValue;
	}
};

//a class which represents a line from a table
class Record {
	string* values;
};


//a class which represents a table
class Table {
	//table goes here
	string name;
	int numColumns;
	int numRecords;
	Column* columns;
	Record* records;
	//to implement ctor, copy ctor, desctor
};


