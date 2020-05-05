#pragma once
#include <string>
#include <map>
#include <vector>

class Macro
{
	std::string						name;
	Macro *							parent;
	std::map<std::string, Macro *>	childreen;
	std::vector<std::string>		body;

public:

};