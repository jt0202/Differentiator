#include <iostream>
#include <string>
#include "Parser.h"

int main()
{
	Number n("2");

	Function f("Kosinus", &n);

	f.save();

	Function ff;

	ff.load();

	std::cout << ff.output() << std::endl;

	/*std::string input;
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
	*/
	
	system("PAUSE");

	return 0;
}