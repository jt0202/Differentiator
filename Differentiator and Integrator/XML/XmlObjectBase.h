#ifndef XML_OBJECT_BASE_HEADER
#define XML_OBJECT_BASE_HEADER


#include <string>
#include <functional>
#include <typeinfo>
#include <variant>

#include "tinyxml2.h"

// ---- CLASS: XML OBJECT BASE

class XmlObjectBase
{
	// ---- TYPES

	public:
		using TypeToSave = std::variant<bool, short, int, unsigned int, long, long long,
								        float, double, std::string, XmlObjectBase>;

		using CreateXmlStringFunction     = std::function<std::string()>;
		using ObtainValuesFromXmlFunction = std::function<void(tinyxml2::XMLElement*)>;

	// ---- METHODS

    public:
        virtual ~XmlObjectBase() = default;

	// -- interface

	public:
		virtual std::string createXmlString() const;
		virtual void obtainValuesFromXml(tinyxml2::XMLElement* in_element);
        
	// -- implementation

	protected:
		template<typename T>
		static std::string toString(const T& in_value);
		static std::string createXmlValueString(TypeToSave&& in_memberValue);
		template<typename TypeToLoad>
		static constexpr void obtainSingleValueFromXml(tinyxml2::XMLElement* in_element, TypeToLoad* out_value);
		// Use this set functions the constructor of your child class to
		// make the methods of your child class available for XmlObjectBase!
		void setCreateXmlStringFunction(CreateXmlStringFunction&& in_function);
		void setObtainValuesFromXmlFunction(ObtainValuesFromXmlFunction&& in_function);
		// Use the method checkOneType in the constructor of your
		// child class to check, that the type T is supported!
		template<typename T>
		static constexpr void checkOneType();
		// Use the method checkTypes in the constructor of your
		// child class to check, that the types T... are supported!
		template<typename ...T>
		static constexpr void checkTypes();		

	// ---- ATTRIBUTES

	private:
		CreateXmlStringFunction m_createXmlStringFunction;
		ObtainValuesFromXmlFunction m_obtainValuesFromXmlFunction;
};

// ---- METHODS

// -- implementation

template<typename T>
inline std::string XmlObjectBase::toString(const T& in_value)
{
	if constexpr (std::is_same<T, bool>::value)
	{
		return std::to_string(*reinterpret_cast<const bool*>(&in_value));
	}
	else if constexpr (std::is_same<T, short>::value)
	{
		return std::to_string(*reinterpret_cast<const short*>(&in_value));
	}
    else if constexpr (std::is_same<T, int>::value)
    {
        return std::to_string(*reinterpret_cast<const int*>(&in_value));
    }
	else if constexpr (std::is_same<T, unsigned int>::value)
	{
		return std::to_string(*reinterpret_cast<const unsigned int*>(&in_value));
	}
	else if constexpr (std::is_same<T, long>::value)
	{
        return std::to_string(*reinterpret_cast<const long*>(&in_value));
	}
    else if constexpr (std::is_same<T, long long>::value)
    {
        return std::to_string(*reinterpret_cast<const long long*>(&in_value));
    }
	else if constexpr (std::is_same<T, float>::value)
	{
		return std::to_string(*reinterpret_cast<const float*>(&in_value));
	}
	else if constexpr (std::is_same<T, double>::value)
	{
		return std::to_string(*reinterpret_cast<const double*>(&in_value));
	}
	else if constexpr (std::is_same<T, std::string>::value)
	{
		return *reinterpret_cast<const std::string*>(&in_value);
	}
	else if constexpr (std::is_base_of<XmlObjectBase, T>::value)
	{
		return reinterpret_cast<const XmlObjectBase*>(&in_value)->createXmlString();
	}
	else
	{
		throw std::exception("The type T is not supported!");
	}
}

