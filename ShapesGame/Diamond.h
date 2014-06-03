#pragma once
#include "Shape.h"
#include "AnimationInterface.h"

class Diamond : public Shape {

public:
	/* Constructors */
	Diamond(const Point& p, char ch, int length, shapeID id);
	Diamond(const Diamond& other);
	Diamond& operator=(const Diamond& other);
	virtual ~Diamond() {}

	/* Override */
	virtual void draw() const;
	virtual void draw(char special) const;
	virtual int getLength() const { return 2 * radius + 1; }
	virtual bool isContainingPoint(const Point& otherPoint) const;
	virtual bool isContainingPoint(int x, int y) const;
	virtual int getLeftX() const { return (p.getX() - radius); }
	virtual int getRightX() const { return (p.getX() + radius); }
	virtual int getTopY() const { return (p.getY() - radius); }
	virtual int getBottomY() const { return (p.getY() + radius); }
	virtual int getTotalWidth() const { return (2 * radius + 1); }
	virtual int getSize() const { return ((2 * radius + 1) * 2); }
	virtual bool overlaps(const Shape& rhs) const;
	virtual void addAnimation() { ai = new AnimationInterface(*this); }

private:
	int radius; 
	bool pointIsInScreen(int currX, int currY) const;
};