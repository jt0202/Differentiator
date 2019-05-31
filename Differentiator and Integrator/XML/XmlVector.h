#ifndef XML_VECTOR_HEADER
#define XML_VECTOR_HEADER


#include <vector>
#include <sstream>

#include "XmlObjectBase.h"

// ---- CLASS TEMPLATE: XML VECTOR

template<typename T>
class XmlVector : public XmlObjectBase
{
	// ---- METHODS

	public:
		XmlVector();
		XmlVector(const std::vector<T>& in_vector);
		XmlVector(std::vector<T>&& cons_vector);
        virtual ~XmlVector() = default;

	// -- interface

	public:
        template<typename StdElementType>
        static XmlVector<T> make(const std::vector<StdElementType>& in_vector);
        void setXmlObjectBaseFunctions();
		virtual std::string createXmlString() const override;
		const std::vector<T>& toStdVector() const;
        const std::vector<T>& toStdType() const;
        template<typename StdElementType>
        static std::vector<StdElementType> withStdElement(const std::vector<T>& in_vector);
        template<typename StdElementType>
        std::vector<StdElementType> toStdVectorWithStdElement() const;
        virtual void obtainValuesFromXml(tinyxml2::XMLElement* in_element) override;

	// -- implementation

	private:
		void construct();
        void emplace_back(T&& cons_value);

	// ---- ATTRIBUTES

	private:
		std::vector<T> m_vector;
};

// ---- METHODS

template<typename T>
inline XmlVector<T>::XmlVector()
{
	checkOneType<T>();
}

template<typename T>
inline XmlVector<T>::XmlVector(const std::vector<T>& in_vector) : m_vector(in_vector)
{
	construct();
}

template<typename T>
inline XmlVector<T>::XmlVector(std::vector<T>&& cons_vector) : m_vector(std::move(cons_vector))
{
	construct();
}

// -- interface

template<typename T>
template<typename StdElementType>
inline XmlVector<T> XmlVector<T>::make(const std::vector<StdElementType>& in_vector)
{
    XmlVector<T> xmlVector;

    for (const StdElementType& element : in_vector)
    {
        xmlVector.emplace_back(T(element));
    }

    xmlVector.setXmlObjectBaseFunctions();

    return xmlVector;
}

template<typename T>
inline void XmlVector<T>::setXmlObjectBaseFunctions()
{
	setCreateXmlStringFunction(std::bind(&XmlVector<T>::createXmlString, *this));
	setObtainValuesFromXmlFunction(std::bind(&XmlVector<T>::obtainValuesFromXml, *this, std::placeholders::_1));
}

template<typename T>
inline std::string XmlVector<T>::createXmlString() const
{
	std::stringstream xmlStream;

	for (std::size_t index = 0; index < m_vector.size(); ++index)
	{
		xmlStream << "<elem_" << index << ">"
				  << toString<T>(m_vector.at(index))
				  << "</elem_" << index << ">";
	}

	return xmlStream.str();
}

template<typename T>
inline const std::vector<T>& XmlVector<T>::toStdVector() const
{
	return m_vector;
}

template<typename T>
inline const std::vector<T>& XmlVector<T>::toStdType() const
{
    return toStdVector();
}

template<typename T>
template<typename StdElementType>
inline std::vector<StdElementType> XmlVector<T>::withStdElement(const std::vector<T>& in_vector)
{
    std::vector<StdElementType> stdVector;

    for (const T& element : in_vector)
    {
        stdVector.emplace_back(element.toStdType());
    }

    return stdVector;
}

template<typename T>
template<typename StdElementType>
inline std::vector<StdElementType> XmlVector<T>::toStdVectorWithStdElement() const
{
    return withStdElement<StdElementType>(toStdVector());
}

template<typename T>
inline void XmlVector<T>::obtainValuesFromXml(tinyxml2::XMLElement* in_element)
{
    m_vector.clear();
    T value;

	for (tinyxml2::XMLElement* element = in_element->FirstChildElement();
		 element != nullptr; element = element->NextSiblingElement())
	{
		obtainSingleValueFromXml<T>(element, &value);

		m_vector.emplace_back(value);
	}
}

// -- implementation

template<typename T>
inline void XmlVector<T>::construct()
{
	checkOneType<T>();

	setXmlObjectBaseFunctions();
}

template<typename T>
inline void XmlVector<T>::emplace_back(T&& cons_value)
{
    m_vector.emplace_back(std::move(cons_value));
}


#endif // XML_VECTOR_HEADER