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
	}

	~ArgumentCreateTable() {
		cout << "ArgumentCreateTable deleted" << endl;
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

	std::string getSize() {
		return this->size;
	}

private:
	std::string columnName = "";
	std::string type = "";
	std::string defaultValue = "";
	std::string size;

	static const char* const ALL_DATA_TYPES[];
	static const int NUMBER_OF_TYPES = 10;

	
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
		short int nrOfWords = std::count(arguments.cbegin(), arguments.cend(), ',') + 1; //counts the number of spaces in arguments
		if (nrOfWords != 4)
			throw Exceptions(INVALID_ARGUMENT);  //We will create an exception for this later in Exceptions.h
		
		int index_firstComma = arguments.find(",");
		int index_secondComma = Util::nthOccurrence(arguments, ",", 2);
		int index_thirdComma = Util::nthOccurrence(arguments, ",", 3);

		std::string columnName = arguments.substr(0, index_firstComma);
		std::string type = arguments.substr(index_firstComma + 1, index_secondComma - index_firstComma - 1);
		std::string size = arguments.substr(index_secondComma + 1, index_thirdComma - index_secondComma - 1);
		std::string defaultValue = arguments.substr(index_thirdComma + 1, arguments.size() - index_thirdComma);

		Util::removeWhiteSpacesBefore(&columnName, INVALID_ARGUMENT);
		Util::removeAllWhiteSpacesAfter(&columnName, INVALID_ARGUMENT);

		Util::removeWhiteSpacesBefore(&type, INVALID_ARGUMENT);
		Util::removeAllWhiteSpacesAfter(&type, INVALID_ARGUMENT);

		Util::removeWhiteSpacesBefore(&size, INVALID_ARGUMENT);
		Util::removeAllWhiteSpacesAfter(&size, INVALID_ARGUMENT);

		Util::removeWhiteSpacesBefore(&defaultValue, INVALID_ARGUMENT);
		Util::removeAllWhiteSpacesAfter(&defaultValue, INVALID_ARGUMENT);

		//cout << columnName << endl << type << endl << size << endl << defaultValue << endl;

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

		// If we passed without any errors, set the fields:
		this->columnName = columnName;
		this->defaultValue = defaultValue;
		this->size = size;
		this->type = type;

		//We don't need validation for BLOB, TEXT and NUMERIC, since any of those 3 would be valid if it had gotten to this point
	}
};

//STATIC CONST VARIABLE DEFINITIONS:
const char* const ArgumentCreateTable::ALL_DATA_TYPES[] = { "INTEGER", "integer", "TEXT", "text", "BLOB", "blob", "REAL", "real", "NUMERIC", "numeric"};
