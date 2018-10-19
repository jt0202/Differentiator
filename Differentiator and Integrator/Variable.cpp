#include "Variable.h"

Variable::Variable(char i_name)
	:m_name(i_name)
{

}

std::string Variable::output()
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
		return new Number("1");
	}
	else
	{
		return new Number("0");
	}
}