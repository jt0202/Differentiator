#include "Lexer.h"

Lexer::Lexer(std::string i_input)
{
	input = i_input;

	if (!input.empty())
	{
		char c = input.front();
	}
}

void Lexer::split()
{
	while (!input.empty())
	{
		char c = input.front();

		if (!input.empty())
		{
			input = input.substr(1, input.length());
		}

		switch (evaluateCharacter(c))
		{
			case NUMBER:
				m_currentState->readNumber();
				break;
			case LETTER:
				m_currentState->readOperator();
				break;
			case OPERATOR: m_currentState->readOperator();
				break;
			default:
				//Error
		}
	}
}

void Lexer::addToStack(char c)
{
	stack += c;
}

void Lexer::publishStack()
{
	if (stack.empty())
	{
		//This shouldn't happen. This would mean an empty term.
	}

	m_tokens.push_back(stack);

	stack.clear();
}

std::vector<std::string> Lexer::getTokens()
{
	return m_tokens;
}