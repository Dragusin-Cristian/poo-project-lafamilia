#pragma once

#include "Source.cpp" //This will be CHANGED to #include "KeyboardInput.h" after that file is implemented, I think
#include <string>

//This is a class for "global" functions (public static functions) that can be used as general utilities
//in any place of the code. This shall be moved to a different header file later on.
class Util {
public:
	static const std::string WHITESPACE;

	//This is from stack overflow: https://stackoverflow.com/questions/18972258/index-of-nth-occurrence-of-the-string
	static int nthOccurrence(const std::string& str, const std::string& findMe, int nth)
	{
		size_t  pos = 0;
		int     cnt = 0;

		while (cnt != nth)
		{
			pos += 1;
			pos = str.find(findMe, pos);
			if (pos == std::string::npos)
				return -1;
			cnt++;
		}
		return pos;
	}

	//This is from stack overflow: https://stackoverflow.com/questions/20303821/how-to-check-if-string-is-in-array-of-strings
	bool in_array(string value, const char* const* array, int size)
	{
		for (int i = 0; i < size; i++)
		{
			if (value == array[i])
			{
				return true;
			}
		}

		return false;
	}

	//This is copy-pasted from somewhere else: https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
	std::string ltrim(const std::string& s)
	{
		size_t start = s.find_first_not_of(WHITESPACE);
		return (start == std::string::npos) ? "" : s.substr(start);
	}

	std::string rtrim(const std::string& s)
	{
		size_t end = s.find_last_not_of(WHITESPACE);
		return (end == std::string::npos) ? "" : s.substr(0, end + 1);
	}

	std::string trim(const std::string& s) {
		return Util::rtrim(Util::ltrim(s));
	}

};
const std::string Util::WHITESPACE = " \n\r\t\f\v";



//NOTE: This class is made to work for the arguments of CREATE TABLE. For other commands, we might need
//to modify it, or create another class altogether
class Argument:Util {
public:
	

private:
	std::string columnName = "";
	std::string type = "";
	std::string defaultValue = "";

	static const char* const ALL_DATA_TYPES[];
	static const int NUMBER_OF_TYPES = 5;

	//INPUT: an argument of the form "columnName TYPE defaultValue", ex: "id INTEGER 1"
	//This function sets the values for this class' private variables: columnName, type and defaultValue
	Argument(const std::string arguments)
	{
		this->validateArgument(arguments);

		this->columnName = arguments.substr(0, arguments.find(" "));
		this->type = arguments.substr(Util::nthOccurrence(arguments, " ", 1), Util::nthOccurrence(arguments, " ", 2));
		this->defaultValue = arguments.substr(Util::nthOccurrence(arguments, " ", 2), Util::nthOccurrence(arguments, " ", 3));
	}

	//INPUT: an argument list of the form "columnName TYPE defaultValue", ex: "id INTEGER 1"
	//OUTPUT: the same argument list, but in each place where there are two or more spaces separating the arguments, only one space is left
	std::string removeExtraSpaces(std::string arguments)
	{
		//Removes spaces from beginning and end
		arguments = Util::trim(arguments);

		//Removes extra spaces between arguments
		for (size_t i = 0; i < arguments.size() - 1; ++i) {
			while (arguments[i] == ' ' && arguments[i + 1] == ' ') {
				arguments.erase(i + 1, 1);
			}
		}
	}

	void validateArgument(std::string arguments)
	{
		arguments = this->removeExtraSpaces(arguments);
		short int nrOfWords = std::count(arguments.cbegin(), arguments.cend(), ' ') + 1; //counts the number of spaces in arguments
		if (nrOfWords != 3)
			throw "Invalid argument!"; //We will create an exception for this later in Exceptions.h

		std::string columnName = arguments.substr(0, arguments.find(" "));
		std::string type = arguments.substr(Util::nthOccurrence(arguments, " ", 1), Util::nthOccurrence(arguments, " ", 2));
		std::string defaultValue = arguments.substr(Util::nthOccurrence(arguments, " ", 2), Util::nthOccurrence(arguments, " ", 3));

		//Check validity of type
		if (!Util::in_array(type, this->ALL_DATA_TYPES, this->NUMBER_OF_TYPES)) {
			throw "Invalid argument type!"; //We will create an exception for this later in Exceptions.h
		}

		//Check validity of default value - TO IMPLEMENT...
		if (type == "INTEGER") {

		}
		else if (type == "TEXT") {

		}
		else if (type == "BLOB") {

		}
		else if (type == "REAL") {

		}
		else if (type == "NUMERIC") {

		}

	}
};

const char* const Argument::ALL_DATA_TYPES[] = { "INTEGER", "TEXT", "BLOB", "REAL", "NUMERIC"};