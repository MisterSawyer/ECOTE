#pragma once
#include <string>

class Macro;

void reportErrorExecute(Macro * where, int body_line, const std::string & message);
void reportErrorReadingFile(Macro * where, int input_line, const std::string & message);
