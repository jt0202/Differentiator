#include "Term.h"

Term Term::differentiate(char variable)
{
	return diff(variable);
}

std::string Term::output()
{
	return outp();
}

std::string Term::getType()
{
	return type;
}

std::vector<Term> Term::getArguments()
{
	return arguments;
}

void Term::setDiff(std::function<Term(char)> i_diff)
{
	diff = i_diff;
}

void Term::setOutp(std::function<std::string()> i_outp)
{
	outp = i_outp;
}
