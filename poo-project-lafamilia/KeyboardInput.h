#include <iostream>
#include <string>
#include "./Exceptions.h";
#include "Util.h";
#include "StringStructureToArray.h";
using namespace std;


enum CommandType { CREATE_TABLE, DROP_TABLE, DISPLAY_TABLE, CREATE_INDEX, DROP_INDEX, INSERT_INTO, DELETE_FROM, SELECT, UPDATE };

#pragma once
// Cristi:
// Handle a command from keyboard input. Ex: CREATE TABLE Stud(id INTEGER 1, name TEXT George)
class KeyboardInput {
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

	CommandType commandType;
	string tableName;
	string* argsStringArray;
	int argsLength;
	bool hasCondition;
	StringStructureToArray* conditions;
	StringStructureToArray* updateArgs;
	string indexName;
	string columnNameForCreateIndex;

	string forbiddenWords[13] = { "CREATE", "TABLE", "UPDATE", "INDEX", "SELECT", "INSERT", "INTO", "DROP", "DISPLAY", "DELETE", "FROM", "WHERE", "SET" };

	KeyboardInput() {
		hasCondition = false;
		argsLength = 0;
	}

	void initializeKi() {
		allWordsBeforeFirstParanthesis = readCommand(&argsLength);
		setCommandType();
		splitCommandAndTableName();
	}

	~KeyboardInput() {
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
		cout << "Destructor called for KI "<< allWordsBeforeFirstParanthesis << endl;
	}


private:
	string allWordsBeforeFirstParanthesis; // CREATE TABLE Stud // for example
	string rawInput;

	void checkCommandValidity(string input) {
		// remove all white spaces before the actual command starts (maybe the user is drunk :) ):
		Util::removeWhiteSpacesBefore(&input);

		checkIfCommandsExist(input);
		checkDoubleQuotesValidity(input);
		checkForbiddenWords(input);
	}

	void checkTableNameValidity() {
		if (tableName == "" || tableName.find(" ") !=string::npos ) {
			throw Exceptions(INVALID_TABLE_NAME);
		}
	} 

	void checkIfCommandsExist(string input) {
		// check for the first 2 words:
		if (!(
			input.find("CREATE TABLE ") == 0 ||
			input.find("DROP TABLE ") == 0 ||
			input.find("DISPLAY TABLE ") == 0 ||
			(input.find("CREATE INDEX ") == 0 && input.find(" ON ") != string::npos) ||
			input.find("DROP INDEX ") == 0 ||
			(input.find("INSERT INTO ") == 0 && input.find(" VALUES ") != string::npos) ||
			input.find("DELETE FROM ") == 0 ||
			(input.find("SELECT ") == 0 && input.find(" FROM ") != string::npos && input.find("(") == string::npos) ||
			(input.find("UPDATE ") == 0 && input.find(" SET ") != string::npos && input.find("(") == string::npos)
			)) {
			throw Exceptions(INVALID_COMMAND);
		}
	}

	void checkDoubleQuotesValidity(string input) {
		if (input.find(",,") != string::npos) {
			throw Exceptions(INVALID_COMMAS);
		}
		//TODO: implement for more cases (paranthesis match, commas match, *swears*) // Andrei
	}

	void workCheckForbiddenWords(string* usedForCommand, int* lengthsOfUsedWords, int noOfUsedWords, string input) {

		for (int i = 0; i < noOfUsedWords; i++) {
			// erase the word, so we can check for the forbidden words below
			input.erase(input.find(usedForCommand[i]), input.find(usedForCommand[i]) + lengthsOfUsedWords[i]);
		}

		// check for all forbidden words:
		for (int i = 0; i < LENGTH_FORBIDDEN_WORDS; i++) {
			if (input.find(forbiddenWords[i]) != string::npos) {
				throw Exceptions(FORBIDDEN_WORDS_USED); 
			}
		}
	}

