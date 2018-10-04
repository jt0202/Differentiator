#pragma once
#include "Sum.h"

Sum::Sum(Term* summand1, Term* summand2)
	: MathOperator(summand1, summand2)
{
	type = "+";
}

Term* Sum::differentiate(char variable)
{
	return nullptr;
}

std::string Sum::output()
{
	std::string out = arguments.at(0)->output();

	for (int i = 1; i < arguments.size(); i++)
	{
		out += "+" + arguments.at(i)->output();
	}

	return out;
}
