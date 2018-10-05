#pragma once
#include <string>

// Here are the precedence information for a correct parse stored.
// Precedence means that e.g. * will be calculated before +

enum PrecedenceLevel
{
	FunctionLevel = 3,
	ExponentLevel = 2,
	MultiplicationLevel = 1,
	AdditionLevel = 0
};

int getPrecedence(std::string i_operator);