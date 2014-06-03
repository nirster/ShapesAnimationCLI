#pragma once
#include "Shape.h"
#include "AnimationInterface.h"

class Square : public Shape {

public:
	
	/* Constructors */
	Square(const Point& p, char ch, int length, shapeID id);
	Square(const Square& other) : Shape(other) {}
	Square& operator=(const Square& other);
	virtual ~Square() {}

	/* Overridden methods */
	virtual void draw() const;
	virtual void draw(char special) const;
	virtual int getLength() const { return length; }
	virtual bool isContainingPoint(const Point& otherPoint) const;
	virtual bool isContainingPoint(int x, int y) const;
	virtual int getLeftX() const { return p.getX(); }
	virtual int getRightX() const { return (p.getX() + length); }
	virtual int getTopY() const { return p.getY(); }
	virtual int getBottomY() const { return (p.getY() + length); }
	virtual int getTotalWidth() const { return length; }
	virtual int getSize() const { return (length * length); }
	virtual bool overlaps(const Shape& rhs) const;
	virtual void addAnimation();
};