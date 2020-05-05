#include "macro-library.h"
#include <fstream>
#include <string>
#include <iostream>
#include "error-log.h"


MacroLibrary::MacroLibrary()
{
	global_scope = new Macro("GLOBAL_SCOPE");
	execution_level = global_scope;
}

MacroLibrary::~MacroLibrary()
{
	delete global_scope;
}

bool MacroLibrary::readInputFromFile(const std::string & filepath)
{
	std::string output = "";
	std::string macro_execution_information = "";

	std::ifstream file(filepath, std::ios::in);
	if (!file.good())
	{
		reportErrorReadingFile(nullptr, 0, "Er mozna odpalic pliku");
		return false;

	}

	unsigned int line_nr = 0;
	while (!file.eof())
	{
		line_nr++;

		std::string line = "";
		std::getline(file, line);
		if (line.size() == 0)break;

		//new definition
		if (line[0] == '&' && line.size()>1)
		{

			if (line.find("&", 1) != std::string::npos || line.find("$", 1) != std::string::npos)
			{
				reportErrorReadingFile(execution_level, line_nr, "Macro name " + line.substr(1) + " contains special character & or $, therefore cannot be registered");
				continue;
			}


			if (execution_level->findMacroInScope(line.substr(1)) != nullptr)
			{
				// already exists in current scope
				reportErrorReadingFile(execution_level, line_nr, "Macro "+ line.substr(1)+" already exisit in this context");
				continue;
			}
			Macro * new_definition = new Macro(line.substr(1));
			if (execution_level->addChild(new_definition) == false)
			{
				reportErrorReadingFile(execution_level, line_nr, "Adding nested macro "+ line.substr(1)+" failed");
				delete new_definition;
				continue;
			}
			
			new_definition->setParent(execution_level);

			execution_level = new_definition;
			continue;
		}
		// end of definitnion
		if (line[0] == '&' && line.size() == 1)
		{
			if (execution_level->getParent() == nullptr)
			{
				reportErrorReadingFile(execution_level, line_nr, "Too many '&', text level to low");
				continue;
			}
			else
			{
				execution_level = execution_level->getParent();
				continue;
			}
		}

		//new call
		if (line[0] == '$' && line.size() > 1)
		{
			if (line.find("&", 1) != std::string::npos || line.find("$", 1) != std::string::npos)
			{
				reportErrorReadingFile(execution_level, line_nr, "Macro name " + line.substr(1) + " contains special character & or $, therefore cannot be called");
				continue;
			}

			Macro * macro_execute = execution_level->findMacroInScope(line.substr(1));
			if (macro_execute == nullptr)
			{
				if (line.substr(1) == execution_level->getName())
				{
					reportErrorReadingFile(execution_level, line_nr, "Forbidden recursive call in macro " + line.substr(1));
					continue;
				}
				reportErrorReadingFile(execution_level, line_nr, "Cannot find macro " + line.substr(1) + " to call");
				continue;
			}
			execution_level->addBodyLine(line);
			if (execution_level->getParent() == nullptr) {
				macro_execution_information += line + "\n";

				std::pair<std::string, std::string> execute_output = macro_execute->execute();
				output += execute_output.first;
				macro_execution_information += execute_output.second;
			}
			continue;
		}
		//wrong call
		if (line[0] == '$' && line.size() == 1)
		{
			reportErrorReadingFile(execution_level, line_nr, "Special character detected but no macro name");
			continue;
		}

		// FREE TEXT / MACRO BODY
		if (execution_level->getParent() == nullptr)
		{
			//FREE TEXT
			output += line + '\n';
		}
		else
		{
			execution_level->addBodyLine(line);
		}

	}

	if (execution_level->getParent() != nullptr)
	{
		reportErrorReadingFile(nullptr, line_nr + 1, "Wrong levels, possible '&' missing");
	}
	std::cout << "Execution order : " << std::endl << std::endl;;
	std::cout << macro_execution_information;
	std::cout << " __________________________" << std::endl << std::endl;

	std::cout <<"Output : \n"<< output;
	return true;
}
