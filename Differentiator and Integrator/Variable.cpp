#include "Variable.h"

Variable::Variable(char i_name)
	:m_name(i_name), Atom(TERMTYPE_VAR)
{

}

std::string Variable::output() const
{
	std::stringstream stream;

	std::string output;

	stream << m_name;

	stream >> output;

	return output;
}

Term* Variable::differentiate(char var)
{
	if (m_name == var)
	{
		return new Number(1,1);
	}
	else
	{
		return new Number(0,1);
	}
}

bool Variable::equals(const Term* t)
{
	if (t->getTermType() == TERMTYPE_VAR)
	{
		const Variable* var = dynamic_cast<const Variable*>(t);

		return var->output() == output();
	}
	return false;
}

bool Variable::containsVar(char var) const
{
	return m_name == var;
}
