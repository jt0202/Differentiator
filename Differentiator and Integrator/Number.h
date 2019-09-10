#pragma once
#include "Atom.h"
#include <string>
#include <cmath>
#include <stdexcept>

// This class represents rational numbers and their operations.
// Irrational numbers only occur as constants such as pi 
// or in Expressions like sqrt(2), which will be kept this way 
// for accurate calculation.
class Number : public Atom
{
private:
	// The textual representation.
	// For now just used for simplicity
	// Todo: Create a real number system.
	int denominator;
	int numerator;

public:

	Number(int value);

	Number(int denominator, int numerator);

	Term* add(std::vector<Number*> summands);

	Term* multiply(std::vector<Number*> factors);

	Term* exponentiate(Number* exponent);

	std::string output();

	Term* differentiate(char var);

	int getDenominator();

	int getNumerator();

	bool isFraction();

	bool equals(Term* t);
};

int euclid(int a, int b);