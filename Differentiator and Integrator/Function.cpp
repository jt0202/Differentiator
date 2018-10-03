#include "Function.h"

Function::Function(std::string name, Term* argument)
	:MathOperator(argument)
{
	functionName = name;
}