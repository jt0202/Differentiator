#pragma once
#include <string>
#include <vector>
#include <stack>
#include <ctype.h>
#include <algorithm>
#include <stdexcept>
#include <cmath>

#include "Lexer.h"
#include "Exponent.h"
#include "Number.h"
#include "Variable.h"
#include "Sum.h"
#include "Product.h"
#include "Precedence.h"
#include "Fraction.h"
#include "Logarithm.h"
#include "Function.h"
// Temporary include
#include <iostream>


enum TokenType
{
	TOKENTYPE_NUMBER,
	TOKENTYPE_VARIABLE,
	TOKENTYPE_OPERATOR,
	TOKENTYPE_FUNCTION,
	TOKENTYPE_COMMA,
	TOKENTYPE_OPENBRACKET,
	TOKENTYPE_CLOSEDBRACKET,
	TOKENTYPE_UNKNOWN
};

class Parser
{
private:	
	std::string m_input;
	std::vector<std::string> m_tokens;

	std::vector<Term*> output;

	const std::vector<std::string> m_operators = {"+", "-", "*", "/", "^"};

	TokenType getType(std::string token);

	bool isNumber(std::string i);

	bool unaryOperator(int pos);

	bool isLeftAssociative(std::string i_operator);

	bool isOperator(std::string i_input);

	bool isFunction(std::string i_input);

	bool createTerm(std::string symbol);

	Term* getLastElement();

	Number* makeFraction(std::string i_input);
public:
	Parser(std::string i_input);

	bool parse();

	Term* getTree();
};

