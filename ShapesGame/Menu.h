#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Definitions.h"
using namespace std;

class Menu {
	/* this class is the menu of the program
	   with ability to add/remove/disable options
	   on runtime */
private:
	typedef struct option {
		bool active;
		std::string text;
		unsigned int id;
	} Option;
	
	vector<Option> options;
	bool checkOption(int choice);

public:
	Menu() = default;
	Menu(const Menu& rhs) = delete;
	Menu& operator=(const Menu& rhs) = delete;
	void setOption(unsigned int id, string text, bool active);
	void toggleOption(unsigned int id, bool flag);
	int displayAndGetInput();
};