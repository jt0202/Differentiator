#include "Logarithm.h"

Logarithm::Logarithm(Term term)
	: MathOperator(term)
{
	setDiff(std::bind(&Logarithm::differentiate, *this, std::placeholders::_1));
	setOutp(std::bind(&Logarithm::output, *this));
}

std::string Logarithm::output()
{
	return "ln(" + arguments.at(0).output() +")";
}

Term Logarithm::differentiate(char var)
{
	// Example ln(x^2) = 1/x^2 * 2x = (x^2)^(-1) * 2x
	return Product(Exponent(Number("-1"), arguments.at(0)), arguments.at(0).differentiate(var));

}