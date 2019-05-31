#ifndef XML_MAP_HEADER
#define XML_MAP_HEADER


#include <map>
#include <sstream>

#include "XmlObjectBase.h"
#include "XmlPair.h"

// ---- CLASS TEMPLATE: XML MAP

template<typename KeyT, typename ValueT>
class XmlMap : public XmlObjectBase
{
	// ---- METHODS

	public:
		XmlMap();
		XmlMap(const std::map<KeyT, ValueT>& in_map);
		XmlMap(std::map<KeyT, ValueT>&& cons_map);
        virtual ~XmlMap() = default;

	// -- interface

	public:
        template<typename KeyStdType, typename ValueStdType>
        static XmlMap<KeyT, ValueT> make(const std::map<KeyStdType, ValueStdType>& in_map);
        void setXmlObjectBaseFunctions();
		virtual std::string createXmlString() const override;
		const std::map<KeyT, ValueT>& toStdMap() const;
        const std::map<KeyT, ValueT>& toStdType() const;
        template<typename KeyStdType, typename ValueStdType>
        static std::map<KeyStdType, ValueStdType> withStdElement(const std::map<KeyT, ValueT>& in_map);
        template<typename KeyStdType, typename ValueStdType>
        std::map<KeyStdType, ValueStdType> toStdMapWithStdElement() const;
        virtual void obtainValuesFromXml(tinyxml2::XMLElement* in_element) override;

	// -- implementation

	private:
		void construct();
        void emplace(KeyT&& cons_key, ValueT&& cons_value);

	// ---- ATTRIBUTES

	private:
		std::map<KeyT, ValueT> m_map;
};

// ---- METHODS

template<typename KeyT, typename ValueT>
inline XmlMap<KeyT, ValueT>::XmlMap()
{
	checkOneType<KeyT>();
	checkOneType<ValueT>();
}

template<typename KeyT, typename ValueT>
inline XmlMap<KeyT, ValueT>::XmlMap(const std::map<KeyT, ValueT>& in_map) : m_map(in_map)
{
	construct();
}

template<typename KeyT, typename ValueT>
inline XmlMap<KeyT, ValueT>::XmlMap(std::map<KeyT, ValueT>&& cons_map) : m_map(std::move(cons_map))
{
	construct();
}

// -- interface

template<typename KeyT, typename ValueT>
template<typename KeyStdType, typename ValueStdType>
inline XmlMap<KeyT, ValueT> XmlMap<KeyT, ValueT>::make(const std::map<KeyStdType, ValueStdType>& in_map)
{
    XmlMap<KeyT, ValueT> xmlMap;

    for (const std::pair<KeyStdType, ValueStdType>& element : in_map)
    {
        xmlMap.emplace(KeyT(element.first), ValueT(element.second));
    }

    xmlMap.setXmlObjectBaseFunctions();

    return xmlMap;
}

template<typename KeyT, typename ValueT>
inline void XmlMap<KeyT, ValueT>::setXmlObjectBaseFunctions()
{
	setCreateXmlStringFunction(std::bind(&XmlMap<KeyT, ValueT>::createXmlString, *this));
	setObtainValuesFromXmlFunction(std::bind(&XmlMap<KeyT, ValueT>::obtainValuesFromXml, *this, std::placeholders::_1));
}

template<typename KeyT, typename ValueT>
inline std::string XmlMap<KeyT, ValueT>::createXmlString() const
{
	std::stringstream xmlStream;
	std::size_t index = 0;

	for (typename std::map<KeyT, ValueT>::const_iterator iter = m_map.begin(); iter != m_map.end(); ++iter)
	{
		xmlStream << "<key_" << index << ">"
				  << toString<KeyT>(iter->first)
				  << "</key_" << index << ">"
				  << "<val_" << index << ">"
				  << toString<ValueT>(iter->second)
				  << "</val_" << index << ">";

		++index;
	}

	return xmlStream.str();
}

template<typename KeyT, typename ValueT>
inline const std::map<KeyT, ValueT>& XmlMap<KeyT, ValueT>::toStdMap() const
{
	return m_map;
}

template<typename KeyT, typename ValueT>
inline const std::map<KeyT, ValueT>& XmlMap<KeyT, ValueT>::toStdType() const
{
    return toStdMap();
}

template<typename KeyT, typename ValueT>
template<typename KeyStdType, typename ValueStdType>
inline std::map<KeyStdType, ValueStdType> XmlMap<KeyT, ValueT>::withStdElement(const std::map<KeyT, ValueT>& in_map)
{
    std::map<KeyStdType, ValueStdType> stdMap;
    std::pair<KeyStdType, ValueStdType> stdPair;

    for (const std::pair<KeyT, ValueT>& element : in_map)
    {
        stdPair = XmlPair<KeyT, ValueT>::template withStdElement<KeyStdType, ValueStdType>(element);
        stdMap.emplace(stdPair.first, stdPair.second);
    }

    return stdMap;
}

template<typename KeyT, typename ValueT>
template<typename KeyStdType, typename ValueStdType>
inline std::map<KeyStdType, ValueStdType> XmlMap<KeyT, ValueT>::toStdMapWithStdElement() const
{
    return withStdElement<KeyStdType, ValueStdType>(toStdMap());
}

template<typename KeyT, typename ValueT>
inline void XmlMap<KeyT, ValueT>::obtainValuesFromXml(tinyxml2::XMLElement* in_element)
{
    m_map.clear();
    KeyT key;
	ValueT value;

	for (tinyxml2::XMLElement* element = in_element->FirstChildElement();
		 element != nullptr; element = element->NextSiblingElement())
	{
		obtainSingleValueFromXml<KeyT>(element, &key);

		if (element = element->NextSiblingElement(); element != nullptr)
		{
			obtainSingleValueFromXml<ValueT>(element, &value);
		}
		else
		{
			throw std::exception("element == nullptr: The key value pair or the map is corrupt!");
		}

		m_map.emplace(key, value);
	}
}

// -- implementation

template<typename KeyT, typename ValueT>
inline void XmlMap<KeyT, ValueT>::construct()
{
	checkOneType<KeyT>();
	checkOneType<ValueT>();

	setXmlObjectBaseFunctions();
}

template<typename KeyT, typename ValueT>
inline void XmlMap<KeyT, ValueT>::emplace(KeyT&& cons_key, ValueT&& cons_value)
{
    m_map.emplace(std::move(cons_key), std::move(cons_value));
}


#endif // XML_MAP_HEADER