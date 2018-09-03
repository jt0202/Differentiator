#pragma once

// Forward declaration to avoid circular dependance between the lexer
// and it's states.
class Lexer;

class State
{
protected:
	Lexer* m_lexer;
public:
	State(Lexer* i_lexer);
	virtual void readNumber(char c) = 0;
	virtual void readOperator(char c) = 0;
	virtual void readLetter(char c) = 0;
};

class NumberState : public State
{
public:
	NumberState(Lexer* i_lexer);
	void readNumber(char c);
	void readOperator(char c);
	void readLetter(char c);
};

class OperatorState : public State
{
public:
	OperatorState(Lexer* i_lexer);
	void readNumber(char c);
	void readOperator(char c);
	void readLetter(char c);
};
class LetterState : public State
{
public:
	LetterState(Lexer* i_lexer);
	void readNumber(char c);
	void readOperator(char c);
	void readLetter(char c);
};