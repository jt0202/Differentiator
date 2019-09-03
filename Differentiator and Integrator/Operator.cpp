#include "Operator.h"

MathOperator::MathOperator(Term* term, TermType termtype)
	: Term(termtype)
{
	arguments.push_back(term);
}

MathOperator::MathOperator(Term* term1, Term* term2, TermType termtype)
	: Term(termtype)
{
	arguments.push_back(term1);
	arguments.push_back(term2);
}

MathOperator::MathOperator(std::vector<Term*> terms, TermType termtype)
	: Term(termtype)
{
	for (Term* t : terms)
	{
		arguments.push_back(t);
	}
}

MathOperator::MathOperator()
	: Term(TERMTYPE_NONE)
{

}