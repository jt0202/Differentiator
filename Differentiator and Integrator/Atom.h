#pragma once
#include "Term.h"

// This class represents the basic elements
// of the equations like numbers or variables
// and are the leafs of this tree
class Atom : public Term
{
public:
	//virtual int degree() = 0;

	virtual std::string output() = 0;

	Atom(TermType termtype);
};