#include "error-log.h"
#include <iostream>
#include "macro.h"

using namespace std;

void reportErrorExecute(Macro * where, int body_line, const std::string & message)
{
	cout << "Error " << message << endl;
}

void reportErrorReadingFile(Macro * where, int input_line , const std::string & message)
{
	if (where != nullptr) {
		cout << "Macro name: " << where->getName() << std::endl;
		std::string body = where->getBody();
		if(body.length() > 0)cout << "Macro body: \n" << body << std::endl;
	}
	cout << "Error, on line " << input_line <<", "<< message << endl;
	cout << "_____________________" << std::endl;
}
