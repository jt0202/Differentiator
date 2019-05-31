#ifndef XML_SET_HEADER
#define XML_SET_HEADER


#include <set>
#include <sstream>

#include "XmlObjectBase.h"

// ---- CLASS TEMPLATE: XML SET

template<typename T>
class XmlSet : public XmlObjectBase
{
	// ---- METHODS

	public:
		XmlSet();
		XmlSet(const std::set<T>& in_set);
		XmlSet(std::set<T>&& cons_set);
        virtual ~XmlSet() = default;

	// -- interface

	public:
        template<typename StdElementType>
        static XmlSet<T> make(const std::set<StdElementType>& in_set);
        void setXmlObjectBaseFunctions();
		virtual std::string createXmlString() const override;
		const std::set<T>& toStdSet() const;
        const std::set<T>& toStdType() const;
        template<typename StdElementType>
        static std::set<StdElementType> withStdElement(const std::set<T>& in_set);
        template<typename StdElementType>
        std::set<StdElementType> toStdSetWithStdElement() const;
        virtual void obtainValuesFromXml(tinyxml2::XMLElement* in_element) override;
        std::size_t size() const noexcept;
        typename std::set<T>::const_iterator begin() const noexcept;
        typename std::set<T>::const_iterator end() const noexcept;

	// -- implementation

	private:
		void construct();
        void emplace(T&& cons_value);

	// ---- ATTRIBUTES

	private:
		std::set<T> m_set;
};

// ---- METHODS

template<typename T>
inline XmlSet<T>::XmlSet()
{
	checkOneType<T>();
}

template<typename T>
inline XmlSet<T>::XmlSet(const std::set<T>& in_set) : m_set(in_set)
{
	construct();
}

template<typename T>
inline XmlSet<T>::XmlSet(std::set<T>&& cons_set) : m_set(std::move(cons_set))
{
	construct();
}

// -- interface

template<typename T>
template<typename StdElementType>
inline XmlSet<T> XmlSet<T>::make(const std::set<StdElementType>& in_set)
{
    XmlSet<T> xmlSet;

    for (const StdElementType& element : in_set)
    {
        xmlSet.emplace(T(element));
    }

    xmlSet.setXmlObjectBaseFunctions();

    return xmlSet;
}

template<typename T>
inline void XmlSet<T>::setXmlObjectBaseFunctions()
{
	setCreateXmlStringFunction(std::bind(&XmlSet<T>::createXmlString, *this));
	setObtainValuesFromXmlFunction(std::bind(&XmlSet<T>::obtainValuesFromXml, *this, std::placeholders::_1));
}

template<typename T>
inline std::string XmlSet<T>::createXmlString() const
{
	std::stringstream xmlStream;
	std::size_t index = 0;
	
	for (typename std::set<T>::const_iterator iter = m_set.begin(); iter != m_set.end(); ++iter)
	{
		xmlStream << "<val_" << index << ">"
				  << toString<T>(*iter)
				  << "</val_" << index << ">";

		++index;
	}

	return xmlStream.str();
}

template<typename T>
inline const std::set<T>& XmlSet<T>::toStdSet() const
{
	return m_set;
}

template<typename T>
inline const std::set<T>& XmlSet<T>::toStdType() const
{
    return toStdSet();
}

template<typename T>
template<typename StdElementType>
inline std::set<StdElementType> XmlSet<T>::withStdElement(const std::set<T>& in_set)
{
    std::set<StdElementType> stdSet;

    for (const T& element : in_set)
    {
        stdSet.emplace(element.toStdType());
    }

    return stdSet;
}

template<typename T>
template<typename StdElementType>
inline std::set<StdElementType> XmlSet<T>::toStdSetWithStdElement() const
{
    return withStdElement<StdElementType>(toStdSet());
}

template<typename T>
inline void XmlSet<T>::obtainValuesFromXml(tinyxml2::XMLElement* in_element)
{
    m_set.clear();
    T value;

	for (tinyxml2::XMLElement* element = in_element->FirstChildElement();
		 element != nullptr; element = element->NextSiblingElement())
	{
		obtainSingleValueFromXml<T>(element, &value);

		m_set.emplace(value);
	}
}

template<typename T>
inline std::size_t XmlSet<T>::size() const noexcept
{
    return m_set.size();
}

template<typename T>
inline typename std::set<T>::const_iterator XmlSet<T>::begin() const noexcept
{
    return m_set.begin();
}

template<typename T>
inline typename std::set<T>::const_iterator XmlSet<T>::end() const noexcept
{
    return m_set.end();
}

// -- implementation

template<typename T>
inline void XmlSet<T>::construct()
{
	checkOneType<T>();

	setXmlObjectBaseFunctions();
}

template<typename T>
inline void XmlSet<T>::emplace(T&& cons_value)
{
    m_set.emplace(std::move(cons_value));
}

// ---- OPERATORS

template<typename T>
inline bool operator<(const XmlSet<T>& in_this, const XmlSet<T>& in_other)
{
    if (in_this.size() == in_other.size())
    {
        int trueResultCount  = 0;
        int falseResultCount = 0;
        typename std::set<T>::const_iterator iterThis  = in_this.begin();
        typename std::set<T>::const_iterator iterOther = in_other.begin();

        for (; iterThis != in_this.end() && iterOther != in_other.end(); ++iterThis, ++iterOther)
        {
            if (*iterThis < *iterOther)
            {
                ++trueResultCount;
            }
            else
            {
                ++falseResultCount;
            }
        }

        return trueResultCount > falseResultCount;
    }
    else
    {
        return in_this.size() < in_other.size();
    }
}


#endif // XML_SET_HEADER