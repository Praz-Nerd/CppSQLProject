#pragma once
#include <regex>
#include <string>
using namespace std;
//a static class which stores regex patterns used in input validation
class regexStatements {
    static string createStatement;
    static string dropStatement;
    static string displayStatement;
    static string insertStatement;
    static string selectStatement;
    static string updateStatement;
    static string deleteStatement;
public:
    static string getCreateStatement() {
        return createStatement;
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

    //statically defined regex patterns, for input validation
    //static boolean function for checking regex patterns, case sensitivity is disabled by default
    static bool checkRegex(string statement, string regexPattern, bool caseSensitive = true) {
        if (!caseSensitive)
            return regex_match(statement, regex(regexPattern, std::regex_constants::icase));
        else
            return regex_match(statement, regex(regexPattern));

    }
    //function that removes spaces, by default replaces them with one space
    static void removeSpaces(string& statement, string replacement = " ") {
        statement = regex_replace(statement, regex("\\s+"), replacement);
    }
};
