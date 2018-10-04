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