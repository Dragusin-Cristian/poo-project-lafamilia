#include <iostream>
#include <string>
#include "./Exceptions.h";
#include "Util.h";
#include "StringStructureToArray.h";
using namespace std;


enum CommandType { CREATE_TABLE, DROP_TABLE, DISPLAY_TABLE, CREATE_INDEX, DROP_INDEX, INSERT_INTO, DELETE_FROM, SELECT, UPDATE };

#pragma once
// Cristi:
// Handle a command from keyboard input. Ex: CREATE TABLE Stud(id INTEGER 64 1, name TEXT 128 'George')
class FileInputs {
public:
	//for each of these static const variables we add 1 in order to take into account the space after the command too (ex: "CREATE TABLE " and not "CREATE TABLE")
	static const int LENGTH_CREATE_TABLE_COMMAND;
	static const int LENGTH_DROP_TABLE_COMMAND;
	static const int LENGTH_DISPLAY_TABLE_COMMAND;
	static const int LENGTH_CREATE_INDEX_COMMAND;
	static const int LENGTH_DROP_INDEX_COMMAND;
	static const int LENGTH_INSERT_INTO_COMMAND;
	static const int LENGTH_DELETE_FROM_COMMAND;
	static const int LENGTH_SELECT_COMMAND;
	static const int LENGTH_UPDATE_COMMAND;
	static const int LENGTH_WHERE_CONDITION;
	static const int LENGTH_SET_COMMAND;
	static const int LENGTH_ON_COMMAND;
	static const int LENGTH_FORBIDDEN_WORDS;
	static const int LENGTH_VALUES_KEYWORD;
	static const string* forbiddenWords;

	CommandType commandType;
	string tableName;
	string* argsStringArray;
	int argsLength;
	bool hasCondition;
	StringStructureToArray* conditions;
	StringStructureToArray* updateArgs;
	string indexName;
	string columnNameForCreateIndex;


	FileInputs() {
		hasCondition = false;
		argsLength = 0;
	}

	void initializeFi(string input) {
		// get the user input and store it in rawInput, and trim it:
		readInput(input);

		// first validation:
		checkIfCommandsExist(rawInput);
		checkDoubleCommasValidity(rawInput);
		checkParanthesisMismatch(rawInput);
		checkInvalidQuotes(rawInput);

		// if there are no errors, we can set the command type:
		setCommandType(); 

		// sometimes is needed
		allWordsBeforeFirstParanthesis = parseInput(&argsLength);

		// here we set the fields foreach command
		splitCommandAndTableName();

		// must be the last one, because we check for the already set fields:
		checkForbiddenWords();
	}

	~FileInputs() {
		if (argsStringArray != nullptr) {
			argsStringArray->~string();
			argsStringArray = nullptr;
		}
		if (conditions != nullptr) {
			conditions->~StringStructureToArray();
			conditions = nullptr;
		}
		if (updateArgs != nullptr) {
			updateArgs->~StringStructureToArray();
			updateArgs = nullptr;
		}
		cout << "Destructor called for FI "<< allWordsBeforeFirstParanthesis << endl;
	}


private:
	string allWordsBeforeFirstParanthesis; // CREATE TABLE Stud // for example
	string rawInput;


	void checkTableNameValidity() {
		Util::checkWordValidity(tableName, INVALID_TABLE_NAME);
	} 

	void checkIndexNameValidity() {
		Util::checkWordValidity(indexName, INVALID_INDEX_NAME);
	}

