#pragma once
#include "Operator.h"

class Fraction : public MathOperator
{
private:
	enum 
	{
		NUMERATOR = 1,
		DENOMINATOR = 0
	};
public:
	Fraction(Term* denominator, Term* numerator);

};