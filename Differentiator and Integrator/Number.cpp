#include "Number.h"

Number::Number(std::string i_input)
	: m_text(i_input)
{
	setDiff(std::bind(&Number::differentiate, *this, std::placeholders::_1));
	setOutp(std::bind(&Number::output, *this));
}

std::string Number::output()
{
	return m_text;
}

Term Number::differentiate(char var)
{
	return Number("0");
}

int Number::getDenominator()
{
	return denominator;
}

int Number::getNumerator()
{
	return numerator;
}