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
	if (exponent->getDenominator() == 0 && this->getDenominator() == 0)
	{
		throw std::invalid_argument("0^0 is not defined.");
	}

	int denominator = getDenominator();
	int numerator = getNumerator();
	int exp_denom = exponent->getDenominator();

	// If the exponent is negative swap denominator and numerator and calculate with positive exponent on
	if (exp_denom < 0)
	{
		int tmp = denominator;
		denominator = numerator;
		numerator = tmp;
		exp_denom = -exp_denom;
	}

	if (exponent->isFraction())
	{
		// Test if both the numerator and the denominator are perfect 
		// k powers with k being the numerator of the exponent.
		int o_denominator = std::lround(std::pow(denominator, (double)1 / exponent->getNumerator()));

		if (std::lround(std::pow(o_denominator, exponent->getNumerator())) == denominator)
		{
			// Casting to double so that 1/exponent->getNumerator won't get rounded to an int.
			int o_numerator = std::lround(std::pow(numerator, (double)1 / exponent->getNumerator()));

			if (std::lround(std::pow(o_denominator, exponent->getNumerator())) == numerator)
			{
				return new Number(std::lround(std::pow(o_denominator, exp_denom)), std::lround(std::pow(o_numerator, exp_denom)));
			}
			else
			{
				return nullptr;
			}
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		return new Number(std::lround(std::pow(denominator,exp_denom)), std::lround(std::pow(numerator, exp_denom)));
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

