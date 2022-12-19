#include <iostream>
#include <string>
#include "Exceptions.h";

using namespace std;

class ConsoleInput {
private:
	string rawInput = "";
	string* inputFileNames = new string[5]{"", "", "", "", ""};
	int nrOfFiles = 0;
	static const int LENGTH_OF_PROJECTPOO_EXE;
	

public:

	ConsoleInput()
	{
		// THIS IS FINE JUST FOR DEVELOPMENT MODE. IN PRODUCTION MODE WE WILL PASS THE FILES NAMES THROUGH the main() PARAMETERS:
		do {
			cout << "Please enter \"projectPOO.exe\" followed by the names of the text files you want to input commands from (MAXIMUM 5)\n";
			getline(cin, this->rawInput);
		} 
		while (this->rawInput.substr(0, ConsoleInput::LENGTH_OF_PROJECTPOO_EXE) != "projectPOO.exe");
		
		//store each file name in this->inputFileNames
		string cur_word = "";

		for (int i = LENGTH_OF_PROJECTPOO_EXE; i < this->rawInput.size(); ++i){
			if (this->rawInput[i] == ' ') {

				this->nrOfFiles++;
				i++;
				while (i < this->rawInput.size()  &&  this->rawInput[i] != ' ') {
					cur_word.push_back(this->rawInput[i]);
					i++;
				}

				this->inputFileNames[this->nrOfFiles - 1] = cur_word;
				cur_word = "";
				i--;
			}//endif
		}//endfor

	}//ConsoleInput()

	~ConsoleInput() {
		if (inputFileNames != nullptr) {
			inputFileNames->~string();
			inputFileNames = nullptr;
		}
	}

	string getRawInput() {
		return this->rawInput;
	}

	string* getInputFileNames() {
		//making a copy, because it's not good to return pointers in getters!
		string* cpy = new string[5];
		for (int i = 0; i < 5; i++)
			cpy[i] = this->inputFileNames[i];

		return cpy;
	}

	string getInputFileName(int index) {
		return inputFileNames[index];
	}

	int getNrOfFiles() {
		return this->nrOfFiles;
	}


};

const int ConsoleInput::LENGTH_OF_PROJECTPOO_EXE = 14; //length of string "projectPOO.exe"