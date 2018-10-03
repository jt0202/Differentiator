#pragma once
#include "Operator.h"
#include <string>

class Function : public MathOperator
{
private:
	std::string functionName;

public:
	Function(std::string name, Term* argument);
};