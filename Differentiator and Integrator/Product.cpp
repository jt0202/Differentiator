#include "Product.h"

Product::Product(Term* factor1, Term* factor2)
	: MathOperator(factor1, factor2, TERMTYPE_PROD)
{

}

Product::Product(std::vector<Term*> terms)
	: MathOperator(terms, TERMTYPE_PROD)
{

}

std::string Product::output()
{
	std::string out;
	if (getPrecedence(convertTermTypeToString(arguments.at(0)->getTermType())) < MultiplicationLevel)
	{
		out = "(" + arguments.at(0)->output() + ")";
	}
	else
	{
		out = arguments.at(0)->output();
	}

	for (int i = 1; i < arguments.size(); i++)
	{
		if(getPrecedence(convertTermTypeToString(arguments.at(i)->getTermType())) < MultiplicationLevel)
		{
			out += "\*(" + arguments.at(i)->output()+ ")";
		}
		else
		{
			out += "*" + arguments.at(i)->output();
		}
	}

	return out;

	std::string a;
}

Term* Product::differentiate(char variable)
{
	std::vector<Term*> summands;

	// Usuage of the product rule but in a more general version.
	// Let f be a product of n factors,
	// then f' = sum_i=0^n (f_i * prod_k=0&&k!=i^n f_k). (// Pseudo-LaTeX)
	// Can be obtained by reducing a product of n factors
	// to a arbitrary factor of these and all others factors in a seperate
	// bracket pair, since the multiplication is associative. 
	for (int i = 0; i < arguments.size(); i++)
	{
		std::vector<Term*> s;
		s.resize(arguments.size());
		for (int j = 0; j < arguments.size();j++ )
		{
			if (i == j)
			{
				s.at(j) = arguments.at(j)->differentiate(variable);
			}
			else
			{
				s.at(j) = arguments.at(j);
			}
		}
		summands.push_back(new Product(s));
	}

	return new Sum(summands);
}