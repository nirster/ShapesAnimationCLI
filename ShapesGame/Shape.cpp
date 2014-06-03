#pragma once
#include "Shape.h"
Shape::Shape(const Point& p, char ch, int length) : 
				p(p), ch(ch), length(length), ai(nullptr) {}

Shape::~Shape() {
	if (ai)
		delete ai;
}

Shape::Shape(const Shape& other) {
	p = other.p;
	ch = other.ch;
	length = other.length;
	id = other.id;
	ai = nullptr;

	if (other.ai != nullptr)
		ai = new AnimationInterface(*this);
}

Shape& Shape::operator=(const Shape& other) {

	if (this == &other)
		return *this;

	p = other.p;
	ch = other.ch;
	length = other.length;
	id = other.id;
	ai = nullptr;

	return *this;
}

bool Shape::operator<(const Shape& rhs) const {
	
	return (getLeftX() > rhs.getLeftX() &&
		getTopY() > rhs.getTopY() &&
		getRightX() < rhs.getRightX() &&
		getBottomY() < rhs.getBottomY()
		);
}

bool Shape::operator>(const Shape& rhs) const {

	return (getLeftX() < rhs.getLeftX() &&
		getRightX() > rhs.getRightX() &&
		getTopY() < rhs.getTopY() &&
		getBottomY() > rhs.getBottomY()
		);
}

bool Shape::operator==(const Shape& rhs) const {

	return (this->p == rhs.p     &&
		    this->ch == rhs.ch   &&
			this->length == rhs.length &&
			this->id == rhs.id 
			);
}

void Shape::save(std::ostream& stream) const {
	
	bool animationFlag = implementsAnimation();

	p.save(stream);
	stream.write((const char*)&ch, sizeof(ch));
	stream.write((const char*)&length, sizeof(length));
	stream.write((const char*)&id, sizeof(id));
	stream.write((const char*)&animationFlag, sizeof(animationFlag));
	
	if (ai)
		ai->save(stream);
}

void Shape::removeAnimation() {
	delete ai; 
	ai = nullptr;
}

std::ostream& operator<<(std::ostream& stream, Shape& shape) {

	int id = (shape.getID() == SQUARE) ? 0 : 1;
	int length = (shape.getID() == SQUARE) ? shape.getLength() : shape.getLength() / 2 - 1;
	double speedX = 0, speedY = 0;

	if (shape.implementsAnimation()) {
		speedX = shape.animation()->getSpeedX();
		speedY = shape.animation()->getSpeedY();
	}

	stream << id << " " << shape.getPoint() << " " << length <<
		" " << shape.getChar() << " "
		<< speedX << " " << speedY << "\n";

	return stream;
}