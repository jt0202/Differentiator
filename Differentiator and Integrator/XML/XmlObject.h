#ifndef XML_OBJECT_HEADER
#define XML_OBJECT_HEADER


#include <tuple>
#include <sstream>
#include <optional>

#include "XmlObjectBase.h"
#include "XmlStdContainerSupportConfig.h"

// ---- CLASS TEMPLATE: XML OBJECT

template<typename... T>
class XmlObject : public XmlObjectBase
{
	// ---- TYPES

	public:
		using DataType = std::tuple<std::pair<std::string, T>...>;
		
		using CreateDataFunction = std::function<DataType()>;
		using ObtainDataFunction = std::function<void(const DataType&)>;

	// ---- METHODS

	public:
		XmlObject(const std::string& in_classname, const std::string& in_filename = {});
        virtual ~XmlObject() = default;

	// -- interface

	public:
		const std::string& getClassname() const noexcept;
		const std::string& getFilename() const noexcept;
        void setClassname(const std::string& in_classname);
		void setFilename(const std::string& in_filename);
		// The constructor of the child class has to call the method setCreateDataFunction!
		void setCreateDataFunction(CreateDataFunction&& in_function);
		// The constructor of the child class has to call the method setObtainDataFunction!
		void setObtainDataFunction(ObtainDataFunction&& in_function);

		tinyxml2::XMLError save();
		tinyxml2::XMLError load();

	// -- implementation

	protected:
		static constexpr DataType createData(std::pair<std::string, T>... in_keyValuePairs);
		// The child class has to implement the method createData,
		// so that the data, which should be saved, can be copied, to save it!
		// To implement the method createData, you should use the createData method above.
		virtual DataType createData() const;
        template<std::size_t in_index>
		static constexpr const auto& getValue(const DataType& in_data);
        template<const char* const in_name, typename ReturnT>
        static constexpr ReturnT getValueByName(const DataType& in_data);
		// The child class has to implement the method obtainData,
		// so that the loaded data can be transfered into the attributes of the child class!
		virtual void obtainData(const DataType& in_data);
		virtual void obtainValuesFromXml(tinyxml2::XMLElement* in_element) override;
		virtual void obtainValuesFromXmlIntern(tinyxml2::XMLElement* in_element);
		virtual std::string createXmlString() const override;

	private:
		constexpr std::string concatenateMemberValues() const;
		template<typename TypeToLoad>
		constexpr void obtainMemberValues(tinyxml2::XMLElement* in_element, const std::string& in_memberName,
							              TypeToLoad* out_memberValue);
		constexpr void obtainMemberValues(tinyxml2::XMLElement* in_rootElement, DataType& out_data);
        template<typename ReturnT, typename InT>
        static constexpr void assignValue(std::optional<ReturnT>& out_value, const InT& in_value);
        template<const char* const in_name, typename ReturnT, std::size_t... in_index>
        static constexpr ReturnT doGetValueByName(const DataType& in_data, std::index_sequence<in_index...>&&);

	// ---- ATTRIBUTES

	private:
		std::string m_classname;
		std::string m_filename;
		CreateDataFunction m_createDataFunction;
		ObtainDataFunction m_obtainDataFunction;
};

// ---- METHODS

template<typename ...T>
XmlObject<T...>::XmlObject(const std::string& in_classname, const std::string& in_filename) :
	m_classname(in_classname), m_filename(in_filename)
{
	checkTypes<T...>();
}

// -- interface

template<typename ...T>
inline const std::string& XmlObject<T...>::getClassname() const noexcept
{
	return m_classname;
}

template<typename ...T>
inline const std::string& XmlObject<T...>::getFilename() const noexcept
{
	return m_filename;
}

template<typename ...T>
inline void XmlObject<T...>::setClassname(const std::string& in_classname)
{
	m_classname = in_classname;
}

template<typename ...T>
inline void XmlObject<T...>::setFilename(const std::string& in_filename)
{
	m_filename = in_filename;
}

template<typename ...T>
inline void XmlObject<T...>::setCreateDataFunction(CreateDataFunction&& in_function)
{
	m_createDataFunction = std::move(in_function);
}

template<typename ...T>
inline void XmlObject<T...>::setObtainDataFunction(ObtainDataFunction&& in_function)
{
	m_obtainDataFunction = std::move(in_function);
}

template<typename ...T>
inline constexpr typename XmlObject<T...>::DataType XmlObject<T...>::createData(std::pair<std::string, T>... in_keyValuePairs)
{
	return std::make_tuple(in_keyValuePairs...);
}

template<typename ...T>
inline typename XmlObject<T...>::DataType XmlObject<T...>::createData() const
{
	return m_createDataFunction();
}

template<typename ...T>
template<std::size_t in_index>
inline constexpr const auto& XmlObject<T...>::getValue(const DataType& in_data)
{
	return std::get<in_index>(in_data).second;
}

