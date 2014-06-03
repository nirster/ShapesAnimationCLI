#pragma once
#include "View.h"
#include "Definitions.h"
#include "Shape.h"
#include "Container.h"
#include "Factory.h"

View::View(Container& c) {
	container = &c;
}

void View::drawAll() {

	clrscr();

	if (container->lst.empty()) {
		std::cout << "Nothing to print!\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(700));
		return;
	}

	auto begin = container->lst.begin();
	auto end = container->lst.end();

	for (auto iter = begin; iter != end; ++iter) {
		if (*iter != nullptr) {
			(*iter)->draw();
		}
	}
}

UserInput View::getUserShapeData() {

	int x, y, length;
	char ch;
	int id;
	UserInput res;

	bool dataIsOK = true;

	do {
		clrscr();

		if (!dataIsOK)
			cout << "\t=== Your input is wrong. Trying again: \n";

		cout << "\t=== Enter data for a new shape ===\n";
		cout << "\t=== Enter 0 for square, 1 for diamond: ";
		cin >> id;
		cout << "\t=== Enter X value: ";
		cin >> x;
		cout << "\t=== Enter Y value: ";
		cin >> y;
		cout << "\t=== Enter Length/Radius: ";
		cin >> length;
		cout << "\t=== Enter character: ";
		cin >> ch;

		res.p.setX(x);
		res.p.setY(y);
		res.length = length;
		res.ch = ch;
		
		switch (id) {
		case 1:
			res.id = DIAMOND;
			break;
		case 0:
			res.id = SQUARE;
			break;
		}

		dataIsOK = validateUserShape(res);

	} while (!dataIsOK);

	return res;
}


Point View::getUserPoint() {

	int x;
	int y;

	clrscr();
	cout << "\t=== Finding shape by coordinates ===\n";
	cout << "\t=== Enter point coordinate ===\n";
	cout << "\t=== Enter X value: ";
	cin >> x;
	cout << "\t=== Enter Y value: ";
	cin >> y;
	Point res(x, y);

	return res;
}

Shape* View::findShape(bool& found) {

	userPoint = getUserPoint();
	auto rbegin = container->lst.rbegin();
	auto rend = container->lst.rend();

	for (auto iter = rbegin; iter != rend; ++iter)
	if ((*iter)->isContainingPoint(userPoint)) {

		userShape = iter;
		found = true;
		
		return *iter;
	}

	found = false;

	return nullptr;
}

void View::createShapeDialog(Shape*& s) {

	userData = getUserShapeData();
	
	s = factory.getShape(userData.p, userData.ch, 
		                 userData.length, userData.id);
}

bool View::validateUserShape(const UserInput& input) {

	if (input.ch == '@')
		return false;

	if (input.id != SQUARE && input.id != DIAMOND)
		return false;

	return true;
}

void View::addAnimationDialog(Shape*& s) {
	
	bool ok = true;
	double speedX, speedY;
	clrscr();

	if (s->implementsAnimation() == false)
		s->addAnimation();

	do {
		if (s->animation()->getSpeedX() != 0 || 
			s->animation()->getSpeedY() != 0) {
			cout << "\t===Shape already has animation info. Change? [y/n]: ";
			char answer;
			cin >> answer;
			if (answer == 'n' || answer == 'N')
				return;
		}

		if (!ok) 
			cout << "\t===WRONG VALUES. TRY AGAIN:\n";

		cout << "\t===Entering / changing animation speed===\n";
		cout << "\t===Values between -1.0 and 1.0===\n";
		cout << "\t   Enter X speed: ";
		cin >> speedX;
		cout <<"\t   Enter Y speed: ";
		cin >> speedY;
		ok = validateSpeed(speedX, speedY);

	} while (!ok);
	
	s->animation()->setSpeed(speedX, speedY);
}

bool View::validateSpeed(double x, double y) const {

	return (x >= -1.0 &&
		x <= 1.0  &&
		y >= -1.0 &&
		y <= 1.0
		);
}

void View::blinkLastSearchedPoint() {
	userPoint.blink();
}

