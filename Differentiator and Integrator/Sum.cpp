#pragma once
#include "Sum.h"
#include "Product.h"

#include <iostream>

namespace SumHelp
{
	void collectTerms(std::vector<Term*>& summands, std::vector<std::vector<Term*>>& factors, Term* summand, Term* factor)
	{
		for (int i = 0; i < summands.size(); i++)
		{
			if (summands.at(i)->equals(summand))
			{
				factors.at(i).push_back(factor);
				return;
			}
		}

		summands.push_back(summand);
		factors.push_back(std::vector<Term*>{factor});
	}
}

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

std::string Sum::output() const
{
	std::string out = arguments.at(0)->output();

	for (int i = 1; i < arguments.size(); i++)
	{
		out += "+" + arguments.at(i)->output();
	}

	return out;
}

Term* Sum::simplify(char mainvar) 
{
	std::vector<Term*> arguments = simplifySubTerms(mainvar);

	combineSameTerms(arguments);

	std::vector<Term*> summands;
	std::vector<std::vector<Term*>> factors;
	std::vector<Number*> numbers;

	for (int i = 0; i < arguments.size(); i++)
	{
		if (arguments.at(i)->getTermType() == TERMTYPE_PROD)
		{			
			std::vector<Term*> containsVar;
			std::vector<Term*> containsNotVar;

			for (int j = 0; j < arguments.at(i)->getArguments().size(); j++)
			{
				Term* currentFactor = arguments.at(i)->getArguments().at(j);

				if (currentFactor->containsVar(mainvar))
				{
					containsVar.push_back(currentFactor);
				}
				else
				{
					containsNotVar.push_back(currentFactor);
				}
			}

			if (containsNotVar.size() > 0 && containsVar.size() > 0)
			{
				SumHelp::collectTerms(summands, factors, new Product(containsVar), new Product(containsNotVar));
			}
			else
			{
				if (containsNotVar.size() == 0)
				{
					SumHelp::collectTerms(summands, factors, new Product(containsVar), new Number(1));
				}
				else
				{
					SumHelp::collectTerms(summands, factors, new Product(containsNotVar), new Number(1));
				}
			}
		}
		else
		{
			if (arguments.at(i)->getTermType() == TERMTYPE_NUM)
			{
				Number* n = dynamic_cast<Number*>(arguments.at(i));
				numbers.push_back(n);
				continue;
			}

			SumHelp::collectTerms(summands, factors, arguments.at(i), new Number(1));
	
		}
	}
	std::vector<Term*> o_arguments;

	for (int i = 0; i < summands.size(); i++)
	{
		Term* t = new Product(new Sum(factors.at(i)), summands.at(i));

		Term* simplified = t->simplify(mainvar);

		if (simplified->getTermType() == TERMTYPE_NUM)
		{
			Number* n = dynamic_cast<Number*>(simplified);

			if (n->getDenominator() == 0)
			{
				continue;
			}

			numbers.push_back(n);
			continue;
		}
		o_arguments.push_back(simplified);
	}


	if (numbers.size() > 0)
	{
		Number* n = numbers.front();

		numbers.erase(numbers.begin());

		Number* num = n->add(numbers);

		if (num->getDenominator() != 0)
		{
			o_arguments.push_back(num);
		}
	}

	if (o_arguments.size() > 0)
	{
		if (o_arguments.size() == 1)
		{
			return o_arguments.at(0);
		}

		return new Sum(o_arguments);
	}
	else
	{
		return new Number(0);
	}
}



