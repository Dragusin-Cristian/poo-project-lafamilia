#include <iostream>
#include "KeyboardInput.h";
#include "ArgumentCreateTable.h";
#include "Exceptions.h";
using namespace std;

void deleteKI(KeyboardInput* ki) {
	delete ki;
	ki = nullptr;
}

int main() {
	// TO-DO LATER: Handle memory leaks:
	try
	{
		KeyboardInput ki;

		ArgumentCreateTable** argsArrayCreateTable = new ArgumentCreateTable * [ki.argsLength];

		for (int i = 0; i < ki.argsLength; i++) {
			argsArrayCreateTable[i] = new ArgumentCreateTable(ki.argsStringArray[i]);
		}

		// ALL THE ACTIONS THAT REQUIRE VALID DATA MUST BE CALLED IN THE END, HERE (BECAUSE OF THE ERROR HANDLING): 
		cout << ki.command << endl;
		ki.interpretArguments();

		return 0;
	}
	catch (Exceptions e)
	{
		cout << e.handleError();
		main();
	}

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