#pragma once
#include "Sum.h"
#include "Product.h"

namespace SumHelp
{
	// For each element in summands exists a vector in factors that represent a sum in such a way:
	//		(factor_11+factor_12+....)*summand_1+(factor_21+factor_22+...)*summand_2+...
	// It checks if there exists already an element in summands and if so adds factor to the corresponding vector.
	// Else it creates for both a new entry
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

	Term* productCreater(std::vector<Term*> factors)
	{
		if (factors.size() > 1)
		{
			return new Product(factors);
		}
		else
		{
			if (factors.size() == 1)
			{
				return factors.at(0);
			}
			else
			{
				return new Number(1);
			}
		}
	}

	// Disassembles a product that appears in a sum in 2 parts: one contains all the factors of that product that contain the mainvar
	// and another part that contains the rest and adds these to the sum structure.
	// If no mainvar is present one part only contains the numbers.
	void productDisassembler(Term* product, char mainvar, std::vector<Term*>& summands, std::vector<std::vector<Term*>>& factors)
	{
		// Every factor that contains mainvar.
		std::vector<Term*> mainVariable;
		// All other factors that aren't numbers.
		std::vector<Term*> otherTerms;
		// Numbers are kept seperately. If no mainvar occurs
		// then the otherTerms are getting collected.
		std::vector<Term*> productNumbers;

		// Go through the product and split it's factors into the 3 upper categories
		for (int j = 0; j < product->getArguments().size(); j++)
		{
			Term* currentFactor = product->getArguments().at(j);

			if (currentFactor->containsVar(mainvar))
			{
				mainVariable.push_back(currentFactor);
			}
			else
			{
				if (currentFactor->getTermType() == TERMTYPE_NUM)
				{
					productNumbers.push_back(currentFactor);
				}
				else
				{
					otherTerms.push_back(currentFactor);
				}
			}
		}

		if (otherTerms.size() > 0 && mainVariable.size() > 0)
		{
			otherTerms.insert(otherTerms.end(), productNumbers.begin(), productNumbers.end());
			SumHelp::collectTerms(summands, factors, SumHelp::productCreater(mainVariable), SumHelp::productCreater(otherTerms));
		}
		else
		{
			if (mainVariable.size() == 0)
			{
				SumHelp::collectTerms(summands, factors, SumHelp::productCreater(otherTerms), SumHelp::productCreater(productNumbers));
			}
			else
			{
				// otherTerms.size() == 0
				SumHelp::collectTerms(summands, factors, SumHelp::productCreater(mainVariable), SumHelp::productCreater(productNumbers));

			}
		}
	}
}

Sum::Sum(Term* summand1, Term* summand2)
	: MathOperator(summand1, summand2, TERMTYPE_SUM)
{
}

Sum::Sum(std::vector<Term*> terms)
	:MathOperator(terms, TERMTYPE_SUM)
{
	// A sum with less than 2 summands might be possible mathematically,
	// it isn't possible to parse and makes looking for identical terms
	// in the simplification process more difficult.
	if (terms.size() < 2)
	{
		throw new std::invalid_argument("Less than 2 summands");
	}
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
	if (arguments.size() == 1)
	{
		return arguments.at(0);
	}

	std::vector<Term*> arguments = simplifySubTerms(mainvar);

	combineSameTerms(arguments);
	
	// Very similar to simplification of products. Look there for more details. 

	std::vector<Term*> summands;
	std::vector<std::vector<Term*>> factors;
	std::vector<Number*> numbers;

	for (int i = 0; i < arguments.size(); i++)
	{
		if (arguments.at(i)->getTermType() == TERMTYPE_PROD)
		{			
			SumHelp::productDisassembler(arguments.at(i), mainvar, summands, factors);
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

	// Rebuilt a sum from the split components
	for (int i = 0; i < summands.size(); i++)
	{
		Term* t;
		if (factors.at(i).size() > 1)
		{
			t = new Product(new Sum(factors.at(i)), summands.at(i));
		}
		else
		{
			t = new Product(factors.at(i).at(0), summands.at(i));
		} 


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

	// If there are numbers present, add all together and add the result(if it's not 0) to the output.
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