	void checkForbiddenWords(string input) {
		switch (commandType)
		{
		case CREATE_TABLE:
		{
			string usedForCommand[2] = { "CREATE", "TABLE" };
			int lengthsOfUsedWords[2] = { 6, 5 };
			workCheckForbiddenWords(usedForCommand, lengthsOfUsedWords, 2, input);
			break;
		}
		case DROP_TABLE:
		{
			string usedForCommand[2] = { "DROP", "TABLE" };
			int lengthsOfUsedWords[2] = { 4, 5 };
			workCheckForbiddenWords(usedForCommand, lengthsOfUsedWords, 2, input);
			break;
		}
			break;
		case DISPLAY_TABLE:
		{
			string usedForCommand[2] = { "DISPLAY", "TABLE" };
			int lengthsOfUsedWords[2] = { 7, 5 };
			workCheckForbiddenWords(usedForCommand, lengthsOfUsedWords, 2, input);
			break;
		}
			break;
		case CREATE_INDEX:
		{
			string usedForCommand[3] = { "CREATE", "INDEX", "ON"};
			int lengthsOfUsedWords[3] = { 6, 5, 2 };
			workCheckForbiddenWords(usedForCommand, lengthsOfUsedWords, 3, input);
			break;
		}
			break;
		case DROP_INDEX:
		{
			string usedForCommand[2] = { "DROP", "INDEX" };
			int lengthsOfUsedWords[2] = { 4, 5 };
			workCheckForbiddenWords(usedForCommand, lengthsOfUsedWords, 2, input);
			break;
		}
			break;
		case INSERT_INTO:
		{
			string usedForCommand[3] = { "INSERT", "INTO", "VALUES"};
			int lengthsOfUsedWords[3] = { 6, 4, 6 };
			workCheckForbiddenWords(usedForCommand, lengthsOfUsedWords, 3, input);
			break;
		}
			break;
		case DELETE_FROM:
		{
			string usedForCommand[2] = { "DELETE", "FROM" };
			int lengthsOfUsedWords[2] = { 6, 4 };
			workCheckForbiddenWords(usedForCommand, lengthsOfUsedWords, 2, input);
			break;
		}
			break;
		case SELECT:
		{
			string usedForCommand[2] = { "SELECT", "FROM" };
			int lengthsOfUsedWords[2] = { 6, 4 };
			workCheckForbiddenWords(usedForCommand, lengthsOfUsedWords, 2, input);
			break;
		}
			break;
		case UPDATE:
		{
			string usedForCommand[2] = { "UPDATE", "SET" };
			int lengthsOfUsedWords[2] = { 6, 3 };
			workCheckForbiddenWords(usedForCommand, lengthsOfUsedWords, 2, input);
			break;
		}
			break;
		default:
			break;
		}
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
			validateDropTable();
			break;
		case DISPLAY_TABLE:
			validateDisplayTable();
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

		Util::removeWhiteSpacesBefore(&tableName);
		Util::removeAllWhiteSpacesAfter(&tableName);

		checkTableNameValidity();
	}

