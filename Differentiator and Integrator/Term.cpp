#include "Term.h"

SmartPointerVector<Term> Term::getArguments()
{
	return arguments;
}

TermType Term::getTermType()
{
	return m_termtype;
}

Term::Term(TermType termtype)
	: m_termtype(termtype)
{

}

std::string convertTermTypeToString(TermType termtype)
{
	std::string out;

	switch (termtype)
	{
	case TERMTYPE_EXP:
		out = "^";
		break;
	case TERMTYPE_SUM:
		out = "+";
		break;
	case TERMTYPE_PROD:
		out = "*";
		break;

	default:
		out = "Func";
		break;
	}

	return out;
}
