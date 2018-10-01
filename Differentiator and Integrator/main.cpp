#include <iostream>
#include <string>
#include "Parser.h"

int main()
{
	std::string input;
	std::getline(std::cin, input);

	Parser p(input);

	p.parse();

	std::cout << "Test" << std::endl;
}