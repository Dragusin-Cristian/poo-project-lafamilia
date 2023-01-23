#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sys/stat.h> // for folder handling
#include <direct.h> // for folder handling
#include <cctype> // for toUpper function
#include <string>
#include <assert.h>
#include "ArgumentCreateTable.h";
#include "Exceptions.h"
using namespace std;


enum createAndDisplayTableTypes { COUNT_LARGEST_VALUES, DISPLAY_VALUES };
const int DISPLAY_SPACE_BUFFER = 5;
const string LINE = "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------";
enum isDataOrStructure {IS_DATA, IS_STRUCTURE};
enum insertIntoTypes {COUNT_COLUMNS, GET_STRUCTURE};

struct Column {
	string columnName;
	string columnType;
	string columnSize;
	string columnDefValue;

	short dimName;
	short dimType;
	short dimSize;
	short dimDefValue;
};


class Table {
private:
	string getTableAddress(string tableName, isDataOrStructure type) {
		if (type == IS_STRUCTURE) {
			return "Tables/" + tableName + ".structure.bin";
		}
		return "Tables/" + tableName + ".data.bin";
	}

	void workForDisplayTable(ifstream& g, createAndDisplayTableTypes mode, int* largestName, int* largestType, int* largestSize, int* largestDefVal) {
		string nameSize;
		string typeSize;
		string sizeSize;
		string defValSize;
		
		if (mode == DISPLAY_VALUES) {
			nameSize = "%" + to_string(*largestName) + "s";
			typeSize = "%" + to_string(*largestType) + "s";
			sizeSize = "%" + to_string(*largestSize) + "s";
			defValSize = "%" + to_string(*largestDefVal) + "s";


			printf("%.*s\n", *largestName + *largestType + *largestSize + *largestDefVal + 4, LINE.c_str());
			printf(nameSize.c_str(), "NAME"); printf("%s", "|");
			printf(typeSize.c_str(), "TYPE"); printf("%s", "|");
			printf(sizeSize.c_str(), "SIZE"); printf("%s", "|");
			printf(defValSize.c_str(), "DEFAULT VALUE"); printf("%s\n", "|");
			printf("%.*s\n", *largestName + *largestType + *largestSize + *largestDefVal + 4, LINE.c_str());
		
			g.clear();
			g.seekg(0);
		}

		while (!g.eof()) {
			string columnName;
			string columnType;
			string columnSize;
			string columnDefValue;

			short dimName;
			short dimType;
			short dimSize;
			short dimDefValue;


			if (!g.read((char*)&dimName, sizeof(dimName))) break;

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

			//cout << dimName << " " << columnName << " " << dimType << " " << columnType << " " << dimSize << " " << columnSize << " " << dimDefValue << " " << columnDefValue << endl; //FOR DEV MODE ONLY

			if (mode == COUNT_LARGEST_VALUES) {
				((dimName + DISPLAY_SPACE_BUFFER) > *largestName) && (*largestName = (dimName + DISPLAY_SPACE_BUFFER));
				((dimType + DISPLAY_SPACE_BUFFER) > *largestType) && (*largestType = (dimType + DISPLAY_SPACE_BUFFER));
				((dimSize + DISPLAY_SPACE_BUFFER) > *largestSize) && (*largestSize = (dimSize + DISPLAY_SPACE_BUFFER));
				((dimDefValue + DISPLAY_SPACE_BUFFER) > *largestDefVal) && (*largestDefVal = (dimDefValue + DISPLAY_SPACE_BUFFER));
			}

			if (mode == DISPLAY_VALUES) {
				printf(nameSize.c_str(), columnName.c_str()); printf("%s", "|");
				printf(typeSize.c_str(), columnType.c_str()); printf("%s", "|");
				printf(sizeSize.c_str(), columnSize.c_str()); printf("%s", "|");
				printf(defValSize.c_str(), columnDefValue.c_str()); printf("%s\n", "|");
			}
		}

		if (mode == DISPLAY_VALUES) {
			printf("%.*s\n", *largestName + *largestType + *largestSize + *largestDefVal+4, LINE.c_str());
		}

	}