inline std::string XmlObjectBase::createXmlValueString(TypeToSave&& in_memberValue)
{
    if (std::holds_alternative<bool>(in_memberValue))
    {
        return std::to_string(std::get<bool>(in_memberValue));
    }
    else if (std::holds_alternative<short>(in_memberValue))
    {
        return std::to_string(std::get<short>(in_memberValue));
    }
    else if (std::holds_alternative<int>(in_memberValue))
    {
        return std::to_string(std::get<int>(in_memberValue));
    }
    else if (std::holds_alternative<unsigned int>(in_memberValue))
    {
        return std::to_string(std::get<unsigned int>(in_memberValue));
    }
    else if (std::holds_alternative<long>(in_memberValue))
    {
        return std::to_string(std::get<long>(in_memberValue));
    }
    else if (std::holds_alternative<long long>(in_memberValue))
    {
        return std::to_string(std::get<long long>(in_memberValue));
    }
    else if (std::holds_alternative<float>(in_memberValue))
    {
        return std::to_string(std::get<float>(in_memberValue));
    }
    else if (std::holds_alternative<double>(in_memberValue))
    {
        return std::to_string(std::get<double>(in_memberValue));
    }
    else if (std::holds_alternative<std::string>(in_memberValue))
    {
        return std::get<std::string>(in_memberValue);
    }
    else if (std::holds_alternative<XmlObjectBase>(in_memberValue))
    {
        return std::get<XmlObjectBase>(in_memberValue).createXmlString();
    }
    else
    {
        throw std::exception("This type is not supported!");
    }
}

template<typename TypeToLoad>
inline constexpr void XmlObjectBase::obtainSingleValueFromXml(tinyxml2::XMLElement* in_element, TypeToLoad* out_value)
{
	if constexpr (std::is_same<TypeToLoad, bool>::value)
	{
		in_element->QueryBoolText(reinterpret_cast<bool*>(out_value));
	}
    else if constexpr (std::is_same<TypeToLoad, short>::value)
    {
        int value;
        in_element->QueryIntText(&value);
        *reinterpret_cast<short*>(out_value) = value;
    }
	else if constexpr (std::is_same<TypeToLoad, int>::value)
	{
		in_element->QueryIntText(reinterpret_cast<int*>(out_value));
	}
	else if constexpr (std::is_same<TypeToLoad, unsigned int>::value)
	{
		in_element->QueryUnsignedText(reinterpret_cast<unsigned int*>(out_value));
	}
    else if constexpr (std::is_same<TypeToLoad, long>::value)
    {
        std::int64_t value;
        in_element->QueryInt64Text(&value);
        *reinterpret_cast<long*>(out_value) = value;
    }
	else if constexpr (std::is_same<TypeToLoad, long long>::value)
	{
		std::int64_t value;
        in_element->QueryInt64Text(&value);
        *reinterpret_cast<long long*>(out_value) = value;
	}
	else if constexpr (std::is_same<TypeToLoad, float>::value)
	{
		in_element->QueryFloatText(reinterpret_cast<float*>(out_value));
	}
	else if constexpr (std::is_same<TypeToLoad, double>::value)
	{
		in_element->QueryDoubleText(reinterpret_cast<double*>(out_value));
	}
	else if constexpr (std::is_same<TypeToLoad, std::string>::value)
	{
		if (const char* text = in_element->GetText(); text != nullptr)
		{
			*reinterpret_cast<std::string*>(out_value) = text;
		}
		else
		{
			*reinterpret_cast<std::string*>(out_value) = {};
		}
	}
	else if constexpr (std::is_base_of<XmlObjectBase, TypeToLoad>::value)
	{
		reinterpret_cast<XmlObjectBase*>(out_value)->obtainValuesFromXml(in_element);
	}
    else
    {
        throw std::exception("The type TypeToLoad is not supported!");
    }
}

template<typename T>
inline constexpr void XmlObjectBase::checkOneType()
{
	static_assert(std::is_same<T, bool>::value || std::is_same<T, short>::value || std::is_same<T, int>::value ||
			      std::is_same<T, unsigned int>::value || std::is_same<T, long>::value || std::is_same<T, long long>::value ||
				  std::is_same<T, float>::value || std::is_same<T, double>::value || std::is_same<T, std::string>::value ||
				  std::is_base_of<XmlObjectBase, T>::value,
                  "The type T is not supported! If you want to use std::container, than use XmlContainer.");
}

template<typename ...T>
inline constexpr void XmlObjectBase::checkTypes()
{
	((checkOneType<T>()), ...);
}


#endif // XML_OBJECT_BASE_HEADER