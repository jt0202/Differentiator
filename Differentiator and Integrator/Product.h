#pragma once
#include "Operator.h"
#include "Precedence.h"
#include "Sum.h"
#include "Exponent.h"
#include <memory>
#include <stdexcept>


class Product : public MathOperator
{
public:
	Product(Term* factor1, Term* factor2);

	Product(std::vector<Term*> terms);

	std::string output() const;

	Term* differentiate(char variable);

	Term* simplify(char mainvar);
};

void collectTerms(std::vector<Term*>& bases, std::vector<std::vector<Term*>>& exponents, Term* newBase, Term* newExponent);
