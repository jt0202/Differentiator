#include "Exponent.h"

Exponent::Exponent(Term* exponent, Term* base)
	: MathOperator(exponent, base)
{

}

std::string Exponent::output()
{
	return "(" + arguments.at(BASE)->output() + ")" + "^" + "(" + arguments.at(EXPONENT)->output() + ")";
}