#include "macro.h"
#include "error-log.h"

Macro::Macro()
{
	parent = nullptr;
}

Macro::Macro(const std::string & name)
{
	parent = nullptr;
	this->name = name;
}

Macro::~Macro()
{
	for (auto & child : childreen)
	{
		delete child.second;
	}
}

std::pair<std::string, std::string> Macro::execute()
{
	std::string output = "";
	std::string execution_information = "";

	execution_information += "Macro name: " + name + "\n";
	std::string body_str = getBody();
	if (body_str.length() > 0)execution_information += "Macro body: \n" + body_str + "\n";
	
	for (unsigned int i = 0; i < body.size(); i++)
	{
		if (body[i][0] != '$')
		{
			output += body[i] + '\n';
			continue;
		}
		else
		{
			Macro * macro_to_execute = findMacroInScope(body[i].substr(1));
			if (macro_to_execute == nullptr) { reportErrorExecute(this, i, "Macro " + body[i].substr(1) + "not found in current scope"); continue; }
			
			std::pair<std::string, std::string> execution_output = macro_to_execute->execute();
			output += execution_output.first;
			execution_information += execution_output.second;
		}
	}

	return std::pair<std::string, std::string>(output, execution_information);
}

void Macro::setName(const std::string & name)
{
	this->name = name;
}

bool Macro::addChild(Macro * child)
{
	if (childreen.find(child->getName()) != childreen.end())
	{
		reportErrorExecute(this, -1, "Macro of this name already exisit in this scope");
		return false;
	}

	childreen.insert(std::pair<std::string, Macro *>(child->getName(), child));
	return true;
}

void Macro::setParent(Macro * parent)
{
	this->parent = parent;
}

std::map<std::string, Macro*> Macro::getSiblings() const
{
	if (parent == nullptr)return std::map<std::string, Macro*>();

	std::map<std::string, Macro*> siblings = parent->getChildreen();

	siblings.erase(name);

	return siblings;
}

std::map<std::string, Macro*> Macro::getChildreen() const
{
	return childreen;
}

Macro * Macro::findMacroInScope(const std::string & name) const
{
	std::map<std::string, Macro *>::const_iterator it_child = childreen.find(name);
	if (it_child != childreen.end())
		return it_child->second;

	std::map<std::string, Macro *> siblings = getSiblings();
	std::map<std::string, Macro *>::const_iterator it_siblings = siblings.find(name);

	if (it_siblings != siblings.end())
		return it_siblings->second;


	Macro * it_parent = parent;
	while (it_parent != nullptr)
	{
		if (it_parent->getName() == name)return it_parent;
		it_parent = it_parent->getParent();
	}

	return nullptr;
}

Macro * Macro::getParent() const
{
	return parent;
}

const std::string & Macro::getName() const
{
	return name;
}

void Macro::addBodyLine(const std::string & line)
{
	body.push_back(line);
}

std::string Macro::getBody()
{
	std::string output  = "";
	if(childreen.size() > 0)output+="\tDefinitions: ";

	for (auto & child : childreen)
	{
		output += child.second->getName() + " ";
	}
	if(childreen.size() > 0)output += '\n';

	for (auto & b : body)
	{
		output += + "\t" + b + "\n";
	}
	return output;
}
