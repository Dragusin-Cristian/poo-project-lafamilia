#include <string>
using namespace std;
#pragma once

// TABLE IDEAS, STILL NO NEED TO IMPLEMENT

const string
CREATE = "CREATE",
INSERT = "INSERT",
DROP = "DROP",
DELETE = "DELETE",
DISPLAY = "DISPLAY",
SELECT = "SELECT",
UPDATE = "UPDATE";

enum dataTypes { VARCHAR, INT, BOOLEAN };


struct field {
	string fieldName;
	dataTypes fieldType;
	int fieldLength = 0;
};

class Table {
private:
	string tableName;
	field fields[3];

	//Table(string tableName, field args[]) {
	//	this->tableName = tableName;
	//	for (int i = 0; i < args.; i++) {
	//		cout << args[i].fieldName;
	//	}
	//}

public:
	Table CREATE_TABLE(string tableName, field args[]) {
		//return Table(tableName, args);
	}
};


