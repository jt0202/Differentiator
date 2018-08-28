#pragma once
#include "Function.h"
#include <vector>

class Sum : public Function
{
private:

public:
	Sum(std::vector<Term> arguments);

	Term differentiate();
};