#include "Precedence.h"

int getPrecedence(std::string i_operator)
{
	if (i_operator == "^")
	{
		return ExponentLevel;
	}
	if (i_operator == "+" || i_operator == "-")
	{
		return AdditionLevel;
	}

	if (i_operator == "*" || i_operator == "/" || i_operator == "neg" )
	{
		return MultiplicationLevel;
	}
}