	void checkIfCommandsExist(string input) {
		//TO-DO: We should make more exact error handlings:
		if 
		(!(
			input.find("CREATE TABLE ") == 0 ||
			input.find("DROP TABLE ") == 0 ||
			input.find("DISPLAY TABLE ") == 0 ||
			(input.find("CREATE INDEX ") == 0 && input.find(" ON ") != string::npos && input.find("(") != string::npos && Util::nthOccurrence(input, "(", 2) == -1) ||
			input.find("DROP INDEX ") == 0 ||
			(input.find("INSERT INTO ") == 0 && input.find(" VALUES ") != string::npos) ||
			(input.find("UPDATE ") == 0 && input.find(" SET ") != string::npos && input.find("(") == string::npos && (input.find(" WHERE ") != string::npos)) ||
			(input.find("DELETE FROM ") == 0 && input.find(" WHERE ") != string::npos && Util::nthOccurrence(input, "WHERE", 2) == -1 ) ||
			(input.find("SELECT ") == 0 && input.find(" FROM ") != string::npos && (
				(input.find("(") != string::npos && Util::nthOccurrence(input, "(", 2) == -1) // check for args
				|| (input.find(" ALL ") && input.find("(") == string::npos) // check for ALL
				) 
			)
		)) {
			throw Exceptions(INVALID_COMMAND);
		}
	}

	void checkParanthesisMismatch(string input) {
		int extraParanthesisCounter = 0; //adds 1 for each '(' and subtracts 1 for each ')'
		for (int i = 0; i < input.size(); i++) {
			if (input[i] == '(')
				extraParanthesisCounter++;
			else if (input[i] == ')')
				extraParanthesisCounter--;

			//if extraParanthesisCounter is negative it means that you closed a paranthesis before you even opened it
			if (extraParanthesisCounter < 0)
				throw Exceptions(PARANTHESIS_MISMATCH);
		}

		//if extraParanthesisCounter is different from 0 at the end then it means that the number of '(' is different from the number of ')'
		if (extraParanthesisCounter != 0)
			throw Exceptions(PARANTHESIS_MISMATCH);
	}

	void checkInvalidQuotes(string input) {
		int nrOfQuotes = 0;

		for (int i = 0; i < input.size(); i++) {
			if (input[i] == '"')
				nrOfQuotes++;
		}

		if (nrOfQuotes % 2 == 1)
			throw Exceptions(INVALID_QUTES);
	}

	void checkDoubleCommasValidity(string input) {
		if (input.find(",,") != string::npos) {
			throw Exceptions(INVALID_COMMAS);
		}
		//TODO: implement for more cases (paranthesis match, commas match, *swears*) // Andrei
	}

	void workCheckForbiddenWords(string word) {
		for (int i = 0; i < FileInputs::LENGTH_FORBIDDEN_WORDS; i++) {
			if (word == FileInputs::forbiddenWords[i]) {
				throw Exceptions(FORBIDDEN_WORDS_USED);
			}
		}
	}

	void checkForbiddenWords() {
		workCheckForbiddenWords(tableName);
		for (int i = 0; i < argsLength; i++) {
			workCheckForbiddenWords(argsStringArray[i]);
		}
		if (conditions != nullptr) {
			for (int i = 0; i < conditions->number; i++) {
				workCheckForbiddenWords(conditions->fields[i]);
				workCheckForbiddenWords(conditions->values[i]);
			}
		}

		if (updateArgs != nullptr) {
			for (int i = 0; i < updateArgs->number; i++) {
				workCheckForbiddenWords(updateArgs->fields[i]);
				workCheckForbiddenWords(updateArgs->values[i]);
			}
		}

		workCheckForbiddenWords(indexName);
		workCheckForbiddenWords(columnNameForCreateIndex);
	}
	

	void setCommandType() {
		if (rawInput.find("CREATE TABLE ") == 0) {
			commandType = CREATE_TABLE;
		}
		else if (rawInput.find("DROP TABLE ") == 0) {
			commandType = DROP_TABLE;
		}
		else if (rawInput.find("DISPLAY TABLE ") == 0) {
			commandType = DISPLAY_TABLE;
		}
		else if (rawInput.find("CREATE INDEX ") == 0) {
			commandType = CREATE_INDEX;
		}
		else if (rawInput.find("DROP INDEX ") == 0) {
			commandType = DROP_INDEX;
		}
		else if (rawInput.find("INSERT INTO ") == 0) {
			commandType = INSERT_INTO;
		}
		else if (rawInput.find("DELETE FROM ") == 0) {
			commandType = DELETE_FROM;
		}
		else if (rawInput.find("SELECT ") == 0) {
			commandType = SELECT;
		}
		else if (rawInput.find("UPDATE ") == 0) {
			commandType = UPDATE;
		}
	}

