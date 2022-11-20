#pragma once
#include <string>

enum ExceptionType {
	INVALID_COMMAND, 
	INVALID_ARGUMENT, 
	INVALID_ARGUMENT_TYPE,
	INVALID_DEFAULT_VALUE,
	PARANTHESIS_MISMATCH,
	NO_FIELDS_SELECTED,
	INVALID_COMMAS,
	FORBIDDEN_WORDS_USED,
};

class Exceptions : public std::exception {
	ExceptionType exceptionType;
public:
	Exceptions(ExceptionType exceptionType) {
		this->exceptionType = exceptionType;
	}
	std::string handleError() {
		switch (exceptionType)
		{
		case INVALID_COMMAND:
			return "You entered an invalid command. Please type again:\n";
		case INVALID_ARGUMENT:
			return "You entered an invalid argument. Please type again:\n";
		case INVALID_ARGUMENT_TYPE:
			return "The argument you entered has an invalid type. Please type again:\n";
		case INVALID_DEFAULT_VALUE:
			return "The argument you entered has an invalid default value. Please type again:\n";
		case NO_FIELDS_SELECTED:
			return "No fields selected. Please type again:\n";
		case INVALID_COMMAS:
			return "Invalid commas. Please type again:\n";
		case FORBIDDEN_WORDS_USED:
			return "Cannot have fields or tables named with forbidden words. Please type again:\n"; // remains to implement
		case PARANTHESIS_MISMATCH:
			return "You missmatched paranthesis. Please type again:\n"; // remains to implement
		default:
			break;
		}
	}

};
