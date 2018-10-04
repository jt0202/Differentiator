#include "Lexer.h"
#include "LexerState.h"
#include <iostream>

State::State(Lexer* i_lexer)
	: m_lexer(i_lexer)
{

}

NumberState::NumberState(Lexer* i_lexer)
	:State(i_lexer)
{

}

void NumberState::readLetter(char c)
{
	// The number has ended
	m_lexer->publishStack();

	// If a letter follows a number than the implicit
	// sign for multiplication must be added( 2x => 2*x)
	m_lexer->addToStack('*');
	m_lexer->publishStack();

	// Now reading letters
	m_lexer->addToStack(c);
	m_lexer->setState(TLETTER);
}

void NumberState::readNumber(char c)
{
	m_lexer->addToStack(c);
}

void NumberState::readOperator(char c)
{
	// The number has ended
	m_lexer->publishStack();

	// Now reading operators
	m_lexer->addToStack(c);
	m_lexer->setState(TOPERATOR);
}

OperatorState::OperatorState(Lexer* i_lexer)
	: State(i_lexer)
{
	lastOperator = ' ';
}

void OperatorState::readLetter(char c)
{
	m_lexer->publishStack();

	m_lexer->addToStack(c);
	m_lexer->setState(TLETTER);
}

void OperatorState::readNumber(char c)
{
	m_lexer->publishStack();

	m_lexer->addToStack(c);
	m_lexer->setState(TNUMBER);
}

void OperatorState::readOperator(char c)
{
	m_lexer->publishStack();

	// Recognize implicit multiplication signs like
	// between (2-3)(4+5)
	if (m_lexer->getLastToken() == ")" && c == '(')
	{
		m_lexer->addToStack('*');
		m_lexer->publishStack();
	}

	m_lexer->addToStack(c);
	m_lexer->setState(TOPERATOR);
}

LetterState::LetterState(Lexer* i_lexer)
	: State(i_lexer)
{

}

void LetterState::readLetter(char c)
{
	m_lexer->addToStack(c);
}

void LetterState::readNumber(char c)
{
	m_lexer->publishStack();

	m_lexer->addToStack(c);
	m_lexer->setState(TNUMBER);
}

void LetterState::readOperator(char c)
{
	m_lexer->publishStack();

	m_lexer->addToStack(c);
	m_lexer->setState(TOPERATOR);
}