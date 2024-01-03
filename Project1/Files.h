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
		string ext[] = { ".bin", ".tab", ".data" };
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
	ofstream openToAppend() {
		ofstream file(this->fileName, ios::binary | ios::app);
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

