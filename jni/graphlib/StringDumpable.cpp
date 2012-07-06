/*
 * $Id: StringDumpable.cpp,v 1.1.2.3 2009/06/15 01:26:18 kpharris Exp $
 *
 * Part of GraphLib
 * Copyright (C) 2009 Kevin Harris
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

/*
 * @author Kevin Harris
 */


#include "StringDumpable.hpp"

namespace graphlib
{
	Indentation::Indentation()
		: m_initialIndent()
		, m_baseIndent()
		, m_levelIndent("  ")
	{
	}

	Indentation::Indentation(const blocxx::String& initial,
		const blocxx::String& base, const blocxx::String& level)
		: m_initialIndent(initial)
		, m_baseIndent(base)
		, m_levelIndent(level)
	{
	}

	blocxx::String Indentation::initial() const
	{
		return m_initialIndent;
	}

	blocxx::String Indentation::level() const
	{
		return m_baseIndent;
	}

	Indentation Indentation::indentInside() const
	{
		return Indentation(blocxx::String(), m_baseIndent + m_levelIndent, m_levelIndent);
	}

	Indentation Indentation::indent() const
	{
		blocxx::String indented = m_baseIndent + m_levelIndent;
		return Indentation(indented, indented, m_levelIndent);
	}

	blocxx::String operator+(const Indentation& indent, const blocxx::String& s)
	{
		return indent.level() + s;
	}

	blocxx::String operator+(const blocxx::String& s, const Indentation& indent)
	{
		return s + indent.level();
	}

	StringDumpable::StringDumpable()
	{
	} // StringDumpable()

	StringDumpable::~StringDumpable()
	{
	} // ~StringDumpable()

	blocxx::String StringDumpable::toString(const Indentation& indent, PrefixType prefix) const
	{
		return toStringAsMembers(*this, indent, prefix);
	}

	blocxx::String StringDumpable::toString() const
	{
		return toString(Indentation(), E_PREFIX_NONE);
	}

	blocxx::String StringDumpable::prefixName(PrefixType prefix) const
	{
		if( prefix == StringDumpable::E_PREFIX_CLASSNAME )
		{
			return name();
		}
		return blocxx::String();
	}

	blocxx::String toString(char i, const Indentation& indent, StringDumpable::PrefixType prefix)
	{
		return indent.initial() + blocxx::Format("%1", int(i));
	}
	blocxx::String toString(unsigned char i, const Indentation& indent, StringDumpable::PrefixType prefix)
	{
		return indent.initial() + blocxx::Format("%<1:x!>", unsigned(i));
	}
	blocxx::String toString(short i, const Indentation& indent, StringDumpable::PrefixType prefix)
	{
		return indent.initial() + blocxx::Format("%1", i);
	}
	blocxx::String toString(unsigned short i, const Indentation& indent, StringDumpable::PrefixType prefix)
	{
		return indent.initial() + blocxx::Format("%1", i);
	}
	blocxx::String toString(int i, const Indentation& indent, StringDumpable::PrefixType prefix)
	{
		return indent.initial() + blocxx::Format("%1", i);
	}
	blocxx::String toString(unsigned int i, const Indentation& indent, StringDumpable::PrefixType prefix)
	{
		return indent.initial() + blocxx::Format("%1", i);
	}
	blocxx::String toString(long i, const Indentation& indent, StringDumpable::PrefixType prefix)
	{
		return indent.initial() + blocxx::Format("%1", i);
	}
	blocxx::String toString(unsigned long i, const Indentation& indent, StringDumpable::PrefixType prefix)
	{
		return indent.initial() + blocxx::Format("%1", i);
	}
 	blocxx::String toString(float i, const Indentation& indent, StringDumpable::PrefixType prefix)
	{
		return indent.initial() + blocxx::Format("%1", i);
	}
	blocxx::String toString(double i, const Indentation& indent, StringDumpable::PrefixType prefix)
	{
		return indent.initial() + blocxx::Format("%1", i);
	}
	blocxx::String toString(long double i, const Indentation& indent, StringDumpable::PrefixType prefix)
	{
		return indent.initial() + blocxx::Format("%1", i);
	}
	blocxx::String toString(const blocxx::String& i, const Indentation& indent, StringDumpable::PrefixType prefix)
	{
		return indent.initial() + i;
	}
	blocxx::String toString(const char* i, const Indentation& indent, StringDumpable::PrefixType prefix)
	{
		return indent.initial() + i;
	}

	MemberStringDumpCollector::MemberStringDumpCollector(StringDumpable::PrefixType prefix, const Indentation& indent)
		: m_prefix(prefix), m_indent(indent)
	{
	}

	blocxx::String MemberStringDumpCollector::toString() const
	{
		return m_text;
	}

	blocxx::String toString(const StringDumpable& object,
		const Indentation& indent,	StringDumpable::PrefixType prefix)
	{
		return object.toString(indent, prefix);
	}
} // namespace graphlib

