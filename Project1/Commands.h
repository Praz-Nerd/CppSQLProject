#pragma once
#include <iostream>
using namespace std;
class Commands {
public:
    static void display(string command) {
        string tableName = "";
        for (int i = command.find_last_of(' ') + 1; i < command.length() - 1; i++)
            tableName.push_back(command[i]);

        cout << "Table to display: " << tableName << endl;
    }
    static void drop(string command) {
        string tableName = "";
        for (int i = command.find_last_of(' ') + 1; i < command.length() - 1; i++)
            tableName.push_back(command[i]);

        cout << "Table to drop: " << tableName << endl;
    }
    static void insert(string command) {
        int length = command.find_last_of(')') - command.find_first_of('(') -1;
        string values = command.substr(command.find_first_of('(')+1, length);
        cout << values << endl;
    }
};