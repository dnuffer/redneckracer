/*******************************************************************************
* Copyright (C) 2008, Quest Software, Inc. All rights reserved.
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

#ifndef BLOCXX_FORMAT_HPP_INCLUDE_GUARD_
#define BLOCXX_FORMAT_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/String.hpp"
#include "miniblocxx/Exception.hpp"
#include "miniblocxx/StringBuffer.hpp"
#include "miniblocxx/stl/algorithm"
#include <string>

namespace BLOCXX_NAMESPACE
{
	// These are to handle internal stream manipulation of format flags.
	struct FormatFlags
	{
		FormatFlags();
		long flags;
		size_t width;
		int precision;
		char fill;

		static const long ALIGN_LEFT = 1;
		static const long SHOW_PLUS = 2; // NOT YET IMPLEMENTED
		static const long UPPERCASE = 4; // NOT YET IMPLEMENTED
		static const long HEX = 8; // NOT YET IMPLEMENTED
		static const long OCT = 16; // NOT YET IMPLEMENTED
		static const long SHOW_BASE = 32; // NOT YET IMPLEMENTED
	};

	String align(const String& s, const FormatFlags& flags);

	String toString(bool b, const FormatFlags& flags);
	String toString(const char* const c, const FormatFlags& flags);
	String toString(const std::string& s, const FormatFlags& flags);
	template <typename T>
	String toString(const T& t, const FormatFlags& flags)
	{
		return align(t.toString(), flags);
	}

	BLOCXX_DECLARE_APIEXCEPTION(Format, BLOCXX_COMMON_API);

	/**
	 * The format class is used to supply a printf-like string conversion.  This
	 * conversion is done by numbered arguments instead of by relative position.
	 * Any text in the format string that is not a format specifier is copied to
	 * the output.
	 *
	 * A format specifier is specified as:
	 *   format <- %%
	 *     | format %digit
	 *     | format %<digits>
	 *     | format %<digits:modifiers>
	 * Where modifiers are:
	 *     -    N      .  M       b    f
	 *   ([-]?digits)?(.digits)?[xXo]?[!]
	 *
	 *  The first digits (N) specify the minimum field width.  If a '-'
	 *  is supplied, the output will be left-justified (the default is
	 *  right).  If '0' is supplied as the first digit of N, zero
	 *  padding will be applied. If both '-' and '0' are supplied the
	 *  '0' will be ignored and the output will be left justified.
	 *  NOTE: Zero padding does not apply to the output of boolean
	 *  values or full objects.
	 *
	 *  The second (M) set of digits (after the dot) specify the precision of
	 *  the output.  This is the same as the stream manipulator
	 *  std::setprecision(digits).
	 *
	 *  The base (b) specifies the base to use for displaying numbers:
	 *    x = hex with lowercase a-f
	 *    X = hex with uppercase A-F
	 *    o = octal
	 *
	 *  The base flag (f) specifies that the base should be shown.  For numbers
	 *  displayed with 'x' or 'X', this will be 0x or '0X'.  For boolean
	 *  variables, this will display "true" or "false" instead of "1" or "0".
	 *
	 * Example:
	 *   This would produce the string "Hello and Goodbye but then Hello again."
	 *   Format("%1 and %2 but then %1 again.", "Hello", "Goodbye")
	 *
	 *   This would produce "123 in hex is 0x7b"
	 *   Format("%1 in hex is %<1:x!>", 123)
	 *
	 * Note: The number of arguments is limited to 9 unless a C++0x compiler is
	 * used with the C++0x features enabled.
	 */

//  Format class declaration  -----------------------------------------------//
class BLOCXX_COMMON_API Format
{
public:

	operator String() const;
	operator std::string() const;
	String toString() const;
	const char* c_str() const;

#if defined(BLOCXX_USE_CXX_0X)
	// Generic templated constructor taking any number (>=1) and type of
	// arguments.
	template <typename... Args>
	Format(const char* ca, const Args&...);
#else
	// generic templated constructors
	template<typename A>
	Format(const char* ca, const A& a);
	template<typename A, typename B>
	Format(const char* ca, const A& a, const B& b);
	template<typename A, typename B, typename C>
	Format(const char* ca, const A& a, const B& b, const C& c);
	template<typename A, typename B, typename C, typename D>
	Format(const char* ca, const A& a, const B& b, const C& c, const D& d);
	template<typename A, typename B, typename C, typename D, typename E>
	Format(const char* ca, const A& a, const B& b, const C& c, const D& d, const E& e);
	template<typename A, typename B, typename C, typename D, typename E, typename F>
	Format(const char* ca, const A& a, const B& b, const C& c, const D& d, const E& e, const F& f);
	template<typename A, typename B, typename C, typename D, typename E, typename F, typename G>
	Format(const char* ca, const A& a, const B& b, const C& c, const D& d, const E& e, const F& f, const G& g);
	template<typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H>
	Format(const char* ca, const A& a, const B& b, const C& c, const D& d, const E& e, const F& f, const G& g, const H& h);
	template<typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H, typename I>
	Format(const char* ca, const A& a, const B& b, const C& c, const D& d, const E& e, const F& f, const G& g, const H& h, const I& i);
#endif

