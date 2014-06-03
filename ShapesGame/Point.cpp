#pragma once
#include "Point.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <conio.h>

void Point::blink() const {

	bool flag = false;

	while (!flag && (!_kbhit() || _getch() != ESC_CODE)) {
		
		if (!isOutsideScreenBounds()) {
			/* only blink if inside the screen boundaries */
			gotoxy(x, y);
			cout << '@';
			std::this_thread::sleep_for(std::chrono::milliseconds(700));
			gotoxy(x, y);
			cout << " ";
			std::this_thread::sleep_for(std::chrono::milliseconds(700));
		}
		
		if (_kbhit())
			flag = (_getch() == ESC_CODE);
	}
}

void Point::save(std::ostream& stream) const {
	
	stream.write((const char*)this, sizeof(*this));
}

Point::Point(std::istream& stream) {

	stream.read((char*)this, sizeof(*this));
}

void Point::singleBlink() {
	gotoxy(x, y);
	cout << '@';
	std::this_thread::sleep_for(std::chrono::milliseconds(400));
	gotoxy(x, y);
	cout << " ";
	std::this_thread::sleep_for(std::chrono::milliseconds(400));
}