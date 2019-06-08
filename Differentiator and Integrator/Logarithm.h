#pragma once
#include "Operator.h"
#include "Product.h"
#include "Exponent.h"
#include "Number.h"

class Logarithm : public MathOperator
{
public:
	Logarithm(Term term);

	Term differentiate(char var);
	std::string output();
};