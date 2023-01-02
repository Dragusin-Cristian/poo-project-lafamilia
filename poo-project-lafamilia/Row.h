#pragma once
#include <iostream>
#include <string>
using namespace std;

class Column {
public:

	string name;
	string type;
	string size;
	string defautlValue;

	Column(string name, string type, string size, string defautlValue) {
		this->type = type;
		this->size = size;
		this->defautlValue = defautlValue;
		this->name = name;
	}

	Column() {}
};
