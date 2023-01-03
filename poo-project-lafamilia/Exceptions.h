#pragma once
#include <string>

enum ExceptionType {
	INVALID_COMMAND, 
	INVALID_ARGUMENT, 
	INVALID_ARGUMENT_TYPE,
	INVALID_DEFAULT_VALUE,
	INVALID_SIZE,
	PARANTHESIS_MISMATCH,
	NO_FIELDS_SELECTED,
	INVALID_COMMAS,
	FORBIDDEN_WORDS_USED,
	INVALID_FIELD,
	INVALID_TABLE_NAME,
	INVALID_INDEX_NAME,
	INDEX_ALREADY_EXISTS,
	INVALID_COLUMN_NAME,
	INVALID_QUTES,
	EMPTY_CONDITION_OR_ARGUMENT,
	NO_FILES_PASSED,
	FILE_DOES_NOT_EXIST,
	ERROR_CREATING_TABLES_FOLDER,
	TABLE_DOES_NOT_EXIST,
	TABLE_ALREADY_EXISTS,
	ERROR_IN_CREATING_THE_TABLE,
	FILE_DELETION_FAILED,
	INVALID_NUMBER_OF_ARGUMENTS_FOR_INSERT_INTO,
	INVALID_ARGUMENT_INSERT_INTO,
	ERROR_IN_INSERTING_INTO,
	TABLE_IS_EMPTY
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
		case INVALID_SIZE:
			return "The size is invalid.\n";
		case NO_FIELDS_SELECTED:
			return "No fields selected. Please type again:\n";
		case INVALID_COMMAS:
			return "Invalid commas. Please type again:\n";
		case FORBIDDEN_WORDS_USED: // like CREATE, TABLE, UPDATE, INDEX, SELECT, INSERT, INTO, DROP, DISPLAY, DELETE, FROM, WHERE, SET 
			return "Cannot have fields or tables named with forbidden words. Please type again:\n"; // remains to implement
		case PARANTHESIS_MISMATCH: // check if the number of ( equals the number of ) in the rawInput
			return "You missmatched paranthesis. Please type again:\n"; // remains to implement
		case INVALID_QUTES: // the number of " should be even in rawInput
			return "You mismatched the quotes. Please type again:\n";
		case INVALID_FIELD:
			return "Invalid field passed. Please type again:\n";
		case INVALID_TABLE_NAME:
			return "Invalid table name. Please type again:\n";
		case INVALID_INDEX_NAME:
			return "Invalid index name. Please type again:\n";
		case INDEX_ALREADY_EXISTS:
			return "Index was already created. Please type again:\n";
		case INVALID_COLUMN_NAME:
			return "Invalid column name. Please type again:\n";
		case EMPTY_CONDITION_OR_ARGUMENT:
			return "Empty condition or argument. Please type again.\n";
		case NO_FILES_PASSED:
			return "You forgot pass at least 1 file.\n";
		case FILE_DOES_NOT_EXIST:
			return "One of the passed files doesn't exist.\n";
		case ERROR_CREATING_TABLES_FOLDER:
			return "Error in creating the Tables folder.\n";
		case TABLE_DOES_NOT_EXIST:
			return "Table doesn't exist.\n";
		case TABLE_ALREADY_EXISTS:
			return "A table named like this already exists.\n";
		case ERROR_IN_CREATING_THE_TABLE:
			return "Could not create the table.\n";
		case FILE_DELETION_FAILED:
			return "Could not delete this table, check if it exists first.\n";
		case INVALID_NUMBER_OF_ARGUMENTS_FOR_INSERT_INTO:
			return "Invalid number of arguments passed. Check structure of the table.\n";
		case INVALID_ARGUMENT_INSERT_INTO:
			return "You entered an invalid argument. Please type again:\n";
		case ERROR_IN_INSERTING_INTO:
			return "An error occured while inserting the values into this table.\n";
		case TABLE_IS_EMPTY:
			return "Table doesn't exist or it's empty.\n";
		default:
			break;
		}
	}

};
