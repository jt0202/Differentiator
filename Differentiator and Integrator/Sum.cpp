#pragma once
#include "Sum.h"

Sum::Sum(Term* summand1, Term* summand2)
	: MathOperator(summand1, summand2)
{
	type = "sum";
}

Term* Sum::differentiate(char variable)
{
	return nullptr;
}