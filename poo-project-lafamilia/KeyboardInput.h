#include <iostream>
#include <string>
#include "./Exceptions.h";
#include "Util.h";
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

	CommandType commandType;
	string tableName;
	string* argsStringArray;
	int argsLength;

	KeyboardInput() {
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
			input.find("SELECT ") == 0 ||
			input.find("UPDATE ") == 0
			)) {
			throw Exceptions(INVALID_COMMAND);
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
		tableName = allWordsBeforeFirstParanthesis.erase(0, 13); // LENGTH OF "CREATE TABLE "
	}
	void validateSelectFrom() {
		// ...
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
	void validateInsertInto() {
		//tableName = ...
	}
	void validateDeleteFrom() {
		// ...
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