#include "Number.h"

// Implementation of Euclid's algorithm to calculate the greater
// common divisor to shorten fractions.
// It does not matter whether a or b is larger.
// It always returns a non-negative number
int euclid(int a, int b)
{
	// Swap variables so that a is greater than b
	if (b > a)
	{
		int tmp = a;
		a = b;
		b = tmp;
	}

	while (b != 0)
	{
		int tmp = a % b;
		a = b;
		b = tmp;
	}

	return std::abs(a);
}

Number::Number(int value)
{
	denominator = value;
	numerator = 1;
}

Number::Number(int denominator, int numerator)
{
	int divisor = euclid(denominator, numerator);

	this->denominator = denominator / divisor;
	this->numerator = numerator / divisor;
}

Term* Number::add(std::vector<Number*> summands)
{
	int numerator = getNumerator();
	int denominator = getDenominator();

	for (Number* num : summands)
	{
		int lcm = std::abs(numerator * num->getNumerator()) / euclid(numerator, num->getNumerator());

		denominator = denominator * lcm / numerator + num->getDenominator() * lcm / num->getNumerator();
		
		numerator = lcm;

		int gcd = euclid(denominator, numerator);
		denominator = denominator / gcd;
		numerator = numerator / gcd;
	}

	return new Number(denominator, numerator);
}

Term* Number::multiply(std::vector<Number*> factors)
{
	int numerator = getNumerator();
	int denominator = getDenominator();

	for (Number* num : factors)
	{
		numerator *= num->getNumerator();
		denominator *= num->getDenominator();

		int gcd = euclid(denominator, numerator);

		denominator /= gcd;
		numerator /= gcd;
	}

	return new Number(denominator, numerator);
}

// Returns a number if the exponent is an integer or
// if the result is a rational number. If the result would 
// be irrational (e.g. sqrt(2)= 2^(1/2)) it returns a nullptr.
Term* Number::exponentiate(Number* exponent)
{
	if (exponent->isFraction())
	{
		// Test if both the numerator and the denominator are perfect 
		// k powers with k being the numerator of the exponent.
		double t = std::pow(denominator,(double) 1 / exponent->getNumerator());

		int o_denominator = std::floor(t);

		if (std::floor(std::pow(o_denominator, exponent->getNumerator())) != denominator)
		{
			return nullptr;
		}
		else
		{
			// Casting to double so that 1/exponent->getNumerator won't get rounded to an int.
			int o_numerator = std::floor(std::pow(numerator, (double) 1 / exponent->getNumerator()));

			if (std::floor(std::pow(o_numerator, exponent->getNumerator())) != numerator)
			{
				return nullptr;
			}
			else
			{
				return new Number(std::pow(o_denominator, exponent->getDenominator()), std::pow(o_numerator, exponent->getDenominator()));
			}
		}
	}
	else
	{
		return new Number(std::pow(denominator, exponent->getDenominator()), std::pow(numerator, exponent->getDenominator()));
	}
}

std::string Number::output()
{
	if (isFraction())
	{
		return std::to_string(denominator) + "/" + std::to_string(numerator);
	}
	else
	{
		return std::to_string(denominator);
	}
}

Term* Number::differentiate(char var)
{
	return new Number(0);
}

int Number::getDenominator()
{
	return denominator;
}

int Number::getNumerator()
{
	return numerator;
}

bool Number::isFraction()
{
	return numerator != 1;
}

