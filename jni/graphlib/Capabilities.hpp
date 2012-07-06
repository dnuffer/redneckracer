/*
 * $Id: Capabilities.hpp,v 1.1.2.1 2009/07/19 17:32:34 kpharris Exp $
 *
 * Part of GraphLib
 *
 * Copyright (C) 2004 Kevin Harris
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if       !defined(GRAPHLIB_CAPABILITIES_HPP)
#define            GRAPHLIB_CAPABILITIES_HPP

#include "graphlib/StringDumpable.hpp"

namespace graphlib
{

   // This macro defines a wrapper struct for a bitmask enum.  This struct is
   // named with the value supplied for cap_name.  The wrapper struct allows a
   // set of values to be used as an enum and also provide real use of "&",
   // "|", "~".  String conversions are also supplied to make printing of these
   // enums clean and easy.
   //
   // The enum values must include the symbolic names NONE and ALL. These are
   // used for masking, initialization and other operations.  The value of ALL
   // should normally be twice the largest value minus 1 (see use below).
#define CAPABILITIES_DECLARATION(cap_name, cap_enum_values...)				\
	struct cap_name																		\
	{																							\
		typedef bool CanBeStringDumpedTag;											\
		enum capabilities																	\
		{																						\
			cap_enum_values																\
		};																						\
		capabilities my_capabilities;													\
																								\
		cap_name() : my_capabilities(cap_name::NONE) { }						\
		cap_name(capabilities c): my_capabilities(c) { }						\
																								\
		cap_name& operator |=(const capabilities c);								\
		cap_name& operator |=(const cap_name& c);									\
		cap_name& operator &=(const capabilities c);								\
		cap_name& operator &=(const cap_name& c);									\
																								\
		void collectInternalMembers(MemberStringDumpCollector& collector) const; \
		blocxx::String toString(const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE ) const; \
		blocxx::String toStringShort() const;										\
																								\
		blocxx::String name() const { return #cap_name; }						\
	};																							\
																								\
	std::ostream& operator<<(std::ostream& o, const cap_name& c);			\
	std::ostream& operator<<(std::ostream& o, cap_name::capabilities c);	\
																								\
	cap_name operator &(const cap_name& c1, cap_name::capabilities c2);	\
	cap_name operator &(cap_name::capabilities c1, const cap_name& c2);	\
	cap_name operator &(const cap_name& c1, const cap_name& c2);			\
	cap_name operator |(const cap_name& c1, const cap_name& c2);			\
	cap_name operator ~(const cap_name::capabilities& c);						\
	cap_name operator ~(const cap_name& c);										\
																								\
	bool operator==(const cap_name& i1, const cap_name& i2);					\
	bool operator!=(const cap_name& i1, const cap_name& i2);

	/*

	CAPABILITIES_DECLARATION(IntersectionCapabilities,
		NONE = 0,
		HIT_FIRST = 1,
		HIT_ALL = 2,
		HIT_CONTAINERS = 4,
		NORMAL_CALCULATION = 8,
		UV_CALCULATION = 16,
		LOCAL_SYSTEM_CALCULATION = 32,
		TIME_SAMPLING = 64,
		ALL = TIME_SAMPLING * 2 - 1
	);

	CAPABILITIES_DECLARATION(ObjectCapabilities,
		NONE = 0,
		INFINITE = 1,
		BOUNDABLE = 2,
		MOVABLE = 4,
		POLYGONIZATION = 8,
		CONTAINER = 16,
		IMPLICIT = 32,
		SIMPLE = 64, // A simple shape with fast intersection test (sphere, plane, etc).
		ALL = 2 * SIMPLE - 1
	);

	CAPABILITIES_DECLARATION(MaterialCapabilities,
		NONE = 0,
		DIFFUSE = 1, // has a color
		EMISSIVE = 2, // emits light
		REFLECTIVE = 4, // specular reflection
		TRANSMISSIVE = 8, // implies refraction of some kind
		ALL = 2 * TRANSMISSIVE - 1
	);
	*/

} // namespace graphlib
#endif /* !defined(GRAPHLIB_CAPABILITIES_HPP) */
