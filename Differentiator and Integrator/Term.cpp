#include "Term.h"

std::string Term::getType()
{
	return type;
}

SmartPointerVector<Term> Term::getArguments()
{
	return arguments;
}



