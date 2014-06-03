#include "Diamond.h"

Diamond::Diamond(const Point& p, char ch, int length, shapeID id) :
	
	/* the parent object */
	Shape(p, ch, length)
	{	

	this->id = id;
	ai = nullptr;
	radius = length;
}

Diamond::Diamond(const Diamond& other) : Shape(other) {
	
	radius = other.radius;
}

Diamond& Diamond::operator=(const Diamond& other) {

	if (this == &other)
		return *this;

	p = other.p;
	ch = other.ch;
	length = other.length;
	id = other.id;
	radius = other.radius;
	ai = nullptr;

	if (other.ai != nullptr) {
		addAnimation();
		animation()->setSpeedX(other.ai->getSpeedX());
		animation()->setSpeedY(other.ai->getSpeedY());
	}

	return *this;
}

void Diamond::draw() const {

	/* Loop through the containing rectangle of the upper
	pyramid of the diamond, and make a mirror for the lower
	pyramid */

	int leftSpace = getX() - 1;
	int rightSpace = getX() + 1;
	int currBottomY = getBottomY();
	int currX = getLeftX();
	int currY = getTopY();
	char c = getChar();

	for (currY; currY <= getY(); ++currY) {

		for (currX = getLeftX(); currX <= getRightX(); ++currX) {

			if (currX < rightSpace && currX > leftSpace) {
				// print the upper pyramid
				if (pointIsInScreen(currX, currY)) {
					gotoxy(currX, currY);
					cout << c;
				}
				// print the lower pyramid
				if (pointIsInScreen(currX, currY)) {
					gotoxy(currX, currBottomY);
					cout << c;
				}
			}
		}

		leftSpace--;
		rightSpace++;
		currBottomY--;
	}
}

void Diamond::draw(char special) const {
	
	int leftSpace = getX() - 1;
	int rightSpace = getX() + 1;
	int currBottomY = getBottomY();
	int currX = getLeftX();
	int currY = getTopY();
	char c = special;

	for (currY; currY <= getY(); ++currY) {

		for (currX = getLeftX(); currX <= getRightX(); ++currX) {


			if (currX < rightSpace && currX > leftSpace) {
				if (pointIsInScreen(currX, currY)) {
					gotoxy(currX, currY);
					cout << c;
				}
				if (pointIsInScreen(currX, currY)) {
					gotoxy(currX, currBottomY);
					cout << c;
				}
			}

		}

		leftSpace--;
		rightSpace++;
		currBottomY--;
	}
}

bool Diamond::isContainingPoint(const Point& p) const {
	
	int leftX = getLeftX();
	int rightX = getRightX();
	int topY = getTopY();
	int bottomY = getBottomY();
	
	if (p.getY() <= bottomY && p.getY() >= topY) {
		int numStars = radius - abs(getY() - p.getY());
		if (p.getX() <= getX() + numStars && p.getX() >= getX() - numStars)
			return true;
	}

	return false;
}

bool Diamond::isContainingPoint(int x, int y) const {
	
	Point p(x, y);
	return isContainingPoint(p);
}

bool Diamond::overlaps(const Shape& rhs) const {

	int topLeftX = getLeftX();
	int topRightX = getRightX();
	int topY = getTopY();
	int bottomY = getBottomY();
	
	for (int i = topLeftX; i <= topRightX; ++i) {
		for (int j = topY; j <= bottomY; ++j) {
			Point currPoint(i, j);
			if (isContainingPoint(p) && rhs.isContainingPoint(p))
				return true;
		}
	}

	return false;
}

bool Diamond::pointIsInScreen(int currX, int currY) const {
	return (currX >= 0 && 
		    currX < MAX_X &&
			currY > 0 &&
			currY <= MAX_Y);
}