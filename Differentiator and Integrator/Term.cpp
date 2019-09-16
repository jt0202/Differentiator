#include "Term.h"

// Replaces every argument by it's simplified variant(might be still the same sometimes).
void Term::simplifySubTerms(char mainvar)
{
	for (int i = 0; i < arguments.size(); i++)
	{
		arguments.replace(arguments.at(i)->simplify(mainvar), i);
	}
}

// Replaces the arguments that have the same type as this object 
// by their arguments.
// Useful to turn sums or products of 2 summands/products into larger ones.
// This allows that 1/x and x can cancel eachother out, which would be more difficult
// if they would be in different branches of the tree.
// Should not be used for other terms than sums or products
void Term::combineSameTerms()
{
	std::vector<Term*> lowerTerms;
	std::vector<int> termsToErase;
	for (int i = 0; i < arguments.size(); i++)
	{
		if (arguments.at(i)->getTermType() == this->getTermType())
		{
			for (std::shared_ptr<Term> sp : arguments.at(i)->getArguments())
			{
				lowerTerms.push_back(sp.get());
			}

			termsToErase.push_back(i);
		}
	}

	for (Term* t : lowerTerms)
	{
		arguments.push_back(t);
	}

	// Deleted later instead of in for loop
	// to not step over terms.
	for (int i : termsToErase)
	{
		arguments.erase(i);
	}
}

Term* Term::simplify(char mainvar)
{
	return this;
}

SmartPointerVector<Term> Term::getArguments() const
{
	return arguments;
}

TermType Term::getTermType() const
{
	return m_termtype;
}

Term::Term(TermType termtype)
	: m_termtype(termtype)
{

}

bool Term::equals(const Term* t) const
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