	// These specific versions are to help prevent template bloat
	Format(const char* ca, const String& a);
	Format(const char* ca, const String& a, const String& b);
	Format(const char* ca, const String& a, const String& b, const String& c);

	virtual ~Format();

private:

	StringBuffer oss;

protected:
	// Used only in derived classes.
	Format();

	enum FormatErrorHandling
	{
		E_FORMAT_ERROR_IN_OUTPUT,
		E_FORMAT_EXCEPTION
	};

	bool processFormatSpecifier(String& str, size_t& offset, size_t& argnum, FormatFlags& flags);
	size_t process(String& f, size_t minArg, size_t maxArg, FormatFlags& flags, FormatErrorHandling errortype = E_FORMAT_ERROR_IN_OUTPUT);
	template<typename T> void put(const T& t, const FormatFlags& flags);
	// These are to help prevent template bloat
	void put (const String& t, const FormatFlags& flags);
	void put (const void* t, const FormatFlags& flags);
	void put (const char* t, const FormatFlags& flags);
	void put (char* t, const FormatFlags& flags);
	void put (char t, const FormatFlags& flags);
	void put (unsigned char t, const FormatFlags& flags);
	void put (short t, const FormatFlags& flags);
	void put (unsigned short t, const FormatFlags& flags);
	void put (int t, const FormatFlags& flags);
	void put (unsigned int t, const FormatFlags& flags);
	void put (long t, const FormatFlags& flags);
	void put (unsigned long t, const FormatFlags& flags);
	void put (long long t, const FormatFlags& flags);
	void put (unsigned long long t, const FormatFlags& flags);
	void put (float t, const FormatFlags& flags);
	void put (double t, const FormatFlags& flags);
	void put (long double t, const FormatFlags& flags);
#if defined(BLOCXX_USE_CXX_0X)
	// This will be turned into something similar to an array of multiple types
	// and doing a chain of if statements (with optimizations turned on a low
	// level):
	//
	// if( n == 1 )
	// {
	//   put(args[0]);
	// }
	// else if( n == 2 )
	// {
	//   put(args[1]);
	// }
	// ...
	//
	// If optimizations are fully on, this should be as good as a
	// switch statement.
	template <typename T, typename... Args>
	inline void putArgs(int n, Flags& flags, const T& t, const Args&... args)
	{
		if( n == 1 )
		{
			put(t, flags);
		}
		else
		{
			putArgs(n-1, flags, args...);
		}
	}
	template <typename T>
	inline void putArgs(int n, Flags& flags, const T& t)
	{
		if( n == 1 )
		{
			put(t, flags);
		}
	}
#endif
}; // class Format

template<typename T>
void Format::put(const T& t, const FormatFlags& flags)
{ // t is inserted into oss

	oss.append(align(::blocxx::toString(t, flags), flags));
}

#if defined(BLOCXX_USE_CXX_0X)
template <typename... Args>
Format::Format(const char* ca, const Args&... args)
{
	String fmt(ca);
	while(!fmt.empty())
	{
		const FormatFlags flags;
		putArgs(process(fmt, 1, sizeof...(args), flags), flags, args...);
	}
}

#else // no c++0x

