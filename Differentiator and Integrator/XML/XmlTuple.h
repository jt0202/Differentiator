#ifndef XML_TUPLE_HEADER
#define XML_TUPLE_HEADER


#include <tuple>
#include <sstream>

#include "XmlObjectBase.h"

// ---- CLASS TEMPLATE: XML TUPLE

template<typename ...T>
class XmlTuple : public XmlObjectBase
{
	// ---- TYPES

	private:
		using IndexElementPairTuple = std::tuple<std::pair<std::size_t, T>...>;
        template<std::size_t in_index, typename ...StdElementType>
        using TupleElementType = std::tuple_element<in_index, std::tuple<StdElementType...>>;
	
	// ---- METHODS

	public:
		XmlTuple();
		XmlTuple(const std::tuple<T...>& in_tuple);
		XmlTuple(std::tuple<T...>&& cons_tuple);
        virtual ~XmlTuple() = default;

	// -- interface

	public:
        template<typename ...StdElementType>
        static XmlTuple<T...> make(const std::tuple<StdElementType...>& in_tuple);
        void setXmlObjectBaseFunctions();
		virtual std::string createXmlString() const override;
		const std::tuple<T...>& toStdTuple() const;
        const std::tuple<T...>& toStdType() const;
        template<typename ...StdElementType>
        static std::tuple<StdElementType...> withStdElement(const std::tuple<T...>& in_tuple);
        template<typename ...StdElementType>
        std::tuple<StdElementType...> toStdTupleWithStdElement() const;
        virtual void obtainValuesFromXml(tinyxml2::XMLElement* io_element) override;

	// -- implementation

	private:
		void construct();
        constexpr std::tuple<T...>& getTuple() noexcept;
        template<typename ...StdElementType, std::size_t... in_index>
        static constexpr XmlTuple<T...> doCreateXmlTuple(const std::tuple<StdElementType...>& in_tuple,
                                                         std::index_sequence<in_index...>&&) ;
        template<typename ...StdElementType>
        static constexpr XmlTuple<T...> createXmlTuple(const std::tuple<StdElementType...>& in_tuple);
        template<std::size_t... in_index>
		constexpr IndexElementPairTuple doCreateIndexElementPairTuple(std::index_sequence<in_index...>&&) const;
		constexpr IndexElementPairTuple createIndexElementPairTuple() const;
        template<std::size_t in_index, typename ...StdElementType>
        static constexpr auto getStdElement(const std::tuple<StdElementType...>& in_tuple);
        template<typename ...StdElementType, std::size_t... in_index>
        static std::tuple<StdElementType...> createStdTuple(const std::tuple<T...>& in_tuple,
                                                            std::index_sequence<in_index...>&&);

	// ---- ATTRIBUTES

	private:
		std::tuple<T...> m_tuple;
};

// ---- METHODS

template<typename ...T>
inline XmlTuple<T...>::XmlTuple()
{
	checkTypes<T...>();
}

template<typename ...T>
inline XmlTuple<T...>::XmlTuple(const std::tuple<T...>& in_tuple) : m_tuple(in_tuple)
{
	construct();
}

template<typename ...T>
inline XmlTuple<T...>::XmlTuple(std::tuple<T...>&& cons_tuple) : m_tuple(std::move(cons_tuple))
{
	construct();
}

// -- interface

template<typename ...T>
template<typename ...StdElementType>
inline XmlTuple<T...> XmlTuple<T...>::make(const std::tuple<StdElementType...>& in_tuple)
{
    XmlTuple<T...> xmlTuple = createXmlTuple(in_tuple);
    xmlTuple.setXmlObjectBaseFunctions();

    return xmlTuple;
}

template<typename ...T>
inline void XmlTuple<T...>::setXmlObjectBaseFunctions()
{
	setCreateXmlStringFunction(std::bind(&XmlTuple<T...>::createXmlString, *this));
	setObtainValuesFromXmlFunction(std::bind(&XmlTuple<T...>::obtainValuesFromXml, *this, std::placeholders::_1));
}

template<typename ...T>
inline std::string XmlTuple<T...>::createXmlString() const
{
	auto doCreateXmlString = [](auto&&... in_indexElementPair)
		{ std::stringstream stream;
	      
	      ((stream << "<elem_" << in_indexElementPair.first << ">"
			       << createXmlValueString(TypeToSave(in_indexElementPair.second))
			       << "</elem_" << in_indexElementPair.first << ">"), ...);

	 	  return stream.str(); };

	std::stringstream xmlStream;

	xmlStream << std::apply(doCreateXmlString, createIndexElementPairTuple());

	return xmlStream.str();
}

