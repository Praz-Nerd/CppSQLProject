#pragma once
#include <iostream>
#include <string>
using namespace std;

//a class which represents a column definition from a table (data is stored using Records)
class Column {
	string* values = nullptr; //name, type, default value
	//string name;
	//string type;//integer, text, numeric
	int dimension;
	//string defaultValue;
	const static int VECTOR_SIZE = 3;
public:
	string getName() {
		return this->values[0];
	}
	string getType() {
		return this->values[1];
	}
	string getDefaultValue() {
		return this->values[2];
	}
	int getDimension() {
		return this->dimension;
	}
	void setName(string s) {
		this->values[0] = s;
	}
	void setType(string s) {
		this->values[1] = s;
	}
	void setDefaultValue(string s) {
		this->values[2] = s;
	}
	void setDimension(int d) {
		this->dimension = d;
	}

	// Default constructor
	Column() : values(nullptr), dimension(0) {}


	Column(string name, string type, int dimension, string defaultValue) {
		//this->name = name;
		//this->type = type;
		//this->dimension = dimension;
		//this->defaultValue = defaultValue;
		//this->dimension = dimension;
		setDimension(dimension);
		this->values = new string[VECTOR_SIZE];
		setName(name);
		setType(type);
		setDefaultValue(defaultValue);
		//this->values[0] = name;
		//this->values[1] = type;
		//this->values[2] = defaultValue;
	}
	Column(string* values, int dimension) {
		this->dimension = dimension;
		if (values == nullptr)
			this->values = nullptr;
		else {
			this->values = new string[VECTOR_SIZE];
			for (int i = 0; i < VECTOR_SIZE; i++)
				this->values[i] = values[i];
		}
	}



	~Column() {
		if (values)
			delete[] values;
		values = nullptr;
	}
};

//a class which represents a line from a table
class Record {
	string* values;
	int numValues;
public:

	void setNumValues(int n) {
		this->numValues = n;
	}
	int getNumValues() {
		return this->numValues;
	}
	void setValues(string* values) {
		if (this->values)
			delete[] this->values;
		this->values = new string[numValues];
		for (int i = 0; i < numValues; i++)
			this->values[i] = values[i];
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
	//constructors

	// Default constructor
	Record() : values(nullptr), numValues(0) {}
	// Constructor with values and numValues
	Record(string* values, int numValues) : numValues(numValues) {
		setValues(values);
	}

	Record(int numValues) {
		setNumValues(numValues);
		this->values = nullptr;
	}

	/*Record(string* values, int numValues) {
		setNumValues(numValues);
		setValues(values);
	}*/

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

	void addValue(string s) {
		if (values) {
			string* copy = new string[numValues];
			for (int i = 0; i < numValues; i++)
				copy[i] = values[i];
			delete[] values;
			numValues++;
			values = new string[numValues];
			for (int i = 0; i < numValues - 1; i++)
				values[i] = copy[i];
			values[numValues - 1] = s;
			delete[] copy;
		}
		else {
			numValues = 1;
			values = new string[numValues];
			values[0] = s;
		}
	}
	//= overloading
	Record& operator=(const Record& r) {
		if (this != &r) {
			this->numValues = r.numValues;
			if (this->values != nullptr) {
				delete[] this->values;
			}
			if (r.values != nullptr) {
				this->values = new string[r.numValues];
				for (int i = 0; i < r.numValues; i++)
					this->values[i] = r.numValues;
			}
			else
				this->values = nullptr;
		}
		return *this;
	}
	//cast operator
	explicit operator int() {
		return this->numValues;
	}
	//index operator
	string operator[](int index) {
		if (values != nullptr && index >= 0 && index < numValues)
			return values[index];
	}
	//== operator
	bool operator==(Record r) {
		return (this->numValues == r.numValues);
	}
	//< operator
	bool operator<(int dimension) {
		return(this->numValues < dimension);
	}
	//! operator
	bool operator!() {
		if (!values)
			return true;
		return false;
	}
	//+ operator
	Record operator+(string s) {
		Record copy = *this;
		copy.addValue(s);

		return copy;
	}
	//++ (increment) operators
	Record& operator++() {
		this->addValue("");
		return *this;
	}
	Record operator++(int i) {
		Record copy = *this;
		this->addValue("");

		return copy;
	}
	//<< and >> operators
	friend ostream& operator<<(ostream&, Record);
	friend istream& operator>>(istream&, Record&);
};

ostream& operator<<(ostream& out, Record r) {
	for (int i = 0; i < r.numValues; i++) {
		out << endl << "Value on column " << i + 1 << " : " << r[i];
	}
	out << endl;
	return out;
}

istream& operator>>(istream& in, Record& r) {
	int i;
	cout << endl << "Change record";
	string* buffer = new string[r.numValues];
	for (i = 0; i < r.numValues; i++) {
		cout << "Value " << i + 1 << " : ";
		in >> buffer[i];
	}
	r.setValues(buffer);
	delete[] buffer;
	return in;
}

//a class which represents a table
class Table {
	//table goes here
	string name;
	int numColumns;
	int numRecords;
	Column* columns;
	Record* records;
	//to implement ctor, copy ctor, desctor, some operators
public:
	// Constructor
	Table(string name, int numColumns, int numRecords, Column* columns, Record* records) :
		name(name), numColumns(numColumns), numRecords(numRecords) {
		this->columns = new Column[numColumns];
		this->records = new Record[numRecords];

		// Copy columns
		for (int i = 0; i < numColumns; i++) {
			this->columns[i] = columns[i];
		}

		// Copy records
		for (int i = 0; i < numRecords; i++) {
			this->records[i] = records[i];
		}
	}

	// Copy Constructor
	Table(const Table& table) : name(table.name), numColumns(table.numColumns), numRecords(table.numRecords) {
		this->columns = new Column[numColumns];
		this->records = new Record[numRecords];

		// Copy columns
		for (int i = 0; i < numColumns; i++) {
			this->columns[i] = table.columns[i];
		}

		// Copy records
		for (int i = 0; i < numRecords; i++) {
			this->records[i] = table.records[i];
		}
	}

	// Destructor
	~Table() {
		delete[] columns;
		delete[] records;
	}

	// Assignment operator
	Table& operator=(const Table& table) {
		if (this != &table) {
			// Delete existing data
			delete[] columns;
			delete[] records;

			// Copy new data
			this->name = table.name;
			this->numColumns = table.numColumns;
			this->numRecords = table.numRecords;

			// Copy columns
			this->columns = new Column[numColumns];
			for (int i = 0; i < numColumns; i++) {
				this->columns[i] = table.columns[i];
			}

			// Copy records
			this->records = new Record[numRecords];
			for (int i = 0; i < numRecords; i++) {
				this->records[i] = table.records[i];
			}
		}
		return *this;
	}

};

//implement index class maybe

