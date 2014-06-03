#pragma once
#include "Controller.h"

Controller::Controller() : view(container) {
	s1 = nullptr;
	s2 = nullptr;
	foundShape1 = false;
	foundShape2 = false;
}

Controller::~Controller() {
}

void Controller::setupMainMenu() {
	mainMenu.setOption(1, "Add Shape", true);
	mainMenu.setOption(2, "Draw Shapes", true);
	mainMenu.setOption(3, "Choose Shape", true);
	mainMenu.setOption(4, "Save to text file", true);
	mainMenu.setOption(5, "Load text file", true);
	mainMenu.setOption(6, "Save to binary file", true);
	mainMenu.setOption(7, "Load binary file", true);
	mainMenu.setOption(8, "Quit", true);
}

void Controller::setupSubMenu() {
	subMenu.setOption(1, "Cancel Selection (Go Back)", true);
	subMenu.setOption(2, "Delete Selected Shape", true);
	subMenu.setOption(3, "Push Selected Shape to Front", true);
	subMenu.setOption(4, "Merge with another Shape", true);
	subMenu.setOption(5, "Add Animation", true);
	subMenu.setOption(6, "Show Double Animation (Select another shape)", true);
}

void Controller::escListen() const {
	int ch;
	ch = _getch();

	while (ch != ESC_CODE) {
		ch = _getch();
	}
	clrscr();
}

void Controller::run() {
	userSelection = mainMenu.displayAndGetInput();
	while (userSelection != 8) {
		switch (userSelection) {
		case 1: // create new shape and add to the list
			try {
			newShapeAction();
			}
			catch (const std::bad_alloc& e) {
				cerr << e.what() << endl;
				cout << "Trying again:\n";
				s1 = nullptr;
				newShapeAction();
			}
			break;

		case 2: // draw all the shapes in the list
			drawShapesAction();
			break;

		case 3: // select a shape by giving coordinate
			selectShapeAction();
			break;

		case 4: // save as text
			try {
			saveTextAction();
			}
			catch (const std::ios::failure& e) {
				cerr << e.what() << endl;
				cout << "error opening file. Trying again\n";
				saveTextAction();
			}
			break;

		case 5: // load text
			try {
			loadTextAction();
			}
			catch (const std::ios::failure& e) {
				cerr << e.what() << endl;
				cout << "error opening file. Trying again\n";
				loadTextAction();
			}
			break;

		case 6: // save binary
			try {
			saveBinaryAction();
			}
			catch (const std::ios::failure& e) {
				cerr << e.what() << endl;
				cout << "error opening file. Trying again\n";
				saveBinaryAction();
			}
			break;

		case 7: // load binary
			try {
			loadBinaryAction();
			}
			catch (const std::ios::failure& e) {
				cerr << e.what() << endl;
				cout << "error opening file. Trying again\n";
				loadBinaryAction();
			}
			catch (const string& e) {
				cerr << e << endl;
				cerr << "Trying again.\n";
				loadBinaryAction();
			}
			break;

		case 8:
			quit();
			break;
		}
		userSelection = mainMenu.displayAndGetInput();
	}
}

void Controller::runSubMenu() {
	clrscr();
	userSubSelection = subMenu.displayAndGetInput();

	switch (userSubSelection) {
	case 1: // go back to main menu, deselect currently selected shape
		goBackAction();
		break;

	case 2: // delete the selected shape
		try {
		deleteShapeAction();
		}
		catch (const string& e) {
			cerr << e;
			cerr << "Trying again: ";
			deleteShapeAction();
		}
		break;

	case 3: // move the selected shape to end of list
		try {
		moveShapeToEndAction();
		}
		catch (const string& e) {
			cerr << e;
			cerr << "Trying again: ";
			deleteShapeAction();
		}
		break;

	case 4: // ask user for another shape to merge with the currently selected
		mergeAction();
		break;

	case 5:
		singleAnimationAction();
		break;

	case 6:
		doubleAnimationAction();
		break;
	}
}

void Controller::newShapeAction() {
	view.createShapeDialog(s1);
	container.addShapeToList(s1);
	view.drawAll();
	escListen();
}

void Controller::drawShapesAction() {
	view.drawAll();
	escListen();
}

void Controller::selectShapeAction() {
	s1 = view.findShape(foundShape1);
	view.drawAll();

	if (!foundShape1)
		view.blinkLastSearchedPoint();
	else {
		s1->draw('@');
		view.blinkLastSearchedPoint();
	}

	if (s1 != nullptr) // a shape was found
		runSubMenu();
}

void Controller::saveTextAction() {
	view.saveDataDialog("text");
}

void Controller::loadTextAction() {
	view.loadDataDialog("text");
	resetFoundFlags();
	resetShapePointers();
	view.drawAll();
	escListen();
}

void Controller::saveBinaryAction() {
	view.saveDataDialog("binary");
}

void Controller::loadBinaryAction() {
	view.loadDataDialog("binary");
	resetFoundFlags();
	resetShapePointers();
	view.drawAll();
	escListen();
}

void Controller::goBackAction() {
	resetFoundFlags(); // when user goes back the selection is lost
	resetShapePointers();
	run();
}

void Controller::deleteShapeAction() {
	if (s1 == nullptr) {
		string err = "trying to access null pointer";
		throw string(err);
	}
	container.deleteShape(s1);
	view.drawAll();
	escListen();
	resetFoundFlags();
	resetShapePointers();
}

void Controller::moveShapeToEndAction() {
	if (s1 == nullptr) {
		string err = "trying to access null pointer";
		throw string(err);
	}
	container.moveShapeToEnd(s1);
	view.drawAll();
	escListen();
	resetFoundFlags();
	resetShapePointers();
}

void Controller::mergeAction() {
	// ask user for another shape to animate with the selected shape
	s2 = view.findShape(foundShape2);
	view.drawAll();

	if (!foundShape2)
		view.blinkLastSearchedPoint();

	else {
		container.mergeShapes(s1, s2);
		view.drawAll();
		escListen();
	}
	resetFoundFlags();
	resetShapePointers();
}

void Controller::singleAnimationAction() {
	// add animation to selected shape
	view.addAnimationDialog(s1);
	view.animateSingleShape(s1);
	runSubMenu();
}

void Controller::doubleAnimationAction() {
	// ask user for another shape to animate with the selected shape
	view.addAnimationDialog(s1);
	s2 = view.findShape(foundShape2);
	if (!foundShape2) {
		view.drawAll();
		view.blinkLastSearchedPoint();
		runSubMenu();
	}
	else {
		view.addAnimationDialog(s2);
		view.animateTwoShapes(s1, s2);
		runSubMenu();
	}
}

void Controller::init() {
	setupMainMenu();
	setupSubMenu();
}

void Controller::quit() {
	exit(EXIT_SUCCESS);
}