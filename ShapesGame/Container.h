#pragma once
#include "Shape.h"
#include "View.h"
#include <list>
#include <iostream>
#include <fstream>

using std::list;

class View; // forward declaration

class Container {
/* this class holds all the shapes in the program
   including merging, deleting, adding and so on */
	friend class View;

private:
	list<Shape*> lst;
	list<Shape*>::iterator item1, item2;
	list<Shape*>::iterator getIterator(Shape*& s);

	void saveDataAsBinary(const string& filename);
	void loadDataAsBinary(const string& filename);
	void saveDataAsText(const string& filename);
	void loadDataAsText(const string& filename);
	bool validateFileRead(int length, shapeID id, char ch, Point p);
	bool validateAnimationFileRead(const AnimationInterface& ai);
	bool validateSpeed(double x, double y);

public:
	Container() = default;
	Container(const Container& rhs) = delete;
	Container& operator=(const Container& rhs) = delete;
	~Container();

	
	void freeShapes();
	void addShapeToList(Shape*& s);
	void deleteShape(Shape*& s); 
	void mergeShapes(Shape*& s1, Shape*& s2);
	void moveShapeToEnd(Shape*& s); 
	void mergeAnimation(Shape*& s1, Shape*& s2);
	void saveData(const string& saveType, const string& filename);
	void loadData(const string& loadType, const string& filename);
	

};