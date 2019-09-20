#pragma once
#include "Operator.h"
#include "Number.h"
#include <vector>

class Sum : public MathOperator
{
private:

public:
	Sum(Term* summand1, Term* summand2);

	Sum(std::vector<Term*> terms);

	Term* differentiate(char variable);

	std::string output() const;

	Term* simplify(char mainvar) override;
};