	void splitCommandAndTableName() {
		switch (commandType)
		{
		case CREATE_TABLE:
			validateCreateTable();
			break;
		case DROP_TABLE:
			validateDropTableOrDisplayTable();
			break;
		case DISPLAY_TABLE:
			validateDropTableOrDisplayTable();
			break;
		case CREATE_INDEX:
			validateCreateIndex();
			break;
		case DROP_INDEX:
			validateDropIndex();
			break;
		case INSERT_INTO:
			validateInsertInto();
			break;
		case DELETE_FROM:
			validateDeleteFrom();
			break;
		case SELECT:
			validateSelectFrom();
			break;
		case UPDATE:
			validateUpdate();
			break;
		default:
			break;
		}
	}

	// Cristi:
	void validateCreateTable() {
		this->tableName = allWordsBeforeFirstParanthesis.erase(0, FileInputs::LENGTH_CREATE_TABLE_COMMAND - 1);
		Util::removeWhiteSpacesBefore(&tableName, INVALID_TABLE_NAME);
		Util::removeAllWhiteSpacesAfter(&tableName, INVALID_TABLE_NAME);
		checkTableNameValidity();
	}

	// Cristi:
	void validateSelectFrom() {
		// TAKE THE ARGUMENTS:
		string argsString = rawInput.substr(LENGTH_SELECT_COMMAND, rawInput.find(" FROM ") - LENGTH_SELECT_COMMAND);
		Util::removeWhiteSpacesBefore(&argsString, INVALID_ARGUMENT);
		Util::removeAllWhiteSpacesAfter(&argsString, INVALID_ARGUMENT);

		if (argsString == "ALL") {
			argsStringArray = new string[1];
			argsStringArray[0] = "ALL";
			argsLength = 1;
		}
		else if (argsString.size() == 0) {
			throw Exceptions(NO_FIELDS_SELECTED);
		}
		else {
			// check for opening and closing paranthesis:
			if (argsString[0] != '(' || argsString[argsString.size() - 1] != ')') {
				throw Exceptions(PARANTHESIS_MISMATCH);
			}
			// remove the paranthesis:
			argsString.erase(0, 1);
			argsString.pop_back();
			// count the args length:
			int i = 0;
			while (argsString[i] != NULL) {
				if (argsString[i] == ',') {
					argsLength++;
				}
				i++;
			}
			argsLength++;

			// set the argsStringArray:
			int j = 0;
			string arg = "";
			argsStringArray = new string[argsLength];
			for (int i = 0; i < argsString.size(); i++) {
				if (argsString[i] == ',') {
					Util::removeWhiteSpacesBefore(&arg, INVALID_ARGUMENT);
					Util::removeAllWhiteSpacesAfter(&arg, INVALID_ARGUMENT);
					argsStringArray[j] = arg;
					arg = "";
					j++;
				}
				else {
					arg.push_back(argsString[i]);
				}
			}
			Util::removeWhiteSpacesBefore(&arg, INVALID_ARGUMENT);
			Util::removeAllWhiteSpacesAfter(&arg, INVALID_ARGUMENT);
			argsStringArray[j] = arg;
		}

		// CHECK FOR WHERE CONDITION:
		hasCondition = rawInput.find("WHERE") != string::npos;
		// TAKE THE TABLE NAME:
		if (hasCondition) {
			tableName = rawInput.substr(rawInput.find("FROM ") + 5, rawInput.find(" WHERE")-(rawInput.find("FROM ") + 5));
			Util::removeWhiteSpacesBefore(&tableName, INVALID_TABLE_NAME);
			Util::removeAllWhiteSpacesAfter(&tableName, INVALID_TABLE_NAME);
			checkTableNameValidity();

			string afterWhere = rawInput.substr(rawInput.find("WHERE ") + 6);
			conditions = new StringStructureToArray(afterWhere);
		}
		else {
			tableName = rawInput.substr(rawInput.find("FROM ") + 5);
			Util::removeWhiteSpacesBefore(&tableName, INVALID_TABLE_NAME);
			Util::removeAllWhiteSpacesAfter(&tableName, INVALID_TABLE_NAME);
			checkTableNameValidity();
		}
	}