void View::forceDirectionIfPartiallyOutside(Shape*& s) {
	
	int leftX = s->getLeftX();
	int rightX = s->getRightX();
	int topY = s->getTopY();
	int bottomY = s->getBottomY();

	if (rightX >= MAX_X)
		s->animation()->setSpeedX(abs(s->animation()->getSpeedX()) * -1);

	if (leftX < 0)
		s->animation()->setSpeedX(abs(s->animation()->getSpeedX()));

	if (topY < 1)
		s->animation()->setSpeedY(abs(s->animation()->getSpeedY()));

	if (bottomY > MAX_Y)
		s->animation()->setSpeedY(abs(s->animation()->getSpeedY()) * -1);
}

void View::animateSingleShape(Shape*& s) {
	
	char           newBuffer[MAX_X][MAX_Y];
	char           oldBuffer[MAX_X][MAX_Y];
	int currX = s->getX();
	int currY = s->getY();
	double XDelta = (double) currX;
	double YDelta = (double) currY;
	bool shapeHasMoved = false;
	bool xChanged = false;
	bool yChanged = false;

	clrscr();
	clearScreenBuffer(newBuffer);
	clearScreenBuffer(oldBuffer);

	s->animation()->drawToLocalBuffer(newBuffer, oldBuffer);
	printBufferToScreen(newBuffer);
	
	int key = 0;
	while (true) {
		if (_kbhit()) {
			key = _getch();
			if (key == ESC_CODE)
				break;
		}
		
		// This can occur only in the first iteration
		forceDirectionIfPartiallyOutside(s);
		
		if (s->animation()->isTouchingBoundries())
			s->animation()->changeDirection();

		// Calculate the change from speed values
		XDelta += s->animation()->getSpeedX();
		YDelta += s->animation()->getSpeedY();

		// check if the total change is at least 1
		// since we are working with integers
		if (abs(XDelta - double(currX)) >= 1) {
			currX = (int) XDelta;
			s->setX(currX);
			xChanged = true;
		}

		if (abs(YDelta - (double)currY) >= 1) {
			currY = (int) YDelta;
			s->setY(currY);
			yChanged = true;
		}
		
		if (xChanged && yChanged || s->animation()->isOneAxisAnimation()) {
			shapeHasMoved = true;
			xChanged = false;
			yChanged = false;
		}

		// if the shape position has changed,
		// update the shape origin point
		// and draw to screen only the different coordinates
		if (shapeHasMoved) { 
			s->animation()->drawToLocalBuffer(newBuffer, oldBuffer);
			shapeHasMoved = false; // reset the flag
		}
		
		clearScreenBuffer(newBuffer);
		std::this_thread::sleep_for(std::chrono::milliseconds(TIME_DELTA));
	}
}

