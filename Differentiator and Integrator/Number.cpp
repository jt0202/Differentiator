#include "Number.h"

Number::Number(std::string i_input)
	: m_text(i_input)
{

}

std::string Number::output()
{
	return m_text;
}
