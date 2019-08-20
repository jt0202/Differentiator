#include "Operator.h"

MathOperator::MathOperator(Term* term)
{
	arguments.push_back(term);
}

MathOperator::MathOperator(Term* term1, Term* term2)
{
	arguments.push_back(term1);
	arguments.push_back(term2);
}

MathOperator::MathOperator(std::vector<Term*> terms)
{
	for (Term* t : terms)
	{
		arguments.push_back(t);
	}
}

MathOperator::MathOperator()
{

}