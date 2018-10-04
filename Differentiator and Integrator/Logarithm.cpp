#include "Logarithm.h"

Logarithm::Logarithm(Term* term)
	: MathOperator(term)
{

}

std::string Logarithm::output()
{
	return "ln(" + arguments.at(0)->output() +")";
}