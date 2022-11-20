#pragma once
#include <iostream>
#include <string>
#include "Util.h"; 
using namespace std;

class Condition {
public:
	string* fields;
	string* values;
	int conditionsLength;
	
	// Receives sth like: "name="Cathy", gender=0"
	Condition(string afterWhere) {
		conditionsLength = 0;
		this->afterWhere = afterWhere;
		splitConditions();
		splitFieldsAndValues();
	}

private:
	string afterWhere;
	string* conditions;

	// "name="Cathy", gender=0"
	void splitConditions() {
		int i = 0;
		while (afterWhere[i] != NULL) {
			if (afterWhere[i] == ',') {
				conditionsLength++;
			}
			i++;
		}
		conditionsLength++;
		conditions = new string[conditionsLength];

		string condition = "";
		int j = 0;
		for (int i = 0; i < afterWhere.size(); i++) {
			if (afterWhere[i] == ',') {
				Util::removeWhiteSpacesBefore(&condition);
				Util::removeAllWhiteSpacesAfter(&condition);
				conditions[j] = condition;
				j++;
				condition = "";
			}
			else {
				condition.push_back(afterWhere[i]);
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

			if (field == "" || value == "" || value == "\"\"")
				throw Exceptions(INVALID_CONDITION);

			if (field.find(" ") != string::npos)
				throw Exceptions(INVALID_CONDITION);

			Util::removeWhiteSpacesBefore(&field);
			Util::removeAllWhiteSpacesAfter(&field);
			Util::removeWhiteSpacesBefore(&value);
			Util::removeAllWhiteSpacesAfter(&value);

			fields[i] = field;
			values[i] = value;
		}
	}

};
