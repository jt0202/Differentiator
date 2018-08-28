#pragma once
#include <string>
#include <vector>

class Parser
{
private:
	std::string m_input;
	std::vector<std::string> m_tokens;

public:
	Parser(std::string i_input);

	// Splits the input string into tokens to make parsing easier.
	// Every token will be stored as an element in m_tokens
	void split();
};