#pragma once
#include <string>

// Here are the precedence information for a correct parse stored.
// Precedence means that e.g. * will be calculated before +

enum PrecedenceLevel
{
	FunctionLevel = 0,
	ExponentLevel = 1,
	MultiplicationLevel = 2,
	AdditionLevel = 3
};

int getPrecedence(std::string i_operator);