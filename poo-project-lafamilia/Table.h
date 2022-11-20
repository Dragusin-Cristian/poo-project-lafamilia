#pragma once
#include <iostream>
#include <string>
#include <assert.h>
#include "ArgumentCreateTable.h";
using namespace std;

class Table {
public:
	// Cristi:
	void createTable(string tableName, ArgumentCreateTable** args, int argsLength) {
		// Used assert for debugging purposes:
		assert(argsLength != 0 && args != nullptr, "Null arguments reference passed.");
		assert(tableName.size() > 0, "Empty table name passed.");

		// do something in phase 2:
		cout << "Table " << tableName << " created with the args ";
		for (int i = 0; i < argsLength; i++) {
			cout << args[i]->getColumnName() << " of type ";
			cout << args[i]->getType() << " and default value ";
			cout << args[i]->getDefaultValue() << ", ";
		}
		cout << endl;
	}

	// Cristi:
	void selectFromTable(string* selectFields, int selectFieldsLength, string tableName) {
		assert(selectFields != nullptr && selectFieldsLength > 0, "Null fields reference passed.");
		assert(tableName.size() > 0, "Empty table name passed.");

		// do something in phase 2:
		cout << "Fields ";
		for (int i = 0; i < selectFieldsLength; i++) {
			cout << selectFields[i] << ", ";
		}
		cout << "selected from table " << tableName << endl;
	}

	// Cristi:
	void updateTable(string tableName, string* fieldsToSet, int fieldsLength, string* values, int valuesLength) {
		assert(fieldsToSet != nullptr && fieldsLength > 0, "Null fields reference passed.");
		assert(values != nullptr && valuesLength > 0, "Null fields reference passed.");
		assert(fieldsLength != valuesLength, "Numbers of fields and values to set are different.");
		assert(tableName.size() > 0, "Empty table name passed.");

		// do something in phase 2:
		cout << "Fields ";
		for (int i = 0; i < fieldsLength; i++) {
			cout << fieldsToSet[i] << ", ";
		}
		cout << "updated to values ";
		for (int i = 0; i < valuesLength; i++) {
			cout << values[i] << ", ";
		}
		cout << endl;
	}

	// Stefan:
	void insertInto();
	void deleteFrom();

	// Andrei:
	void dropTable();
	void displayTable();

private:
	string tableName;
};
