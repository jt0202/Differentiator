#pragma once
#include "Operator.h"
#include "Product.h"
#include "Logarithm.h"

enum
{
	EXPONENT = 0,
	BASE = 1
};

class Exponent : public MathOperator
{
private:
	
public:
	Exponent(Term* exponent, Term* base);

	Term* differentiate(char var);

	std::string output() const;

	bool equals(const Term* t) const;
};