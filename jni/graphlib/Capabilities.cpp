/*
 * $Id: Capabilities.cpp,v 1.1.2.1 2009/07/19 17:32:34 kpharris Exp $
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

#include "graphlib/Capabilities.hpp"
#include "miniblocxx/String.hpp"

namespace graphlib
{

	namespace // anonymous
	{
      void add_to_string_list(blocxx::String& s, const blocxx::String& item)
      {
			if( !s.empty() )
			{
				s += ", " + item;
			}
			else
			{
				s += item;
			}
      }

#define ADD_TO_RESULT_LONG(flag, name)          \
      do                                        \
      {                                         \
			if( (*this & (flag)) != NONE )			\
			{													\
            add_to_string_list(retval, name);   \
			}													\
      } while(0)
#define ADD_TO_RESULT_SHORT(flag, character)    \
      do                                        \
      {                                         \
			if( (*this & (flag)) != NONE )			\
			{													\
            retval += character;                \
			}													\
      } while(0)

#define CAPABILITIES_DEFINITION(cap_name)                               \
      cap_name& cap_name::operator |=(const cap_name::capabilities c)   \
      {                                                                 \
			my_capabilities = (cap_name::capabilities)(unsigned(c) | unsigned(my_capabilities)); \
			return *this;																	\
      }                                                                 \
                                                                        \
      cap_name& cap_name::operator |=(const cap_name& c)                \
      {                                                                 \
			my_capabilities = (cap_name::capabilities)(unsigned(c.my_capabilities) | unsigned(my_capabilities)); \
			return *this;																	\
      }                                                                 \
                                                                        \
      cap_name& cap_name::operator &=(const cap_name::capabilities c)   \
      {                                                                 \
			my_capabilities = (cap_name::capabilities)(unsigned(c) & unsigned(my_capabilities)); \
			return *this;																	\
      }                                                                 \
      cap_name& cap_name::operator &=(const cap_name& c)                \
      {                                                                 \
			my_capabilities = (cap_name::capabilities)(unsigned(c.my_capabilities) & unsigned(my_capabilities)); \
			return *this;																	\
      }                                                                 \
                                                                        \
      cap_name operator &(const cap_name& c1, cap_name::capabilities c2)    \
      {                                                                 \
			return cap_name(cap_name::capabilities(unsigned(c1.my_capabilities) & unsigned(c2))); \
      }                                                                 \
      cap_name operator &(cap_name::capabilities c1, const cap_name& c2)    \
      {                                                                 \
			return cap_name(cap_name::capabilities(unsigned(c1) & unsigned(c2.my_capabilities))); \
      }                                                                 \
      cap_name operator &(const cap_name& c1, const cap_name& c2)       \
      {                                                                 \
			return cap_name(cap_name::capabilities(unsigned(c1.my_capabilities) & unsigned(c2.my_capabilities))); \
      }                                                                 \
      cap_name operator |(const cap_name& c1, const cap_name& c2)       \
      {                                                                 \
			return cap_name(cap_name::capabilities(unsigned(c1.my_capabilities) | unsigned(c2.my_capabilities))); \
      }                                                                 \
      cap_name operator ~(const cap_name::capabilities& c)              \
      {                                                                 \
			return cap_name(cap_name::capabilities(cap_name::ALL & ~unsigned(c))); \
      }                                                                 \
                                                                        \
      cap_name operator ~(const cap_name& c)                            \
      {                                                                 \
			return cap_name(cap_name::capabilities(cap_name::ALL & ~unsigned(c.my_capabilities))); \
      }                                                                 \
                                                                        \
      bool operator==(const cap_name& i1, const cap_name& i2)           \
      {                                                                 \
			return unsigned(i1.my_capabilities) == unsigned(i2.my_capabilities); \
      }                                                                 \
      bool operator!=(const cap_name& i1, const cap_name& i2)           \
      {                                                                 \
			return unsigned(i1.my_capabilities) != unsigned(i2.my_capabilities); \
      }																						\
		std::ostream& operator<<(std::ostream& o, const cap_name& c)		\
		{																						\
			return o << c.toString();													\
		}																						\
		std::ostream& operator<<(std::ostream& o, cap_name::capabilities c) \
		{																						\
			return o << cap_name(c).toString();										\
		}


	} // end anonymous namespace
} // namespace amethyst
