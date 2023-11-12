#pragma once
#include <string>
using namespace std;


//a class which represents a column from a table
class Column {
	string name;
	string type;//integer, text, numeric
	int dimension;
	string defaultValue;
};


//a class which represents a table
class Table {
	//table goes here
	string name;
	int numColumns;
	Column* columns;
	//to implement ctor, copy ctor, desctor
};


