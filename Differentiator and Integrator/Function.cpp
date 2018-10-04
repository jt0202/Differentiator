#include "Function.h"

Function::Function(std::string name, Term* argument)
	:MathOperator(argument)
{
	functionName = name;
}

std::string Function::output()
{
	return functionName + "(" + arguments.at(0)->output() + ")";
}