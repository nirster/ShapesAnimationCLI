#pragma once
#include <chrono>
#include <thread>
#include <list>
#include "Point.h"
#include "Definitions.h"
#include "Shape.h"
#include "Factory.h"

class Container;
typedef struct userInput_ {
	Point p;
	int length;
	char ch;
	shapeID id;
} UserInput;

class View {

private:

	Container* container; 
	Factory factory;
	
	std::list<Shape*>::reverse_iterator userShape;
	UserInput      userData;
	Point		   userPoint;

	UserInput getUserShapeData();
	Point     getUserPoint();
	bool	  validateUserShape(const UserInput& input);
	bool      validateSpeed(double x, double y) const;
	void      clearScreenBuffer(char buffer[][MAX_Y]);
	void      printBufferToScreen(char buffer[][MAX_Y]);
	void      forceDirectionIfPartiallyOutside(Shape*& s);
	bool      checkForCollision(Shape*& s1, Shape*& s2);

public:
	
	View(Container& c);
	View(const View& rhs) = delete;
	View& operator=(const View& rhs) = delete;
	
	Shape* findShape(bool& found);
	void drawAll();
	void createShapeDialog(Shape*& s);
	void blinkLastSearchedPoint();
	void addAnimationDialog(Shape*& s);
	void animateSingleShape(Shape*& s);
	void animateTwoShapes(Shape*& s1, Shape*& s2);
	void saveDataDialog(const string& saveType);
	void loadDataDialog(const string& loadType);
};