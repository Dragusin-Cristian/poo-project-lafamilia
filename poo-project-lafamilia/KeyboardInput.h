#include <iostream>
#include <string>
#include "./Exceptions.h";
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
		while (input[0] == ' ') {
			input.erase(0, 1);
		}
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

	// some way to interpret the commands:
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
			tableName = allWordsBeforeFirstParanthesis.erase(0, 13); // LENGTH OF "CREATE TABLE "
			break;
		default:
			break;
		}

		// remove all white spaces before the actual tableName starts (maybe the user is drunk :) ):
		while (tableName[0] == ' ') {
			tableName.erase(0, 1);
		}
		// remove all white spaces after the actual tableName
		while (tableName[tableName.size()-1] == ' ') {
			tableName.erase(tableName.size() - 1, tableName.size());
		}
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