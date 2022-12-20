#pragma once
#include <iostream>
#include <fstream>
#include <sys/stat.h> // for folder handling
#include <direct.h> // for folder handling
#include <cctype> // for toUpper function
#include <string>
#include <assert.h>
#include "ArgumentCreateTable.h";
#include "Exceptions.h"
using namespace std;

struct Row {
	string name;
	string type;
	string size;
	string defautlValue;
};

class Table {
public:

	Table() {
		struct stat info;

		if (stat("Tables", &info) != 0){ // directory doesn't exist
			if (_mkdir("Tables") == -1) { // error in creating the directory
				throw Exceptions(ERROR_CREATING_TABLES_FOLDER);
			}	
		}
	}

	~Table() {
		cout << "Table destructured" << endl;
	}
	// Cristi:
	void createTable(string tableName, ArgumentCreateTable** args, int argsLength) {
		// Used assert for debugging purposes:
		assert(argsLength != 0 && args != nullptr, "Null arguments reference passed.");
		assert(tableName.size() > 0, "Empty table name passed.");

		string fileName = "Tables/" + tableName + ".bin";

		// check if the table already exists:
		struct stat sb;
		if (stat(fileName.c_str(), &sb) == 0 && !(sb.st_mode & S_IFDIR)) { // if already exists
			//throw Exceptions(TABLE_ALREADY_EXISTS); // UNCOMMENT THIS AFTER YOU FINISH THIS METHOD!!!!!!!!
		} 


		ofstream f(fileName, ios::binary);
		if (f) {
			cout << "FROM CREATE TABLE METHOD:" << endl;

			string columnName;
			string columnType;
			string columnSize;
			string columnDefValue;

			short dimName;
			short dimType;
			short dimSize;
			short dimDefValue;

			for (int i = 0; i < argsLength; i++) {

				columnName = args[i]->getColumnName();
				columnType = args[i]->getType();
				columnSize = args[i]->getSize();
				columnDefValue = args[i]->getDefaultValue();

				dimName = columnName.length();
				dimType = columnType.length();
				dimSize = columnSize.length();
				dimDefValue = columnDefValue.length();

				f.write((char*)&dimName, sizeof(dimName)); // we need the length of the string when we will be reading from the file
				f.write(columnName.c_str(), dimName + 1);

				f.write((char*)&dimType, sizeof(dimType));
				f.write(columnType.c_str(), dimType + 1);

				f.write((char*)&dimSize, sizeof(dimSize));
				f.write(columnSize.c_str(), dimSize + 1);

				f.write((char*)&dimDefValue, sizeof(dimDefValue));
				f.write(columnDefValue.c_str(), dimDefValue + 1);

				f.close();

				cout << dimName << " " << columnName << " " << dimType << " " << columnType << " " << dimSize << " " << columnSize << " " << dimDefValue << " " << columnDefValue << endl;;

			}

			this->displayTable(tableName);
		}
		else {
			f.close();
			throw Exceptions(ERROR_IN_CREATING_THE_TABLE);
		}


		//// do something in phase 2:
		//cout << "Table " << tableName << " created with the args ";
		//for (int i = 0; i < argsLength; i++) {
		//	cout << args[i]->getColumnName() << " of type ";
		//	cout << args[i]->getType() << " having an allocated size of ";
		//	cout << args[i]->getSize() << " and default value ";
		//	cout << args[i]->getDefaultValue() << ", ";
		//}
		//cout << endl;
	}

	// WE SHOULD REFACTOR THIS TO DISPLAY A REAL TABLE:
	void displayTable(string tableName) {
		ifstream g("Tables/" + tableName + ".bin", ios::binary);



		if (g) {
			cout << "FROM DISPLAY METHOD:" << endl;
			string columnName;
			string columnType;
			string columnSize;
			string columnDefValue;

			short dimName;
			short dimType;
			short dimSize;
			short dimDefValue;
			for (int i = 0; i < 3; i++) {

				
				g.read((char*)&dimName, sizeof(dimName));

				char* n = new char[dimName + 1];
				g.read(n, dimName + 1);
				columnName = n;
				delete[] n;

				
				g.read((char*)&dimType, sizeof(dimType));

				char* t = new char[dimType + 1];
				g.read(t, dimType + 1);
				columnType = t;
				delete[] t;

				
				g.read((char*)&dimSize, sizeof(dimSize));

				char* s = new char[dimSize + 1];
				g.read(s, dimSize + 1);
				columnSize = s;
				delete[] s;

				
				g.read((char*)&dimDefValue, sizeof(dimDefValue));

				char* d = new char[dimDefValue + 1];
				g.read(d, dimDefValue + 1);
				columnDefValue = d;
				delete[] d;

				cout << dimName << " " << columnName << " " << dimType << " " << columnType << " " << dimSize << " " << columnSize << " " << dimDefValue << " " << columnDefValue << endl;;

			}

			g.close();
		}
		else {
			g.close();
			throw Exceptions(TABLE_DOES_NOT_EXIST);
		}
	}


	// Cristi:
	void selectFromTable(string* selectFields, int selectFieldsLength, string tableName, string* conditionFileds=nullptr, string* conditionValues=nullptr, int conditionsLength=NULL) {
		assert(selectFields != nullptr && selectFieldsLength > 0, "Null fields reference passed.");
		assert(tableName.size() > 0, "Empty table name passed.");

		ifstream f("Tables/" + tableName + ".bin", ios::binary);

		if (f) {
			string temp;
			f >> temp;
			cout << endl << endl;
			cout << temp;

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


			cout<< endl << endl;
			f.close();
		}
		else {
			f.close();
			throw Exceptions(TABLE_DOES_NOT_EXIST);
		}
		

		//// do something in phase 2:
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

private:
	string tableName;
};
