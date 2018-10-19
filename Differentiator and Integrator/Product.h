#pragma once
#include "Operator.h"
#include "Precedence.h"
#include "Sum.h"

class Product : public MathOperator
{
public:
	Product(Term* factor1, Term* factor2);

	Product(std::vector<Term*> terms);

	std::string output();

	Term* differentiate(char variable);
};