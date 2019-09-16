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
	:MathOperator(argument, TERMTYPE_FUN), XmlObject<std::string>("Function", "Function.xml")
{
	functionName = name;

	setCreateDataFunction(std::bind(&Function::createData, *this));

	setObtainDataFunction(std::bind(&Function::obtainData, *this, std::placeholders::_1));
}

Function::Function()
	:MathOperator(), XmlObject<std::string>("Function", "Function.xml")
{
	setCreateDataFunction(std::bind(&Function::createData, *this));

	setObtainDataFunction(std::bind(&Function::obtainData, *this, std::placeholders::_1));
}

std::string Function::output() const
{
	return functionName + "(" + arguments.at(0)->output() + ")";
}

std::string Function::getFunctionName()
{
	return functionName;
}

bool Function::equals(Term* t) const
{
	if (t->getTermType() == TERMTYPE_FUN)
	{
		Function* f = dynamic_cast<Function*>(t);

		return (functionName == f->getFunctionName()) && (arguments.at(0)->equals(f->getArguments().at(0)));
	}

	return false;
}

Term* Function::differentiate(char var)
{
	// For now the derivates of functions other than the logarithm are unknown
	Function* f= new Function(functionName, arguments.at(0));

	return new Product(new Function("Deriv", f), arguments.at(0)->differentiate(var));
}