	void workForIsertInto(ifstream& g, int* columnsCounter, insertIntoTypes mode, Column* columns = nullptr) {
		int i = 0;
		while (!g.eof()) {
			short dimName;

			if (!g.read((char*)&dimName, sizeof(dimName))) break;

			if (mode == COUNT_COLUMNS) {
				// this is just for iterating over the bin file:
				char* trash = new char[std::numeric_limits<char>::max()];
				g.read(trash, dimName + 1);
				g.read((char*)&dimName, sizeof(dimName));
				g.read(trash, dimName + 1);
				g.read((char*)&dimName, sizeof(dimName));
				g.read(trash, dimName + 1);
				g.read((char*)&dimName, sizeof(dimName));
				g.read(trash, dimName + 1);
				delete[] trash;

				// what we are interested in, is the count of the columns:
				(*columnsCounter)++;
			}
			else if (mode == GET_STRUCTURE) {
				string columnName;
				string columnType;
				string columnSize;
				string columnDefValue;

				short dimType;
				short dimSize;
				short dimDefValue;

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

				columns[i].dimName = dimName;
				columns[i].dimType = dimType;
				columns[i].dimSize = dimSize;
				columns[i].dimDefValue = dimDefValue;

				columns[i].columnName = columnName;
				columns[i].columnType = columnType;
				columns[i].columnSize = columnSize;
				columns[i].columnDefValue = columnDefValue;
				i++;

			}
		}
	}

