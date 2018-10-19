#include "Exponent.h"

Exponent::Exponent(Term* exponent, Term* base)
	: MathOperator(exponent, base)
{

}

std::string Exponent::output()
{
	return "(" + arguments.at(BASE)->output() + ")" + "^" + "(" + arguments.at(EXPONENT)->output() + ")";
}

// Instead of using power rule or exponent rule as in school
// and deciding between the cases, we are using a more general rule.
// By using the identity a^b = exp(ln(a^b)) = exp(ln(a)*b) we transform it.
// exp(u)' = exp(u), so only the argument has to be differentiated by the 
// chain rule. This allows to differentiate terms like x^x and x^n and a^x
// are special cases that appear after simplification. 
Term* Exponent::differentiate(char var)
{
	Term* helpTerm = new Product(arguments.at(EXPONENT), new Logarithm(arguments.at(BASE)));

	Term* differentiatedTerm = helpTerm->differentiate(var);

	return new Product(differentiatedTerm, this);
}