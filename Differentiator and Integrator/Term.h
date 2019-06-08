#pragma once
#include <vector>
#include <string>
#include <functional>

// This class represents the terms that compose the equation,
// like sums, trigonometric functions or variables. They build
// a abstract syntax tree for easier representation using the
// composite pattern. This class is abstract like it's leaf
// and  composites.
class Term
{
protected:
	//The arguments of the "function". 0 arguments if it's a variable or number.
	std::vector<Term> arguments;

	//Some functions like sin, cos have limited number of arguments, whereas
	// sums or products may have infinite number of arguments.
	//int maxArguments;

	std::string type = " ";

	std::function<Term(char)> diff;
	std::function<std::string()> outp;
public:
	Term differentiate(char variable);

	// virtual Term* evaluateSym() = 0;

	virtual std::string output();

	std::string getType();

	std::vector<Term> getArguments();

	void setDiff(std::function<Term(char)> i_diff);
	void setOutp(std::function<std::string()> i_outp);

};