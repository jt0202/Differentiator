#pragma once
#include "Operator.h"
#include "Product.h"
#include "Logarithm.h"

class Exponent : public MathOperator
{
private:
	enum 
	{
		EXPONENT = 1, 
		BASE = 0
	};
public:
	Exponent(Term* exponent, Term* base);

	Term* differentiate(char var);

	std::string output();
};