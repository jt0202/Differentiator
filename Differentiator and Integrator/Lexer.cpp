#include "Lexer.h"

Lexer::Lexer(std::string i_input)
{
	m_input = i_input;


	// This ensures that every State just exists once and can be easily 
	// accessed using the enums value. There after a change of state
	// no new state has to be created
	m_states.resize(numberOfTypes);

	m_states.at(TNUMBER) = new NumberState(this);
	m_states.at(TLETTER) = new LetterState(this);
	m_states.at(TOPERATOR) = new OperatorState(this);

	// The Operator State carries no side effects,
	// If we would start e.g. in the number state and first
	// read a character the system would assume that a * is missing
	// (consider 2x => 2*x) and therefore would lead to a parsing failure later.
	m_currentState = m_states.at(TOPERATOR);

	split();
}

void Lexer::split()
{
	for (int i = 0; i < m_input.size(); i++)
	{
		char c = m_input.at(i);

		// Spaces aren't important for any calculation in 
		// the normal sense and are therefore ignored
		if (c == ' ')
		{
			continue;
		}

		// Reaction on the read character. The states handle 
		// to combine the characters to tokens. Unknown characters
		// will already throw an exception in the evaluate character function.
		switch (evaluateCharacter(c))
		{
		case TNUMBER:
			m_currentState->readNumber(c);
			break;
		case TLETTER:
			m_currentState->readLetter(c);
			break;
		case TOPERATOR:
			m_currentState->readOperator(c);
		default:
			break;
		}
	}

	m_tokens.push_back(stack);
}

// See the comment to stack in Lexer.h
void Lexer::addToStack(char c)
{
	stack += c;
}

void Lexer::publishStack()
{
	if (stack.empty())
	{
		return;
	}

	m_tokens.push_back(stack);

	stack.clear();
}

std::vector<std::string> Lexer::getTokens()
{
	return m_tokens;
}

// This function determines the type of each character therefore
// let the system choose the right function to combine them.
// Unknown characters will throw an exception.
Types Lexer::evaluateCharacter(char c)
{
	// Checking whether it's a decimal number(3.14)
	if (isdigit(c) != 0 || c =='.')
	{
		return TNUMBER;
	}
	if (isalpha(c) != 0)
	{
		return TLETTER;
	}

	// TODO: Better system for operators.
	std::vector<char> operators = { '+','-','*', '/', '^', '(', ')' , ','};

	if (find(operators.begin(), operators.end(), c) != operators.end())
	{
		return TOPERATOR;
	}

	//throw Exception, that unknown token was read. Spaces will already be ignored in split function
}

void Lexer::setState(Types i_type)
{
	try
	{
		m_currentState = m_states.at(i_type);
	}
	catch (std::out_of_range e)
	{
		// Throw new Parsing exception, due to unknown character type.
	}
}