	void checkValueValidity(string value, string type, string size) {
		if (type == "INTEGER" ||
			type == "integer" ||
			type == "REAL" ||
			type == "real" ||
			type == "NUMERIC" ||
			type == "numeric")
		{
			// if type is number, then that argument must be made of digits only,
			for (size_t i = 0; i < value.size(); ++i)
				if (!std::isdigit(value[i]))
					throw Exceptions(INVALID_ARGUMENT_INSERT_INTO);

			// then, check if the value is not bigger then the max. size:
			if (stoi(value) > stoi(size)) {
				throw Exceptions(INVALID_ARGUMENT_INSERT_INTO);
			}
		}
		else if (type == "TEXT" || type == "text") {
			// if it's a text, then it should be surounded by ''
			if (value[0] != '\'' || value[value.size() - 1] != '\'') {
				throw Exceptions(INVALID_ARGUMENT_INSERT_INTO);
			}

			// then, check if the length of the string (without the 2 ') is <= size
			if (value.size() >= stoi(size)) {
				throw Exceptions(INVALID_ARGUMENT_INSERT_INTO);
			}
		}
	}
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
		//cout << "Table destructured" << endl;
	}
	// Cristi:
	void createTable(string tableName, ArgumentCreateTable** args, int argsLength) {
		// Used assert for debugging purposes:
		assert(argsLength != 0 && args != nullptr, "Null arguments reference passed.");
		assert(tableName.size() > 0, "Empty table name passed.");

		string fileName = getTableAddress(tableName, IS_STRUCTURE);

		// check if the table already exists:
		struct stat sb;
		if (stat(fileName.c_str(), &sb) == 0 && !(sb.st_mode & S_IFDIR)) { // if already exists
			throw Exceptions(TABLE_ALREADY_EXISTS); // UNCOMMENT THIS AFTER YOU FINISH THIS METHOD!!!!!!!!
		} 

		ofstream f(fileName, ios::binary);
		if (f) {
			for (int i = 0; i < argsLength; i++) {
				// TO-DO: add validation for the default value to respect the type and size!!!
				string columnName;
				string columnType;
				string columnSize;
				string columnDefValue;

				short dimName;
				short dimType;
				short dimSize;
				short dimDefValue;

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

			}
			f.close();
			if (f.good()) {
				cout << endl << "Table " << tableName << " created successfully" << endl;
			}
			else {
				throw Exceptions(ERROR_IN_CREATING_THE_TABLE);
			}
		}
		else {
			f.close();
			throw Exceptions(ERROR_IN_CREATING_THE_TABLE);
		}
	}

	// Cristi:
	void displayTable(string tableName) {
		ifstream g(getTableAddress(tableName, IS_STRUCTURE), ios::binary);

		if (g) {
			int largestName = 20;
			int largestType= 20;
			int largestSize = 20;
			int largestDefValue = 20;

			this->workForDisplayTable(g, COUNT_LARGEST_VALUES, &largestName, &largestType, &largestSize, &largestDefValue);
			this->workForDisplayTable(g, DISPLAY_VALUES, &largestName, &largestType, &largestSize, &largestDefValue);
			
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

		// First, get the structure of the table:
		ifstream g(this->getTableAddress(tableName, IS_STRUCTURE));
		// check if the table was created first:
		if (g.fail()) {
			throw Exceptions(TABLE_DOES_NOT_EXIST);
		}

		// get the structure of the table
		Column* columns;

		// Count the columns:
		int columnsCounter = 0;
		this->workForIsertInto(g, &columnsCounter, COUNT_COLUMNS);
		g.close();


		// Store the columns names, types, and sizes:
		ifstream t(this->getTableAddress(tableName, IS_STRUCTURE));
		columns = new Column[columnsCounter];
		this->workForIsertInto(t, &columnsCounter, GET_STRUCTURE, columns);
		t.close();

		if (selectFieldsLength == 1 && (selectFields[0] == "ALL" || selectFields[0] == "all")) {
			// if select ALL, then display all the columns:

			// Find the positions of which fields we need to check (from the structured table):
			int* posConditions = new int[conditionsLength];
			for (int i = 0; i < conditionsLength; i++) {
				for (int j = 0; j < columnsCounter; j++) {
					if (conditionFileds[i] == columns[j].columnName) {
						posConditions[i] = j;
					}
				}
			}

			
			// the header:
			// calculate table width:
			string header = "";
			for (int i = 0; i < columnsCounter; i++) {
				if (columns[i].columnType == "INTEGER" ||
					columns[i].columnType == "integer" ||
					columns[i].columnType == "REAL" ||
					columns[i].columnType == "real" ||
					columns[i].columnType == "NUMERIC" ||
					columns[i].columnType == "numeric")
				{
					int s = stoi(columns[i].columnSize);
					int actualSizeForString = 0;
					while (s != 0) {
						actualSizeForString++;
						s /= 10;
					}

					for (int k = 0; k < actualSizeForString - columns[i].dimName; k++) {
						header += " ";
					}
				}
				else
				{
					for (int k = 0; k < stoi(columns[i].columnSize) - columns[i].dimName; k++) {
						header += " ";
					}
				}

				header += columns[i].columnName + "|";
			}
			int width = header.size();
			printf("%.*s\n", width, LINE.c_str());
			cout << header<<endl;
			printf("%.*s\n", width, LINE.c_str());

			// the body:
			ifstream f(getTableAddress(tableName, IS_DATA), ios::binary);
			if (f) {
				string rowString = "";

				int i = 0;
				bool conditionPassed = true;
				while (!f.eof()) {
					short dim;

					if (!f.read((char*)&dim, sizeof(dim))) break;

					string value;
					char* d = new char[dim + 1];
					f.read(d, dim + 1);
					value = d;
					delete[] d;

					// If we are at the position of a condition:
					int* conditionPosition = find(posConditions, posConditions + conditionsLength, i);
					if (conditionPosition != (posConditions + conditionsLength)) {
						for (int j = 0; j < conditionsLength; j++) {
							if (posConditions[j] == (*conditionPosition)) {
								if (value != conditionValues[j]) {
									conditionPassed = false;
								}
							}
						}
					}

					// we add the record to the rowString:
					if (columns[i].columnType == "INTEGER" || 
						columns[i].columnType == "integer" || 
						columns[i].columnType == "REAL" || 
						columns[i].columnType == "real" ||
						columns[i].columnType == "NUMERIC" ||
						columns[i].columnType == "numeric")
					{
						int s = stoi(columns[i].columnSize);
						int actualSizeForString = 0;
						while (s != 0) {
							actualSizeForString++;
							s /= 10;
						}

						if (actualSizeForString < columns[i].dimName) {
							actualSizeForString = columns[i].dimName;
						}
						for (int k = 0; k < actualSizeForString - value.size(); k++) {
							rowString += " ";
						}
					}
					else
					{	
						int columnSize = stoi(columns[i].columnSize);
						if (columnSize < columns[i].dimName) {
							columnSize = columns[i].dimName;
						}
						for (int k = 0; k < columnSize - value.size(); k++) {
							rowString += " ";
						}
					}

					rowString += value+"|";

					i++;
					// when we get to the end of the row:
					if (i % columnsCounter == 0) {
						// if we pass the condition, then do the cout:
						if (conditionPassed) {
							cout << rowString << endl;
						}
						conditionPassed = true;
						rowString = "";
						i = 0;
					}
				}
				printf("%.*s\n", width, LINE.c_str());
				f.close();
			}
			else {
				throw Exceptions(TABLE_IS_EMPTY);
				f.close();
			}
		}
		else {
				// If we have defined the fields, then display only them:
				
				// We reorder the selectFields as in the columns structure:
				string* temp = new string[selectFieldsLength];
				int a = 0;
				for (int i = 0; i < columnsCounter; i++) {
					for (int j = 0; j < selectFieldsLength; j++) {
						if (columns[i].columnName == selectFields[j]) {
							temp[a] = selectFields[j];
							a++;
						}
					}
				}
				selectFields = temp;
				
				// Print the header:
				// caluclate table width:
				string header = "";
				for (int i = 0; i < selectFieldsLength; i++) {
					// find the corresponding column from the table structure:
					Column c;
					for (int j = 0; j < columnsCounter; j++) {
						if (selectFields[i] == columns[j].columnName) {
							c = columns[j];
						}
					}
					
					if (c.columnType == "INTEGER" ||
						c.columnType == "integer" ||
						c.columnType == "REAL" ||
						c.columnType == "real" ||
						c.columnType == "NUMERIC" ||
						c.columnType == "numeric")
					{
						int s = stoi(c.columnSize);
						int actualSizeForString = 0;
						while (s != 0) {
							actualSizeForString++;
							s /= 10;
						}

						for (int k = 0; k < actualSizeForString - c.dimName; k++) {
							header += " ";
						}
					}
					else
					{
						for (int k = 0; k < stoi(c.columnSize) - c.dimName; k++) {
							header += " ";
						}
					}

					header += c.columnName + "|";
				}
				int width = header.size();
				printf("%.*s\n", width, LINE.c_str());
				cout << header << endl;
				printf("%.*s\n", width, LINE.c_str());


				// Find the positions of the desired fields in the structured table:
				int* pos = new int[selectFieldsLength];
				for (int i = 0; i < columnsCounter; i++) {
					for (int j = 0; j < selectFieldsLength; j++) {
						if (columns[i].columnName == selectFields[j]) {
							pos[j] = i;
							break;
						}
					}
				}

				// Find the positions of which fields we need to check, aka the conditions (from the structured table):
				int* posConditions = new int[conditionsLength];
				for (int i = 0; i < conditionsLength; i++) {
					for (int j = 0; j < columnsCounter; j++) {
						if (conditionFileds[i] == columns[j].columnName) {
							posConditions[i] = j;
						}
					}
				}

				// the body:
				ifstream f(getTableAddress(tableName, IS_DATA), ios::binary);
				if (f) {
					int i = 0;
					string rowString = "";
					bool conditionPassed = true;
					while (!f.eof()) {
						short dim;

						if (!f.read((char*)&dim, sizeof(dim))) break;

						string value;
						char* d = new char[dim + 1];
						f.read(d, dim + 1);
						value = d;
						delete[] d;

						//// If we are at the position of a condition:
						int* conditionPosition = find(posConditions, posConditions + conditionsLength, i);
						if (conditionPosition != (posConditions + conditionsLength)) {
							for (int j = 0; j < conditionsLength; j++) {
								if (posConditions[j] == (*conditionPosition)) {
									if (value != conditionValues[j]) {
										conditionPassed = false;
									}
								}
							}
						}

						// If we are at the position of a desired column value:
						if(find(pos, pos + selectFieldsLength, i) != pos + selectFieldsLength){
							// we add the record to the rowString:
							if (columns[i].columnType == "INTEGER" ||
								columns[i].columnType == "integer" ||
								columns[i].columnType == "REAL" ||
								columns[i].columnType == "real" ||
								columns[i].columnType == "NUMERIC" ||
								columns[i].columnType == "numeric")
							{
								int s = stoi(columns[i].columnSize);
								int actualSizeForString = 0;
								while (s != 0) {
									actualSizeForString++;
									s /= 10;
								}

								if (actualSizeForString < columns[i].dimName) {
									actualSizeForString = columns[i].dimName;
								}
								for (int k = 0; k < actualSizeForString - value.size(); k++) {
									rowString += " ";
								}
							}
							else
							{
								int columnSize = stoi(columns[i].columnSize);
								if (columnSize < columns[i].dimName) {
									columnSize = columns[i].dimName;
								}
								for (int k = 0; k < columnSize - value.size(); k++) {
									rowString += " ";
								}
							}

							rowString += value + "|";

						}
						i++;

						// when we get to the end of the row:
						if (i % columnsCounter == 0) {
							// if we pass the condition, then do the cout:
							if (conditionPassed) {
								cout << rowString << endl;
							}
							conditionPassed = true;
							rowString = "";
							i = 0;
						}

					}
					printf("%.*s\n", width, LINE.c_str());
					f.close();
				}
				else {
					throw Exceptions(TABLE_DOES_NOT_EXIST);
					f.close();
				}
		}

		cout << endl;
	}

		

	// -- NOT IMPLEMENTED:
	void updateTable(string tableName, string* fieldsToSet = nullptr, string* valuesToSet = nullptr, int toSetLength = NULL, 
					 string* conditionsFields = nullptr, string* conditionsValues = nullptr, int conditionsLength = NULL) 
	{
		assert(fieldsToSet != nullptr && valuesToSet != nullptr && toSetLength > 0, "Null fields reference passed.");
		assert(tableName.size() > 0, "Empty table name passed.");

		cout << endl << endl;
		for (int i = 0; i < toSetLength; i++) {
			cout << fieldsToSet[i] << " " << valuesToSet[i] << endl; 
		}
		cout << endl << endl;
		for (int i = 0; i < conditionsLength; i++) {
			cout << conditionsFields[i] << " " << conditionsValues[i] << endl;
		}
		cout << endl;
	}

	// -- NOT IMPLEMENTED:
	void createIndex(string tableName, string indexName, string columnName) {
		assert(tableName != "" && indexName != "" && columnName != "", "Empty table name or index name or column name.");

		cout << "Index " << indexName << " created on table " << tableName << " on " << columnName << " column." << endl;
	}


	// Cristi:
	void insertInto(string* argsArray, int argsLength, string tableName) {
		assert(argsArray != nullptr, "Args array cannot be nullptr.");
		assert(argsLength > 0, "Args array length must be higher then 0.");
		assert(tableName.size() > 0, "Cannot have empty table name");

		ifstream g(this->getTableAddress(tableName, IS_STRUCTURE)); 
		// check if the table was created first:
		if (g.fail()) {
			throw Exceptions(TABLE_DOES_NOT_EXIST);
		}

		// get the structure of the table
		Column* columns;

		// First, we need to count the columns:
		int columnsCounter = 0;
		this->workForIsertInto(g, &columnsCounter, COUNT_COLUMNS);
		g.close();
		

		// Second, we need to store the columns names, types, and sizes:
		ifstream t(this->getTableAddress(tableName, IS_STRUCTURE));
		columns = new Column[columnsCounter];
		this->workForIsertInto(t, &columnsCounter, GET_STRUCTURE, columns);
		t.close();

		// validate the values (number of fields, corresponding types)
		if (argsLength != columnsCounter) {
			throw Exceptions(INVALID_NUMBER_OF_ARGUMENTS_FOR_INSERT_INTO);
		}
		for (int i = 0; i < columnsCounter; i++) {
			this->checkValueValidity(argsArray[i], columns[i].columnType, columns[i].columnSize);
		}

		// Finally, insert the values in append mode:
		ofstream f(this->getTableAddress(tableName, IS_DATA), ios_base::app);

		if (f) {
			for (int i = 0; i < argsLength; i++) {
				string value;
				short dim;

				value = argsArray[i];
				dim = value.size();

				f.write((char*)&dim, sizeof(dim));
				f.write(value.c_str(), dim + 1);

			}
			f.close();
			if (f.good()) {
				cout << endl << "Values inserted successfully in table " << tableName << "!" << endl << endl;
			}
			else {
				throw Exceptions(ERROR_IN_INSERTING_INTO);
			}
		}
		else {
			f.close();
			throw Exceptions(ERROR_IN_INSERTING_INTO);
		}

	}
	
	// -- NOT IMPLEMENTED:
	void deleteFrom(string tableName, string* fields = nullptr, string* values = nullptr, int length = NULL) {
		cout << endl;
		cout << tableName << endl;
		for (int i = 0; i < length; i++) {
			cout << fields[i] << " " << values[i] << endl;
		}
		cout << endl;
	}

	// Cristi:
	void dropTable(string tableName) {
		assert(tableName != "", "Empty table name passed.");

		int status = remove(this->getTableAddress(tableName, IS_STRUCTURE).c_str());
		if (status == 0)
			cout << "\nTable structure deleted successfully!";
		else
			throw Exceptions(FILE_DELETION_FAILED);

		status = remove(this->getTableAddress(tableName, IS_DATA).c_str());
		if (status == 0)
			cout << "\nTable data deleted successfully!";
		else
			cout << "\nTable data didn't exist, so it couldn't be deleted.";
	}

	// -- NOT IMPLEMENTED:
	void dropIndex(string indexName) {
		assert(indexName != "", "Empty index name passed.");
		cout << "Index " << indexName << " was dropped (deleted)." << endl;
	}

private:
	string tableName;
};
