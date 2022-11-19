#pragma once

#include "Util.h"
#include <string>
#include "Exceptions.h";

//NOTE: This class is made to work for the arguments of CREATE TABLE. For other commands, we might need
//to modify it, or create another class altogether
class ArgumentCreateTable {
public:
	//INPUT: an argument of the form "columnName TYPE defaultValue", ex: "id INTEGER 1"
	//This function sets the values for this class' private variables: columnName, type and defaultValue
	ArgumentCreateTable(std::string arguments)
	{
		arguments = this->removeExtraSpaces(arguments);
		this->validateArgument(arguments);

		int index_firstSpace = arguments.find(" ");
		int index_secondSpace = Util::nthOccurrence(arguments, " ", 2);

		this->columnName = arguments.substr(0, index_firstSpace);
		this->type = arguments.substr(index_firstSpace + 1, index_secondSpace - index_firstSpace - 1);
		this->defaultValue = arguments.substr(index_secondSpace + 1, arguments.size() - index_secondSpace);
	}

	//GETTERS:
	std::string getColumnName()
	{
		return this->columnName;
	}

	std::string getType()
	{
		return this->type;
	}

	std::string getDefaultValue()
	{
		return this->defaultValue;
	}

private:
	std::string columnName = "";
	std::string type = "";
	std::string defaultValue = "";

	static const char* const ALL_DATA_TYPES[];
	static const int NUMBER_OF_TYPES = 5;

	
	//INPUT: an argument list of the form "columnName TYPE defaultValue", ex: "id INTEGER 1"
	//OUTPUT: the same argument list, but in each place where there are two or more spaces separating the arguments, only one space is left
	std::string removeExtraSpaces(std::string arguments)
	{
		//Removes spaces from beginning and end
		std::string output = Util::trim(arguments);

		//Removes extra spaces between arguments
		for (size_t i = 0; i < output.size() - 1; ++i) {
			while (output[i] == ' ' && output[i + 1] == ' ') {
				output.erase(i + 1, 1);
			}
		}

		return output;
	}

	void validateArgument(std::string arguments)
	{
		arguments = this->removeExtraSpaces(arguments);
		short int nrOfWords = std::count(arguments.cbegin(), arguments.cend(), ' ') + 1; //counts the number of spaces in arguments
		if (nrOfWords != 3)
			throw Exceptions(INVALID_ARGUMENT);  //We will create an exception for this later in Exceptions.h
		
		int index_firstSpace = arguments.find(" ");
		int index_secondSpace = Util::nthOccurrence(arguments, " ", 2);

		std::string columnName = arguments.substr(0, index_firstSpace);
		std::string type = arguments.substr(index_firstSpace + 1, index_secondSpace - index_firstSpace - 1);
		std::string defaultValue = arguments.substr(index_secondSpace + 1, arguments.size() - index_secondSpace);

		//Check validity of type
		if (Util::in_array(type, this->ALL_DATA_TYPES, this->NUMBER_OF_TYPES) == false) {
			throw Exceptions(INVALID_ARGUMENT_TYPE); 
		}

		//Check validity of default value
		if (type == "INTEGER") {
			for (size_t i = 0; i < defaultValue.size(); ++i)
				if (!std::isdigit(defaultValue[i]))
					throw Exceptions(INVALID_DEFAULT_VALUE); 
		}
		else if (type == "REAL") {
			for (size_t i = 0; i < defaultValue.size(); ++i)
				if (!std::isdigit(defaultValue[i]) && !defaultValue[i] == '.')
					throw Exceptions(INVALID_DEFAULT_VALUE);
		}

		//We don't need validation for BLOB, TEXT and NUMERIC, since any of those 3 would be valid if it had gotten to this point
	}
};

//STATIC CONST VARIABLE DEFINITIONS:
const char* const ArgumentCreateTable::ALL_DATA_TYPES[] = { "INTEGER", "TEXT", "BLOB", "REAL", "NUMERIC"};
