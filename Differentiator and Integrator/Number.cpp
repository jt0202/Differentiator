#include "Number.h"

Number::Number(std::string i_input)
	: m_text(i_input)
{

}

std::string Number::output()
{
	return m_text;
}

Term* Number::differentiate(char var)
{
	return new Number("0");
}

int Number::getDenominator()
{
	return denominator;
}

int Number::getNumerator()
{
	return numerator;
}