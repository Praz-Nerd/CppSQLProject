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
	static string readString(ifstream& fin) {
		unsigned length;
		//read length of string
		fin.read((char*)&length, sizeof(length));
		char* s = new char[length + 1];
		//read the actual string
		fin.read(s, length + 1);
		string sol = s;
		return sol;
	}
	static int readInteger(ifstream& fin) {
		int x;
		fin.read((char*)&x, sizeof(x));
		return x;
	}
	static float readFloat(ifstream& fin) {
		float x;
		fin.read((char*)&x, sizeof(x));
		return x;
	}
	static void writeString(ofstream& fout, string s) {
		unsigned length = s.length();
		const char* n = s.c_str();
		fout.write((char*)&length, sizeof(length));
		fout.write(n, length + 1);
	}
	static void writeInteger(ofstream& fout, int x) {
		fout.write((char*)&x, sizeof(x));
	}
	static void writeFloat(ofstream& fout, float x) {
		fout.write((char*)&x, sizeof(x));
	}
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

