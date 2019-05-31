#ifndef XML_PAIR_HEADER
#define XML_PAIR_HEADER


#include <utility>
#include <sstream>

#include "XmlObjectBase.h"

// ---- CLASS TEMPLATE: XML PAIR

template<typename FirstT, typename SecondT>
class XmlPair : public XmlObjectBase
{
	// ---- METHODS

	public:
		XmlPair();
		XmlPair(const std::pair<FirstT, SecondT>& in_pair);
		XmlPair(std::pair<FirstT, SecondT>&& cons_map);
        virtual ~XmlPair() = default;

	// -- interface

	public:
        template<typename FirstStdType, typename SecondStdType>
        static XmlPair<FirstT, SecondT> make(const std::pair<FirstStdType, SecondStdType>& in_pair);
		void setXmlObjectBaseFunctions();
		virtual std::string createXmlString() const override;
		const std::pair<FirstT, SecondT>& toStdPair() const;
		const std::pair<FirstT, SecondT>& toStdType() const;
        template<typename FirstStdType, typename SecondStdType>
        static std::pair<FirstStdType, SecondStdType> withStdElement(const std::pair<FirstT, SecondT>& in_pair);
        template<typename FirstStdType, typename SecondStdType>
        std::pair<FirstStdType, SecondStdType> toStdPairWithStdElement() const;
		virtual void obtainValuesFromXml(tinyxml2::XMLElement* in_element) override;

	// -- implementation

	private:
		void construct();
        FirstT& first();
        SecondT& second();

	// ---- ATTRIBUTES

	private:
		std::pair<FirstT, SecondT> m_pair;
};

// ---- METHODS

template<typename FirstT, typename SecondT>
inline XmlPair<FirstT, SecondT>::XmlPair()
{
	checkOneType<FirstT>();
	checkOneType<SecondT>();
}

template<typename FirstT, typename SecondT>
inline XmlPair<FirstT, SecondT>::XmlPair(const std::pair<FirstT, SecondT>& in_pair) : m_pair(in_pair)
{
	construct();
}

template<typename FirstT, typename SecondT>
inline XmlPair<FirstT, SecondT>::XmlPair(std::pair<FirstT, SecondT>&& cons_pair) : m_pair(std::move(cons_pair))
{
	construct();
}

// -- interface

template<typename FirstT, typename SecondT>
template<typename FirstStdType, typename SecondStdType>
inline XmlPair<FirstT, SecondT> XmlPair<FirstT, SecondT>::make(const std::pair<FirstStdType, SecondStdType>& in_pair)
{
    XmlPair<FirstT, SecondT> xmlPair;
    
    xmlPair.first()  = FirstT(in_pair.first);
    xmlPair.second() = SecondT(in_pair.second);
    xmlPair.setXmlObjectBaseFunctions();

    return xmlPair;
}

template<typename FirstT, typename SecondT>
template<typename FirstStdType, typename SecondStdType>
inline std::pair<FirstStdType, SecondStdType> XmlPair<FirstT, SecondT>::withStdElement(const std::pair<FirstT, SecondT>& in_pair)
{
    std::pair<FirstStdType, SecondStdType> stdPair;

    if constexpr (std::is_base_of<XmlObjectBase, FirstT>::value)
    {
        stdPair.first = in_pair.first.toStdType();
    }
    else
    {
        stdPair.first = in_pair.first;
    }
    
    if constexpr (std::is_base_of<XmlObjectBase, SecondT>::value)
    {
        stdPair.second = in_pair.second.toStdType();
    }
    else
    {
        stdPair.second = in_pair.second;
    }
    
    return stdPair;
}

template<typename FirstT, typename SecondT>
template<typename FirstStdType, typename SecondStdType>
inline std::pair<FirstStdType, SecondStdType> XmlPair<FirstT, SecondT>::toStdPairWithStdElement() const
{
    return withStdElement<FirstStdType, SecondStdType>(toStdPair());
}

template<typename FirstT, typename SecondT>
inline void XmlPair<FirstT, SecondT>::setXmlObjectBaseFunctions()
{
	setCreateXmlStringFunction(std::bind(&XmlPair<FirstT, SecondT>::createXmlString, *this));
	setObtainValuesFromXmlFunction(std::bind(&XmlPair<FirstT, SecondT>::obtainValuesFromXml, *this, std::placeholders::_1));
}

template<typename FirstT, typename SecondT>
inline std::string XmlPair<FirstT, SecondT>::createXmlString() const
{
	std::stringstream xmlStream;
	
	xmlStream << "<first>"
			  << toString<FirstT>(m_pair.first)
			  << "</first>"
			  << "<second>"
			  << toString<SecondT>(m_pair.second)
			  << "</second>";

	return xmlStream.str();
}

template<typename FirstT, typename SecondT>
inline const std::pair<FirstT, SecondT>& XmlPair<FirstT, SecondT>::toStdPair() const
{
	return m_pair;
}

template<typename FirstT, typename SecondT>
inline const std::pair<FirstT, SecondT>& XmlPair<FirstT, SecondT>::toStdType() const
{
	return toStdPair();
}

template<typename FirstT, typename SecondT>
inline void XmlPair<FirstT, SecondT>::obtainValuesFromXml(tinyxml2::XMLElement* in_element)
{
	tinyxml2::XMLElement* element;

	if (element = in_element->FirstChildElement(); element != nullptr)
	{
		obtainSingleValueFromXml<FirstT>(element, &m_pair.first);
	}
	else
	{
		throw std::exception("element == nullptr: The pair is corrupt!");
	}
	
	if (element = element->NextSiblingElement(); element != nullptr)
	{
		obtainSingleValueFromXml<SecondT>(element, &m_pair.second);
	}
	else
	{
		throw std::exception("element == nullptr: The pair is corrupt!");
	}
}

// -- implementation

template<typename FirstT, typename SecondT>
inline void XmlPair<FirstT, SecondT>::construct()
{
	checkOneType<FirstT>();
	checkOneType<SecondT>();

	setXmlObjectBaseFunctions();
}

template<typename FirstT, typename SecondT>
inline FirstT& XmlPair<FirstT, SecondT>::first()
{
    return m_pair.first;
}

template<typename FirstT, typename SecondT>
inline SecondT& XmlPair<FirstT, SecondT>::second()
{
    return m_pair.second;
}


#endif // XML_PAIR_HEADER