template<typename ...T>
template<const char* in_name, typename ReturnT>
inline constexpr ReturnT XmlObject<T...>::getValueByName(const DataType& in_data)
{
    return doGetValueByName<in_name, ReturnT>(in_data, std::make_index_sequence<std::tuple_size<DataType>::value>());
}

template<typename ...T>
inline tinyxml2::XMLError XmlObject<T...>::save()
{
    setCreateXmlStringFunction(std::bind(&XmlObject<T...>::createXmlString, *this));

    std::string xmlString;

    xmlString.append("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>");
    xmlString.append(createXmlString());
    xmlString.append("<!-- This xml file was generated by the XmlObjectLibrary and TinyXml2. -->");

	tinyxml2::XMLDocument xmlDocument;
	tinyxml2::XMLError xmlError = xmlDocument.Parse(xmlString.c_str());

	if (xmlError == tinyxml2::XMLError::XML_SUCCESS)
	{
		xmlError = xmlDocument.SaveFile(m_filename.c_str());
	}

	return xmlError;
}

template<typename ...T>
inline tinyxml2::XMLError XmlObject<T...>::load()
{
    setObtainValuesFromXmlFunction(std::bind(&XmlObject<T...>::obtainValuesFromXml, *this, std::placeholders::_1));
    
    tinyxml2::XMLDocument xmlDocument;
	tinyxml2::XMLError xmlError = xmlDocument.LoadFile(m_filename.c_str());

	if (xmlError == tinyxml2::XMLError::XML_SUCCESS)
	{
		obtainValuesFromXmlIntern(xmlDocument.FirstChildElement(m_classname.c_str()));
	}

	return xmlError;
}

// -- implementation

template<typename ...T>
inline void XmlObject<T...>::obtainData(const DataType& in_data)
{
	m_obtainDataFunction(in_data);
}

template<typename ...T>
inline void XmlObject<T...>::obtainValuesFromXml(tinyxml2::XMLElement* in_element)
{
	obtainValuesFromXmlIntern(in_element->FirstChildElement(m_classname.c_str()));
}

template<typename ...T>
inline void XmlObject<T...>::obtainValuesFromXmlIntern(tinyxml2::XMLElement* in_element)
{
	DataType data = createData();
	obtainMemberValues(in_element, data);
	obtainData(data);
}

template<typename ...T>
inline std::string XmlObject<T...>::createXmlString() const
{
	std::string xmlString;

	xmlString.append("<" + m_classname + ">");
	xmlString.append(concatenateMemberValues());
	xmlString.append("</" + m_classname + ">");

	return xmlString;
}

template<typename ...T>
inline constexpr std::string XmlObject<T...>::concatenateMemberValues() const
{
	auto doConcatenateMemberValues = [](auto&&... in_keyValuePair)
		{ std::stringstream stream;
	      
	      ((stream << "<" << in_keyValuePair.first << ">"
			       << createXmlValueString(TypeToSave(in_keyValuePair.second))
			       << "</" << in_keyValuePair.first << ">"), ...);

	 	  return stream.str(); };

	return std::apply(doConcatenateMemberValues, createData());
}

template<typename ...T>
template<typename TypeToLoad>
inline constexpr void XmlObject<T...>::obtainMemberValues(tinyxml2::XMLElement* in_element, const std::string& in_memberName,
											              TypeToLoad* out_memberValue)
{
    obtainSingleValueFromXml<TypeToLoad>(in_element->FirstChildElement(in_memberName.c_str()), out_memberValue);
}

template<typename ...T>
inline constexpr void XmlObject<T...>::obtainMemberValues(tinyxml2::XMLElement* in_rootElement, DataType& out_data)
{
	auto doObtainMemberValues = [&](auto&... io_keyValuePair)
		{ ((obtainMemberValues<T>(in_rootElement, io_keyValuePair.first, &io_keyValuePair.second)), ...); };

	std::apply(doObtainMemberValues, out_data);
}

template<typename ...T>
template<typename ReturnT, typename InT>
inline constexpr void XmlObject<T...>::assignValue(std::optional<ReturnT>& out_value, const InT& in_value)
{
    if constexpr (std::is_same<ReturnT, InT>::value)
    {
        out_value.emplace(in_value);
    }
}

template<typename ...T>
template<const char* const in_name, typename ReturnT, std::size_t ...in_index>
inline constexpr ReturnT XmlObject<T...>::doGetValueByName(const DataType& in_data, std::index_sequence<in_index...>&&)
{
    std::string name(in_name);

    constexpr auto doNothing = []() constexpr {};

    std::optional<ReturnT> value;
    ((std::get<in_index>(in_data).first == name ?
        assignValue<ReturnT>(value, std::get<in_index>(in_data).second) : doNothing()), ...);

    if (value.has_value() == false)
    {
        throw std::exception(("Could not find the value for the name " + name + "!").c_str());
    }

    return value.value();
}


#endif // XML_OBJECT_HEADER