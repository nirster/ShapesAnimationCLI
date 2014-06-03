#pragma once
#include "Square.h"
#include "Diamond.h"
#include <exception>

class Factory {
/* this class is responsible for the creation
   of different shapes and assigning them to a general
   Shape pointer */

public:
	 Factory() = default;
	 Factory(const Factory& rhs) = delete;
	 Factory& operator=(const Factory& rhs) = delete;

	 Shape* getShape(const Point& p, char ch, int length, shapeID id);
	 Shape* getShape(const Shape& other);
};

