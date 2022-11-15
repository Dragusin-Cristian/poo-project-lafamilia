#include <iostream>
#include <string>
#include "./Exceptions.h";
using namespace std;

#pragma once
// Cristi:
// Handle a command from keyboard input. Ex: CREATE TABLE Stud(id INTEGER 1, name TEXT George)
class KeyboardInput {
public:
	int commandsLength;
	int argsLength;
	string* commands;
	string* args;

	KeyboardInput() {
		commandsLength = 0;
		argsLength = 0;
		commands = readCommand(&commandsLength, &argsLength);
	}

	// some way to interpret the commands:
	void interpretCommands() {
		for (int i = 0; i < commandsLength; i++) {
			cout << commands[i] << endl;
		}
	}

	// some way to interpret the arguments found in the command
	// *args may look like ["id INTEGER 1", "name TEXT Gigi"]
	void interpretArguments() {

		// Interpret somehow the values from the arguments ... 
		for (int i = 0; i < argsLength; i++) {
			cout << args[i] << endl;
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
		//TODO: implement for more cases
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
	string* readCommand(int* commandsLength, int* argsLength) {
		string input, word;
		//* uncomment when developping:
		//input = "CREATE TABLE Student(id NUMBER PRIMARY KEY, name TEXT Cristi, ceva CEVA)";
		//cout << input << endl << endl;

		//* and comment while developping:
		getInput(&input);

		// count spaces to set the commands array length:
		for (int i = 0; i < input.length(); i++) {
			if (input[i] == ' ') {
				(*commandsLength)++;
			}
		}
		(*commandsLength)++;
		string* commands = new string[*commandsLength];


		*commandsLength = 0;
		// put the commands inside the array:
		for (int i = 0; i < input.length(); i++) {
			if (input[i] == ' ') {
				commands[*commandsLength] = word;
				(*commandsLength)++;
				word = "";
			}
			else if (input[i] == '(') {
				//int* argsNo = new int(0);
				args = splitArguments(input.substr(i + 1), argsLength);
				//interpretArguments(args, argsNo);
				break;
			}
			else {
				word.push_back(input[i]);
			}
		}
		commands[*commandsLength] = word;
		(*commandsLength)++;

		return commands;
	}

	// to implement (IMPLMENT CLASSES):
	void interpretCommandsArray(string* commands, int commandsLength);
};