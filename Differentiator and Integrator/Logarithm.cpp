#include "Logarithm.h"

Logarithm::Logarithm(Term* term)
	: MathOperator(term, TERMTYPE_LOG)
{
	
}

std::string Logarithm::output() const
{
	return "ln(" + arguments.at(0)->output() +")";
}

Term* Logarithm::differentiate(char var)
{
	// Example ln(x^2) = 1/x^2 * 2x = (x^2)^(-1) * 2x
	return new Product(new Exponent(new Number(-1,1), arguments.at(0)), arguments.at(0)->differentiate(var));

}