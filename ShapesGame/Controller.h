#pragma once
#include "Menu.h"
#include "Square.h"
#include "View.h"
#include "Container.h"

class Controller {

private:
	Container container;
	View      view;
	
	Menu mainMenu,
		 subMenu;

	Shape *s1, 
		  *s2;

	int userSelection, userSubSelection;
	bool foundShape1, foundShape2;

	void setupMainMenu();
	void setupSubMenu();
	void runSubMenu();
	void escListen() const;
	void resetFoundFlags() { foundShape1 = foundShape2 = false; }
	void resetShapePointers() { s1 = s2 = nullptr; }
	
	// Main menu actions
	void showMainAction();
	void newShapeAction();
	void drawShapesAction();
	void selectShapeAction();
	void saveTextAction();
	void loadTextAction();
	void saveBinaryAction();
	void loadBinaryAction();
	void quit();

	// Sub menu actions
	void goBackAction();
	void deleteShapeAction();
	void moveShapeToEndAction();
	void mergeAction();
	void singleAnimationAction();
	void doubleAnimationAction();
	

public:
	Controller();
	Controller(const Controller& rhs) = delete;
	Controller& operator=(const Controller& rhs) = delete;
	~Controller();
	void run();
	void init();
};