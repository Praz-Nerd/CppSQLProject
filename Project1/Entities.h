#pragma once
#include <iostream>
#include <string>
#include "Files.h"
using namespace std;

//a class which represents a column definition from a table (data is stored using Records)
class Column {
	string* values = nullptr; //name, type, default value
	//integer, text, float
	int dimension;
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
	void setName(string y) {

		if (y == "text")
			throw exception("invalid name");
		else
			this->values[0] = y;
	}
	void setType(string s) {
		if (s == "integer" || s == "text" || s == "float")
			this->values[1] = s;
		else {
			throw exception("invalid type");
		}
	}
	void setDefaultValue(string s) {
		if (!s.empty())
			this->values[2] = s;
		else {
			throw exception("invalid default value");
		}
	}
	void setDimension(int d) {
		if (d > 0)
			this->dimension = d;
		else {
			throw exception("invalid dimension");

		}
	}

	Column(string name, string type, int dimension, string defaultValue) {
		setDimension(dimension);
		this->values = new string[VECTOR_SIZE];
		setName(name);
		setType(type);
		setDefaultValue(defaultValue);
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
	//copy constructor
	Column(const Column& c) {
		this->dimension = c.dimension;
		if (c.dimension) {
			this->values = new string[VECTOR_SIZE];
			for (int i = 0; i < VECTOR_SIZE; i++)
				this->values[i] = c.values[i];
		}
		else
			this->values = nullptr;
	}

	//destructor
	~Column() {
		if (this->values)
			delete[] this->values;
		this->values = nullptr;
	}

	// a function for displaying the column
	void displayColumn()
	{
		cout << values[1] << " " << values[2] << " " << values[3] << endl;

	}
	//= overloading
	Column& operator=(const Column& c) {
		if (this != &c) {
			this->dimension = c.dimension;
			if (this->values != nullptr) {
				delete[] this->values;
			}
			if (c.values != nullptr) {
				this->values = new string[VECTOR_SIZE];
				for (int i = 0; i < VECTOR_SIZE; i++)
					this->values[i] = c.values[i];
			}
			else
				this->values = nullptr;
		}
		return *this;
	}

	//cast operator
	explicit operator int() {
		return this->dimension;
	}
	//index operator
	string operator[](int index) {
		if (values != nullptr && index >= 0 && index < dimension)
			return values[index];
	}
	//== operator
	bool operator==(Column c) {
		return (this->values[0] == c.values[0]);
	}
	//< operator
	bool operator<(int dimension) {
		return(this->dimension < dimension);
	}
	//! operator
	bool operator!() {
		if (!this->dimension)
			return true;
		return false;
	}
	void setValues(string* values) {
		if (this->values)
			delete[] this->values;
		this->values = new string[dimension];
		for (int i = 0; i < dimension; i++)
			this->values[i] = values[i];
	}
	string* getValues() {
		string* copy = nullptr;
		if (this->values == nullptr)
			return copy;
		else {
			copy = new string[dimension];
			for (int i = 0; i < dimension; i++)
				copy[i] = this->values[i];
		}
		return copy;
	}

	//Default constructor
	Column()
	{
		this->dimension = 0;
		this->values = nullptr;
	}

	void addValue(string a)
	{
		if (values) {
			string* copy = new string[dimension];
			for (int i = 0; i < dimension; i++)
				copy[i] = values[i];
			delete[] values;
			dimension++;
			values = new string[dimension];
			for (int i = 0; i < dimension - 1; i++)
				values[i] = copy[i];
			values[dimension - 1] = a;
			delete[] copy;
		}
		else {
			dimension = 1;
			values = new string[dimension];
			values[0] = a;
		}
	}

	//- operator
	Column operator-(string k)
	{
		Column copy = *this;
		copy.addValue(k);
		return copy;

	}
	//+ operator
	Column operator+(string a) {
		Column copy = *this;
		copy.addValue(a);
		return copy;
	}

	//++ (increment) operators
	Column& operator++() {
		this->addValue("");
		return *this;
	}
	Column operator++(int i) {
		Column copy = *this;
		this->addValue("");
		return copy;
	}

	//<< and >> operators
	friend ostream& operator<<(ostream&, Column);
	friend istream& operator>>(istream&, Column&);
};

ostream& operator<<(ostream& out, Column c) {
	for (int i = 0; i < c.dimension; i++) {
		out << endl << "Value on column " << i + 1 << " : " << c[i];
	}
	out << endl;
	return out;
}

istream& operator>>(istream& in, Column& c)
{
	int i;
	cout << endl << "Change column";
	string* buffer = new string[c.dimension];
	for (i = 0; i < c.dimension; i++) {
		cout << "Value " << i + 1 << " : ";
		in >> buffer[i];
	}
	c.setValues(buffer);
	delete[] buffer;
	return in;
}

//a class which represents a line from a table
class Record {
	string* values;
	int numValues;
public:

