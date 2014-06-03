#include "Container.h"

Container::~Container() {
	
	freeShapes();
	lst.clear();
}

void Container::freeShapes() {
	if (!lst.empty()) {
		for (auto iter = lst.begin(); iter != lst.end(); ++iter)
			delete *iter;
	}
	lst.clear();
}

list<Shape*>::iterator Container::getIterator(Shape*& s) {
	
	list<Shape*>::iterator res;

	for (res = lst.begin(); res != lst.end(); ++res)
		if (**res == *s)
				break;

	return res;
}

void Container::moveShapeToEnd(Shape*& s) {

	if (s == nullptr) {
		cout << "Error: No such shape!\n";
		return;
	}
	
	item1 = getIterator(s);
	
	if (item1 != lst.end())
		lst.splice(lst.end(), lst, item1);
	
}

void Container::deleteShape(Shape*& s) {

	if (s == nullptr) {
		
		cout << "Error: No such shape!\n";
		return;
	}
	
	item1 = getIterator(s);

	if (item1 != lst.end()) {
		delete *item1;
		lst.erase(item1);
	}
}

void Container::addShapeToList(Shape*& s) {
	lst.push_back(s);
}

void Container::mergeShapes(Shape*& s1, Shape*& s2) {

	if (s1 == nullptr || s2 == nullptr) {
		
		cout << "Error: No shapes to merge!\n";
		return;
	}

	item1 = getIterator(s1);
	item2 = getIterator(s2);

	auto smaller = ((*item1)->getLength() <= (*item2)->getLength()) ? item1 : item2;
	auto bigger = ((*item1)->getLength() < (*item2)->getLength()) ? item2 : item1;

	if (*item1 < *item2) {
		delete *item1;
		lst.erase(item1);
	}
	
	else if ((*item1) > (*item2)) {
		delete *item2;
		lst.erase(item2);
	}
	
	else if ((*item1)->overlaps(**item2)) {
		delete *bigger;
		lst.erase(bigger);
	}
	
	else {
		delete *smaller;
		lst.erase(smaller);
	}
}

void Container::mergeAnimation(Shape*& s1, Shape*& s2) {

	double s1speed = s1->animation()->getSpeedX();
	double s2speed = s2->animation()->getSpeedX();
	int s1size = s1->getSize();
	int s2size = s2->getSize();

	// both squares
	if (s1->getID() == SQUARE && s2->getID() == SQUARE) {

		if (s1speed >= s2speed && s1size <= s2size) {
			deleteShape(s1);
		}
		else {
			deleteShape(s2);
		}
	}
	// both diamonds
	else if (s1->getID() == DIAMOND && s1->getID() == DIAMOND) {
		if (s1speed >= s2speed && s1size >= s2size) {
			deleteShape(s2);
		}
		else {
			deleteShape(s1);
		}
	}
	// different shapes
	else {
		Shape* slowerShape = (s1speed <= s2speed) ? s1 : s2;
		deleteShape(slowerShape);
	}
}

void Container::saveData(const string& saveType, const string& filename) {

	if (saveType == "binary")
		saveDataAsBinary(filename);

	else if (saveType == "text")
		saveDataAsText(filename);
}

void Container::loadData(const string& loadType, const string& filename) {

	if (loadType == "text")
		loadDataAsText(filename);

	else if (loadType == "binary")
		loadDataAsBinary(filename);
}

void Container::saveDataAsText(const string& filename) {
	
	std::ofstream saveFile;
	saveFile.exceptions(std::ios::failbit);
	saveFile.open(filename);
	
	saveFile << lst.size() << "\n";

	for (auto iter = lst.begin(); iter != lst.end(); ++iter)
		saveFile << **iter;
	
	saveFile.close();
}

void Container::saveDataAsBinary(const string& filename) {

	int numItems = lst.size();
	std::ofstream saveFile;
	saveFile.exceptions(std::ios::failbit);
	saveFile.open(filename, ios::binary);

	saveFile.write((const char*)&numItems, sizeof(numItems));

	for (auto iter = lst.begin(); iter != lst.end(); ++iter)
		(*iter)->save(saveFile);
	
	saveFile.close();
}

void Container::loadDataAsBinary(const string& filename) {
	
	freeShapes();
	bool dataOK;
	bool animationFlag;
	char ch;
	int length;
	shapeID id;
	Shape* currShape = nullptr;
	int numItems;
	std::ifstream loadFile;
	loadFile.exceptions(std::ios::failbit);
	loadFile.open(filename, ios::binary);
	
	loadFile.read((char*)&numItems, sizeof(numItems));

	for (int i = 0; i < numItems; ++i) {
		
		Point p(loadFile);
		loadFile.read((char*)&ch, sizeof(ch));
		loadFile.read((char*)&length, sizeof(length));
		loadFile.read((char*)&id, sizeof(id));
		loadFile.read((char*)&animationFlag, sizeof(animationFlag));

		dataOK = validateFileRead(length, id, ch, p);
		if (!dataOK) {
			string err = "Corrupted file";
			throw string(err);
		}

		switch (id) {
		case SQUARE:
			currShape = new Square(p, ch, length, id);
			break;
		case DIAMOND:
			currShape = new Diamond(p, ch, length, id);
			break;
		}

		if (animationFlag) {
			currShape->addAnimation();
			currShape->animation()->load(loadFile);
			dataOK = validateAnimationFileRead(*currShape->animation());
			if (!dataOK) {
				currShape->removeAnimation();
				string err = "Corrupted file";
				throw string(err);
			}
		}
		else
			currShape->resetAnimation();

		addShapeToList(currShape);
	}

	loadFile.close();
}

void Container::loadDataAsText(const string& filename) {

	freeShapes();
	bool dataOK;
	std::ifstream loadFile;
	loadFile.exceptions(std::ios::failbit);
	loadFile.open(filename);
	int numItems;
	loadFile >> numItems;
	int id_;
	shapeID id;
	char ch;
	int length;
	Point p;
	double speedX, speedY;
	Shape* currShape = nullptr;

	for (int i = 0; i < numItems; ++i) {

		loadFile >> id_;
		id = id_ == 0 ? SQUARE : DIAMOND;
		loadFile >> p >> length >> ch >> speedX >> speedY;

		dataOK = validateFileRead(length, id, ch, p);
		dataOK = validateSpeed(speedX, speedY);
		if (!dataOK) {
			string err = "Corrupted file";
			throw string(err);
		}

		switch (id) {

		case SQUARE:
			currShape = new Square(p, ch, length, id);
			break;

		case DIAMOND:
			currShape = new Diamond(p, ch, length, id);
			break;
		}

		if (speedX != 0 || speedY != 0 && currShape != nullptr) {
			currShape->addAnimation();
			currShape->animation()->setSpeedX(speedX);
			currShape->animation()->setSpeedY(speedY);
		}
		else
			currShape->resetAnimation();

		addShapeToList(currShape);
	}
}

bool Container::validateFileRead(int length, shapeID id, char ch, Point p) {
	
	bool res = true;

	if (length < 1)
		res = false;

	if (id != 0 && id != 1)
		res = false;

	if (ch == '@')
		res = false;

	return res;
}

bool Container::validateAnimationFileRead(const AnimationInterface& ai) {

	if (ai.getSpeedX() < -1 || ai.getSpeedX() > 1)
		return false;
	if (ai.getSpeedY() < -1 || ai.getSpeedX() > 1)
		return false;

	return true;
}

bool Container::validateSpeed(double x, double y) {

	if (x < -1 || x > 1)
		return false;
	if (y < -1 || y > 1)
		return false;

	return true;
}
