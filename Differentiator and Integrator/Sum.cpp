#pragma once
#include "Sum.h"

Sum::Sum(Term* summand1, Term* summand2)
	: MathOperator(summand1, summand2, TERMTYPE_SUM)
{
}

Sum::Sum(std::vector<Term*> terms)
	:MathOperator(terms, TERMTYPE_SUM)
{
}

Term* Sum::differentiate(char variable)
{
	std::vector<Term*> summands;

	// The sum rule of differentiation.
	for (std::shared_ptr<Term> sp : arguments)
	{
		summands.push_back(sp.get()->differentiate(variable));
	}

	return new Sum(summands);
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