	void validateUpdate() {
		// GET THE TABLE NAME:
		tableName = rawInput.substr(LENGTH_UPDATE_COMMAND, rawInput.find(" SET")-LENGTH_UPDATE_COMMAND);
		Util::removeWhiteSpacesBefore(&tableName, INVALID_TABLE_NAME);
		Util::removeAllWhiteSpacesAfter(&tableName, INVALID_TABLE_NAME);
		checkTableNameValidity();

		string afterSet = rawInput.substr(rawInput.find(" SET ") + LENGTH_SET_COMMAND);
		updateArgs = new StringStructureToArray(afterSet);
	}

	void validateCreateIndex() {
		indexName = allWordsBeforeFirstParanthesis.substr(LENGTH_CREATE_INDEX_COMMAND, allWordsBeforeFirstParanthesis.find(" ON ") - LENGTH_CREATE_INDEX_COMMAND);
		
		Util::removeWhiteSpacesBefore(&indexName, INVALID_INDEX_NAME);
		Util::removeAllWhiteSpacesAfter(&indexName, INVALID_INDEX_NAME);
		// VALIDATE THE INDEX NAME:
		if (indexName == "", indexName.find(" ") != string::npos) {
			throw Exceptions(INVALID_INDEX_NAME);
		}
		
		tableName = allWordsBeforeFirstParanthesis.substr(rawInput.find(" ON ") + LENGTH_ON_COMMAND);
		Util::removeWhiteSpacesBefore(&tableName, INVALID_TABLE_NAME);
		Util::removeAllWhiteSpacesAfter(&tableName, INVALID_TABLE_NAME);
		checkTableNameValidity();

		columnNameForCreateIndex = rawInput.substr(allWordsBeforeFirstParanthesis.size()+1);
		
		columnNameForCreateIndex.pop_back();
		Util::removeWhiteSpacesBefore(&columnNameForCreateIndex, INVALID_INDEX_NAME);
		Util::removeAllWhiteSpacesAfter(&columnNameForCreateIndex, INVALID_INDEX_NAME);

		if (columnNameForCreateIndex == "" || columnNameForCreateIndex.find(" ") != string::npos) {
			throw Exceptions(INVALID_COLUMN_NAME);
		}

	}


	// Stefan:
	string* splitInsertIntoArguments(string input, int& argsNo)
	{
		while (input[input.size() - 1] == ' ' || input[input.size() - 1] == ')') {
			input.pop_back();
		}
		argsNo = 0;
		// get the fields number:
		for (int i = 0; i < input.length(); i++) {
			if (input[i] == ',') {
				argsNo++;
			}
		}
		argsNo++;

		string* args = new string[argsNo];
		argsNo = 0;
		string arg = "";
		for (int i = 0; i < input.length(); i++) {
			if (input[i] == ',') {
				if (arg[0] == ' ') arg = arg.substr(1);
				args[argsNo] = arg;
				arg = "";
				argsNo++;
			}
			else {
				arg.push_back(input[i]);
			}
		}
		if (arg[0] == ' ') arg = arg.substr(1);
		args[argsNo] = arg;
		arg = "";
		argsNo++;

		return args;
	}

