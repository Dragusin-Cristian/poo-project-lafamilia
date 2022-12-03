#include <iostream>
#include "KeyboardInput.h";
#include "ArgumentCreateTable.h";
#include "Exceptions.h";
#include "Table.h";
using namespace std;

void deleteKI(KeyboardInput* ki) {
	delete ki;
	ki = nullptr;
}

int main() {

	KeyboardInput ki;
	ArgumentCreateTable** argsArrayCreateTable;
	Table table;

	try
	{
		ki.initializeKi();

		 argsArrayCreateTable = new ArgumentCreateTable * [ki.argsLength];

		if (ki.commandType == CREATE_TABLE) {
			for (int i = 0; i < ki.argsLength; i++) {
				argsArrayCreateTable[i] = new ArgumentCreateTable(ki.argsStringArray[i]);
			}
		}


		// ALL THE ACTIONS THAT REQUIRE VALID DATA MUST BE CALLED IN THE END, HERE (BECAUSE OF THE ERROR HANDLING): 

		switch (ki.commandType)
		{
		case CREATE_TABLE:
			table.createTable(ki.tableName, argsArrayCreateTable, ki.argsLength);
			break;
		case SELECT:
			ki.conditions // handles nullptr exception
				? table.selectFromTable(ki.argsStringArray, ki.argsLength, ki.tableName, ki.conditions->fields, ki.conditions->values, ki.conditions->number)
				: table.selectFromTable(ki.argsStringArray, ki.argsLength, ki.tableName);
			break;
		case UPDATE:
			table.updateTable(ki.tableName, ki.updateArgs->fields, ki.updateArgs->values, ki.updateArgs->number);
			break;
		case INSERT_INTO:
			//table.insertInto(...)
			break;
		case DELETE_FROM:
			//table.deleteFrom(...)
			break;
		case DROP_TABLE:
			table.dropTable(ki.tableName);
			break;
		case DISPLAY_TABLE:
			table.displayTable(ki.tableName);
			break;
		case CREATE_INDEX:
			table.createIndex(ki.tableName, ki.indexName, ki.columnNameForCreateIndex);
			break;
		case DROP_INDEX:
			table.dropIndex(ki.indexName);
			break;
		default:
			break;
		}

		return 0;
	}
	catch (Exceptions e)
	{
		// deinatialize alocated and useless memory:
		ki.~KeyboardInput();
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
