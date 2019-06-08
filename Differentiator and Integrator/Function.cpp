#include "Function.h"

XmlObject<std::string>::DataType Function::createData() const
{
	return Xml::createData({ functionMemberName::functionName , functionName });
}

void Function::obtainData(const DataType& ref)
{
	//functionName = Xml::getValue<0>(ref);

	functionName = Xml::getValueByName<functionMemberName::functionName, std::string>(ref);
}

Function::Function(std::string name, Term argument)
	:MathOperator(argument), XmlObject<std::string>("Function", "Function.xml")
{
	functionName = name;

	setDiff(std::bind(&Function::differentiate, *this, std::placeholders::_1));
	setOutp(std::bind(&Function::output, *this));

	setCreateDataFunction(std::bind(&Function::createData, *this));

	setObtainDataFunction(std::bind(&Function::obtainData, *this, std::placeholders::_1));
}

Function::Function()
	:MathOperator(), XmlObject<std::string>("Function", "Function.xml")
{
	setDiff(std::bind(&Function::differentiate, *this, std::placeholders::_1));
	setOutp(std::bind(&Function::output, *this));

	setCreateDataFunction(std::bind(&Function::createData, *this));

	setObtainDataFunction(std::bind(&Function::obtainData, *this, std::placeholders::_1));
}

std::string Function::output()
{
	return functionName + "(" + arguments.at(0).output() + ")";
}

std::string Function::getFunctionName()
{
	return functionName;
}

Term Function::differentiate(char var)
{
	// For now the derivates of functions other than the logarithm are unknown
	Function f(functionName, arguments.at(0));

	return Product(Function("Deriv", f), arguments.at(0).differentiate(var));
}