template<typename A>
Format::Format(const char* ca, const A& a) : oss()
{
	String fmt(ca);
	while (!fmt.empty())
	{
		FormatFlags flags;
		switch (process(fmt, 1, 1, flags))
		{
		case 1: put(a, flags); break;
		}
	}
}
template<typename A, typename B>
Format::Format(const char* ca, const A& a, const B& b) : oss()
{
	String fmt(ca);
	while (!fmt.empty())
	{
		FormatFlags flags;
		switch (process(fmt, 1, 2, flags))
		{
		case 1: put(a, flags); break;
		case 2: put(b, flags); break;
		}
	}
}
template<typename A, typename B, typename C>
Format::Format(const char* ca, const A& a, const B& b, const C& c) : oss()
{
	String fmt(ca);
	while (!fmt.empty())
	{
		FormatFlags flags;
		switch (process(fmt, 1, 3, flags))
		{
		case 1: put(a, flags); break;
		case 2: put(b, flags); break;
		case 3: put(c, flags); break;
		}
	}
}
template<typename A, typename B, typename C, typename D>
Format::Format(const char* ca, const A& a, const B& b, const C& c, const D& d) : oss()
{
	String fmt(ca);
	while (!fmt.empty())
	{
		FormatFlags flags;
		switch (process(fmt, 1, 4, flags))
		{
		case 1: put(a, flags); break;
		case 2: put(b, flags); break;
		case 3: put(c, flags); break;
		case 4: put(d, flags); break;
		}
	}
}
template<typename A, typename B, typename C, typename D, typename E>
Format::Format(const char* ca, const A& a, const B& b, const C& c, const D& d, const E& e) : oss()
{
	String fmt(ca);
	while (!fmt.empty())
	{
		FormatFlags flags;
		switch (process(fmt, 1, 5, flags))
		{
		case 1: put(a, flags); break;
		case 2: put(b, flags); break;
		case 3: put(c, flags); break;
		case 4: put(d, flags); break;
		case 5: put(e, flags); break;
		}
	}
}
template<typename A, typename B, typename C, typename D, typename E, typename F>
Format::Format(const char* ca, const A& a, const B& b, const C& c, const D& d, const E& e, const F& f) : oss()
{
	String fmt(ca);
	while (!fmt.empty())
	{
		FormatFlags flags;
		switch (process(fmt, 1, 6, flags))
		{
		case 1: put(a, flags); break;
		case 2: put(b, flags); break;
		case 3: put(c, flags); break;
		case 4: put(d, flags); break;
		case 5: put(e, flags); break;
		case 6: put(f, flags); break;
		}
	}
}
template<typename A, typename B, typename C, typename D, typename E, typename F, typename G>
Format::Format(const char* ca, const A& a, const B& b, const C& c, const D& d, const E& e, const F& f, const G& g) : oss()
{
	String fmt(ca);
	while (!fmt.empty())
	{
		FormatFlags flags;
		switch (process(fmt, 1, 7, flags))
		{
		case 1: put(a, flags); break;
		case 2: put(b, flags); break;
		case 3: put(c, flags); break;
		case 4: put(d, flags); break;
		case 5: put(e, flags); break;
		case 6: put(f, flags); break;
		case 7: put(g, flags); break;
		}
	}
}
template<typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H>
Format::Format(const char* ca, const A& a, const B& b, const C& c, const D& d, const E& e, const F& f, const G& g, const H& h) : oss()
{
	String fmt(ca);
	while (!fmt.empty())
	{
		FormatFlags flags;
		switch (process(fmt, 1, 8, flags))
		{
		case 1: put(a, flags); break;
		case 2: put(b, flags); break;
		case 3: put(c, flags); break;
		case 4: put(d, flags); break;
		case 5: put(e, flags); break;
		case 6: put(f, flags); break;
		case 7: put(g, flags); break;
		case 8: put(h, flags); break;
		}
	}
}
template<typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H, typename I>
Format::Format(const char* ca, const A& a, const B& b, const C& c, const D& d, const E& e, const F& f, const G& g, const H& h, const I& i) : oss()
{
	String fmt(ca);
	while (!fmt.empty())
	{
		FormatFlags flags;
		switch (process(fmt, 1, 9, flags))
		{
		case 1: put(a, flags); break;
		case 2: put(b, flags); break;
		case 3: put(c, flags); break;
		case 4: put(d, flags); break;
		case 5: put(e, flags); break;
		case 6: put(f, flags); break;
		case 7: put(g, flags); break;
		case 8: put(h, flags); break;
		case 9: put(i, flags); break;
		}
	}
}
#endif // c++0x

#ifndef BLOCXX_NO_IOSTREAMS
BLOCXX_COMMON_API std::ostream& operator<<(std::ostream& os, const Format& f);
#endif

} // end namespace BLOCXX_NAMESPACE

#endif // BLOCXX_FORMAT_HPP_INCLUDE_GUARD_