	void setNumValues(int n) {

		if (n < 0)

			this->numValues = n;
		else {
			throw exception("invalid number of values");

		}
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
class Table
{
	string name;
	int numColumns;
	int numRecords;
	Column* columns;
	Record* records;

public:
	string getName()
	{
		return name;
	}
	int getNumColumns()
	{
		return numColumns;
	}

	int getNumRecords()
	{
		return numRecords;
	}

	void setName(const string& newName)
	{
		if (newName != "")
			this->name = newName;
		else
			throw exception("Can't have empty space for Index name");
	}

	void setNumColumns(int numColumns)
	{
		if (numColumns <= 0)
			throw exception("Can't have less than one column");
		else
			this->numColumns = numColumns;
	}

	void setNumRecords(int numRecords)
	{
		if (numRecords <= 0)
			throw exception("Can't have less than one record");
		else
			this->numRecords = numRecords;
	}

	// a function for displaying the table
	void displayTableInfo()
	{
		cout << "Table Name: " << name << ", Number of Columns: " << numColumns << ", Number of Records: " << numRecords << endl;
	}

	//default constructor
	Table()
	{
		this->numColumns = 0;
		this->numRecords = 0;
		this->columns = nullptr;
		this->records = nullptr;
	}

	//constructor for a table with no records
	Table(string name, int numColumns, Column* columns) {
		this->name = name;
		this->numColumns = numColumns;
		this->columns = new Column[numColumns];
		for (int i = 0; i < numColumns; i++)
			this->columns[i] = columns[i];
		this->numRecords = 0;
		this->records = nullptr;
	}

	//constructor
	Table(string name, int numColumns, int numRecords, Column* columns, Record* records) :
		name(name), numColumns(numColumns), numRecords(numRecords)
	{
		this->columns = new Column[numColumns];
		this->records = new Record[numRecords];
		for (int i = 0; i < numColumns; i++)
		{
			this->columns[i] = columns[i];
		}
		for (int i = 0; i < numRecords; i++)
		{
			this->records[i] = records[i];
		}
	}

	// Copy Constructor
	Table(const Table& table) : name(table.name), numColumns(table.numColumns), numRecords(table.numRecords)
	{
		this->columns = new Column[numColumns];
		this->records = new Record[numRecords];
		for (int i = 0; i < numColumns; i++)
		{
			this->columns[i] = table.columns[i];
		}
		for (int i = 0; i < numRecords; i++)
		{
			this->records[i] = table.records[i];
		}
	}
	//destructor
	~Table()
	{
		delete[] columns;
		delete[] records;
	}

	// Assignment operator
	Table& operator=(const Table& table)
	{
		if (this != &table)
		{
			//delete existing data
			delete[] columns;
			delete[] records;
			// copy new data
			this->name = table.name;
			this->numColumns = table.numColumns;
			this->numRecords = table.numRecords;

			//copy columns
			this->columns = new Column[numColumns];
			for (int i = 0; i < numColumns; i++)
			{
				this->columns[i] = table.columns[i];
			}

			//copy records
			this->records = new Record[numRecords];
			for (int i = 0; i < numRecords; i++)
			{
				this->records[i] = table.records[i];
			}
		}
		return *this;
	}
	// index operator
	Column& operator[](int index) {
		if (index >= 0 && index < numColumns)
		{
			return columns[index];
		}
		else {
			throw out_of_range("Column index out of range");
		}
	}

	//operator +
	Table operator+(const Table& b)
	{
		Table result;
		result.name = this->name + b.name;
		result.numColumns = this->numColumns + b.numColumns;
		result.numRecords = this->numRecords + b.numRecords;
		result.columns = new Column[result.numColumns];
		result.records = new Record[result.numRecords];
		for (int i = 0; i < this->numColumns; ++i)
		{
			result.columns[i] = this->columns[i];
		}
		for (int i = 0; i < b.numColumns; ++i)
		{
			result.columns[this->numColumns + i] = b.columns[i];
		}
		for (int i = 0; i < this->numRecords; ++i)
		{
			result.records[i] = this->records[i];
		}
		for (int i = 0; i < b.numRecords; ++i)

		{
			result.records[this->numRecords + i] = b.records[i];
		}
		return result;
	}
	// Increment operators ++
	Table& operator++()
	{
		++numColumns;
		++numRecords;
		return *this;
	}
	Table operator++(int)
	{
		Table copy = *this;
		++(*this);
		return copy;
	}

	// >> operator
	friend istream& operator>>(istream& in, Table& table)
	{
		cout << "Enter Table Name: ";
		in >> table.name;
		cout << "Enter Number of Columns: ";
		in >> table.numColumns;
		cout << "Enter Number of Records: ";
		in >> table.numRecords;
		return in;
	}

	//cast operator
	explicit operator string()
	{
		return "Table: " + name + ", Columns: " + to_string(numColumns) + ", Records: " + to_string(numRecords);
	}

	//negation operator !
	bool operator!()
	{
		return numColumns == 0 && numRecords == 0;
	}

	//conditional operator <=

	bool operator<=(Table& b)
	{
		return numColumns <= b.numColumns && numRecords <= b.numRecords;
	}
	// operator ==
	bool operator==(const Table& b)
	{
		return name == b.name && numColumns == b.numColumns && numRecords == b.numRecords;
	}

	void writeToBFile(BinaryFile bFile) {
		ofstream fout = bFile.openToWrite();
		if (!fout) {
			cout << "File error\n";
		}
		else {
			//write the table name
			unsigned length = this->name.length();
			const char* name = this->name.c_str();
			fout.write((char*)&length, sizeof(length));
			fout.write(name, length + 1);

			//write number of columns
			fout.write((char*)&(this->numColumns), sizeof(this->numColumns));

			//write columns name, type, default value, dimension
			for (int i = 0; i < this->numColumns; i++) {
				length = this->columns[i].getName().length();
				const char* colName = this->columns[i].getName().c_str();
				fout.write((char*)&length, sizeof(length));
				fout.write(colName, length + 1);

				length = this->columns[i].getType().length();
				const char* colType = this->columns[i].getType().c_str();
				fout.write((char*)&length, sizeof(length));
				fout.write(colType, length + 1);

				length = this->columns[i].getDefaultValue().length();
				const char* colDVal = this->columns[i].getDefaultValue().c_str();
				fout.write((char*)&length, sizeof(length));
				fout.write(colDVal, length + 1);

				int dim = this->columns[i].getDimension();
				fout.write((char*)&dim, sizeof(this->columns[i].getDimension()));
			}

			fout.close();
			cout << "Table created and written to file\n";
		}
	}


};
class Index

{
private:
	string name;
	Table** tables; //dynamic array of Table pointers
	int numTables;
public:
	//accessor functions for reading and writing values
	int getNumTables() const
	{
		return numTables;
	}

	string getName() const
	{
		return name;
	}

	void setName(const string& newName)
	{
		if (newName != "")
			this->name = newName;
		else
			throw exception("Can't have empty space for Index name");
	}

	void setNumtables(int numTables)
	{
		if (numTables <= 0)
			throw exception("Can't have less than one table");
		else
			this->numTables = numTables;
	}

	//a function for displaying the table
	void displayIndexInfo()
	{
		cout << "Index Name: " << name << ", Number of Tables in Index: " << getNumTables() << endl;
	}


	//Default constructor
	Index()
	{
		this->name = name;
		this->tables = nullptr;
		this->numTables = 0;
	}
	//constructors
	Index(const string& name, Table** tables, int numTables)
	{
		this->name = name;
		this->tables = tables;
		this->numTables = numTables;
	}

	//copy constructor
	Index(const Index& b)
	{
		this->name = b.name;
		this->numTables = b.numTables;
		this->tables = new Table * [numTables];

		for (int i = 0; i < numTables; ++i)

		{
			this->tables[i] = new Table(*(b.tables[i]));
		}
	}
	//destructor
	~Index() {
		//delete tables if necessary
		for (int i = 0; i < numTables; i++)
		{
			delete tables[i];
		}
		delete[] tables;
	}


	//operator = overloading
	Index& operator=(const Index& b)
	{
		if (this != &b)
		{
			clearTables();
			name = b.name;
			numTables = b.numTables;
			tables = new Table * [numTables];
			for (int i = 0; i < numTables; i++)
			{
				tables[i] = new Table(*(b.tables[i]));
			}
		}
		return *this;
	}

	//index operator
	const Table& operator[](int index)
	{
		if (index >= 0 && index < numTables)
		{
			return *(tables[index]);
		}
		else
		{
			throw exception("Table index out of range");
		}
	}
	//increment operators ++
	Index& operator++()
	{
		for (int i = 0; i < numTables; ++i)
		{
			++(*(tables[i]));
		}
		return *this;
	}

	Index operator++(int)
	{
		Index copy = *this;
		++(*this);
		return copy;
	}

	//cast operator (explicit)
	explicit operator string()
	{
		return "Index with " + to_string(getNumTables()) + " tables";
	}

	//negation operator !
	bool operator!() {
		return numTables == 0;
	}

	//conditional operators <
	bool operator<(const Index& b)
	{
		return getNumTables() < b.getNumTables();
	}

	// >> operator
	friend istream& operator>>(istream& in, Index& index)
	{
		cout << "Enter Index Name: ";
		in >> index.name;

		int numTables;
		cout << "Enter the number of tables in the index: ";
		in >> numTables;

		index.clearTables(); // Clear existing tables

		index.numTables = numTables;
		index.tables = new Table * [numTables];

		for (int i = 0; i < numTables; ++i)
		{
			index.tables[i] = new Table();
			cout << "Enter details for Table " << i + 1 << ":" << endl;
			in >> *(index.tables[i]);
		}

		return in;
	}

private:
	//helper function to clear tables
	void clearTables()
	{
		if (tables != nullptr)
		{
			for (int i = 0; i < numTables; ++i)
			{
				delete tables[i];
			}
			delete[] tables;
			tables = nullptr;
			numTables = 0;
		}
	}
};

class DB {
private:
	Table* tables;
	Index* indexes;
	int numTables;
	int numIndexes;
	string err = "";

public:

	int getNumTables()
	{
		return this->numTables;
	}
	int getNumIndexes()
	{
		return this->numIndexes;
	}
	void setNumTables(int numT)
	{
		if (numT != 0)
		{

			throw exception("invalid number of tables");
		}
		else this->numTables = numT;
	}
	void setNumIndexes(int numI)
	{
		if (numI != 0)
		{
			throw exception("invalid number of indexes");
		}
		else this->numIndexes = numI;
	}
	//constructors
	DB(int numTables, int numIndexes)
	{
		this->numTables = numTables;
		this->numIndexes = numIndexes;
		this->tables = nullptr;
		this->indexes = nullptr;

	}
	DB(Table* tables, Index* indexes, int numTables, int numIndexes)
	{
		this->tables = new Table[numTables];
		for (int i = 0; i < this->numTables; i++)
			this->tables[i] = tables[i];
		this->indexes = new Index[numIndexes];
		for (int j = 0; j < this->numIndexes; j++)
			this->indexes[j] = indexes[j];

	}
	//copy constructor
	DB(const DB& db)
	{
		this->numTables = db.numTables;
		if (db.numTables)
		{
			this->tables = new Table[numTables];
			for (int i = 0; i < this->numTables; ++i)
			{
				this->tables[i] = db.tables[i];
			}
		}
		else
		{
			this->tables = nullptr;
		}

		this->numIndexes = db.numIndexes;
		if (db.numIndexes)
		{
			this->indexes = new Index[numIndexes];
			for (int j = 0; j < this->numIndexes; ++j)
			{
				this->indexes[j] = db.indexes[j];
			}
		}
		else
		{
			this->indexes = nullptr;
		}

	}
	//destructor
	~DB()
	{
		if (this->tables != nullptr)
		{
			delete[] tables;
			this->tables = nullptr;
		}
		if (this->indexes != nullptr)
		{
			delete[] indexes;
			this->indexes = nullptr;
		}
	}

	//conditional operator(>=)
	bool operator>=(DB& db)
	{
		return numTables >= db.numTables && numIndexes >= db.numIndexes;
	}

	//cast operator
	explicit operator int()
	{
		return this->numTables;

	}
	//index operator
	Table operator[](int numTables) {
		if (tables != nullptr && numTables >= 0 && numTables < this->numTables)
			return tables[numTables];
	}

	//+ operator
	DB operator+(DB db)
	{
		db.numTables += numTables; //we are actually working with a copy so we can modify the value

		return db;
	}

	//- operator
	DB operator-(DB db)
	{
		db.numIndexes = numIndexes;
	}

	//== operator
	bool operator==(DB db) {
		return (this->numTables == db.numTables);
	}
	//< operator
	bool operator<(int numTables) {
		return(this->numTables < numTables);
	}
	//! operator
	bool operator!() {
		if (!this->numIndexes)
			return true;
		return false;
	}
	//Default constructor
	DB()
	{
		this->numTables = 0;
		this->numIndexes = 0;
		this->tables = nullptr;
		this->indexes = nullptr;
	}

	//= overloading
	DB& operator=(const DB& db) {
		if (this != &db) {
			this->numTables = db.numTables;
			if (this->tables != nullptr) {
				delete[] this->tables;
			}
			if (db.tables != nullptr) {
				this->tables = new Table[numTables];
				for (int i = 0; i < numTables; i++)
					this->numTables = db.numTables;
			}
			else
				this->tables = nullptr;
		}
		return *this;

		if (this != &db) {
			this->numIndexes = db.numIndexes;
			if (this->indexes != nullptr) {
				delete[] this->indexes;
			}
			if (db.indexes != nullptr) {
				this->indexes = new Index[numIndexes];
				for (int i = 0; i < numIndexes; i++)
					this->numIndexes = db.numIndexes;
			}
			else
				this->indexes = nullptr;
		}
		return *this;
	}
	//pre-incrementaation ++indexes
	DB& operator++()
	{
		this->numIndexes++;
		return *this;
	}
	//post-incrementation indexes++
	DB operator++(int i)
	{
		DB copy = *this;
		this->numIndexes++;
		return copy;
	}

	//<< and >> operators
	friend ostream& operator<<(ostream&, DB);
	friend istream& operator>>(istream&, DB&);
};
ostream& operator<<(ostream& out, DB db)
{
	out << db.numTables << " " << db.numIndexes << '\n';
	return out;
}

istream& operator>>(istream& in, DB& db)
{
	cout << "Number of tables: ";
	int buffer;
	in >> buffer;
	db.setNumTables(buffer);

	cout << "Number of Indexes: ";
	int buffer2;
	in >> buffer2;
	db.setNumIndexes(buffer2);
	return in;
}


