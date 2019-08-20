#pragma once
#include "Atom.h"
#include <string>

// This class represents rational numbers and their operations.
// Irrational numbers only occur as constants such as pi 
// or in Expressions like sqrt(2), which will kept this way 
// for accurate calculation.
class Number : public Atom
{
private:
	// The textual representation.
	// For now just used for simplicity
	// Todo: Create a real number system.
	std::string m_text;

	int denominator;
	int numerator;

public:
	Number(std::string i_input);

	std::string output();

	Term* differentiate(char var);

	int getDenominator();

	int getNumerator();
};