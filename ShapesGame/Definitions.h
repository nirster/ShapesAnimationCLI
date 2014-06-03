#pragma once
#include <string>
#include <windows.h>
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <utility>
#include <memory>
#include "Point.h"
/* global definitions */
using namespace std;


void gotoxy(int x, int y);
void clrscr();


/* CONSTANTS */
const int MAX_X = 80;
const int MAX_Y = 25;
const int ESC_CODE = 27;
const int TIME_DELTA = 100; // tenth of a second in milliseconds
const string BINARY_FILENAME = "shapesData.bin";
const string TEXT_FILENAME = "shapesData.txt";

enum shapeID { SQUARE = 0, DIAMOND = 1 };

/* EXCEPITION STRINGS */
const string ERROR_OPENING_FILE = "Error opening file";
const string ERROR_ALLOCATING_MEMORY = "Error allocating memory";