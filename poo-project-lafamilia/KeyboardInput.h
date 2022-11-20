#include <iostream>
#include <string>
#include "./Exceptions.h";
#include "Util.h";
#include "Condition.h";
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

	CommandType commandType;
	string tableName;
	string* argsStringArray;
	int argsLength;
	bool hasCondition;
	Condition* conditions;

	KeyboardInput() {
		hasCondition = false;
		argsLength = 0;
		allWordsBeforeFirstParanthesis = readCommand(&argsLength);
		setCommandType();
		splitCommandAndTableName();
	}

	//~KeyboardInput() {
	//	cout << "Destructor called for KI "<< allWordsBeforeFirstParanthesis << endl;
	//}


private:
	string allWordsBeforeFirstParanthesis; // CREATE TABLE Stud // for example
	string rawInput;

	static void checkCommandValidity(string input) {
		// remove all white spaces before the actual command starts (maybe the user is drunk :) ):
		Util::removeWhiteSpacesBefore(&input);

		// check for the first 2 words:
		if (!(
			input.find("CREATE TABLE ") == 0 ||
			input.find("DROP TABLE ") == 0 ||
			input.find("DISPLAY TABLE ") == 0 ||
			input.find("CREATE INDEX ") == 0 ||
			input.find("DROP INDEX ") == 0 ||
			input.find("INSERT INTO ") == 0 ||
			input.find("DELETE FROM ") == 0 ||
			(input.find("SELECT ") == 0 && input.find(" FROM ") != string::npos && input.find("(") == string::npos) ||
			(input.find("UPDATE ") == 0 && input.find(" SET ") != string::npos)
			)) {
			throw Exceptions(INVALID_COMMAND);
		}
		if (input.find(",,") != string::npos) {
			throw Exceptions(INVALID_COMMAS);
		}
		//TODO: implement for more cases (paranthesis match, commas match, *swears*) // Andrei
	}

	void setCommandType() {
		if (allWordsBeforeFirstParanthesis.find("CREATE TABLE ") == 0) {
			commandType = CREATE_TABLE;
		}
		else if (allWordsBeforeFirstParanthesis.find("DROP TABLE ") == 0) {
			commandType = DROP_TABLE;
		}
		else if (allWordsBeforeFirstParanthesis.find("DISPLAY TABLE ") == 0) {
			commandType = DISPLAY_TABLE;
		}
		else if (allWordsBeforeFirstParanthesis.find("CREATE INDEX ") == 0) {
			commandType = CREATE_INDEX;
		}
		else if (allWordsBeforeFirstParanthesis.find("DROP INDEX ") == 0) {
			commandType = DROP_INDEX;
		}
		else if (allWordsBeforeFirstParanthesis.find("INSERT INTO ") == 0) {
			commandType = INSERT_INTO;
		}
		else if (allWordsBeforeFirstParanthesis.find("DELETE FROM ") == 0) {
			commandType = DELETE_FROM;
		}
		else if (allWordsBeforeFirstParanthesis.find("SELECT ") == 0) {
			commandType = SELECT;
		}
		else if (allWordsBeforeFirstParanthesis.find("UPDATE ") == 0) {
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
	}

	//Cristi:
	void validateCreateTable() {
		this->tableName = allWordsBeforeFirstParanthesis.erase(0, KeyboardInput::LENGTH_CREATE_TABLE_COMMAND);
	}
	void validateSelectFrom() {
		cout << allWordsBeforeFirstParanthesis<<endl;
		// TAKE THE ARGUMENTS:
		string argsString = allWordsBeforeFirstParanthesis.substr(7, allWordsBeforeFirstParanthesis.find(" FROM ")-7);
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
		hasCondition = allWordsBeforeFirstParanthesis.find("WHERE") != string::npos;
		// TAKE THE TABLE NAME:
		if (hasCondition) {
			tableName = allWordsBeforeFirstParanthesis.substr(allWordsBeforeFirstParanthesis.find("FROM ") + 5, allWordsBeforeFirstParanthesis.find(" WHERE")-(allWordsBeforeFirstParanthesis.find("FROM ") + 5));
			
			string afterWhere = allWordsBeforeFirstParanthesis.substr(allWordsBeforeFirstParanthesis.find("WHERE ") + 6);
			conditions = new Condition(afterWhere);
		}
		else {
			tableName = allWordsBeforeFirstParanthesis.substr(allWordsBeforeFirstParanthesis.find("FROM ") + 5);
		}
	}
	void validateUpdate() {
		// ...
	}
	void validateCreateIndex() {
		// ...
	}
	void validateDropIndex() {
		// ...
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

		Condition condition(this->rawInput.substr(indexAfterWhere, this->rawInput.size()-1));
		this->hasCondition = true;

	}

	// Adnrei:
	void validateDropTable() {
		// ...
	}
	void validateDisplayTable() {
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