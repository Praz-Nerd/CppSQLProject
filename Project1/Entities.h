#pragma once
#include <iostream>
#include <string>
using namespace std;

//a class which represents a column definition from a table (data is stored using Records)
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
	void setNumValues(int n) {
		try {
			string err;
			if (n < 0) {
				err = "Invalid number of values";
				throw(err);
			}
			else {
				this->numValues = n;
			}
		}
		catch (string err) {
			cout << endl << err << endl;
		}
	}
	int getNumValues() {
		return this->numValues;
	}

	void setValues(string* values) {
		try {
			string err;
			if (values == nullptr) {
				err = "Invalid value array";
				throw(err);
			}
			else {
				if (this->values)
					delete[] values;
				this->values = new string[numValues];
				for (int i = 0; i < numValues; i++)
					this->values[i] = values[i];
			}
		}
		catch (string err) {
			cout << endl << err << endl;
		}
	}
	string* getValues() {
		string* copy = nullptr;
		if (this->values == nullptr)
			return copy;
		else {
			copy = new string[numValues];
			for (int i = 0; i < numValues; i++)
				copy[i] = this->values[i];
		}
		return copy;
	}

	Record(int numValues) {
		setNumValues(numValues);
		this->values = nullptr;
	}

	Record(string* values, int numValues) {
		setNumValues(numValues);
		setValues(values);
		/*this->numValues = numValues;
		if (values) {
			this->values = new string[numValues];
			for (int i = 0; i < numValues; i++)
				this->values[i] = values[i];
		}
		else
			this->values = nullptr;*/
	}
	//copy constructor
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
	//destructor
	~Record() {
		if (this->values)
			delete[] this->values;
		this->values = nullptr;
	}
	//function for displaying a record
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


