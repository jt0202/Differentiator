#pragma once
#include <string>
#include "Term.h"

// This class represents all functions of 
// the abstract syntax tree like cos, sin,
// exponents or sums.
class MathOperator: public Term
{
protected:
	std::string symbol;

public:
	// One argument constructor for logarithms or other functions
	MathOperator(Term* term);

	// 2 argument constructor for sums or products
	MathOperator(Term* term1, Term* term2);

};