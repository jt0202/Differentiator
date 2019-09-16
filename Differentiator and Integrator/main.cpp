#include <iostream>
#include <string>
#include "Parser.h"
#include "Product.h"

int main()
{
	
	std::string input;
	std::getline(std::cin, input);

	Parser p(input);

	if (p.parse())
	{
		Term* t = p.getTree();

		std::cout << t->output() << std::endl;

		/*Term* diff = t->differentiate('x');

		std::cout << diff->output() << std::endl;

		std::cout << diff->simplify('x')->output() << std::endl;*/

		std::cout << t->simplify('x')->output() << std::endl;

	}
	else
	{
		std::cout << "Parsing failed." << std::endl;
	}
	


	system("PAUSE");

	return 0;
}