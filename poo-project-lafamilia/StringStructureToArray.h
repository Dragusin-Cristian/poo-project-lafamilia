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
	int conditionsLength;
	
	// Receives sth like: "name="Cathy", gender=0"
	StringStructureToArray(string stringToWorkWith) {
		conditionsLength = 0;
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
				conditionsLength++;
			}
			i++;
		}
		conditionsLength++;
		conditions = new string[conditionsLength];

		string condition = "";
		int j = 0;
		for (int i = 0; i < stringToWorkWith.size(); i++) {
			if (stringToWorkWith[i] == ',') {
				Util::removeWhiteSpacesBefore(&condition);
				Util::removeAllWhiteSpacesAfter(&condition);
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


		fields = new string[conditionsLength];
		values = new string[conditionsLength];

		for (int i = 0; i < conditionsLength; i++) {
			if (conditions[i].find("=") == string::npos)
				throw Exceptions(INVALID_CONDITION);

			string field = conditions[i].substr(0, conditions[i].find("="));
			string value = conditions[i].substr(conditions[i].find("=") + 1);

			Util::removeWhiteSpacesBefore(&field);
			Util::removeAllWhiteSpacesAfter(&field);
			Util::removeWhiteSpacesBefore(&value);
			Util::removeAllWhiteSpacesAfter(&value);

			if (field == "" || value == "" || value == "\"\"")
				throw Exceptions(INVALID_CONDITION);

			if (field.find(" ") != string::npos)
				throw Exceptions(INVALID_CONDITION);

			fields[i] = field;
			values[i] = value;
		}
	}
};