	void validateInsertInto() {
		//VALIDATION CHECK 1: CRASH IF THE LAST CHARACTER OF THE INPUT IS NOT ')'
		if (this->rawInput[this->rawInput.size() - 1] != ')')
			throw Exceptions(INVALID_COMMAND);

		int index_firstSpaceAfterTableName = FileInputs::LENGTH_INSERT_INTO_COMMAND;
		while (this->rawInput[index_firstSpaceAfterTableName] != ' ')
			index_firstSpaceAfterTableName++;

		this->tableName = this->rawInput.substr(
			FileInputs::LENGTH_INSERT_INTO_COMMAND, 
			index_firstSpaceAfterTableName - FileInputs::LENGTH_INSERT_INTO_COMMAND);


		//VALIDATION CHECK 2: CRASH THE PROGRAM IF TABLE NAME HAS SPACES
		Util::removeWhiteSpacesBefore(&tableName, INVALID_TABLE_NAME);
		Util::removeAllWhiteSpacesAfter(&tableName, INVALID_TABLE_NAME);
		checkTableNameValidity();


		string substrWhereVALUESisSupposedToBe = this->rawInput.substr(FileInputs::LENGTH_INSERT_INTO_COMMAND 
			+ this->tableName.size()  
			+ 1, 
			FileInputs::LENGTH_VALUES_KEYWORD);

		//VALIDATION CHECK 3: CRASH THE PROGRAM IF YOU DON'T SEE " VALUES " AFTER THE FIRST PARANTHESIS
		if (substrWhereVALUESisSupposedToBe != "VALUES")
			throw Exceptions(INVALID_COMMAND);


		string argsValuesString;

		int indexFirstOpenParanthesis = FileInputs::LENGTH_INSERT_INTO_COMMAND + this->tableName.size() + 1 + FileInputs::LENGTH_VALUES_KEYWORD;
		while (this->rawInput[indexFirstOpenParanthesis] == ' ')
			indexFirstOpenParanthesis++;

		//until we find the ')' character, keep increasing the index of it by 1
		int indexFirstClosedParanthesis = indexFirstOpenParanthesis;
		while (this->rawInput[indexFirstClosedParanthesis] != ')')
			indexFirstClosedParanthesis++;

		//VALIDATION CHECK 4: CRASH THE PROGRAM IF PARANTHESIS AREN'T WHERE I EXPECTED THEM TO BE
		if (this->rawInput[indexFirstOpenParanthesis] != '(' || this->rawInput[indexFirstClosedParanthesis] != ')')
			throw Exceptions(INVALID_COMMAND);


		argsValuesString = this->rawInput.substr(indexFirstOpenParanthesis + 1, indexFirstClosedParanthesis - indexFirstOpenParanthesis - 1); //+1 and -1 in order to not have paranthesis taken into account as well

		this->argsStringArray = this->splitInsertIntoArguments(argsValuesString, this->argsLength);

		//VALIDATION CHECK 5: ARGUMENTS CAN NOT HAVE SPACE INSIDE THEM
		for (int i = 0; i < this->argsLength; ++i) {
			if (this->argsStringArray[i].find(' ') != string::npos)
				throw Exceptions(INVALID_ARGUMENT);
		}
	}

