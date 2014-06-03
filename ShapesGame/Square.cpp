#pragma once
#include "Square.h"

Square::Square(const Point& p, char ch, int length, shapeID id): 
									Shape(p, ch, length) {
	this->id = id;
	ai = nullptr;
}

Square& Square::operator=(const Square& other) {

	if (this == &other)
		return *this;

	p = other.p;
	ch = other.ch;
	length = other.length;
	id = other.id;
	ai = nullptr;

	if (other.ai != nullptr)
		addAnimation();

	return *this;
}

void Square::draw() const {
	
	int cordY = p.getY(), cordX = p.getX();
	char ch = this->ch;
	int lengthX, lengthY;
	lengthX = lengthY = length;

	gotoxy(p.getX(), p.getY());

	if (p.getX() < 0) {
		if (cordX + length >= 0) {
			lengthX = length + cordX;
			cordX = 0;

		}
		else
			return;
	}

	if (p.getY() < 0) {
		if (cordY + length >= 0) {
			lengthY = length + cordY;
			cordY = 0;

		}
		else
			return;
	}

	gotoxy(cordX, cordY);
	for (auto i = 1; i <= lengthY; ++i) {
		for (auto j = 1; j <= lengthX; ++j) {
			if (i == 1) {
				if (p.getX() < 0 && p.getY() < 0 && j != lengthX)
					cout << " ";
				else if (p.getY() < 0) {
					if (j == 1 || j == lengthX)
						cout << ch;
					else
						cout << " ";
				}

				else
					cout << ch;
			}
			else if (i == lengthY)
				cout << ch;
			else {
				if (p.getX() < 0 && j != lengthX)
					cout << " ";
				else if (j == 1)
					cout << ch;
				else if (j == lengthX)
					cout << ch;
				else
					cout << " ";
			}
		}

		gotoxy(cordX, ++cordY);
	}
}

void Square::draw(char special) const {
	
	int cordY = p.getY(), cordX = p.getX();
	char ch = special;
	int lengthX, lengthY;
	lengthX = lengthY = length;

	gotoxy(p.getX(), p.getY());

	if (p.getX() < 0) {
		if (cordX + length >= 0) {
			lengthX = length + cordX;
			cordX = 0;

		}
		else
			return;
	}

	if (p.getY() < 0) {
		if (cordY + length >= 0) {
			lengthY = length + cordY;
			cordY = 0;

		}
		else
			return;
	}

	gotoxy(cordX, cordY);
	for (auto i = 1; i <= lengthY; ++i) {
		for (auto j = 1; j <= lengthX; ++j) {
			if (i == 1) {
				if (p.getX() < 0 && p.getY() < 0 && j != lengthX)
					cout << " ";
				else if (p.getY() < 0) {
					if (j == 1 || j == lengthX)
						cout << ch;
					else
						cout << " ";
				}

				else
					cout << ch;
			}
			else if (i == lengthY)
				cout << ch;
			else {
				if (p.getX() < 0 && j != lengthX)
					cout << " ";
				else if (j == 1)
					cout << ch;
				else if (j == lengthX)
					cout << ch;
				else
					cout << " ";
			}
		}

		gotoxy(cordX, ++cordY);
	}
}

bool Square::isContainingPoint(const Point& otherPoint) const {
	
	int leftX = getLeftX();
	int rightX = getRightX();
	int topY = getTopY();
	int bottomY = getBottomY();

	// compare the otherPoint coordinate with the current shape boundaries
	if (otherPoint.getX() <= rightX && otherPoint.getX() >= leftX)
		if (otherPoint.getY() <= bottomY && otherPoint.getY() >= topY)
			return true;
	
	return false;
}

bool Square::isContainingPoint(int x, int y) const {

	int leftX = getLeftX();
	int rightX = getRightX();
	int topY = getTopY();
	int bottomY = getBottomY();

	if (x <= rightX && x >= leftX)
		if (y <= bottomY && y >= topY)
			return true;

	return false;
}

bool Square::overlaps(const Shape& rhs) const {

	Point currPoint;

	for (int currX = p.getX(); currX <= p.getX() + length; ++currX)
		for (int currY = p.getY(); currY <= p.getY() + length; ++currY) {
			currPoint.setX(currX);
			currPoint.setY(currY);
			if (rhs.isContainingPoint(currPoint))
				return true;
		}

	return false;
}

void Square::addAnimation() {
	ai = new AnimationInterface(*this);

	if (!ai)
		throw std::bad_alloc();
}