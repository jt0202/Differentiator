#include "Parser.h"

// Wrapper function that isdigit is more convinient 
// to use with bool
bool isDigit(char c)
{
	return isdigit(c) != 0;
}

Parser::Parser(std::string i_input)
{
	m_input = i_input;

	Lexer l(m_input);
	
	m_tokens = l.getTokens();
}

// Returns true, if expression could sucessfully transformed into 
// an abstract syntax tree.
// To parse, it uses Dijkstra's shunting yard algorithm.
bool Parser::parse()
{
	std::vector<Term*> output;

	std::stack<std::string> functionStack;

	// I don't use a for each loop, since I need the current position 
	// to determine whether a plus or minus is unary
	for (int i = 0; i < m_tokens.size(); i++)
	{
		std::string& token = m_tokens.at(i);

		switch (getType(token))
		{
		case NUMBER:
			// push back number at output
			output.push_back(new Number(token));
			break;
		case VARIABLE:
			// push back variable at output
			// token.front converts this 1 length string 
			// to a char to fulfill the requirements
			output.push_back(new Variable(token.front()));
			break;
		case FUNCTION:
			functionStack.push(token);
			break;
		case COMMA:
			
			break;
		case OPERATOR:
			if (unaryOperator(i))
			{
				if (token == "+")
				{
					// Unary plus has no effect
					continue;
				}
				else
				{
					// It's an unary minus.
					// To differentiate from the normal minus,
					// we push a different token onto the stack
					functionStack.push("neg");
				}
			}
			else
			{
				functionStack.push(token);
			}

			break;
		case OPENBRACKET:
			break;
		case CLOSEDBRACKET:
			break;
		case UNKNOWN:
			// Error.
			// Stop the whole parsing
			break;

		}
	}

	return true;
}

bool Parser::isNumber(std::string i_input)
{
	// It's a decimal number if two valid numbers are 
	// seperated by a dot
	int dotPos= i_input.find('.');

	bool isNumber = true;

	// No dot found
	if (dotPos == std::string::npos)
	{

		for (int i = 0; i < i_input.size(); i++)
		{
			isNumber = isNumber && isDigit(i_input.at(i));
		}

	}
	else
	{
		// Split the string
		std::string first = i_input.substr(0, dotPos);
		std::string second = i_input.substr(dotPos + 1);

		for (int i = 0; i < first.size(); i++)
		{
			isNumber = isNumber && isDigit(first.at(i));
		}

		for (int i = 0; i < second.size(); i++)
		{
			isNumber = isNumber && isDigit(second.at(i));
		}
	}

	return isNumber;
}

TokenType Parser::getType(std::string token)
{
	if (isNumber(token))
	{
		return NUMBER;
	}

	if (isOperator(token))
	{
		return OPERATOR;
	}

	if (token == "(")
	{
		return OPENBRACKET;
	}

	if (token == ")")
	{
		return CLOSEDBRACKET;
	}

	// Just one letter variables for simplicity so that it doesn't get confused with
	// functions that are unknown to the parser.
	// But since there are 26 of those avaible that should be sufficient.
	if (token.size() == 1)
	{
		return VARIABLE;
	}

	return UNKNOWN;
}

// This functions determines whether a + or - is
// in the string to show a sum and is therefore binary
// (e.g. 4-x) or whether it's unary (5*(-3))
bool Parser::unaryOperator(int pos)
{
	// Only for + or - can both variaties occur
	if (m_tokens.at(pos) != "+" || m_tokens.at(pos) != "-")
	{
		return false;
	}

	// The term starts with a plus or minus, therefore
	// it's definetly unary.
	if (pos == 0)
	{
		return true;
	}

	if (m_tokens.at(pos - 1) == "(" || isOperator(m_tokens.at(pos - 1)))
	{
		return true;
	}
}
