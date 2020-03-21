#include "Product.h"

Product::Product(Term* factor1, Term* factor2)
	: MathOperator(factor1, factor2, TERMTYPE_PROD)
{

}

Product::Product(std::vector<Term*> terms)
	: MathOperator(terms, TERMTYPE_PROD)
{

}

std::string Product::output() const
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

Term* Product::simplify(char mainvar)
{
	std::vector<Term*> arguments = simplifySubTerms(mainvar);
	
	combineSameTerms(arguments);

	// At a position i bases holds the base of a factor and exponents the to this base
	// belonging exponent. By this I want to collect the exponents if they appear multiple times in 
	// a product(e.g. x^2 * x^3 leads to x in bases and {2,3} in exponents at position 1
	// I first thought about an associative container but I need a better criteria than pointer equality
	// to determine equality( the equals method in term). std::map demands some kind of order, which I 
	// couldn't think of and for std::unordered_map I would need a custom hash function, which was equally 
	// difficult.

	std::vector<Term*> bases;
	std::vector<std::vector<Term*>> exponents;

	std::vector<Number*> numbers;
	
	// Filling the vectors.
	for (int i = 0; i < arguments.size(); i++)
	{
		if (bases.size() != exponents.size())
		{
			throw std::out_of_range("Each base should have an exponent.");
		}

		if (arguments.at(i)->getTermType() == TERMTYPE_EXP)
		{
			collectTerms(bases, exponents, arguments.at(i)->getArguments().at(BASE), arguments.at(i)->getArguments().at(EXPONENT));
		}
		else
		{
			if (arguments.at(i)->getTermType() == TERMTYPE_NUM)
			{
				numbers.push_back(dynamic_cast<Number*>(arguments.at(i)));
				continue;
			}

			collectTerms(bases, exponents, arguments.at(i), new Number(1));
		}
	}
	std::vector<Term*> o_arguments;

	// Convert vectors back into term tree
	for (int i = 0; i < bases.size(); i++)
	{
		Term* t;
		if (exponents.at(i).size() > 1)
		{
			t = new Exponent(new Sum(exponents.at(i)), bases.at(i));
		}
		else
		{
			t = new Exponent(exponents.at(i).at(0), bases.at(i));
		}

		// Simplification deals with exponent 1 and combines them.

		Term* simplified = t->simplify(mainvar);

		// If the simplification leads to a number, it gets processed later.
		if (simplified->getTermType() == TERMTYPE_NUM)
		{
			Number* n = dynamic_cast<Number*>(simplified);

			// It's a 1
			if (n->getDenominator() == n->getNumerator())
			{
				continue;
			}

			numbers.push_back(n);
			continue;
		}

		o_arguments.push_back(simplified);
	}

	if (numbers.size() >= 1)
	{
		Number* start = numbers.front();

		numbers.erase(numbers.begin());

		Number* num = start->multiply(numbers);

		// Zero leads to making the whole term zero.
		if (num->getDenominator() == 0)
		{
			return new Number(0);
		}
		else
		{
			// If all numbers not a 1, add them to the new product.
			if (num->getDenominator() != num->getNumerator())
			{
				// mainly for aesthetical reasons. 2*x looks more familar than x*2
				o_arguments.insert(o_arguments.begin(), num);
			}
		}

	}
	if (o_arguments.size() > 0)
	{
		// A product of just one term isn't really a product.
		if (o_arguments.size() == 1)
		{
			return o_arguments.at(0);
		}

		return new Product(o_arguments);
	}
	else
	{
		// Everything cancelled each other out. There return the identity of multiplication
		return new Number(1);
	}
}

// This is a helper function for the simplifcation. If the base is already present it adds the newExponent to the current ones,
// otherwise it pushs both at the end.
void collectTerms(std::vector<Term*>& bases, std::vector<std::vector<Term*>>& exponents, Term* newBase, Term* newExponent)
{
	for (int i = 0; i < bases.size(); i++)
	{
		if (bases.at(i)->equals(newBase))
		{
			exponents.at(i).push_back(newExponent);

			return;
		}
	}

	bases.push_back(newBase);
	exponents.push_back(std::vector<Term*>{newExponent});
}
