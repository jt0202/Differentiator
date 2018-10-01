#pragma once
#include "Operator.h"

class Product : public MathOperator
{
public:
	Product(Term* factor1, Term* factor2);
};