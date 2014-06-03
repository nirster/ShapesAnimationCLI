#pragma once
#include "AnimationInterface.h"


AnimationInterface::AnimationInterface(Shape& s) : s(s) {
	speedX = 0.0;
	speedY = 0.0;
}

void AnimationInterface::drawToLocalBuffer(char newBuffer[][MAX_Y], 
	                                       char oldBuffer[][MAX_Y]) {

	for (int i = 0; i < MAX_X; ++i) {
		for (int j = 0; j < MAX_Y; ++j) {

			if (s.isContainingPoint(i, j) && s.getID() == SQUARE) {

				if (pointIsOnShapeOutline(i, j))
					newBuffer[i][j] = s.getChar();
				else
					newBuffer[i][j] = ' ';
			}
			
			else if (s.isContainingPoint(i, j) && s.getID() == DIAMOND) {
				newBuffer[i][j] = s.getChar();
			}

			if (oldBuffer[i][j] != newBuffer[i][j]) {
				gotoxy(i, j);
				cout << newBuffer[i][j];
				oldBuffer[i][j] = newBuffer[i][j];
			}
		}
	}
}

char AnimationInterface::getCharAtPosition(int i, int j) const {
	return pointIsOnShapeOutline(i, j) ? s.getChar() : ' ';
}

void AnimationInterface::changeDirection() {

	if (s.getLeftX() == 0)
		speedX = abs(speedX);
	
	if (s.getRightX() == MAX_X - 1)
		speedX = -1 * abs(speedX);
	
	if (s.getTopY() == 0)
		speedY *= abs(speedY);

	if (s.getBottomY() == MAX_Y - 1)
		speedY = -1 * abs(speedY);
}

bool AnimationInterface::pointIsOnShapeOutline(int i, int j) const {

	int leftX = s.getLeftX();
	int rightX = s.getRightX();
	int topY = s.getTopY();
	int bottomY = s.getBottomY();
	int x = i;
	int y = j;
	if (s.getID() == SQUARE)
		return ((x == leftX && y >= topY && y <= bottomY) ||     // point on left line
				(x == rightX && y >= topY && y <= bottomY) ||    // point on right line
		        (y == topY && x >= leftX && x <= rightX) ||      // point on top line
		        (y == bottomY && x >= leftX && x <= rightX)      // point on bottom line
		);
	else
		return s.isContainingPoint(i, j);
}

bool AnimationInterface::pointIsOutOfScreenBounds(int i, int j) const {

	if (i <= 0 || i >= MAX_X - 1)
		return true;

	if (j <= 0 || j >= MAX_Y - 1)
		return true;

	return false;
}

bool AnimationInterface::isTouchingBoundries() const {
	
	return (s.getTopY() == 1 ||
		s.getBottomY() == MAX_Y - 1 ||
		s.getLeftX() == 0 ||
		s.getRightX() == MAX_X - 1
		);
}

void AnimationInterface::save(std::ostream& stream) const {

	stream.write((const char*)&speedX, sizeof(speedX));
	stream.write((const char*)&speedY, sizeof(speedY));

	/* NOTE: we can't load/save this object as one piece
	   because of the back-reference to the containing shape.
	   this object is useless without being created from
	   inside an actual shape, so this method will only
	   save the speed values.                             
	*/
}

void AnimationInterface::load(std::istream& stream) {

	stream.read((char*)&speedX, sizeof(speedX));
	stream.read((char*)&speedY, sizeof(speedY));
}

bool AnimationInterface::isOneAxisAnimation() const {

	return speedX != 0 && speedY == 0 ||
		   speedX == 0 && speedY != 0;
}