template<typename ...T>
inline const std::tuple<T...>& XmlTuple<T...>::toStdTuple() const
{
	return m_tuple;
}

template<typename ...T>
inline const std::tuple<T...>& XmlTuple<T...>::toStdType() const
{
    return toStdTuple();
}

template<typename ...T>
template<typename ...StdElementType>
inline std::tuple<StdElementType...> XmlTuple<T...>::withStdElement(const std::tuple<T...>& in_tuple)
{
    return createStdTuple<StdElementType...>(in_tuple,
                                             std::make_index_sequence<std::tuple_size<std::tuple<T...>>::value>());
}

template<typename ...T>
template<typename ...StdElementType>
inline std::tuple<StdElementType...> XmlTuple<T...>::toStdTupleWithStdElement() const
{
    return withStdElement<StdElementType...>(toStdTuple());
}

template<typename ...T>
inline void XmlTuple<T...>::obtainValuesFromXml(tinyxml2::XMLElement* io_element)
{
    io_element = io_element->FirstChildElement();

    auto doObtainValuesFromXml = [&](auto&... out_value)
		{ ((obtainSingleValueFromXml<T>(io_element, &out_value),
            io_element = io_element->NextSiblingElement()), ...); };

	std::apply(doObtainValuesFromXml, m_tuple);
}

// -- implementation

template<typename ...T>
inline void XmlTuple<T...>::construct()
{
	checkTypes<T...>();

	setXmlObjectBaseFunctions();
}

template<typename ...T>
inline constexpr std::tuple<T...>& XmlTuple<T...>::getTuple() noexcept
{
    return m_tuple;
}

template<typename ...T>
template<typename ...StdElementType, std::size_t ...in_index>
inline constexpr XmlTuple<T...> XmlTuple<T...>::doCreateXmlTuple(const std::tuple<StdElementType...>& in_tuple,
                                                                 std::index_sequence<in_index...>&&)
{
    XmlTuple<T...> xmlTuple;

    auto setXmlTupleValues = [&](auto&... out_value)
		{ ((out_value = T(std::get<in_index>(in_tuple))), ...); };

	std::apply(setXmlTupleValues, xmlTuple.getTuple());

    return xmlTuple;
}

template<typename ...T>
template<typename ...StdElementType>
inline constexpr XmlTuple<T...> XmlTuple<T...>::createXmlTuple(const std::tuple<StdElementType...>& in_tuple)
{
    return doCreateXmlTuple<StdElementType...>(in_tuple,
                                               std::make_index_sequence<std::tuple_size<std::tuple<StdElementType...>>::value>());
}

template<typename ...T>
template<std::size_t ...in_index>
inline constexpr typename XmlTuple<T...>::IndexElementPairTuple
	XmlTuple<T...>::doCreateIndexElementPairTuple(std::index_sequence<in_index...>&&) const
{
	return std::make_tuple(std::make_pair(in_index, std::get<in_index>(m_tuple))...);
}

template<typename ...T>
inline constexpr typename XmlTuple<T...>::IndexElementPairTuple XmlTuple<T...>::createIndexElementPairTuple() const
{
	return doCreateIndexElementPairTuple(std::make_index_sequence<std::tuple_size<std::tuple<T...>>::value>());
}

#include <type_traits>

template<typename ...T>
template<std::size_t in_index, typename ...StdElementType>
inline constexpr auto XmlTuple<T...>::getStdElement(const std::tuple<StdElementType...>& in_tuple)
{
    if constexpr (std::is_base_of<XmlObjectBase, TupleElementType<in_index, StdElementType...>::type>::value)
    {
        return std::get<in_index>(in_tuple).toStdType();
    }
    else
    {
        return std::get<in_index>(in_tuple);
    }
}

template<typename ...T>
template<typename ...StdElementType, std::size_t ...in_index>
inline std::tuple<StdElementType...> XmlTuple<T...>::createStdTuple(const std::tuple<T...>& in_tuple,
                                                                    std::index_sequence<in_index...>&&)
{
    std::tuple<StdElementType...> stdTuple;

    auto setStdTupleValues = [&](auto&... out_value)
		{ ((out_value = getStdElement<in_index>(in_tuple)), ...); };

	std::apply(setStdTupleValues, stdTuple);

    return stdTuple;
}


#endif // XML_TUPLE_HEADER