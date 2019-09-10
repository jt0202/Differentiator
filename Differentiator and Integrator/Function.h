#pragma once
#include "Operator.h"
#include <string>
#include "Product.h"
#include "XML\XmlObject.h"

class Function : public MathOperator, public XmlObject<std::string>
{
private:
	std::string functionName;

	DataType createData() const override;

	void obtainData(const DataType& ref) override;

public:
	Function(std::string name, Term* argument);

	Function();

	Term* differentiate(char var);

	std::string output();

	using Xml = XmlObject<std::string>;

	std::string getFunctionName();

	bool equals(Term* t);

};

namespace functionMemberName 
{
	const char functionName[] = "Name";
}