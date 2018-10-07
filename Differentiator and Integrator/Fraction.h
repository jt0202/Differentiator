#pragma once
#include "Operator.h"

class Fraction : public MathOperator
{
private:
	enum 
	{
		NUMERATOR = 0,
		DENOMINATOR = 1
	};
public:
	Fraction(Term* denominator, Term* numerator);

	std::string output();
};