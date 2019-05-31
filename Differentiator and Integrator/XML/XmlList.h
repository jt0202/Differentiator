#ifndef XML_LIST_HEADER
#define XML_LIST_HEADER


#include <list>
#include <sstream>

#include "XmlObjectBase.h"

// ---- CLASS TEMPLATE: XML LIST

template<typename T>
class XmlList : public XmlObjectBase
{
	// ---- METHODS

	public:
		XmlList();
		XmlList(const std::list<T>& in_list);
		XmlList(std::list<T>&& cons_list);
        virtual ~XmlList() = default;

	// -- interface

	public:
        template<typename StdElementType>
        static XmlList<T> make(const std::list<StdElementType>& in_list);
        void setXmlObjectBaseFunctions();
		virtual std::string createXmlString() const override;
		const std::list<T>& toStdList() const;
        const std::list<T>& toStdType() const;
        template<typename StdElementType>
        static std::list<StdElementType> withStdElement(const std::list<T>& in_list);
        template<typename StdElementType>
        std::list<StdElementType> toStdListWithStdElement() const;
        virtual void obtainValuesFromXml(tinyxml2::XMLElement* in_element) override;

	// -- implementation

	private:
		void construct();
        void emplace_back(T&& cons_value);

	// ---- ATTRIBUTES

	private:
		std::list<T> m_list;
};

// ---- METHODS

template<typename T>
inline XmlList<T>::XmlList()
{
	checkOneType<T>();
}

template<typename T>
inline XmlList<T>::XmlList(const std::list<T>& in_list) : m_list(in_list)
{
	construct();
}

template<typename T>
inline XmlList<T>::XmlList(std::list<T>&& cons_list) : m_list(std::move(cons_list))
{
	construct();
}

// -- interface

template<typename T>
template<typename StdElementType>
inline XmlList<T> XmlList<T>::make(const std::list<StdElementType>& in_list)
{
    XmlList<T> xmlList;

    for (const StdElementType& element : in_list)
    {
        xmlList.emplace_back(T(element));
    }

    xmlList.setXmlObjectBaseFunctions();

    return xmlList;
}

template<typename T>
inline void XmlList<T>::setXmlObjectBaseFunctions()
{
	setCreateXmlStringFunction(std::bind(&XmlList<T>::createXmlString, *this));
	setObtainValuesFromXmlFunction(std::bind(&XmlList<T>::obtainValuesFromXml, *this, std::placeholders::_1));
}

template<typename T>
inline std::string XmlList<T>::createXmlString() const
{
	std::stringstream xmlStream;
	std::size_t index = 0;

	for (const T& element : m_list)
	{
		xmlStream << "<elem_" << index << ">"
				  << toString<T>(element)
				  << "</elem_" << index << ">";

		++index;
	}

	return xmlStream.str();
}

template<typename T>
inline const std::list<T>& XmlList<T>::toStdList() const
{
	return m_list;
}

template<typename T>
inline const std::list<T>& XmlList<T>::toStdType() const
{
    return toStdList();
}

template<typename T>
template<typename StdElementType>
inline std::list<StdElementType> XmlList<T>::withStdElement(const std::list<T>& in_list)
{
    std::list<StdElementType> stdList;

    for (const T& element : in_list)
    {
        stdList.emplace_back(element.toStdType());
    }

    return stdList;
}

template<typename T>
template<typename StdElementType>
inline std::list<StdElementType> XmlList<T>::toStdListWithStdElement() const
{
    return withStdElement<StdElementType>(toStdList());
}

template<typename T>
inline void XmlList<T>::obtainValuesFromXml(tinyxml2::XMLElement* in_element)
{
    m_list.clear();
    T value;
    
	for (tinyxml2::XMLElement* element = in_element->FirstChildElement();
		 element != nullptr; element = element->NextSiblingElement())
	{
		obtainSingleValueFromXml<T>(element, &value);

		m_list.emplace_back(value);
	}
}

// -- implementation

template<typename T>
inline void XmlList<T>::construct()
{
	checkOneType<T>();

	setXmlObjectBaseFunctions();
}

template<typename T>
inline void XmlList<T>::emplace_back(T&& cons_value)
{
    m_list.emplace_back(std::move(cons_value));
}


#endif // XML_LIST_HEADER