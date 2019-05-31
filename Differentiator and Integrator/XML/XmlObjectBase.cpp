#include "XmlObjectBase.h"

// ---- METHODS

// -- interface

std::string XmlObjectBase::createXmlString() const
{
	return m_createXmlStringFunction();
}

void XmlObjectBase::obtainValuesFromXml(tinyxml2::XMLElement* in_element)
{
	m_obtainValuesFromXmlFunction(in_element);
}

// -- implemenation

void XmlObjectBase::setCreateXmlStringFunction(CreateXmlStringFunction&& in_function)
{
	m_createXmlStringFunction = std::move(in_function);
}

void XmlObjectBase::setObtainValuesFromXmlFunction(ObtainValuesFromXmlFunction&& in_function)
{
	m_obtainValuesFromXmlFunction = std::move(in_function);
}