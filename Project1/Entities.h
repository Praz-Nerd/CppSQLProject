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

public:
	// Accessor functions for reading and writing values
	string getName() const {
		return name;
	}

	int getNumColumns() const {
		return numColumns;
	}

	int getNumRecords() const {
		return numRecords;
	}

	// Generic methods for processing/displaying attributes
	void displayTableInfo() const {
		cout << "Table Name: " << getName() << ", Number of Columns: " << getNumColumns() << ", Number of Records: " << getNumRecords() << endl;
	}

	void performOperation() const {
		cout << "Performing an operation on the table" << endl;
	}
	//Default constructor
	Table() {
		this->numColumns = 0; 
		this->numRecords = 0;
		this->columns = nullptr;
		this->records = nullptr;
	} 

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
	// Indexing operator []
	Column& operator[](int index) {
		if (index >= 0 && index < numColumns) {
			return columns[index];
		}
		else {
			throw out_of_range("Column index out of range");
		}
	}

	// Mathematical operator +
	Table operator+(const Table& other) const {
		Table result;
		result.name = this->name + other.name;
		result.numColumns = this->numColumns + other.numColumns;
		result.numRecords = this->numRecords + other.numRecords;

		// Allocate memory for columns and records in the result table
		result.columns = new Column[result.numColumns];
		result.records = new Record[result.numRecords];

		// Copy columns and records from both tables to the result table
		for (int i = 0; i < this->numColumns; ++i) {
			result.columns[i] = this->columns[i];
		}

		for (int i = 0; i < other.numColumns; ++i) {
			result.columns[this->numColumns + i] = other.columns[i];
		}

		for (int i = 0; i < this->numRecords; ++i) {
			result.records[i] = this->records[i];
		}

		for (int i = 0; i < other.numRecords; ++i) {
			result.records[this->numRecords + i] = other.records[i];
		}

		return result;
	}

	// Increment operators ++
	Table& operator++() {
		++numColumns;
		++numRecords;
		return *this;
	}

	Table operator++(int) {
		Table copy = *this;
		++(*this);
		return copy;
	}

	// << operator
	friend ostream& operator<<(ostream& out, const Table& table) {
		out << "Table: " << table.getName() << ", Columns: " << table.getNumColumns() << ", Records: " << table.getNumRecords();
		return out;
	}

	// >> operator
	friend istream& operator>>(istream& in, Table& table) {
		cout << "Enter Table Name: ";
		in >> table.name;
		cout << "Enter Number of Columns: ";
		in >> table.numColumns;
		cout << "Enter Number of Records: ";
		in >> table.numRecords;
		return in;
	}

	// Cast operator
	explicit operator string() const {
		return "Table: " + name + ", Columns: " + to_string(numColumns) + ", Records: " + to_string(numRecords);
	}

	// Negation operator !
	bool operator!() const {
		return numColumns == 0 && numRecords == 0;
	}

	// Conditional operators (<, >, <=, >=)
	bool operator<(const Table& other) const {
		return numColumns < other.numColumns && numRecords < other.numRecords;
	}

	bool operator>(const Table& other) const {
		return numColumns > other.numColumns && numRecords > other.numRecords;
	}

	bool operator<=(const Table& other) const {
		return numColumns <= other.numColumns && numRecords <= other.numRecords;
	}

	bool operator>=(const Table& other) const {
		return numColumns >= other.numColumns && numRecords >= other.numRecords;
	}

	// Equality operator ==
	bool operator==(const Table& other) const {
		return name == other.name && numColumns == other.numColumns && numRecords == other.numRecords;
	}
};

//implement index class maybe

class Index {
private:
	vector<Table> tables;

public:
	// Accessor functions for reading and writing values
	int getNumTables() const {
		return tables.size();
	}

	// Generic methods for processing/displaying attributes
	void displayIndexInfo() const {
		cout << "Number of Tables in Index: " << getNumTables() << endl;
	}

	void performOperationOnTables() const {
		cout << "Performing an operation on all tables in the index" << endl;
	}

	// Constructors, copy constructor, destructor, and operator overloads
	Index() {}  // Default constructor

	Index(const vector<Table>& tables) : tables(tables) {}

	Index(const Index& other) : tables(other.tables) {}

	~Index() {}

	Index& operator=(const Index& other) {
		if (this != &other) {
			this->tables = other.tables;
		}
		return *this;
	}

	// Indexing operator []
	const Table& operator[](int index) const {
		if (index >= 0 && index < tables.size()) {
			return tables[index];
		}
		else {
			throw out_of_range("Table index out of range");
		}
	}

	// Mathematical operator +
	Index operator+(const Index& other) const {
		Index result = *this;
		result.tables.insert(result.tables.end(), other.tables.begin(), other.tables.end());
		return result;
	}

	// Increment operators ++
	Index& operator++() {
		for (Table& table : tables) {
			++table;
		}
		return *this;
	}

	Index operator++(int) {
		Index copy = *this;
		++(*this);
		return copy;
	}

	// Cast operator (explicit)
	explicit operator string() const {
		return "Index with " + to_string(getNumTables()) + " tables";
	}

	// Negation operator !
	bool operator!() const {
		return tables.empty();
	}

	// Conditional operators (<, >, <=, >=)
	bool operator<(const Index& other) const {
		return getNumTables() < other.getNumTables();
	}

	bool operator>(const Index& other) const {
		return getNumTables() > other.getNumTables();
	}

	bool operator<=(const Index& other) const {
		return getNumTables() <= other.getNumTables();
	}

	bool operator>=(const Index& other) const {
		return getNumTables() >= other.getNumTables();
	}

	// Equality operator ==
	bool operator==(const Index& other) const {
		return tables == other.tables;
	}

	// << operator
	friend ostream& operator<<(ostream& out, const Index& index) {
		out << "Index with " << index.getNumTables() << " tables" << endl;
		for (const Table& table : index.tables) {
			out << table << endl;
		}
		return out;
	}

	// >> operator
	friend istream& operator>>(istream& in, Index& index) {
		int numTables;
		cout << "Enter the number of tables in the index: ";
		in >> numTables;

		index.tables.clear(); // Clear existing tables

		for (int i = 0; i < numTables; ++i) {
			Table table;
			cout << "Enter details for Table " << i + 1 << ":" << endl;
			in >> table;
			index.tables.push_back(table);
		}

		return in;
	}
};