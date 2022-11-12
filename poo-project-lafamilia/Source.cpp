#include <iostream>
#include <string>
#include "./Exceptions.h";
using namespace std;


// also trim from beggining?
void checkCommandValidity(string input) {
	
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
}

// ["id INTEGER 1", "name TEXT Gigi"]
string* splitArguments(string argsString, int* argsNo) {
	argsString.pop_back();
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

void interpretArguments(string* args, int* argsNo) {
	
	// Interpret somehow the values from the arguments ... 

	for (int i = 0; i < *argsNo; i++) {
		cout << args[i] << endl;
	}

	// validateArguments
}

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
			int* argsNo = new int(0);
			string* args = splitArguments(input.substr(i + 1), argsNo);
			interpretArguments(args, argsNo);
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

class InputFromKeyboard {
	//...
};

class Table {
	//...
};

int main() {

	int commandsLength = 0, argsLength = 0;
	string* commands = readCommand(&commandsLength, &argsLength);

	for (int i = 0; i < commandsLength; i++) {
		cout << commands[i] << endl;
	}

	return 0;
}

// PHASE 2:
// - string checking:
// CREATE TABLE Cats(id INT(6), name VARCHAR(30), race VARCHAR(30)); 
// INSERT INTO Cats(id, name, race) VALUES (1, "Cathy", "Vagaboanda");
// DROP TABLE Cats;
// DISPLAY TABLE Cats;
// DELETE FROM Cats WHERE name="Cathy";
// SELECT id | ALL FROM Cats WHERE name="Cathy"; - WHERE condition is optional
// UPDATE Cats SET name="Amanda"
// - array iteration:
// CREATE_TABLE("Cats", [["id", types::INT, 6], ["name", types::VARCHAR, 30], ["race", types::VARCHAR, 30]]);
// INSERT_INTO_VALUES(["id", "name", "race"], [1, "Cathy", "Vagaboanda"]);
// DROP_TABLE("Cats")
// DISPLAY_TABLE("Cats");
// DELETE_FROM_WHERE("Cats", "name", "Cathy"); 
// SELECT_FROM_WHERE(["id"] | ALL, "Cats", "name", "Cathy");
// 
//