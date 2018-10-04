#include "Product.h"

Product::Product(Term* factor1, Term* factor2)
	: MathOperator(factor1, factor2)
{

}

std::string Product::output()
{
	std::string out;
	if (getPrecedence(arguments.at(0)->getType()) > MultiplicationLevel)
	{
		out = "(" + arguments.at(0)->output() + ")";
	}
	else
	{
		out = arguments.at(0)->output();
	}

	for (int i = 1; i < arguments.size(); i++)
	{

		if (getPrecedence(arguments.at(i)->getType()) > MultiplicationLevel)
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