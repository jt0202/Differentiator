#pragma once
#include "Atom.h"
#include <sstream>

class Variable : public Atom
{
private:
	char m_name;
public:
	Variable(char i_name);

	std::string output();
};