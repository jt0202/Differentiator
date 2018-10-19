#pragma once
#include "Atom.h"
#include <string>

class Number : public Atom
{
private:
	// The textual representation.
	// For now just used for simplicity
	// Todo: Create a real number system.
	std::string m_text;

public:
	Number(std::string i_input);

	std::string output();

	Term* differentiate(char var);
};