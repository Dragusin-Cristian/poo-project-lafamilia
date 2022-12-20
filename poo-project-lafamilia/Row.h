#pragma once
#include <iostream>
#include <string>
using namespace std;

class Row {
public:

	string name;
	string type;
	string size;
	string defautlValue;

	Row(string name, string type, string size, string defautlValue) {
		this->type = type;
		this->size = size;
		this->defautlValue = defautlValue;
		this->name = name;
	}

	Row() {}
};
