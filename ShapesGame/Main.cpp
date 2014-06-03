#pragma once
#include <iostream>
#include <string>
#include "Controller.h"

using namespace std;

int main() {

	Controller app;
	app.init();
	app.run();

	return EXIT_SUCCESS;
}