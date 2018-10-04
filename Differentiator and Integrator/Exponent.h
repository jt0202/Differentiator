#pragma once
#include "Operator.h"

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

	std::string output();
};