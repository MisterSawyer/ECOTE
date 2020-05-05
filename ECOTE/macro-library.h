#pragma once
#include "macro.h"
#include <string>

class MacroLibrary
{
	Macro * global_scope;
	Macro * execution_level;

public:
	MacroLibrary();
	~MacroLibrary();

	bool readInputFromFile(const std::string & filepath);
	void readInputFromString(const std::string & filepath);
};