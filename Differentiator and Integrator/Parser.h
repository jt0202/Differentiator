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

	bool isOperator(std::string i_input);
public:
	Parser(std::string i_input);

	bool parse();

	Term* getTree();
};