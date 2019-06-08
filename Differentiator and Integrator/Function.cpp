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

Function::Function(std::string name, Term* argument)
	:MathOperator(argument), XmlObject<std::string>("Function", "Function.xml")
{
	functionName = name;

	setCreateDataFunction(std::bind(&Function::createData, *this));

	setObtainDataFunction(std::bind(&Function::obtainData, *this, std::placeholders::_1));
}

Function::Function()
	:MathOperator(nullptr), XmlObject<std::string>("Function", "Function.xml")
{
	setCreateDataFunction(std::bind(&Function::createData, *this));

	setObtainDataFunction(std::bind(&Function::obtainData, *this, std::placeholders::_1));
}

std::string Function::output()
{
	return functionName + "(" + arguments.at(0)->output() + ")";
}

Term* Function::differentiate(char var)
{
	// For now the derivates of functions other than the logarithm are unknown
	return new Product(new Function("Deriv", this), arguments.at(0)->differentiate(var));
}