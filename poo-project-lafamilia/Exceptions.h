#pragma once
#include <string>

class Exceptions : public std::exception {
public:
	std::string invalid_command() {
		return "You entered an invalid command. Please type again:\n";
	}

	std::string invalid_paranthesis() {
		return "You missmatched paranthesis. Please type again:\n";
	}
};
