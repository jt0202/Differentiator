#pragma once
#include <string>
#include <vector>
#include "LexerState.h"

enum States
{
	DEFAULT = 0,
	NUMBER = 1,
	OPERATOR = 2,
	LETTER = 3
};

class Lexer
{
private:
	State* m_currentState;
	std::vector<State*> states;

	std::string input;

	std::string m_currentToken;

	std::vector<std::string> m_tokens;

	std::string stack;

	States evaluateCharacter(char c);

	void split();
public:
	Lexer(std::string input);

	void addToStack(char c);

	void publishStack();

	std::vector<std::string> getTokens();

};