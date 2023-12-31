#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <cstdio>
using namespace std;

class File {
protected:
	string fileName = "";
public:
	string getFileName() {
		return this->fileName;
	}
	void setFileName(string s) { //with extension
		if (!s.empty()) {
			this->fileName = s;
		}
		else
			throw exception("Invalid file name");
	}

	bool hasExtention(string extensions[], int num) {
		for (int i = 0; i < num; i++)
			if (this->fileName.find(extensions[i]) != string::npos)
				return true;
		return false;
	}

	bool exists() {
		fstream file(this->fileName);
		bool exists = false;
		if (file.is_open()) {
			exists = true;
			file.close();
		}
		return exists;
	}

	bool deleteFile() {
		if (remove(this->fileName.c_str()) != 0) {
			return false;
		}
		return true;
	}

	File(){}

	File(string fileName) {
		setFileName(fileName);
	}
};

//.txt
class TextFile : public File {
public:
	//constructor
	TextFile(string fileName) : File(fileName) {
		string ext[] = { ".txt" };

		if (!this->hasExtention(ext, 1))
			throw exception("Not a text file");
	}

	//funtions for opening the file
	ifstream openToRead() {
		ifstream file(this->fileName);
		return file;
	}
	ofstream openToWrite() {
		ofstream file(this->fileName);
		return file;
	}
};

//.bin, .table, .data
class BinaryFile : public File {
public:
	//constructor
	BinaryFile(string fileName) : File(fileName) {
		string ext[] = { ".bin", ".tab", ".dat" };
		if (!this->hasExtention(ext, 3))
			throw exception("Not a binary file");
	}

	//functions for opening the file
	ifstream openToRead() {
		ifstream file(this->fileName, ios::binary);
		return file;
	}
	ofstream openToWrite() {
		ofstream file(this->fileName, ios::binary);
		return file;
	}
};

//.csv
class CSVFile : public File {
public:
	//constructor
	CSVFile(string fileName) : File(fileName) {
		string ext[] = { ".csv" };

		if (!this->hasExtention(ext, 1))
			throw exception("Not a CSV file");
	}

	//funtions for opening the file
	ifstream openToRead() {
		ifstream file(this->fileName);
		return file;
	}
	ofstream openToWrite() {
		ofstream file(this->fileName);
		return file;
	}
};

class executeCommands {
private:
	string tableName = "";
	const string TABLE_EXTENSION = ".tab";
public:

	// function for checking the existence of tables
	bool existTables(string& tableName)
	{
		fstream file(tableName + TABLE_EXTENSION);
		return file.is_open();
	}

	//function for checking the correct structure
	bool correctStructure(string& tableName, string& str2)
	{

		if (!existTables(tableName)) return 0; 
		fstream file(tableName + TABLE_EXTENSION);
		if (!file.is_open())
		{
			cout << "error for checking the correct structure" << tableName << '\n';
			return 0;
		}
		string str1;
		getline(file, str1);
		file.close();
		if (str1 != str2)
		{
			cout << "the structure isn't correct " << tableName << '\n';
			cout << str2 << " " << str1 << '\n';
			return 0;
		}

	}
	//function for not allowing users to create 2 tables with the same name
	bool createTable(string& tableName, string& str2)
	{
		if (existTables(tableName))
		{
			cout << "table exists " << tableName << '\n';
			return 0;
		}
		ofstream file(tableName + TABLE_EXTENSION);
		if (!file.is_open())
		{
			cout << "table was not created " << tableName << '\n';
			return 0;
		}
		file << str2 << '\n';
		file.close();
		cout << "table was created " << tableName << '\n';
		return 1;
	}
	
};
