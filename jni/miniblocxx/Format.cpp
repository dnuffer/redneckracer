/*******************************************************************************
* Copyright (C) 2005, Quest Software, Inc. All rights reserved.
* Copyright (C) 2006, Novell, Inc. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright notice,
*       this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of
*       Quest Software, Inc.,
*       nor Novell, Inc.,
*       nor the names of its contributors or employees may be used to
*       endorse or promote products derived from this software without
*       specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/


/**
 * @author Dan Nuffer
 * @author Kevin Harris
 */

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/Format.hpp"
#include "miniblocxx/stl/vector.hpp"
#include "miniblocxx/stl/algorithm"
#include "miniblocxx/Exception.hpp"
#include <ctype.h>

namespace BLOCXX_NAMESPACE
{
BLOCXX_DEFINE_EXCEPTION_WITH_ID(Format);

String align(const String& s, const FormatFlags& flags)
{
	if( flags.width != 0 )
	{
		if( s.length() < flags.width )
		{
			size_t difference = flags.width - s.length();

			if( flags.flags & FormatFlags::ALIGN_LEFT )
			{
				return s + repeat(flags.fill, difference);
			}
			return repeat(flags.fill, difference) + s;
		}
	}
	return s;
}

String toString(bool b, const FormatFlags& flags)
{
	if( flags.flags & FormatFlags::SHOW_BASE)
	{
		if( flags.flags & FormatFlags::UPPERCASE)
		{
			return align(b ? "TRUE" : "FALSE", flags);
		}
		return align(b ? "true" : "false", flags);
	}
	return align(b ? "1" : "0", flags);
}
String toString(const char* const c, const FormatFlags& flags)
{
	return align(String(c), flags);
}
String toString(const std::string& s, const FormatFlags& flags)
{
	return toString(s.c_str(), flags);
}

namespace
{
	template <typename T>
	String convertify(T val, int radix, bool uppercase)
	{
		if( val == T(0) )
		{
			return "0";
		}

		if( !((T(0) < val) || (val == T(0))) && (radix == 10) )
		{
			return "-" + convertify(-val, radix, uppercase);
		}

		const char ldigits[17] = "0123456789abcdef";
		const char hdigits[17] = "0123456789ABCDEF";
		const char* digits = uppercase ? hdigits : ldigits;

		int ndigits;
		if( radix == 16 )
		{
			ndigits = sizeof(val) * 2;
		}
		else if( radix == 8 )
		{
			ndigits = sizeof(val) * 4;
		}
		else
		{
			// radix == 10
			ndigits = 128; // FIXME! Calculate this.
		}
		vector<char> chars(ndigits, '0');

		for( int i = 0; i < ndigits; ++i )
		{
			char c = digits[val % radix];
			val = val / radix;
			chars[ndigits - 1 - i] = c;
		}
		// Trim off useless leading zeroes.
		int nonzero = 0;
		for( ; nonzero < ndigits; ++nonzero )
		{
			if( chars[nonzero] != '0' )
			{
				break;
			}
		}
		return String(&chars[nonzero], ndigits - nonzero);
	}

	template <typename T>
	String hexify(T val, bool uppercase = false)
	{
		return convertify(val, 16, uppercase);
	}

	template <typename T>
	String octify(T val)
	{
		return convertify(val, 8, false);
	}

	template <typename T>
	String decify(T val)
	{
		return convertify(val, 10, false);
	}
}

/////////////////////////////////////////////////////////////////////////////
Format::operator String() const
{
	return oss.toString();
}
/////////////////////////////////////////////////////////////////////////////
Format::operator std::string() const
{
	return oss.c_str();
}
/////////////////////////////////////////////////////////////////////////////
String Format::toString() const
{
	return oss.toString();
}
/////////////////////////////////////////////////////////////////////////////
const char* Format::c_str() const
{
	return oss.c_str();
}

Format::~Format()
{
}

Format::Format()
	: oss()
{
}

FormatFlags::FormatFlags()
	: flags(0)
	, width(0)
	, precision(-1)
	, fill(' ')
{
}

namespace
{
	bool scanNumber(const String& str, size_t& offset, int& result, int& sign, bool& leadingZero)
	{
		int c = 0;
		int len = str.length();
		int i = offset;
		bool havedigit = false;
		sign = 0;
		leadingZero = false;

		if( i < len )
		{
			if( str[i] == '-' )
			{
				sign = -1;
				++i;
			}
			else if( str[i] == '+' )
			{
				sign = 1;
				++i;
			}
			if( (i < len) && (str[i] == '0') )
			{
				leadingZero = true;
			}
		}

		while( i < len && isdigit(str[i]) )
		{
			havedigit = true;
			c = c * 10 + (str[i] - '0');
			++i;
		}

		if ( sign < 0 )
		{
			c *= -1;
		}

		if( !havedigit )
		{
			return false;
		}

		result = c;
		offset = i;

		return true;
	}

