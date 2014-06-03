#pragma once
#include "Menu.h"

bool Menu::checkOption(int choice) {
	bool res = false;
	for (unsigned int i = 0; i < options.size(); ++i) {
		if (choice == options[i].id && options[i].active == true)
			res = true;
	}

	return res;
}

void Menu::setOption(unsigned int id, string text, bool active) {
	Option op;
	op.id = id;
	op.text = text;
	op.active = active;

	options.push_back(op);
}

int Menu::displayAndGetInput() {
	clrscr();

	int choice;
	bool ok = true;

	for (unsigned int i = 0; i < options.size(); ++i) {
		if (options[i].active == true)
			cout << options[i].id << ".\t" << options[i].text << endl;
	}
	cout << "Your input: ";
	cin >> choice;
	ok = checkOption(choice);

	while (!ok) {
		cout << "Wrong option. Try again:\n";
		cout << "Your input: ";
		cin >> choice;
		ok = checkOption(choice);
	}

	return choice;
}

void Menu::toggleOption(unsigned int id, bool flag) {
	for (unsigned int i = 0; i < options.size(); ++i) {
		if (options[i].id == id)
			options[i].active = flag;
	}
}