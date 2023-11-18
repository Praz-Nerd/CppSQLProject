#pragma once
#include <regex>
#include <string>
using namespace std;
//a static class which stores regex patterns used in input validation
class regexStatements {
    //statically defined regex patterns, for input validation
public:
    static const std::string createStatement;
    static const std::string dropStatement;
    static const std::string displayStatement;
    static const std::string insertStatement;
    static const std::string selectStatement;
    static const std::string updateStatement;
    static const std::string deleteStatement;

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
