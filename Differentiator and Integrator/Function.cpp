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

Term* Function::differentiate(char var)
{
	// For now the derivates of functions other than the logarithm are unknown
	return new Product(new Function("Deriv(" + functionName + ")", arguments.at(0)), arguments.at(0)->differentiate(var));
}