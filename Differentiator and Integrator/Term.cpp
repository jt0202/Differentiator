#include "Term.h"

Term* Term::simplify(char mainvar)
{
	return this;
}

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

bool Term::equals(Term* t)
{
	// Easier to conditions to check first, but not sufficient.
	if (getTermType() == t->getTermType() && arguments.size() == t->getArguments().size())
	{
		// Stores the position of an argument of t that is equal to an argument of this Term.
		// Prevents that an argument of t isn't twice counted for 2 arguments in this term.
		// Since just equality is checked, this is sufficient and doesn't need more information like
		// the pairs of equal elements.
		// Set used because of faster search and uniqueness. Also no random access needed.
		std::set<int> equivalentTermPos;

		for (int i = 0; i < arguments.size(); i++)
		{
			for (int j = 0; j < t->getArguments().size(); j++)
			{
				// This term already has an equal term.
				if (equivalentTermPos.count(j) > 0)
				{
					continue;
				}

				// Equal found.
				if (arguments.at(i)->equals(t->getArguments().at(j)))
				{
					equivalentTermPos.emplace(j);
					break;
				}
			}
		}

		// Every term has an equal element.
		if (equivalentTermPos.size() == arguments.size())
		{
			return true;
		}
	}
	
	return false;
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
