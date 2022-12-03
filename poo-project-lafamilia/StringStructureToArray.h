#pragma once
#include <iostream>
#include <string>
#include "Util.h"; 
#include "Exceptions.h";
using namespace std;

// TODO: MAKE Condition class and UpdateArgs class that inherit this class, and use them instead
class StringStructureToArray {
public:
	string* fields;
	string* values;
	int number;
	
	// Receives sth like: "name="Cathy", gender=0"
	StringStructureToArray(string stringToWorkWith) {
		number = 0;
		this->stringToWorkWith = stringToWorkWith;
		splitConditions();
		splitFieldsAndValues();
	}

	~StringStructureToArray() {
		if (fields != nullptr) {
			fields->~string();
			fields = nullptr;
		}
		if (values != nullptr) {
			values->~string();
			values = nullptr;
		}
		if (conditions != nullptr) {
			conditions->~string();
			conditions = nullptr;
		}
		cout << "StringStructureToArray destructed" << endl;
	}

private:
	string stringToWorkWith;
	string* conditions;

	// "name="Cathy", gender=0"
	void splitConditions() {
		int i = 0;
		while (stringToWorkWith[i] != NULL) {
			if (stringToWorkWith[i] == ',') {
				number++;
			}
			i++;
		}
		number++;
		conditions = new string[number];

		string condition = "";
		int j = 0;
		for (int i = 0; i < stringToWorkWith.size(); i++) {
			if (stringToWorkWith[i] == ',') {
				Util::removeWhiteSpacesBefore(&condition, EMPTY_CONDITION_OR_ARGUMENT);
				Util::removeAllWhiteSpacesAfter(&condition, EMPTY_CONDITION_OR_ARGUMENT);
				conditions[j] = condition;
				j++;
				condition = "";
			}
			else {
				condition.push_back(stringToWorkWith[i]);
			}
		}
		conditions[j] = condition;
	}

	// ["name="Cathy"", "gender=0"]
	void splitFieldsAndValues() {


		fields = new string[number];
		values = new string[number];

		for (int i = 0; i < number; i++) {
			if (conditions[i].find("=") == string::npos)
				throw Exceptions(INVALID_FIELD);

			string field = conditions[i].substr(0, conditions[i].find("="));
			string value = conditions[i].substr(conditions[i].find("=") + 1);

			Util::removeWhiteSpacesBefore(&field, EMPTY_CONDITION_OR_ARGUMENT);
			Util::removeAllWhiteSpacesAfter(&field, EMPTY_CONDITION_OR_ARGUMENT);
			Util::removeWhiteSpacesBefore(&value, EMPTY_CONDITION_OR_ARGUMENT);
			Util::removeAllWhiteSpacesAfter(&value, EMPTY_CONDITION_OR_ARGUMENT);

			if (field == "" || value == "" || value == "\"\"")
				throw Exceptions(INVALID_FIELD);

			if (field.find(" ") != string::npos)
				throw Exceptions(INVALID_FIELD);

			fields[i] = field;
			values[i] = value;
		}
	}
};