void View::animateTwoShapes(Shape*& s1, Shape*& s2) {
	
	char           newBuffer[MAX_X][MAX_Y];
	char           oldBuffer[MAX_X][MAX_Y];
	bool collisionFlag = false;
	int currX1 = s1->getX();
	int currY1 = s1->getY();
	double XDelta1 = (double)currX1;
	double YDelta1 = (double)currY1;
	bool shapeHasMoved1 = false;
	bool xChanged1 = false;
	bool yChanged1 = false;
	int currX2 = s2->getX();
	int currY2 = s2->getY();
	double XDelta2 = (double)currX2;
	double YDelta2 = (double)currY2;
	bool shapeHasMoved2 = false;
	bool xChanged2 = false;
	bool yChanged2 = false;

	clrscr();
	clearScreenBuffer(newBuffer);
	clearScreenBuffer(oldBuffer);

	collisionFlag = checkForCollision(s1, s2);
	if (!collisionFlag) {
		s1->animation()->drawToLocalBuffer(newBuffer, oldBuffer);
		s2->animation()->drawToLocalBuffer(newBuffer, oldBuffer);
		printBufferToScreen(newBuffer);
	}

	int key = 0;
	while (!collisionFlag) {
		if (_kbhit()) {
			key = _getch();
			if (key == ESC_CODE)
				break;
		}

		forceDirectionIfPartiallyOutside(s1);
		forceDirectionIfPartiallyOutside(s2);

		if (s1->animation()->isTouchingBoundries())
			s1->animation()->changeDirection();

		if (s2->animation()->isTouchingBoundries())
			s2->animation()->changeDirection();

		// Calculate the change from speed values
		XDelta1 += s1->animation()->getSpeedX();
		YDelta1 += s1->animation()->getSpeedY();
		// same for second shape
		XDelta2 += s2->animation()->getSpeedX();
		YDelta2 += s2->animation()->getSpeedY();

		// check if the total change is at least 1
		// since we are working with integers
		if (abs(XDelta1 - double(currX1)) >= 1) {
			currX1 = (int)XDelta1;
			s1->setX(currX1);
			xChanged1 = true;
		}

		if (abs(YDelta1 - (double)currY1) >= 1) {
			currY1 = (int)YDelta1;
			s1->setY(currY1);
			yChanged1 = true;
		}

		if (xChanged1 && yChanged1 || s1->animation()->isOneAxisAnimation()) {
			shapeHasMoved1 = true;
			xChanged1 = false;
			yChanged1 = false;
		}
		
		// do the same for the second shape
		if (abs(XDelta2 - double(currX2)) >= 1) {
			currX2 = (int)XDelta2;
			s2->setX(currX2);
			xChanged2 = true;
		}

		if (abs(YDelta2 - (double)currY2) >= 1) {
			currY2 = (int)YDelta2;
			s2->setY(currY2);
			yChanged2 = true;
		}

		if (xChanged2 && yChanged2 || s2->animation()->isOneAxisAnimation()) {
			shapeHasMoved2 = true;
			xChanged2 = false;
			yChanged2 = false;
		}

		// check for collision after one of the shapes moved
		// "logically" but before the actual drawing happens
		if (shapeHasMoved1 || shapeHasMoved2) {
			collisionFlag = checkForCollision(s1, s2);
			if (collisionFlag)
				break;
		}

		if (shapeHasMoved1) {
			s1->animation()->drawToLocalBuffer(newBuffer, oldBuffer);
			shapeHasMoved1 = false; // reset the flag
		}

		if (shapeHasMoved2) {
			s2->animation()->drawToLocalBuffer(newBuffer, oldBuffer);
			shapeHasMoved2 = false; // reset the flag
		}

		clearScreenBuffer(newBuffer);
		std::this_thread::sleep_for(std::chrono::milliseconds(TIME_DELTA));

	}

	if (collisionFlag) {
		std::this_thread::sleep_for(std::chrono::milliseconds(400));
		container->mergeAnimation(s1, s2);
	}
}

void View::clearScreenBuffer(char buffer[][MAX_Y]) {

	for (int i = 0; i < MAX_X; ++i)
		for (int j = 0; j < MAX_Y; ++j)
			buffer[i][j] = ' ';
}

void View::printBufferToScreen(char buffer[][MAX_Y]) {

	for (int i = 0; i < MAX_X; ++i) {
		for (int j = 0; j < MAX_Y; ++j) {
			if (buffer[i][j] != ' ') {
				gotoxy(i, j);
				cout << buffer[i][j];
			}
		}
	}
}

void View::saveDataDialog(const string& saveType) {

	cout << "\t=== Saving data as " << saveType << "\n";
	cout << "\t=== WARNING: existing file will be overwritten!\n";
	cout << "\t=== Continue? [y/n]: ";
	char ans;
	cin >> ans;
	
	if (ans == 'y' || ans == 'Y') {
		cout << "\t===Enter local filename: ";
		string filename;
		cin >> filename;
		container->saveData(saveType, filename);
		cout << "Data saved.\n";
	}

	else
		cout << "No changed made.\n";
}

void View::loadDataDialog(const string& loadType) {
	
	cout << "\t=== Loading data from " << loadType << "\n";
	cout << "\t=== WARNING: existing unsaved data will be lost!\n";
	cout << "\t=== Continue? [y/n]: ";
	char ans;
	cin >> ans;
	
	if (ans == 'y' || ans == 'Y') {
		string filename;
		cout << "\t===Enter local filename: ";
		cin >> filename;
		container->loadData(loadType, filename);
		cout << "Data Loaded.\n";
	}

	else
		cout << "No changes made.\n";

	std::this_thread::sleep_for(std::chrono::milliseconds(800));
}

bool View::checkForCollision(Shape*& s1, Shape*& s2) {
	
	for (int i = 0; i < MAX_X; ++i) {
		for (int j = 0; j < MAX_Y; ++j) {
			if (s1->animation()->pointIsOnShapeOutline(i, j) &&
				s2->animation()->pointIsOnShapeOutline(i, j))
				return true;
		}
	}

	return false;
}