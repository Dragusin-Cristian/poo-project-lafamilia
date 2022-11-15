#include <iostream>
#include "KeyboardInput.h";
using namespace std;

int main() {

	KeyboardInput ki;

	ki.interpretCommands();
	ki.interpretArguments();

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