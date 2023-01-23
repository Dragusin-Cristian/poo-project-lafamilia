#include <iostream>
#include <fstream>
#include "FileInputs.h";
#include "ArgumentCreateTable.h";
#include "Exceptions.h";
#include "Table.h";
#include "ConsoleInput.h"
using namespace std;

// WHERE WE NEED THIS?
void deleteFI(FileInputs* fi) {
	delete fi;
	fi = nullptr;
}


void workForCommands(string commandString, FileInputs fi, Table table) {
	fi.initializeFi(commandString);

	ArgumentCreateTable** argsArrayCreateTable = new ArgumentCreateTable * [fi.argsLength];

	if (fi.commandType == CREATE_TABLE) {
		for (int i = 0; i < fi.argsLength; i++) {
			argsArrayCreateTable[i] = new ArgumentCreateTable(fi.argsStringArray[i]);
		}
	}


	// ALL THE ACTIONS THAT REQUIRE VALID DATA MUST BE CALLED IN THE END, HERE (BECAUSE OF THE ERROR HANDLING): 

	switch (fi.commandType)
	{
	case CREATE_TABLE:
		table.createTable(fi.tableName, argsArrayCreateTable, fi.argsLength);
		break;
	case SELECT:
		fi.conditions // handles nullptr exception
			? table.selectFromTable(fi.argsStringArray, fi.argsLength, fi.tableName, fi.conditions->fields, fi.conditions->values, fi.conditions->number)
			: table.selectFromTable(fi.argsStringArray, fi.argsLength, fi.tableName);
		break;
	case UPDATE:
		table.updateTable(fi.tableName, fi.updateArgs->fields, fi.updateArgs->values, fi.updateArgs->number, fi.conditions->fields, fi.conditions->values, fi.conditions->number);
		break;
	case INSERT_INTO:
		table.insertInto(fi.argsStringArray, fi.argsLength, fi.tableName);
		break;
	case DELETE_FROM:
		table.deleteFrom(fi.tableName, fi.conditions->fields, fi.conditions->values, fi.conditions->number);
		break;
	case DROP_TABLE:
		table.dropTable(fi.tableName);
		break;
	case DISPLAY_TABLE:
		table.displayTable(fi.tableName);
		break;
	case CREATE_INDEX:
		table.createIndex(fi.tableName, fi.indexName, fi.columnNameForCreateIndex);
		break;
	case DROP_INDEX:
		table.dropIndex(fi.indexName);
		break;
	default:
		break;
	}

	fi.~FileInputs();
	table.~Table();
}

string notImprtant = "";

int main() {

	ConsoleInput ci;

	FileInputs fi;
	Table table;

	try
	{
		if (ci.getNrOfFiles() == 0) {
			throw Exceptions(NO_FILES_PASSED);
		}

		for (int i = 0; i < ci.getNrOfFiles(); i++) {
			string commandString = "";
			ifstream f(ci.getInputFileName(i));
			if (f) {
				while (f) {
					string temp="";
					f >> temp;
					commandString += temp + " ";

					if (temp.find(';') != string::npos) { // last word contains ; means is the end of the command
						Util::removeAllWhiteSpacesAfter(&commandString, INVALID_COMMAND); // remove last space added before
						commandString.pop_back(); // remove the ; from the end of the command
						workForCommands(commandString, fi, table);

						commandString = "";
					}
				}

				// For checking when we have extra white spaces at the end of the file (maybe the user fell asleep on the space bar):
				commandString = Util::trim(commandString);
				if (commandString != "") { // if last command doesn't have ; at the end (becasue if it has, is processed before)
					workForCommands(commandString, fi, table);
				}
				
				f.close();
			}
			else {
				throw Exceptions(FILE_DOES_NOT_EXIST);
				break;
			}
		}

		cout << endl << "Enter anything followed by enter to exit the program." << endl;
		cin >> notImprtant;
		return 0;
	}
	catch (Exceptions e)
	{
		// deinitialize alocated and useless memory:
		ci.~ConsoleInput();
		fi.~FileInputs();
		table.~Table();

		cout << e.handleError();
		main();
	}
}

// PHASE 2:
// - string checking:
// CREATE TABLE Cats(id INT 6, name VARCHAR 30, race VARCHAR 30); //Cristi X
// INSERT INTO Cats(id, name, race) VALUES (1, "Cathy", "Vagaboanda"); // Stefan
// DROP TABLE Cats; // Andrei
// DISPLAY TABLE Cats; // Andrei
// DELETE FROM Cats WHERE name="Cathy"; // Stefan
// SELECT id | ALL FROM Cats WHERE name="Cathy"; - WHERE condition is optional // Cristi X
// UPDATE Cats SET name="Amanda" // Cristi X

// CREATE INDEX [IF NOT EXISTS] index_name ON table_name
// DROP INDEX index_name


// - array iteration:
// CREATE_TABLE("Cats", [["id", types::INT, 6], ["name", types::VARCHAR, 30], ["race", types::VARCHAR, 30]]);
// INSERT_INTO_VALUES(["id", "name", "race"], [1, "Cathy", "Vagaboanda"]);
// DROP_TABLE("Cats")
// DISPLAY_TABLE("Cats");
// DELETE_FROM_WHERE("Cats", "name", "Cathy"); 
// SELECT_FROM_WHERE(["id"] | ALL, "Cats", "name", "Cathy");
// 
//
