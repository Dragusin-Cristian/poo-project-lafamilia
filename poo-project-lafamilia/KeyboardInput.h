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
		tableName = allWordsBeforeFirstParanthesis.erase(0, 13); // LENGTH OF "CREATE TABLE "
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