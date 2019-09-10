#pragma once
#include "Operator.h"
#include "Product.h"
#include "Logarithm.h"

class Exponent : public MathOperator
{
private:
	enum 
	{
		EXPONENT = 0, 
		BASE = 1
	};
public:
	Exponent(Term* exponent, Term* base);

	Term* differentiate(char var);

	std::string output();

	bool equals(Term* t);
};