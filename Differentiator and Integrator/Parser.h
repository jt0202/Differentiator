#pragma once
#include <string>
#include <vector>
#include <stack>
#include <ctype.h>

#include "Lexer.h"
#include "Exponent.h"
#include "Number.h"
#include "Variable.h"
#include "Sum.h"
#include "Product.h"
#include "Precedence.h"

// Temporary include
#include <iostream>




enum TokenType
{
	NUMBER,
	VARIABLE,
	OPERATOR,
	FUNCTION,
	COMMA,
	OPENBRACKET,
	CLOSEDBRACKET,
	UNKNOWN
};

class Parser
{
private:	
	std::string m_input;
	std::vector<std::string> m_tokens;

	TokenType getType(std::string token);

	Term* m_tree;

	bool isNumber(std::string i);

	bool unaryOperator(int pos);

	bool isLeftAssociative(std::string i_operator);

	bool isOperator(std::string i_input);

	bool isFunction(std::string i_input);

	bool createTerm(std::string symbol, std::vector<Term*>* output);
public:
	Parser(std::string i_input);

	bool parse();

	Term* getTree();
};