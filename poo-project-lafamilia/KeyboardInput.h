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
	int argsLength;
	string command;
	string* argsStringArray;

	KeyboardInput() {
		argsLength = 0;
		command = readCommand(&argsLength);
	}

	// some way to interpret the commands:
	void interpretCommands() {
		cout << command << endl;
	}

	// some way to interpret the arguments found in the command
	// *args may look like ["id INTEGER 1", "name TEXT Gigi"]
	void interpretArguments() {

		// Interpret somehow the values from the arguments ... 
		for (int i = 0; i < argsLength; i++) {
			cout << argsStringArray[i] << endl;
		}
		// validateArguments
	}

private:
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
			throw Exceptions();
		}
		//TODO: implement for more cases (paranthesis match, commas match, *swears*) // Andrei
	}

	// should return ["id INTEGER 1", "name TEXT Gigi"]
	string* splitArguments(string argsString, int* argsNo) {
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


	// just get the input and check for validity:
	void getInput(string* input) {
		getline(cin, *input);

		try
		{
			checkCommandValidity(*input);
		}
		catch (Exceptions e)
		{
			cout << e.invalid_command();
			getInput(input);
		}
	}

	// Basically, the first function that is called for manipulating the input.
	string readCommand(int* argsLength) {
		string input, word;
		//* uncomment when developping:
		//input = "CREATE TABLE Student(id NUMBER PRIMARY KEY, name TEXT Cristi, ceva CEVA)";
		//cout << input << endl << endl;

		//* and comment while developping:
		getInput(&input);
		string command;

		// put the commands inside the array:
		for (int i = 0; i < input.length(); i++) {
			if (input[i] == ' ') {
				command += word+" ";
				word = "";
			}
			else if (input[i] == '(') {
				this->argsStringArray = splitArguments(input.substr(i + 1), argsLength);
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

	// to implement (IMPLMENT CLASSES):
	void interpretCommandsArray(string* commands, int commandsLength);
};