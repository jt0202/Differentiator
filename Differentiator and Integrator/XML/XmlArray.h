#ifndef XML_ARRAY_HEADER
#define XML_ARRAY_HEADER


#include <array>
#include <sstream>

#include "XmlObjectBase.h"

// ---- CLASS TEMPLATE: XML ARRAY

template<typename T, std::size_t in_size>
class XmlArray : public XmlObjectBase
{
	// ---- METHODS

	public:
		XmlArray();
		XmlArray(const std::array<T, in_size>& in_array);
		XmlArray(std::array<T, in_size>&& cons_array);
        virtual ~XmlArray() = default;

	// -- interface

	public:
		template<typename StdElementType>
		static XmlArray<T, in_size> make(const std::array<StdElementType, in_size>& in_array);
		void setXmlObjectBaseFunctions();
		virtual std::string createXmlString() const override;
		const std::array<T, in_size>& toStdArray() const;
        const std::array<T, in_size>& toStdType() const;
		template<typename StdElementType>
		static std::array<StdElementType, in_size> withStdElement(const std::array<T, in_size>& in_array);
		template<typename StdElementType>
		std::array<StdElementType, in_size> toStdArrayWithStdElement() const;
		virtual void obtainValuesFromXml(tinyxml2::XMLElement* in_element) override;

	// -- implementation

	private:
		void construct();
		T& at(std::size_t in_index);

	// ---- ATTRIBUTES

	private:
		std::array<T, in_size> m_array;
};

// ---- METHODS

template<typename T, std::size_t in_size>
inline XmlArray<T, in_size>::XmlArray()
{
	checkOneType<T>();
}

template<typename T, std::size_t in_size>
inline XmlArray<T, in_size>::XmlArray(const std::array<T, in_size>& in_array) : m_array(in_array)
{
	construct();
}

template<typename T, std::size_t in_size>
inline XmlArray<T, in_size>::XmlArray(std::array<T, in_size>&& cons_array) : m_array(std::move(cons_array))
{
	construct();
}

// -- interface

template<typename T, std::size_t in_size>
template<typename StdElementType>
inline XmlArray<T, in_size> XmlArray<T, in_size>::make(const std::array<StdElementType, in_size>& in_array)
{
	XmlArray<T, in_size> xmlArray;

	for (std::size_t index = 0; index < in_array.size(); ++index)
	{
		xmlArray.at(index) = T(in_array.at(index));
	}

	xmlArray.setXmlObjectBaseFunctions();

	return xmlArray;
}

template<typename T, std::size_t in_size>
inline void XmlArray<T, in_size>::setXmlObjectBaseFunctions()
{
	setCreateXmlStringFunction(std::bind(&XmlArray<T, in_size>::createXmlString, *this));
	setObtainValuesFromXmlFunction(std::bind(&XmlArray<T, in_size>::obtainValuesFromXml, *this, std::placeholders::_1));
}

template<typename T, std::size_t in_size>
inline std::string XmlArray<T, in_size>::createXmlString() const
{
	std::stringstream xmlStream;

	for (std::size_t index = 0; index < m_array.size(); ++index)
	{
		xmlStream << "<elem_" << index << ">"
				  << toString<T>(m_array.at(index))
			      << "</elem_" << index << ">";
	}

	return xmlStream.str();
}

template<typename T, std::size_t in_size>
inline const std::array<T, in_size>& XmlArray<T, in_size>::toStdArray() const
{
	return m_array;
}

template<typename T, std::size_t in_size>
inline const std::array<T, in_size>& XmlArray<T, in_size>::toStdType() const
{
    return toStdArray();
}

template<typename T, std::size_t in_size>
template<typename StdElementType>
inline std::array<StdElementType, in_size> XmlArray<T, in_size>::withStdElement(const std::array<T, in_size>& in_array)
{
	std::array<StdElementType, in_size> stdArray;

	for (std::size_t index = 0; index < stdArray.size(); ++index)
	{
		stdArray.at(index) = in_array.at(index).toStdType();
	}

	return stdArray;
}

template<typename T, std::size_t in_size>
template<typename StdElementType>
inline std::array<StdElementType, in_size> XmlArray<T, in_size>::toStdArrayWithStdElement() const
{
	return withStdElement<StdElementType>(toStdArray());
}

template<typename T, std::size_t in_size>
inline void XmlArray<T, in_size>::obtainValuesFromXml(tinyxml2::XMLElement* in_element)
{
	T value;
	std::size_t index = 0;

	for (tinyxml2::XMLElement* element = in_element->FirstChildElement();
		 element != nullptr; element = element->NextSiblingElement())
	{
		obtainSingleValueFromXml<T>(element, &value);

		m_array.at(index) = value;
		++index;
	}
}

// -- implementation

template<typename T, std::size_t in_size>
inline void XmlArray<T, in_size>::construct()
{
	checkOneType<T>();

	setXmlObjectBaseFunctions();
}

template<typename T, std::size_t in_size>
inline T& XmlArray<T, in_size>::at(std::size_t in_index)
{
	return m_array.at(in_index);
}


#endif // XML_ARRAY_HEADER