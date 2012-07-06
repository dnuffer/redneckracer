/*
 * $Id: StringDumpable.hpp,v 1.1.2.4 2010/03/03 17:55:22 kpharris Exp $
 *
 * Part of GraphLib
 *
 * Copyright (C) 2008 Kevin Harris
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

#if       !defined(GRAPHLIB__STRING_DUMPABLE_HPP)
#define            GRAPHLIB__STRING_DUMPABLE_HPP

/**
 * @author Kevin Harris
 */

#include "miniblocxx/String.hpp"
#include "miniblocxx/Format.hpp"
#include "miniblocxx/Reference.hpp"
#include "miniblocxx/stl/vector"

namespace graphlib
{
	class Indentation;
	class MemberStringDumpCollector;

	/**
	 *
	 * The base class for string dumpable objects.  Anything derived
	 * from this can have its members converted (one way) to a string.
	 *
	 * @author Kevin Harris <kpharris@users.sourceforge.net>
	 * @version $Revision: 1.1.2.4 $
	 *
	 */
	class StringDumpable
	{
	public:
		enum PrefixType { E_PREFIX_NONE, E_PREFIX_CLASSNAME };

		typedef bool CanBeStringDumpedTag;

		StringDumpable();
		virtual ~StringDumpable();

		virtual void collectInternalMembers(MemberStringDumpCollector& collector) const = 0;

		virtual blocxx::String toString(const Indentation& indent, PrefixType prefix) const;
		virtual blocxx::String name() const = 0;

		blocxx::String prefixName(PrefixType prefix) const;

		blocxx::String toString() const;

	}; // class StringDumpable

	class Indentation
	{
	public:
		Indentation();
		Indentation(const blocxx::String& initial, const blocxx::String& base, const blocxx::String& level);

		blocxx::String initial() const;
		blocxx::String level() const;

		Indentation indent() const;
		Indentation indentInside() const;

	private:
		blocxx::String m_initialIndent;
		blocxx::String m_baseIndent;
		blocxx::String m_levelIndent;
	};

	// This prefixes the given string with the indent.
	blocxx::String operator+(const Indentation& indent, const blocxx::String& s);

	// This one appends the indent to the string.  This is normally only used to
	// concatenate a bunch of strings together while formatting some output.
	blocxx::String operator+(const blocxx::String& s, const Indentation& indent);

	template <typename RefType>
	blocxx::String refToString(const RefType& p,
		const Indentation& indent = Indentation(),
		StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE)
	{
		if( p )
		{
			return indent.initial() +
				blocxx::Format("(%1)->", reinterpret_cast<const void*>(p.getPtr())) +
				toString(*p, indent.indentInside(), prefix);
		}
		else
		{
			return "NULL";
		}
	}

	template <typename T>
	blocxx::String toString(
		const blocxx::Reference<T>& p,
		const Indentation& indent = Indentation(),
		StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE)
	{
		return refToString(p, indent, prefix);
	}

	template <typename T>
	blocxx::String toString(
		const blocxx::IntrusiveReference<T>& p,
		const Indentation& indent = Indentation(),
		StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE)
	{
		return refToString(p, indent, prefix);
	}

	// This version uses a simplified tag-based operation instead of requiring
	// virtual functions.  This makes it more suitable for use in objects such
	// as numeric types where the virtual function calls would be undesirable.
	template <typename T>
	blocxx::String toString(const T& object,
		const Indentation& indent = Indentation(),
		StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE,
		typename T::CanBeStringDumpedTag tag = typename T::CanBeStringDumpedTag())
	{
		return object.toString(indent, prefix);
	}

	blocxx::String toString(const StringDumpable& object,
		const Indentation& indent = Indentation(),
		StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE);


	// A generic templated toString() can't be used because it eats classes
	// derived from StringDumpable.  Provide string conversions for the common
	// native types to avoid the need.
	blocxx::String toString(char i, const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE);
	blocxx::String toString(unsigned char c, const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE);
	blocxx::String toString(short i, const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE);
	blocxx::String toString(unsigned short i, const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE);
	blocxx::String toString(int i, const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE);
	blocxx::String toString(unsigned int i, const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE);
	blocxx::String toString(long i, const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE);
	blocxx::String toString(unsigned long i, const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE);
 	blocxx::String toString(float i, const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE);
	blocxx::String toString(double i, const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE);
	blocxx::String toString(long double i, const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE);
	blocxx::String toString(const blocxx::String& i, const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE);
	blocxx::String toString(const char* i, const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE);

	template <typename ArrayType>
	blocxx::String arrayToString(const ArrayType& vec,
		const Indentation& indent = Indentation(),
		StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE)
	{
		blocxx::String retval = indent.initial() + "array\n";

		retval += indent + "{\n";

		Indentation nextIndent = indent.indent();

		typename ArrayType::const_iterator iter = vec.begin();
		if( iter != vec.end() )
		{
			retval += toString(*iter, nextIndent, prefix);
			for( ++iter; iter != vec.end(); ++iter )
			{
				retval += ",\n" + toString(*iter, nextIndent, prefix);
			}
			retval += "\n";
		}
		retval += indent + "}";

		return retval;
	}

	template <typename T>
	blocxx::String toString(const blocxx::Array<T>& vec,
		const Indentation& indent = Indentation(),
		StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE)
	{
		return arrayToString(vec, indent, prefix);
	}

	template <typename T>
	blocxx::String toString(const std::vector<T>& vec,
		const Indentation& indent = Indentation(),
		StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE)
	{
		return arrayToString(vec, indent, prefix);
	}

	class MemberStringDumpCollector
	{
	public:
		MemberStringDumpCollector(StringDumpable::PrefixType prefix, const Indentation& indent);

		template <typename T>
		void addMember(const blocxx::String& name, const T& value)
		{
			Indentation indent = m_indent.indentInside();
			m_text += m_indent + name + " = " + ::graphlib::toString(value, indent, m_prefix) + ";\n";
		}

		blocxx::String toString() const;
	private:
		StringDumpable::PrefixType m_prefix;
		Indentation m_indent;
		blocxx::String m_text;
	};

	template <typename T>
	blocxx::String toStringAsMembers(const T& object, const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE, typename T::CanBeStringDumpedTag tag = typename T::CanBeStringDumpedTag())
	{
		MemberStringDumpCollector collector(prefix, indent.indent());
		object.collectInternalMembers(collector);
		return (
			indent.initial() + object.name() + "\n" +
			indent + "{\n" +
			collector.toString() +
			indent + "}"
		);
	}

} // namespace graphlib


#endif /* !defined(GRAPHLIB__STRING_DUMPABLE_HPP) */

