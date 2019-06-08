#pragma once
#include "Sum.h"

Sum::Sum(Term summand1, Term summand2)
	: MathOperator(summand1, summand2)
{
	type = "+";

	setDiff(std::bind(&Sum::differentiate, *this, std::placeholders::_1));
	setOutp(std::bind(&Sum::output, *this));
}

Sum::Sum(std::vector<Term> terms)
	:MathOperator(terms)
{
	setDiff(std::bind(&Sum::differentiate, *this, std::placeholders::_1));
	setOutp(std::bind(&Sum::output, *this));
}

Term Sum::differentiate(char variable)
{
	std::vector<Term> summands;

	// The sum rule of differentiation.
	for (Term t : arguments)
	{
		summands.push_back(t.differentiate(variable));
	}

	return Sum(summands);
}

std::string Sum::output()
{
	std::string out = arguments.at(0).output();

	for (int i = 1; i < arguments.size(); i++)
	{
		out += "+" + arguments.at(i).output();
	}

	return out;
}
