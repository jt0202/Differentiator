#pragma once
#include "Operator.h"
#include <string>
#include "Product.h"

class Function : public MathOperator
{
private:
	std::string functionName;

public:
	Function(std::string name, Term* argument);

	Term* differentiate(char var);

	std::string output();
};