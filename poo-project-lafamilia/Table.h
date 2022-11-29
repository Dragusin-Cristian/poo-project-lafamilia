#pragma once
#include <iostream>
#include <string>
#include <assert.h>
#include "ArgumentCreateTable.h";
using namespace std;

class Table {
public:

	~Table() {
		cout << "Table destructured" << endl;
	}
	// Cristi:
	void createTable(string tableName, ArgumentCreateTable** args, int argsLength) {
		// Used assert for debugging purposes:
		assert(argsLength != 0 && args != nullptr, "Null arguments reference passed.");
		assert(tableName.size() > 0, "Empty table name passed.");

		// do something in phase 2:
		cout << "Table " << tableName << " created with the args ";
		for (int i = 0; i < argsLength; i++) {
			cout << args[i]->getColumnName() << " of type ";
			cout << args[i]->getType() << " having an allocated size of ";
			cout << args[i]->getSize() << " and default value ";
			cout << args[i]->getDefaultValue() << ", ";
		}
		cout << endl;
	}

	// Cristi:
	void selectFromTable(string* selectFields, int selectFieldsLength, string tableName, string* conditionFileds=nullptr, string* conditionValues=nullptr, int conditionsLength=NULL) {
		assert(selectFields != nullptr && selectFieldsLength > 0, "Null fields reference passed.");
		assert(tableName.size() > 0, "Empty table name passed.");

		// do something in phase 2:
		cout << "Fields ";
		for (int i = 0; i < selectFieldsLength; i++) {
			cout << selectFields[i] << ", ";
		}
		cout << "selected from table " << tableName << endl;
		if (conditionFileds != nullptr && conditionValues != nullptr && conditionsLength > 0) {
			cout << "with the conditions: ";

			for (int i = 0; i < conditionsLength; i++) {
				cout << conditionFileds[i] << " = " << conditionValues[i] << ", ";
			}
		}

	}

	// Cristi:
	void updateTable(string tableName, string* fieldsToSet=nullptr, string* values=nullptr, int fieldsLength=NULL) {
		assert(fieldsToSet != nullptr && values != nullptr && fieldsLength > 0, "Null fields reference passed.");
		assert(tableName.size() > 0, "Empty table name passed.");

		// do something in phase 2:
		cout << "Fields ";
		for (int i = 0; i < fieldsLength; i++) {
			cout << fieldsToSet[i] << ", ";
		}
		cout << "from table "<< tableName <<" updated to values ";
		for (int i = 0; i < fieldsLength; i++) {
			cout << values[i] << ", ";
		}
		cout << endl;
	}

	// Cristi:
	void createIndex(string tableName, string indexName, string columnName) {
		assert(tableName != "" && indexName != "" && columnName != "", "Empty table name or index name or column name.");

		cout << "Index " << indexName << " created on table " << tableName << " on " << columnName << " column." << endl;
	}

	// Stefan:
	void insertInto();
	void deleteFrom();

	// Andrei:
	void dropTable(string tableName) {
		assert(tableName != "", "Empty table name passed.");
		cout << "Table " << tableName << " was dropped (deleted)." << endl;
	}
	void dropIndex(string indexName) {
		assert(indexName != "", "Empty index name passed.");
		cout << "Index " << indexName << " was dropped (deleted)." << endl;
	}
	void displayTable(string tableName) {
		assert(tableName != "", "Empty table name passed.");
		cout << "Table " << tableName << " is displayed." << endl;
	}

private:
	string tableName;
};
