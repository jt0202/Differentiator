#pragma once
#include "Operator.h"
#include <vector>

class Sum : public MathOperator
{
private:

public:
	Sum(Term* summand1, Term* summand2);

	Term* differentiate(char variable);
};