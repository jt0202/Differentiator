#pragma once
#include "Operator.h"

class Exponent : public MathOperator
{
public:
	Exponent(Term* exponent, Term* base);
};