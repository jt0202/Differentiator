#pragma once
#include "Operator.h"

class Logarithm : public MathOperator
{
public:
	Logarithm(Term* term);

	std::string output();
};