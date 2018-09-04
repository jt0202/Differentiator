#pragma once
#include "Atom.h"

class Variable : public Atom
{
private:
	char m_name;
public:
	Variable(char i_name);
};