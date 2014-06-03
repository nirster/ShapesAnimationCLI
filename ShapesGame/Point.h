#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include "Definitions.h"

using namespace std;

class Point {

private:
	int x, y;

public:
	
	Point(int x, int y) : x(x), y(y) {}
	Point() : x(0), y(0) {}
	Point(std::istream& stream);
	Point(const Point& other) = default;
	Point& operator=(const Point& other) = default;

	int getX() const { return x; }
	int getY() const { return y; }
	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }
	bool isOutsideScreenBounds() const { return (x < 0 || x > 80 || y >= 25 || y <= 0); }
	void blink() const;
	void singleBlink();
	void save(std::ostream& stream) const;
	
	bool operator==(const Point& rhs) const {
		return (this->x == rhs.x && this->y == rhs.y); }
	

	friend std::ostream& operator<<(std::ostream& stream, const Point& point) {
		stream << point.getX() << " " << point.getY() << " ";
		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Point& point) {
		stream >> point.x;
		stream >> point.y;
		return stream;
	}
	
};