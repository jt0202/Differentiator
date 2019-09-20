#pragma once
#include <vector>
#include <string>
#include <functional>
#include <stdexcept>
#include <set>
#include "SmartPointerVector.h"


enum TermType
{
	TERMTYPE_EXP,
	TERMTYPE_FUN,
	TERMTYPE_LOG,
	TERMTYPE_NUM,
	TERMTYPE_PROD,
	TERMTYPE_SUM,
	TERMTYPE_VAR,
	TERMTYPE_NONE
};

// This class represents the terms that compose the equation,
// like sums, trigonometric functions or variables. They build
// a abstract syntax tree for easier representation using the
// composite pattern. This class is abstract like it's leaf
// and  composites.
class Term
{
protected:
	//The arguments of the "function". 0 arguments if it's a variable or number.
	SmartPointerVector<Term> arguments;

	//Some functions like sin, cos have limited number of arguments, whereas
	// sums or products may have infinite number of arguments.
	//int maxArguments;

	const TermType m_termtype;

	std::vector<Term*> simplifySubTerms(char mainvar);

	void combineSameTerms(std::vector<Term*>& arguments);

public:
	virtual Term* differentiate(char variable) = 0;

	virtual Term* simplify(char mainvar);

	virtual std::string output() const = 0;

	SmartPointerVector<Term> getArguments() const;

	TermType getTermType() const;

	Term(TermType termtype);

	virtual bool equals(const Term* t) const;

	Term() = delete;

	virtual bool containsVar(char var) const;
};

std::string convertTermTypeToString(TermType termtype);