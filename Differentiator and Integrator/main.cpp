#include <iostream>
#include <string>
#include "Parser.h"

int main()
{
	std::string input;
	std::getline(std::cin, input);

	Parser p(input);

	if (p.parse())
	{
		std::cout << p.getTree()->output() << std::endl;

		std::cout << p.getTree()->differentiate('x')->output() << std::endl;
	}
	else
	{
		std::cout << "Parsing failed." << std::endl;
	}
	
	
	system("PAUSE");

	return 0;
}