#pragma once
#include <string>
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
};

