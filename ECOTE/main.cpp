#pragma once
#include "macro-library.h"
#include <conio.h>

int main()
{
	MacroLibrary lib;
	lib.readInputFromFile("tests/t6.txt");
	_getch();
}