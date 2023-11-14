#pragma once
#include <iostream>
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
	int numValues;
public:
	Record(string* values, int numValues) {
		this->numValues = numValues;
		if (values) {
			this->values = new string[numValues];
			for (int i = 0; i < numValues; i++)
				this->values[i] = values[i];
		}
		else
			this->values = nullptr;
	}
	Record(const Record& r) {
		this->numValues = r.numValues;
		if (r.values) {
			this->values = new string[r.numValues];
			for (int i = 0; i < r.numValues; i++)
				this->values[i] = r.values[i];
		}
		else
			this->values = nullptr;
	}
	~Record() {
		if (this->values)
			delete[] this->values;
		this->values = nullptr;
	}
	void displayRecord() {
		for (int i = 0; i < numValues; i++) {
			cout << endl << "Value on column " << i + 1 << " : " << values[i];
		}
		cout << endl;
	}
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


