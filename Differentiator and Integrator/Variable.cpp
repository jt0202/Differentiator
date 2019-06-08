#include "Variable.h"

Variable::Variable(char i_name)
	:m_name(i_name)
{
	setDiff(std::bind(&Variable::differentiate, *this, std::placeholders::_1));
	setOutp(std::bind(&Variable::output, *this));
}

std::string Variable::output()
{
	std::stringstream stream;

	std::string output;

	stream << m_name;

	stream >> output;

	return output;
}

Term Variable::differentiate(char var)
{
	if (m_name == var)
	{
		return Number("1");
	}
	else
	{
		return Number("0");
	}
}