	//Cristi:
	void validateCreateTable() {
		try
		{
			this->tableName = allWordsBeforeFirstParanthesis.erase(0, KeyboardInput::LENGTH_CREATE_TABLE_COMMAND - 1);
		}
		catch (const std::exception&)
		{
			cout << "Here is the error";
		}

	}
	void validateSelectFrom() {
		// TAKE THE ARGUMENTS:
		string argsString = rawInput.substr(7, rawInput.find(" FROM ")-7);
		Util::removeWhiteSpacesBefore(&argsString);
		Util::removeAllWhiteSpacesAfter(&argsString);
		if (argsString == "ALL") {
			argsStringArray = new string[1];
			argsStringArray[0] = "ALL";
		}
		else if (argsString.size() == 0) {
			throw Exceptions(NO_FIELDS_SELECTED);
		}
		else {
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
					Util::removeWhiteSpacesBefore(&arg);
					Util::removeAllWhiteSpacesAfter(&arg);
					argsStringArray[j] = arg;
					arg = "";
					j++;
				}
				else {
					arg.push_back(argsString[i]);
				}
			}
			Util::removeWhiteSpacesBefore(&arg);
			Util::removeAllWhiteSpacesAfter(&arg);
			argsStringArray[j] = arg;
		}

		// CHECK FOR WHERE CONDITION:
		hasCondition = rawInput.find("WHERE") != string::npos;
		// TAKE THE TABLE NAME:
		if (hasCondition) {
			tableName = rawInput.substr(rawInput.find("FROM ") + 5, rawInput.find(" WHERE")-(rawInput.find("FROM ") + 5));
			
			string afterWhere = rawInput.substr(rawInput.find("WHERE ") + 6);
			conditions = new StringStructureToArray(afterWhere);
		}
		else {
			tableName = rawInput.substr(rawInput.find("FROM ") + 5);
		}
	}

	void validateUpdate() {
		// GET THE TABLE NAME:
		tableName = rawInput.substr(LENGTH_UPDATE_COMMAND, rawInput.find(" SET")-LENGTH_UPDATE_COMMAND);

		string afterSet = rawInput.substr(rawInput.find(" SET ") + LENGTH_SET_COMMAND);
		updateArgs = new StringStructureToArray(afterSet);
	}

	void validateCreateIndex() {
		indexName = allWordsBeforeFirstParanthesis.substr(LENGTH_CREATE_INDEX_COMMAND, allWordsBeforeFirstParanthesis.find(" ON ") - LENGTH_CREATE_INDEX_COMMAND);
		
		Util::removeWhiteSpacesBefore(&indexName);
		Util::removeAllWhiteSpacesAfter(&indexName);
		if (indexName == "", indexName.find(" ") != string::npos) {
			throw Exceptions(INVALID_INDEX_NAME);
		}
		
		tableName = allWordsBeforeFirstParanthesis.substr(rawInput.find(" ON ") + LENGTH_ON_COMMAND);

		columnNameForCreateIndex = rawInput.substr(allWordsBeforeFirstParanthesis.size()+1);
		
		columnNameForCreateIndex.pop_back();
		Util::removeWhiteSpacesBefore(&columnNameForCreateIndex);
		Util::removeAllWhiteSpacesAfter(&columnNameForCreateIndex);

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


		this->tableName = allWordsBeforeFirstParanthesis.erase(0, KeyboardInput::LENGTH_INSERT_INTO_COMMAND);

		//VALIDATION CHECK 2: CRASH THE PROGRAM IF TABLE NAME HAS SPACES
		if (this->tableName.find(' ') != string::npos)
			throw Exceptions(INVALID_COMMAND);

		string argsFieldsString, argsValuesString;

		int indexFirstOpenParanthesis = KeyboardInput::LENGTH_INSERT_INTO_COMMAND + this->tableName.size();

		//until we find the ')' character, keep increasing the index of it by 1
		int indexFirstClosedParanthesis = indexFirstOpenParanthesis;
		while (this->rawInput[indexFirstClosedParanthesis] != ')')
			indexFirstClosedParanthesis++;

		argsFieldsString = this->rawInput.substr(indexFirstOpenParanthesis + 1, indexFirstClosedParanthesis - indexFirstOpenParanthesis - 1); //+1 and -1 in order to not have paranthesis taken into account as well

		const int LENGTH_OF_VALUES_KEYWORD_WITH_SPACES_AROUND_IT = 8;
		string substrWhereVALUESisSupposedToBe = this->rawInput.substr(indexFirstClosedParanthesis + 1, LENGTH_OF_VALUES_KEYWORD_WITH_SPACES_AROUND_IT);


		//VALIDATION CHECK 3: CRASH THE PROGRAM IF YOU DON'T SEE " VALUES " AFTER THE FIRST PARANTHESIS
		if (substrWhereVALUESisSupposedToBe != " VALUES ")
			throw Exceptions(INVALID_COMMAND);


		int indexSecondOpenParanthesis = indexFirstClosedParanthesis + 1 + LENGTH_OF_VALUES_KEYWORD_WITH_SPACES_AROUND_IT;
		int indexSecondClosedParanthesis = indexSecondOpenParanthesis;
		while (this->rawInput[indexSecondClosedParanthesis] != ')')
			indexSecondClosedParanthesis++;

		argsValuesString = this->rawInput.substr(indexSecondOpenParanthesis + 1, indexSecondClosedParanthesis - indexSecondOpenParanthesis - 1); //+1 and -1 in order to not have paranthesis taken into account as well
		
		int noOfFieldArguments = 0;
		string* argsFields = this->splitInsertIntoArguments(argsFieldsString, noOfFieldArguments);

		int noOfValueArguments = 0;
		string* argsValues = this->splitInsertIntoArguments(argsValuesString, noOfValueArguments);


		//VALIDATION CHECK 4: CRASH THE PROGRAM IF THE NUMBER OF ARGUMENTS DON'T MATCH IN EACH PARANTHESIS
		if (noOfFieldArguments != noOfValueArguments)
			throw Exceptions(INVALID_ARGUMENT);
	}

	void validateDeleteFrom() {
		int indexFirstSpaceAfterTableName = KeyboardInput::LENGTH_DELETE_FROM_COMMAND;
		while (this->rawInput[indexFirstSpaceAfterTableName] != ' ')
			indexFirstSpaceAfterTableName++;

		this->tableName = this->rawInput.substr(
			KeyboardInput::LENGTH_DELETE_FROM_COMMAND, 
			indexFirstSpaceAfterTableName - KeyboardInput::LENGTH_DELETE_FROM_COMMAND);

		//VALIDATION CHECK 1: CRASH THE PROGRAM IF TABLE NAME HAS SPACES
		if (this->tableName.find(' ') != string::npos)
			throw Exceptions(INVALID_COMMAND);

		int indexAfterTableName = KeyboardInput::LENGTH_DELETE_FROM_COMMAND + this->tableName.size();

		//VALIDATION CHECK 2: CRASH THE PROGRAM IF "WHERE" IS WRITTEN INCORRECTLY
		if (indexAfterTableName != this->rawInput.size() &&
			this->rawInput.substr(indexAfterTableName, KeyboardInput::LENGTH_WHERE_CONDITION + 1) != " WHERE ") {

			throw Exceptions(INVALID_COMMAND);
		}

		int indexAfterWhere = indexAfterTableName + KeyboardInput::LENGTH_WHERE_CONDITION;
		//cout << "index after where is " << indexAfterWhere << ".";

		StringStructureToArray condition(this->rawInput.substr(indexAfterWhere, this->rawInput.size()-1));
		this->hasCondition = true;

	}

	// Adnrei:
	void validateDropTable() {
		// ...
	}
	void validateDisplayTable() {
		// ...
	}
	void validateDropIndex() {
		// ...
	}

	// should return ["id INTEGER 1", "name TEXT Gigi"]
	string* splitCreateTableArguments(string argsString, int* argsNo) {
		// remove the last character (the closing paranthesis or extra spaces)
		while (argsString[argsString.size()-1] == ' ' || argsString[argsString.size()-1] == ')') {
			argsString.pop_back();
		}
		*argsNo = 0;
		// get the fields number:
		for (int i = 0; i < argsString.length(); i++) {
			if (argsString[i] == ',') {
				(*argsNo)++;
			}
		}
		(*argsNo)++;

		string* args = new string[*argsNo];
		*argsNo = 0;
		string arg = "";
		for (int i = 0; i < argsString.length(); i++) {
			if (argsString[i] == ',') {
				if (arg[0] == ' ') arg = arg.substr(1);
				args[*argsNo] = arg;
				arg = "";
				(*argsNo)++;
			}
			else {
				arg.push_back(argsString[i]);
			}
		}
		if (arg[0] == ' ') arg = arg.substr(1);
		args[*argsNo] = arg;
		arg = "";
		(*argsNo)++;

		return args;
	}


	// Basically, the first function that is called for manipulating the input.
	string readCommand(int* argsLength) {
		string input, word;

		getline(cin, input);
		checkCommandValidity(input);

		this->rawInput = input;
		this->rawInput = Util::trim(this->rawInput);

		string command;

		// put the commands inside the array:
		for (int i = 0; i < input.length(); i++) {
			if (input[i] == ' ') {
				command += word+" ";
				word = "";
			}
			else if (input[i] == '(') {
				this->argsStringArray = splitCreateTableArguments(input.substr(i + 1), argsLength);
				//interpretArguments(args, argsNo);
				break;
			}
			else {
				word.push_back(input[i]);
			}
		}
		command += word + " ";
		command.pop_back(); // last character is a space (check the if before)


		return command;
	}
};

//for each of these static const variables we add 1 in order to take into account the space after the command too (ex: "CREATE TABLE " and not "CREATE TABLE")
const int KeyboardInput::LENGTH_CREATE_TABLE_COMMAND = 13;
const int KeyboardInput::LENGTH_DROP_TABLE_COMMAND = 11;
const int KeyboardInput::LENGTH_DISPLAY_TABLE_COMMAND = 14;
const int KeyboardInput::LENGTH_CREATE_INDEX_COMMAND = 13;
const int KeyboardInput::LENGTH_DROP_INDEX_COMMAND = 11;
const int KeyboardInput::LENGTH_INSERT_INTO_COMMAND = 12;
const int KeyboardInput::LENGTH_DELETE_FROM_COMMAND = 12;
const int KeyboardInput::LENGTH_SELECT_COMMAND = 7;
const int KeyboardInput::LENGTH_UPDATE_COMMAND = 7;
const int KeyboardInput::LENGTH_WHERE_CONDITION = 6;
const int KeyboardInput::LENGTH_SET_COMMAND = 5; // Also count the space before SET
const int KeyboardInput::LENGTH_ON_COMMAND = 4; // Also count the space before SET
const int KeyboardInput::LENGTH_FORBIDDEN_WORDS = 13;
