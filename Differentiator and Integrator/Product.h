#pragma once
#include "Operator.h"
#include "Precedence.h"

class Product : public MathOperator
{
public:
	Product(Term* factor1, Term* factor2);

	std::string output();
};