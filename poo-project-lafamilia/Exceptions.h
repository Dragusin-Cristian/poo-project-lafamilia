#pragma once
#include <string>

enum ExceptionType {
	INVALID_COMMAND, 
	INVALID_ARGUMENT, 
	INVALID_ARGUMENT_TYPE,
	INVALID_PARANTHESIS,
	INVALID_DEFAULT_VALUE
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
		case INVALID_PARANTHESIS:
			return "You missmatched paranthesis. Please type again:\n";
		default:
			break;
		}
	}

};
