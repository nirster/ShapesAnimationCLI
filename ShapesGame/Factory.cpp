#pragma once
#include "Factory.h"

Shape* Factory::getShape(const Point& p, char ch, int length, shapeID id) {
	
	Shape* res = nullptr;

	switch (id) {
	
	case SQUARE:
		res = new Square(p, ch, length, id);
		break;
	
	case DIAMOND:
		res = new Diamond(p, ch, length, id);
		break;
	
	default:
		string error = "Unknown type";
		throw invalid_argument(error);
		break;
	}

	if (!res) 
		throw std::bad_alloc();

	return res;
}