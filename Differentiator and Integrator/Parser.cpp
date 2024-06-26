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
	std::stack<std::string> functionStack;

	// I don't use a for each loop, since I need the current position 
	// to determine whether a plus or minus is unary
	for (int i = 0; i < m_tokens.size(); i++)
	{
		std::string token = m_tokens.at(i);

		switch (getType(token))
		{
		case TOKENTYPE_NUMBER:
			// push back number at output
			if (token.find('.') != std::string::npos)
			{
				output.push_back(makeFraction(token));
			}
			else
			{
				output.push_back(new Number(std::stoi(token)));
			}
			break;
		case TOKENTYPE_VARIABLE:
			// push back variable at output
			// token.front converts this 1 length string 
			// to a char to fulfill the requirements
			output.push_back(new Variable(token.front()));
			break;
		case TOKENTYPE_FUNCTION:
			functionStack.push(token);
			break;
		case TOKENTYPE_COMMA:
			// Not implemented
			std::cout << "Functions with multiple arguments are not implemented." << std::endl;
			return false;
		case TOKENTYPE_OPERATOR:
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
					token = "neg";
				}
			}
			while (!functionStack.empty() && isOperator(functionStack.top()) 
				  &&
					(
						(isLeftAssociative(functionStack.top()) && getPrecedence(functionStack.top()) >= getPrecedence(token))
						||
						(getPrecedence(functionStack.top()) > getPrecedence(token))
					)
				)
			{
				createTerm(functionStack.top());
				functionStack.pop();
			}

			functionStack.push(token);

			break;
		case TOKENTYPE_OPENBRACKET:
			functionStack.push("(");
			break;
		case TOKENTYPE_CLOSEDBRACKET:

			if (functionStack.empty())
			{
				std::cout << "A closed bracket without an open bracket before occurs." << std::endl;

				return false;
			}
			while (functionStack.top() != "(")
			{
				if (functionStack.empty())
				{
					std::cout << "A closed bracket without an open bracket before occurs." << std::endl;

					return false;
				}
				createTerm(functionStack.top());

				functionStack.pop();
			}

			// Remove open bracket.
			functionStack.pop();

			// In case of an expression like ln(1) the brackets belong to the function.
			if (!functionStack.empty() && isFunction(functionStack.top()))
			{
				createTerm(functionStack.top());
				functionStack.pop();
			}
			break;
		case TOKENTYPE_UNKNOWN:
			// Error.
			// Stop the whole parsing
			std::cout << "Unknown token. Parsing stopped" << std::endl;
			return false;
		}
	}

	while (!functionStack.empty())
	{
		if (functionStack.top() == "(")
		{
			std::cout << "Open bracket without closing bracket found !" << std::endl;
			return false;
		}

		createTerm(functionStack.top());
		functionStack.pop();
	}

	if (output.size() != 1)
	{
		return false;
	}


	return true;
}

bool Parser::isNumber(std::string i_input)
{
	// It's a decimal number if two valid numbers are 
	// seperated by a dot
	size_t dotPos= i_input.find('.');

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
		return TOKENTYPE_NUMBER;
	}

	if (isOperator(token))
	{
		return TOKENTYPE_OPERATOR;
	}

	if (token == "(")
	{
		return TOKENTYPE_OPENBRACKET;
	}

	if (token == ")")
	{
		return TOKENTYPE_CLOSEDBRACKET;
	}

	// Just one letter variables for simplicity so that it doesn't get confused with
	// functions that are unknown to the parser.
	// But since there are 26 of those avaible that should be sufficient.
	if (token.size() == 1)
	{
		return TOKENTYPE_VARIABLE;
	}

	if (isFunction(token))
	{
		return TOKENTYPE_FUNCTION;
	}
	return TOKENTYPE_UNKNOWN;
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

	if (m_tokens.at((size_t)pos - 1) == "(" || isOperator(m_tokens.at((size_t)pos - 1)))
	{
		return true;
	}

	return true;
}

bool Parser::isOperator(std::string i_operator)
{
	return std::find(m_operators.begin(), m_operators.end(), i_operator) != m_operators.end();
}

bool Parser::isFunction(std::string i_input)
{
	bool output = true;

	for (int i = 0; i < i_input.size(); i++)
	{
		output = output && (isalpha(i_input.at(i)) != 0);
	}

	if (i_input.empty())
	{
		return false;
	}

	return output;
}

// Returns the associativity for the parsing algorithm
bool Parser::isLeftAssociative(std::string i_operator)
{
	// +, -, *, / are left associative.
	// ^ is right associative
	if (i_operator == "^")
	{
		return false;
	}
	return true;
}

bool Parser::createTerm(std::string symbol)
{
	if (symbol == "+")
	{
		output.push_back(new Sum(getLastElement(), getLastElement()));

		return true;
	}
	if (symbol == "-")
	{
		output.push_back(new Sum(new Product(new Number(-1,1), getLastElement()), getLastElement()));

		return true;
	}
	if (symbol == "*")
	{
		output.push_back(new Product(getLastElement(), getLastElement()));

		return true;
	}
	if (symbol == "/")
	{
		Term* denominator = getLastElement();
		Term* numerator = getLastElement();

		output.push_back(new Product(numerator,new Exponent(new Number(-1, 1), denominator)));
		return true;
	}
	if (symbol == "ln")
	{
		output.push_back(new Logarithm(getLastElement()));

		return true;
	}
	if (symbol == "^")
	{
		Term* exponent = getLastElement();
		Term* base = getLastElement();
		output.push_back(new Exponent(exponent, base));

		return true;
	}
	if (isFunction(symbol))
	{
		output.push_back(new Function(symbol, getLastElement()));

		return true;
	}

	return false;
}

Term* Parser::getLastElement()
{
	if (output.empty())
	{

	}

	Term* out = output.back();
	output.pop_back();

	return out;
}

Term* Parser::getTree()
{
	if (output.size() == 0)
	{
		throw std::out_of_range("No Term tree created");
	}

	return output.at(0);
}

// Takes a string that contains a number written with a decimal
// point and turns it into a fraction. Thereby it escapes possible
// inaccurarcies made by first converting it into a floating point number.
Number* Parser::makeFraction(std::string i_input)
{
	size_t dotPosition = i_input.find('.');

	// Determine the number of decimals.
	size_t numberOfDecimals = i_input.substr(dotPosition+1).length();

	// Remove the dot 258,42 == 25842/10^2
	i_input.erase(dotPosition,1);

	// Constructor simplifies fraction
	return new Number(std::stoi(i_input), std::lround(std::pow(10, numberOfDecimals)));

}