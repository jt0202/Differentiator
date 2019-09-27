#include "Exponent.h"

Exponent::Exponent(Term* exponent, Term* base)
	: MathOperator(exponent, base, TERMTYPE_EXP)
{
	
}

std::string Exponent::output() const
{
	return "(" + arguments.at(BASE)->output() + ")" + "^" + "(" + arguments.at(EXPONENT)->output() + ")";
}

bool Exponent::equals(const Term* t) const
{
	if (t->getTermType() == TERMTYPE_EXP)
	{
		const Exponent* exp = dynamic_cast<const Exponent*>(t);

		return arguments.at(BASE)->equals(exp->getArguments().at(BASE))
			&& arguments.at(EXPONENT)->equals(exp->getArguments().at(EXPONENT));
	}

	return false;
}

// Instead of using power rule or exponent rule as in school
// and deciding between the cases, we are using a more general rule.
// By using the identity a^b = exp(ln(a^b)) = exp(ln(a)*b) we transform it.
// exp(u)' = exp(u), so only the argument has to be differentiated by the 
// chain rule. This allows to differentiate terms like x^x ; x^n and a^x
// are special cases that appear after simplification. 
Term* Exponent::differentiate(char var)
{
	Product* helpTerm = new Product(arguments.at(EXPONENT),  new Logarithm(arguments.at(BASE)));

	Term* differentiatedTerm = helpTerm->differentiate(var);

	return new Product(differentiatedTerm, this);
}

// Simplifaction for now mainly occurs for numbers since the
// simplification rules have requirements that are difficult to 
// check for other terms.
Term* Exponent::simplify(char mainvar)
{
	
	std::vector<Term*> arguments = simplifySubTerms(mainvar);

	if (arguments.at(EXPONENT)->getTermType() == TERMTYPE_NUM)
	{
		Number* n = dynamic_cast<Number*>(arguments.at(EXPONENT));

		if (n->getDenominator() == n->getNumerator())
		{
			return arguments.at(BASE);
		}
		if (n->getDenominator() == 0)
		{
			return new Number(0);
		}
		if (arguments.at(BASE)->getTermType() == TERMTYPE_NUM)
		{
			Number* base = dynamic_cast<Number*>(arguments.at(BASE));

			Number* result = base->exponentiate(n);

			if (result != nullptr)
			{
				return result;
			}
		}

	}

	return this;
}