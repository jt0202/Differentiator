#pragma once
#include <vector>

// This class represents the terms that compose the equation,
// like sums, trigonometric functions or variables. They build
// a abstract syntax tree for easier representation using the
// composite pattern. This class is abstract like it's leaf
// and  composites.
class Term
{
protected:
	//The arguments of the "function". 0 if it's a variable or number.
	std::vector<Term*> arguments;

	//Some functions like sin, cos have limited number of arguments, whereas
	// sums or products may have infinite number of arguments.
	//int maxArguments;

	std::string type;
public:
	// virtual Term* differentiate() = 0;

	// virtual Term* evaluateSym() = 0;

};