	void validateDeleteFrom() {
		int indexFirstSpaceAfterTableName = FileInputs::LENGTH_DELETE_FROM_COMMAND;
		while (this->rawInput[indexFirstSpaceAfterTableName] != ' ')
			indexFirstSpaceAfterTableName++;

		this->tableName = this->rawInput.substr(
			FileInputs::LENGTH_DELETE_FROM_COMMAND, 
			indexFirstSpaceAfterTableName - FileInputs::LENGTH_DELETE_FROM_COMMAND);

		//VALIDATION CHECK 1: CRASH THE PROGRAM IF TABLE NAME HAS SPACES
		Util::removeWhiteSpacesBefore(&tableName, INVALID_TABLE_NAME);
		Util::removeAllWhiteSpacesAfter(&tableName, INVALID_TABLE_NAME);
		checkTableNameValidity();

		int indexAfterTableName = FileInputs::LENGTH_DELETE_FROM_COMMAND + this->tableName.size();

		//VALIDATION CHECK 2: CRASH THE PROGRAM IF "WHERE" IS WRITTEN INCORRECTLY
		if (indexAfterTableName != this->rawInput.size() &&
			this->rawInput.substr(indexAfterTableName, FileInputs::LENGTH_WHERE_CONDITION + 1) != " WHERE ") {

			throw Exceptions(INVALID_COMMAND);
		}

		int indexAfterWhere = indexAfterTableName + FileInputs::LENGTH_WHERE_CONDITION;
		//cout << "index after where is " << indexAfterWhere << ".";

		StringStructureToArray condition(this->rawInput.substr(indexAfterWhere, this->rawInput.size()-1));
		this->hasCondition = true;

	}

	// Andrei:
	void validateDropTableOrDisplayTable() {
		// Get the table name (and this also checks for exactely 3 words):
		if (commandType == DROP_TABLE) {
			tableName = rawInput.substr(LENGTH_DROP_TABLE_COMMAND);
		}
		else if (commandType == DISPLAY_TABLE) {
			tableName = rawInput.substr(LENGTH_DISPLAY_TABLE_COMMAND);
		}
		Util::removeWhiteSpacesBefore(&tableName, INVALID_TABLE_NAME);
		Util::removeAllWhiteSpacesAfter(&tableName, INVALID_TABLE_NAME);

		checkTableNameValidity();
	}


	void validateDropIndex() {
		// Get the index name (and this also checks for exactely 3 words):
		indexName = rawInput.substr(LENGTH_DROP_INDEX_COMMAND);
		Util::removeWhiteSpacesBefore(&indexName, INVALID_INDEX_NAME);
		Util::removeAllWhiteSpacesAfter(&indexName, INVALID_INDEX_NAME);

		checkIndexNameValidity();

	}

	// should return ["id, integer, 1000, 0", "nume, text, 128, ''", "grupa, text, 50, '1000'"]
	string* splitCreateTableArguments(string argsString, int* argsNo) {
		// remove the last character (the last closing paranthesis of all arguments (not of the last argument) and all extra spaces)
		while (argsString[argsString.size()-1] == ' ' || argsString[argsString.size() - 1] == ')') {
			argsString.pop_back();
			if (argsString[argsString.size()-1] == ')') {
				break;
			}
		}
		// remove the white spaces at the begining
		Util::removeWhiteSpacesBefore(&argsString, INVALID_ARGUMENT);

		// check for the first argument to have the right paranthesis:
		// for the closing paranthesis we already check for the rawInput, so we're fine
		if (argsString[0] != '(') throw Exceptions(PARANTHESIS_MISMATCH);

		// check for every arguemnt after the first one to have right paranthesis:
		int t = 1;
		while (Util::nthOccurrence(argsString, ")", t) != -1) {
			int index_closingP = Util::nthOccurrence(argsString, ")", t);
			// keep erasing spaces between ) and , (if there are any):
			while (argsString[index_closingP + 1] == ' ') {
				argsString.erase(index_closingP, index_closingP + 1);
			}

			// check for, and remove the following comma after the closing paranthesis (except fot the last argument):
			if (argsString[index_closingP + 1] != ',' && index_closingP != argsString.size()-1) {
				throw Exceptions(INVALID_COMMAS);
			}
			argsString.replace(index_closingP+1, 1, "");


			// remove also the possible spaces between comma and following opening (:
			while (argsString[index_closingP + 1] == ' ') {
				argsString.replace(index_closingP+1, 1, "");
			}

			// if the following argument doesn't have paranthesis (for the closing ones we check in the rawInput, so we're fine):
			if (argsString[index_closingP + 1] != '(' && index_closingP != argsString.size() - 1) {
				throw Exceptions(PARANTHESIS_MISMATCH);
			}

			t++;
		}

		*argsNo = 0;
		// get the fields number:
		for (int i = 0; i < argsString.size(); i++) {
			if (argsString[i] == '(') {
				(*argsNo)++;
			}
		}

		string* args = new string[*argsNo];
		*argsNo = 0;
		string arg = "";
		for (int i = 1; i < argsString.size(); i++) {
			if (argsString[i] == ')') {
				Util::removeWhiteSpacesBefore(&arg, INVALID_ARGUMENT);
				Util::removeAllWhiteSpacesAfter(&arg, INVALID_ARGUMENT);
				args[*argsNo] = arg;
				arg = "";
				(*argsNo)++;
			}
			else {
				arg.push_back(argsString[i]);
			}
		}

		// clean (again :) ) the arguments strings in order to be processed by ArgumentCreateTable class constructor:
		for (int i = 0; i < (*argsNo); i++) {
			while (args[i][args[i].size() - 1] == ' ' || args[i][args[i].size() - 1] == ')' || args[i][args[i].size() - 1] == ',' || args[i][args[i].size() - 1] == '(') {
				args[i].pop_back();
			}
			while (args[i][0] == ' ' || args[i][0] == ')' || args[i][0] == ',' || args[i][0] == '(') {
				args[i].erase(0, 1);
			}
		}

		return args;
	}

