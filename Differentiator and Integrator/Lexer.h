#pragma once
#include <string>
#include <vector>
#include <ctype.h>
#include <algorithm>
#include <stdexcept>
#include "LexerState.h"

// If a new type has been added change this variable.
// It determines the number of states and is therefore crucial to be greater
// or equal to the number of states.
const int numberOfTypes = 3;

// Types that will occur in the terms.
enum Types
{
	NUMBER = numberOfTypes-3,
	OPERATOR = numberOfTypes-2,
	LETTER = numberOfTypes-1,
};

class Lexer
{
private:
	// State pattern
	State* m_currentState;
	// All possible states
	std::vector<State*> m_states;

	std::string m_input;

	// Output
	std::vector<std::string> m_tokens;

	// Place to store expressions like cosh or 924568 that can't be expressed in
	// one character. They will get build char by char until an end is found,
	// when stack will be added to the tokens.
	std::string stack;

	Types evaluateCharacter(char c);

	void split();
public:
	Lexer(std::string input);

	void addToStack(char c);

	void publishStack();

	std::vector<std::string> getTokens();

	void setState(Types i_type);
};