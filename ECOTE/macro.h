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
	Macro();
	Macro(const std::string & name);

	~Macro();

	std::pair<std::string, std::string>		execute();

	void							setName(const std::string & name);
	bool							addChild(Macro * child);
	void							setParent(Macro * parent);

	std::map<std::string, Macro *>	getSiblings() const;
	std::map<std::string, Macro *>	getChildreen() const;
	Macro *							findMacroInScope(const std::string & name) const;
	Macro *							getParent()const;
	const std::string &				getName() const;

	void							addBodyLine(const std::string & line);
	std::string						getBody();
};