	void readInput(string input) {
		//getline(cin, this->rawInput); // used when we were reading the rawInput from the console input
		rawInput = input;
		this->rawInput = Util::trim(this->rawInput);
	}

	
	string parseInput(int* argsLength) {
		string word="", command="";

		// put the commands inside the array:
		for (int i = 0; i < rawInput.size(); i++) {
			if (rawInput[i] == ' ') {
				command += word+" ";
				word = "";
			}
			else if (rawInput[i] == '(') {
				if (this->commandType == CREATE_TABLE) {
					this->argsStringArray = splitCreateTableArguments(rawInput.substr(i + 1), argsLength);
				}
				break;
			}
			else {
				word.push_back(rawInput[i]);
			}
		}
		command += word + " ";
		command.pop_back(); // last character is a space (check the if before)


		return command;
	}
};

//for each of these static const variables we add 1 in order to take into account the space after the command too (ex: "CREATE TABLE " and not "CREATE TABLE")
const int FileInputs::LENGTH_CREATE_TABLE_COMMAND = 13;
const int FileInputs::LENGTH_DROP_TABLE_COMMAND = 11;
const int FileInputs::LENGTH_DISPLAY_TABLE_COMMAND = 14;
const int FileInputs::LENGTH_CREATE_INDEX_COMMAND = 13;
const int FileInputs::LENGTH_DROP_INDEX_COMMAND = 11;
const int FileInputs::LENGTH_INSERT_INTO_COMMAND = 12;
const int FileInputs::LENGTH_DELETE_FROM_COMMAND = 12;
const int FileInputs::LENGTH_SELECT_COMMAND = 7;
const int FileInputs::LENGTH_UPDATE_COMMAND = 7;
const int FileInputs::LENGTH_WHERE_CONDITION = 6;
const int FileInputs::LENGTH_SET_COMMAND = 5; // Also count the space before SET
const int FileInputs::LENGTH_ON_COMMAND = 4; // Also count the space before SET
const int FileInputs::LENGTH_FORBIDDEN_WORDS = 13;
const int FileInputs::LENGTH_VALUES_KEYWORD = 6; //without spaces around it

// TO-DO: Handle word "ALL" & "all": should be forbidden as fields name for table creation, and allowed as argument for SELECT command
const string* FileInputs::forbiddenWords = new string[26]{"CREATE", "create", "TABLE", "table", "UPDATE", "update", "INDEX", "index", "SELECT", "select", "INSERT", "insert", "INTO", "into", "DROP", "drop", "DISPLAY", "display", "DELETE", "delete", "FROM", "from", "WHERE", "where", "SET", "set"};
