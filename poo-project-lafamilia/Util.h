#pragma once

#include <string>

//This is a class for our "global" functions (public static functions) that can be used as general utilities
//in any place of the code.
class Util {
public:
	static const std::string WHITESPACE;

	//This is from stack overflow: https://stackoverflow.com/questions/18972258/index-of-nth-occurrence-of-the-string
	static int nthOccurrence(const std::string& str, const std::string& findMe, int nth)
	{
		size_t  pos = 0;
		int     cnt = 0;

		while (cnt != nth)
		{
			pos += 1;
			pos = str.find(findMe, pos);
			if (pos == std::string::npos)
				return -1;
			cnt++;
		}
		return pos;
	}

	static void removeWhiteSpacesBefore(std::string* str) {
		// remove all white spaces before the actual str content
		while ((*str)[0] == ' ') {
			(*str).erase(0, 1);
		}
	}

	static void removeAllWhiteSpacesAfter(std::string* str) {
		// remove all white spaces after the actual str content
		while ((*str)[(*str).size() - 1] == ' ') {
			(*str).erase((*str).size() - 1, (*str).size());
		}
	}

	//This is modified from stack overflow: https://stackoverflow.com/questions/20303821/how-to-check-if-string-is-in-array-of-strings
	static bool in_array(std::string value, const char* const* array, int size)
	{
		for (int i = 0; i < size; i++){
			if (value == array[i])
				return true;
		}

		return false;
	}

	//These are copy-pasted from somewhere else: https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
	static std::string ltrim(const std::string& s)
	{
		size_t start = s.find_first_not_of(WHITESPACE);
		return (start == std::string::npos) ? "" : s.substr(start);
	}

	static std::string rtrim(const std::string& s)
	{
		size_t end = s.find_last_not_of(WHITESPACE);
		return (end == std::string::npos) ? "" : s.substr(0, end + 1);
	}

	static std::string trim(const std::string& s) {
		return Util::rtrim(Util::ltrim(s));
	}

};

//STATIC CONST VARIABLE DEFINITIONS:
const std::string Util::WHITESPACE = " \n\r\t\f\v";