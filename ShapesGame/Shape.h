#pragma once
#include <iostream>
#include <fstream>
#include <ostream>
#include <memory>
#include "Point.h"
#include "Definitions.h"
#include "AnimationInterface.h"
using namespace std;


class AnimationInterface;

class Shape {

protected:
	
	Point p;
	char ch;
	int length;
	shapeID id; /* square or diamond */
	AnimationInterface* ai; 
	
public:
	/* Constructors */
	Shape(const Point& p, char ch, int length);
	Shape(const Shape& other);
	Shape& operator=(const Shape& other);
	virtual ~Shape();
	
	
	/* Generic methods for all shapes */
	int getX() const { return p.getX(); }
	int getY() const { return p.getY(); }
	void setX(int x) { p.setX(x);       }
	void setY(int y) { p.setY(y);       }
	char getChar() const { return ch;   }
	shapeID getID() const { return id; }
	Point getPoint() const { return p; }
	
	/* Operators */
	bool operator==(const Shape& rhs) const;
	bool operator<(const Shape& rhs) const;
	bool operator>(const Shape& rhs) const;
	void save(std::ostream& stream) const; /* binary save */
	friend std::ostream& operator<<(std::ostream& stream, Shape& shape);
	
	/* Generic animation methods */
	bool implementsAnimation() const { return (ai != nullptr); }
	AnimationInterface* animation() { return ai; }
	void removeAnimation(); 
	void resetAnimation() { ai = nullptr; }
	virtual void addAnimation()  = 0;

	
	/* Virtual methods with different implementation
	   for each shape. */
	virtual void draw() const = 0;
	virtual void draw(char special) const = 0;
	virtual int getLength() const = 0;
	virtual bool isContainingPoint(const Point& otherPoint) const = 0;
	virtual bool isContainingPoint(int x, int y) const = 0;
	virtual int getLeftX() const = 0;
	virtual int getRightX() const = 0;
	virtual int getTopY() const = 0;
	virtual int getBottomY() const = 0;
	virtual int getTotalWidth() const = 0;
	virtual int getSize() const = 0;
	virtual bool overlaps(const Shape& rhs) const = 0;
};