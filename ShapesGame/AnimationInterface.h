#pragma once
#include "Shape.h"
#include "Definitions.h"
#include <algorithm>
class Shape;
class AnimationInterface {
	/* this class is added to every shape in the program
	   that wants to have animation methods */
private:
	double speedX, speedY;
	Shape& s;

public:
	AnimationInterface(Shape& s);
	~AnimationInterface() = default;

	/* Animation methods */
	void setSpeed(double x, double y) { this->speedX = x; this->speedY = y; }
	void setSpeedX(double x) { speedX = x; }
	void setSpeedY(double y) { speedY = y; }
	double  getSpeedX() const { return speedX; }
	double  getSpeedY() const { return speedY; }
	void drawToLocalBuffer(char newBuffer[][MAX_Y], char oldBuffer[][MAX_Y]);

	
	void changeDirection();
	bool pointIsOnShapeOutline(int i, int j) const;
	bool pointIsOutOfScreenBounds(int i, int j) const;
	bool isTouchingBoundries() const;
	char getCharAtPosition(int i, int j) const;
	bool isOneAxisAnimation() const;

	/* binary save and load */
	void save(std::ostream& stream) const;
	void load(std::istream& stream);
};