	const char BRACE_OPEN = '<';
	const char BRACE_CLOSE = '>';
}

// BRACE_OPEN [0-9]+ ( ':' ([0-9]* '.' [0-9]* )? ('x' | 'X' | 'o')? '!'? )? BRACE_CLOSE
bool Format::processFormatSpecifier(String& str, size_t& offset, size_t& argnum, FormatFlags& flags)
{
	bool retval = true;

	if( str[offset] == BRACE_OPEN )
	{
		++offset;

		// Get the argument number.
		int arg;
		int argsign;
		bool unused;
		if( scanNumber(str, offset, arg, argsign, unused) )
		{
			// Don't allow signs on the number.
			if( argsign != 0 )
			{
				retval = false;
			}

			argnum = arg;
		}
		else
		{
			retval = false;
		}

		// Get the format width/output specifiers.
		if( str[offset] == ':' )
		{
			++offset;

			int val;
			int sign;
			bool leadingZero;
			if( scanNumber(str, offset, val, sign, leadingZero) )
			{
				if( sign == -1 )
				{
					val *= -1;
					flags.flags |= FormatFlags::ALIGN_LEFT;
				}
				else if( sign == 1 )
				{
					flags.flags |= FormatFlags::SHOW_PLUS;
				}

				if( leadingZero && sign != -1)
				{
					flags.fill = '0';
				}
				flags.width = val;
			}

			if( (offset < str.length()) && (str[offset] == '.') )
			{
				size_t j = offset + 1;
				if( scanNumber(str, j, val, sign, leadingZero) )
				{
					flags.precision = val;
					offset = j;
				}
			}

			if( offset < str.length() )
			{
				if( str[offset] == 'x' )
				{
					flags.flags |= FormatFlags::HEX;
					++offset;
				}
				else if( str[offset] == 'X' )
				{
					flags.flags |= FormatFlags::HEX | FormatFlags::UPPERCASE;
					++offset;
				}
				else if( str[offset] == 'o' )
				{
					flags.flags |= FormatFlags::OCT;
					++offset;
				}

				if( offset < str.length() && str[offset] == '!' )
				{
					flags.flags |= FormatFlags::SHOW_BASE;
					++offset;
				}
			}
		}

		if( (offset < str.length()) && (str[offset] == BRACE_CLOSE) )
		{
			++offset;
		}
		else
		{
			// No close brace.
			retval = false;
		}
	}

	return retval;
}

/////////////////////////////////////////////////////////////////////////////
size_t Format::process(String& str, size_t minArg, size_t maxArg, FormatFlags& flags, FormatErrorHandling errortype)
{
	size_t len(str.length());
	size_t c = static_cast<size_t>(-1);
	bool err = false;
	size_t i = 0;
	size_t percentStart = 0;
	for (; (i < len) && (c == size_t(-1)) && !err; ++i)
	{
		if( str[i] == '%' )
		{
			percentStart = i;
			if (i + 1 < len)
			{
				++i;

				if( str[i] == '%' )
				{
					oss.append('%');
				}
				else if( str[i] == BRACE_OPEN )
				{
					if( !processFormatSpecifier(str, i, c, flags) )
					{
						// No close brace or some other error.
						err = true;
					}
					else
					{
						// Incremented next loop.
						--i;
					}
				}
				else if( isdigit(str[i]) )
				{
					c = str[i] - '0';
				}
				else
				{
					// Random junk after '%'
					err = true;
				}
			}
			else
			{
				err = true;
			}
		}
		else // anything other than a '%'
		{
			oss.append(str[i]);
		}
	} // for
	if ( (i <= len) && (c != size_t(-1)))
	{
		if( c > maxArg )
		{
			String error = String("Parameter specifier ") + c + " is too large (>" + maxArg + ")";

			if( errortype == E_FORMAT_EXCEPTION )
			{
				BLOCXX_THROW(FormatException, error.c_str());
			}
			oss.append("\n*** " + error);
			err = true;
		}
		else if( c < minArg )
		{
			String error = String("Parameter specifier ") + c + " must be >= " + minArg;
			if( errortype == E_FORMAT_EXCEPTION )
			{
				BLOCXX_THROW(FormatException, error.c_str());
			}
			oss.append("\n*** " + error);

			err = true;
		}
	}
	if (err)
	{
		// Print the percent and all of the text causing the error.
		size_t textlength = std::max(i, percentStart + 1) - percentStart;

		String error = "Error in format string at \"" + str.substring(percentStart, textlength) + "\"";

		if( errortype == E_FORMAT_EXCEPTION )
		{
			BLOCXX_THROW(FormatException, error.c_str());
		}
		oss.append("\n*** " + error + "\n");
		str.erase();
		return '0';
	}
	str.erase(0, i);

	return c;
} // process

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void Format::put(const String& t, const FormatFlags& flags)
{
	oss.append(align(t, flags));
}
void Format::put(const void* vp, const FormatFlags& flags)
{
	String s;
	if( !vp )
	{
		s = "<NULL>";
	}
	else
	{
#if __SIZEOF_POINTER__ == 4
		s = "0x" + hexify(Int32(vp));
#else
		s = "0x" + hexify(Int64(vp));
#endif
	}
	oss.append(align(s, flags));
}
void Format::put(const char* t, const FormatFlags& flags)
{
	oss.append(align(String(t ? t : "<NULL>"), flags));
}
void Format::put(char* t, const FormatFlags& flags)
{
	oss.append(align(String(t ? t : "<NULL>"), flags));
}
void Format::put(char t, const FormatFlags& flags)
{
	oss.append(align(String(t), flags));
}
void Format::put(float t, const FormatFlags& flags)
{
	// FIXME! Floating point precision...
	oss.append(align(String(t), flags));
}
void Format::put(double t, const FormatFlags& flags)
{
	// FIXME! Floating point precision...
	oss.append(align(String(t), flags));
}
void Format::put(long double t, const FormatFlags& flags)
{
	// FIXME! Floating point precision...
	oss.append(align(String(Real64(t)), flags));
}

/////////////////////////////////////////////////////////////////////////////
#define BLOCXX_DEFINE_PUT(type) \
void Format::put(type t, const FormatFlags& flags) \
{ \
	String base; \
	String s; \
	if( flags.flags & FormatFlags::HEX ) \
	{ \
		s = hexify(t, (flags.flags & FormatFlags::UPPERCASE) != 0); \
		if( flags.flags & FormatFlags::SHOW_BASE) \
		{ \
			base = "0x"; \
		} \
	} \
	else if( flags.flags & FormatFlags::OCT ) \
	{ \
		s = octify(t); \
	} \
	else \
	{ \
		s = decify(t); \
		if( flags.flags & FormatFlags::SHOW_PLUS  && (t > (type)(0)) ) \
		{ \
			base = "+"; \
		} \
	} \
	\
	if( int(s.length()) < flags.precision ) \
	{ \
		s = repeat('0', flags.precision - s.length()) + s; \
	} \
	\
	oss.append(align(base + s, flags)); \
}

#ifdef BLOCXX_WIN32
#pragma warning(push)
// We know that these have constant conditional expressions for numeric types.  Ignore it.
#pragma warning(disable:4127)
#endif

BLOCXX_DEFINE_PUT(unsigned char);
BLOCXX_DEFINE_PUT(short);
BLOCXX_DEFINE_PUT(unsigned short);
BLOCXX_DEFINE_PUT(int);
BLOCXX_DEFINE_PUT(unsigned int);
BLOCXX_DEFINE_PUT(long);
BLOCXX_DEFINE_PUT(unsigned long);
BLOCXX_DEFINE_PUT(long long);
BLOCXX_DEFINE_PUT(unsigned long long);
#ifdef BLOCXX_WIN32
#pragma warning(pop)
#endif
#undef BLOCXX_DEFINE_PUT

Format::Format(const char* ca, const String& a) : oss()
{
	String fmt(ca);
	while (!fmt.empty())
	{
		FormatFlags flags;
		switch( process(fmt, 1, 1, flags) )
		{
		case 1: put(a, flags); break;
		}
	}
}
Format::Format(const char* ca, const String& a, const String& b) : oss()
{
	String fmt(ca);
	while (!fmt.empty())
	{
		FormatFlags flags;
		switch( process(fmt, 1, 2, flags) )
		{
		case 1: put(a, flags); break;
		case 2: put(b, flags); break;
		}
	}
}
Format::Format(const char* ca, const String& a, const String& b, const String& c) : oss()
{
	String fmt(ca);
	while (!fmt.empty())
	{
		FormatFlags flags;
		switch( process(fmt, 1, 3, flags) )
		{
		case 1: put(a, flags); break;
		case 2: put(b, flags); break;
		case 3: put(c, flags); break;
		}
	}
}
#ifndef BLOCXX_NO_IOSTREAMS
BLOCXX_COMMON_API std::ostream& operator<<(std::ostream& os, const Format& f)
{
	os << f.toString();
	return os;
}
#endif


} // end namespace BLOCXX_NAMESPACE

