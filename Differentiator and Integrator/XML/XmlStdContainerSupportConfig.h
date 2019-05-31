#ifndef XML_STD_CONTAINER_SUPPORT_CONFIG_HEADER
#define XML_STD_CONTAINER_SUPPORT_CONFIG_HEADER


// ---- XML STD CONTAINER SUPPORT CONFIGURATION

// Outcomment the #define for the containers, which you do not need.
#define XML_ARRAY
#define XML_VECTOR
#define XML_LIST
#define XML_PAIR
#define XML_TUPLE
#define XML_SET
#define XML_MAP

#ifdef XML_ARRAY
	#include "XmlArray.h"
#endif // XML_ARRAY

#ifdef XML_VECTOR
	#include "XmlVector.h"
#endif // XML_VECTOR

#ifdef XML_LIST
	#include "XmlList.h"
#endif // XML_LIST

#ifdef XML_PAIR
	#include "XmlPair.h"
#endif // XML_PAIR

#ifdef XML_TUPLE
	#include "XmlTuple.h"
#endif // XML_TUPLE

#ifdef XML_SET
	#include "XmlSet.h"
#endif // XML_VECTOR

#ifdef XML_MAP
	#include "XmlMap.h" // XmlMap.h includes also XmlPair
#endif // XML_MAP


#endif // XML_STD_CONTAINER_SUPPORT_CONFIG_HEADER