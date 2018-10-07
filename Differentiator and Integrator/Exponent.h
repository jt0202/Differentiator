#pragma once
#include "Operator.h"

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

	std::string output();
};