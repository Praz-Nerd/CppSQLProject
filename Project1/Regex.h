#pragma once
#include <regex>
#include <string>
using namespace std;
//a static class which stores regex patterns used in input validation
class regexStatements {
    static string createTableStatement;
    static string createIndexStatement;
    static string dropStatement;
    static string displayStatement;
    static string insertStatement;
    static string selectStatement;
    static string updateStatement;
    static string deleteStatement;
    static string importStatement;
public:
    static string getCreateTableStatement() {
        return createTableStatement;
    }
    static string getCreateIndexStatement() {
        return createIndexStatement;
    }
    static string getDropStatement() {
        return dropStatement;
    }
    static string getDisplayStatement() {
        return displayStatement;
    }
    static string getInsertStatement() {
        return insertStatement;
    }
    static string getSelectStatement() {
        return selectStatement;
    }
    static string getUpdateStatement() {
        return updateStatement;
    }
    static string getDeleteStatement() {
        return deleteStatement;
    }
    static string getImportStatement() {
        return importStatement;
    }

    //statically defined regex patterns, for input validation
    //static boolean function for checking regex patterns, case sensitivity is disabled by default
    static bool checkRegex(string statement, string regexPattern, bool caseSensitive = true) {
        if (!caseSensitive)
            return regex_match(statement, regex(regexPattern, std::regex_constants::icase));
        else
            return regex_match(statement, regex(regexPattern));

    }
    //function that checks if a string is an integer, float or text
    static bool isInteger(string& s) {
        regex integerRegex("^[-+]?\\d+$");
        return regex_match(s, integerRegex);
    }
    static bool isFloat(string& s) {
        regex floatRegex("^[-+]?\\d*\\.?\\d+$");
        return regex_match(s, floatRegex);
    }
    static bool isText(string& s) {
        //begins and end with single quote
        regex textRegex("^'[^']*'$");
        return regex_match(s, textRegex);
    }
    //function that removes spaces, by default replaces them with one space
    static void removeSpaces(string& statement, string replacement = " ") {
        statement = regex_replace(statement, regex("\\s+"), replacement);
    }
    //function that removes sapces from beginning and end of a string
    static string trimString(string& s) {
        int start = s.find_first_not_of(" \t");
        int end = s.find_last_not_of(" \t");
        if (start == string::npos) {
            return "";
        }
        return s.substr(start, end - start + 1);
    }
    static string removeQuote(string s, string replacement = " ") {
        return regex_replace(s, regex("'"), replacement);
    }

    //functions for checking conditions
    static bool condition(int val1, int val2, string op)
    {
        if (op == "<")
            return val1 < val2;

        if (op == "<=")
            return val1 <= val2;

        if (op == ">")
            return val1 > val2;

        if (op == ">=")
            return val1 >= val2;

        if (op == "!=")
            return val1 != val2;

        if (op == "=")
            return val1 == val2;
    }

    static bool condition(float val1, float val2, string op)
    {
        if (op == "<")
            return val1 < val2;

        if (op == "<=")
            return val1 <= val2;

        if (op == ">")
            return val1 > val2;

        if (op == ">=")
            return val1 >= val2;

        if (op == "!=")
            return val1 != val2;

        if (op == "=")
            return val1 == val2;
    }
    static bool condition(string val1, string val2, string op)
    {
        if (op == "<")
            return val1 < val2;

        if (op == "<=")
            return val1 <= val2;

        if (op == ">")
            return val1 > val2;

        if (op == ">=")
            return val1 >= val2;

        if (op == "!=")
            return val1 != val2;

        if (op == "=")
            return val1 == val2;
    }



};
