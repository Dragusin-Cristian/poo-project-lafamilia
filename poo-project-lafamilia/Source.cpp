#include <iostream>
#include "KeyboardInput.h";
#include "ArgumentCreateTable.h";
using namespace std;

int main() {

	KeyboardInput ki;

	ki.interpretCommands();
	ki.interpretArguments();


	ArgumentCreateTable** argsArrayCreateTable = new ArgumentCreateTable *[ki.argsLength];
	
	for (int i = 0; i < ki.argsLength; i++) {
		argsArrayCreateTable[i] = new ArgumentCreateTable(ki.argsStringArray[i]);
	}


	return 0;
}

// PHASE 2:
// - string checking:
// CREATE TABLE Cats(id INT 6, name VARCHAR 30, race VARCHAR 30); X
// INSERT INTO Cats(id, name, race) VALUES (1, "Cathy", "Vagaboanda"); // Stefan
// DROP TABLE Cats; // Andrei
// DISPLAY TABLE Cats; // Andrei
// DELETE FROM Cats WHERE name="Cathy"; // Stefan
// SELECT id | ALL FROM Cats WHERE name="Cathy"; - WHERE condition is optional // Cristi
// UPDATE Cats SET name="Amanda" // Cristi


// - array iteration:
// CREATE_TABLE("Cats", [["id", types::INT, 6], ["name", types::VARCHAR, 30], ["race", types::VARCHAR, 30]]);
// INSERT_INTO_VALUES(["id", "name", "race"], [1, "Cathy", "Vagaboanda"]);
// DROP_TABLE("Cats")
// DISPLAY_TABLE("Cats");
// DELETE_FROM_WHERE("Cats", "name", "Cathy"); 
// SELECT_FROM_WHERE(["id"] | ALL, "Cats", "name", "Cathy");
// 
//