/*******************************************************************************
* Copyright (C) 2009-2010, Quest Software, Inc. All rights reserved.
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
 * @author Jon Carey
 * @author Dan Nuffer
 */

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/String.hpp"
#include "miniblocxx/Char16.hpp"
#include "miniblocxx/Array.hpp"
#include "miniblocxx/Assertion.hpp"
#include "miniblocxx/Assertion.hpp"
#include "miniblocxx/AutoPtr.hpp"
#include "miniblocxx/Bool.hpp"
#include "miniblocxx/UTF8Utils.hpp"
#include "miniblocxx/COWIntrusiveCountableBase.hpp"
#include "miniblocxx/StringConversionImpl.hpp"
#include "miniblocxx/stl/vector.hpp"
#include "miniblocxx/Format.hpp"

#ifndef BLOCXX_NO_IOSTREAMS
#include <iostream>
#include "miniblocxx/BinarySerialization.hpp"
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#ifdef BLOCXX_WIN32
#define SNPRINTF _snprintf
#else
#define SNPRINTF snprintf
#endif

namespace BLOCXX_NAMESPACE
{

BLOCXX_DEFINE_EXCEPTION(StringConversion);


//////////////////////////////////////////////////////////////////////////////
static inline int
strncmpi(const char* s1, const char* s2, size_t n)
{
	String ls1(s1, n);
	String ls2(s2, n);
	return ls1.compareToIgnoreCase(ls2);
}

// class invariant: m_buf points to a null-terminated sequence of characters. m_buf is m_len+1 bytes long.
class String::ByteBuf : public COWIntrusiveCountableBase
{
public:
	ByteBuf(const char* s) :
		m_len(::strlen(s)), m_buf(new char[m_len+1])
	{
		strcpy(m_buf, s);
	}

	ByteBuf(const ByteBuf& arg)
		: COWIntrusiveCountableBase(arg)
		, m_len(arg.m_len)
		, m_buf(new char[m_len+1])
	{
		strcpy(m_buf, arg.m_buf);
	}

	ByteBuf(AutoPtrVec<char>& s, size_t len)
		: m_len(len), m_buf(s.release())
	{
	}

	~ByteBuf() { delete [] m_buf; }

	ByteBuf& operator= (const ByteBuf& arg)
	{
		char* buf = new char[arg.m_len+1];
		strcpy(buf, arg.m_buf);
		delete [] m_buf;
		m_buf = buf;
		m_len = arg.m_len;
		return *this;
	}

	size_t length() const { return m_len; }
	char* data() const { return m_buf; }
	ByteBuf* clone() const { return new ByteBuf(*this); }
private:
	size_t m_len;
	char* m_buf;
};
//////////////////////////////////////////////////////////////////////////////
#if defined(BLOCXX_AIX)
const size_t String::npos = ~0;
#endif
//////////////////////////////////////////////////////////////////////////////
String::String() :
	m_buf(0)
{
}

#if defined(BLOCXX_WIN32)
#define snprintf _snprintf // stupid windoze...
#endif
//////////////////////////////////////////////////////////////////////////////
String::String(Int32 val) :
	m_buf(NULL)
{
	char tmpbuf[32];
	int len = snprintf(tmpbuf, sizeof(tmpbuf), "%d", val);
	AutoPtrVec<char> bfr(new char[len+1]);
	::snprintf(bfr.get(), len+1, "%d", val);
	m_buf = new ByteBuf(bfr, len);
}
//////////////////////////////////////////////////////////////////////////////
String::String(UInt32 val) :
	m_buf(NULL)
{
	char tmpbuf[32];
	int len = ::snprintf(tmpbuf, sizeof(tmpbuf), "%u", val);
	AutoPtrVec<char> bfr(new char[len+1]);
	::snprintf(bfr.get(), len+1, "%u", val);
	m_buf = new ByteBuf(bfr, len);
}
#if defined(BLOCXX_INT32_IS_INT) && defined(BLOCXX_INT64_IS_LONG_LONG)
//////////////////////////////////////////////////////////////////////////////
String::String(long val) :
	m_buf(NULL)
{
	char tmpbuf[32];
	int len = snprintf(tmpbuf, sizeof(tmpbuf), "%ld", val);
	AutoPtrVec<char> bfr(new char[len+1]);
	::snprintf(bfr.get(), len+1, "%ld", val);
	m_buf = new ByteBuf(bfr, len);
}
//////////////////////////////////////////////////////////////////////////////
String::String(unsigned long val) :
	m_buf(NULL)
{
	char tmpbuf[32];
	int len = ::snprintf(tmpbuf, sizeof(tmpbuf), "%lu", val);
	AutoPtrVec<char> bfr(new char[len+1]);
	::snprintf(bfr.get(), len+1, "%lu", val);
	m_buf = new ByteBuf(bfr, len);
}
#endif
#if defined(BLOCXX_WIN32)
#undef snprintf
#endif
//////////////////////////////////////////////////////////////////////////////
String::String(Int64 val) :
	m_buf(NULL)
{
	char tmpbuf[32];
	int len = ::snprintf(tmpbuf, sizeof(tmpbuf), "%llu", (long long)val);
	AutoPtrVec<char> bfr(new char[len+1]);
	::snprintf(bfr.get(), len+1, "%llu", (long long)val);
	m_buf = new ByteBuf(bfr, len);
}
//////////////////////////////////////////////////////////////////////////////
String::String(UInt64 val) :
	m_buf(NULL)
{
	char tmpbuf[32];
	::snprintf(tmpbuf, sizeof(tmpbuf), "%llu", (long long)val);
	m_buf = new ByteBuf(tmpbuf);
}
//////////////////////////////////////////////////////////////////////////////
// decimal digits = ceiling((bits)*ln(2)/ln(10))
String::String(Real32 val) :
	m_buf(NULL)
{
	char tmpbuf[128];
	int len = ::snprintf(tmpbuf, sizeof(tmpbuf), "%lf", val);
	AutoPtrVec<char> bfr(new char[len+1]);
	::snprintf(bfr.get(), len+1, "%lf", val);
	m_buf = new ByteBuf(bfr, len);
}
//////////////////////////////////////////////////////////////////////////////
String::String(Real64 val) :
	m_buf(NULL)
{
	char tmpbuf[128];
	int len = ::snprintf(tmpbuf, sizeof(tmpbuf), "%lf", val);
	AutoPtrVec<char> bfr(new char[len+1]);
	::snprintf(bfr.get(), len+1, "%lf", val);
	m_buf = new ByteBuf(bfr, len);
}
//////////////////////////////////////////////////////////////////////////////
String::String(const char* str) :
	m_buf(NULL)
{
	m_buf = (NULL == str) ? 0 : new ByteBuf(str);
}
//////////////////////////////////////////////////////////////////////////////
String::String(ETakeOwnershipFlag, char* allocatedMemory, size_t len) :
	m_buf(NULL)
{
	BLOCXX_ASSERT(allocatedMemory != 0);
	AutoPtrVec<char> p(allocatedMemory);
	m_buf = new ByteBuf(p, len);
}
//////////////////////////////////////////////////////////////////////////////
String::String(const char* str, size_t len) :
	m_buf(NULL)
{
	if (NULL == str)
	{
		m_buf = 0;
	}
	else
	{
		AutoPtrVec<char> bfr(new char[len+1]);
		::memcpy(bfr.get(), str, len);
		bfr[len] = '\0';
		m_buf = new ByteBuf(bfr, len);
	}
}
//////////////////////////////////////////////////////////////////////////////
String::String(const String& arg) :
	m_buf(arg.m_buf)
{
}
//////////////////////////////////////////////////////////////////////////////
String::String(char c) :
	m_buf(NULL)
{
	if (c != '\0')
	{
		char bfr[2];
		bfr[0] = c;
		bfr[1] = '\0';
		m_buf = new ByteBuf(bfr);
	}
	else
	{
		m_buf = 0;
	}
}
//////////////////////////////////////////////////////////////////////////////
String::String(const std::string& str)
{
	m_buf = new ByteBuf(str.c_str());
}
//////////////////////////////////////////////////////////////////////////////
String::~String()
{
}
//////////////////////////////////////////////////////////////////////////////
void
String::swap(String& x)
{
	m_buf.swap(x.m_buf);
}
//////////////////////////////////////////////////////////////////////////////
char*
String::allocateCString() const
{
	size_t len = length() + 1;
	char* str = static_cast<char*>(malloc(len));
	::strcpy(str, c_str());
	return str;
}
//////////////////////////////////////////////////////////////////////////////
String::operator std::string() const
{
	return std::string(c_str());
}
//////////////////////////////////////////////////////////////////////////////
size_t
String::length() const
{
	return (m_buf) ? m_buf->length() : 0;
}
//////////////////////////////////////////////////////////////////////////////
size_t
String::UTF8Length() const
{
	return UTF8Utils::charCount(c_str());
}
//////////////////////////////////////////////////////////////////////////////
#ifdef BLOCXX_WIN32
#define vsnprintf _vsnprintf // stupid windoze
#endif
int
String::format(const char* fmt, ...)
{
	int n, size = 64;
	AutoPtrVec<char> p(new char[size]);

	va_list ap;

	// Try to print in the allocated space
	while (true)
	{
		va_start(ap, fmt);
		n = vsnprintf(p.get(), size, fmt, ap);
		va_end(ap);                // If that worked, return the string.
		if (n > -1 && n < size)
		{
			m_buf = new ByteBuf(p, n);
			return static_cast<int>(length());
		}
		if (n > -1)    // glibc 2.1
			size = n+1; // precisely what is needed
		else           // glibc 2.0
			size *= 2;  // twice the old size
		p = new char[size];
	}
	// Not reachable.
	return 0;
}
#ifdef BLOCXX_WIN32
#undef vsnprintf // stupid windoze
#endif
//////////////////////////////////////////////////////////////////////////////
char
String::charAt(size_t ndx) const
{
	return (m_buf) ? m_buf->data()[ndx] : '\0';
}
//////////////////////////////////////////////////////////////////////////////
int
String::compareTo(const char* arg) const
{
	const char* lhs = "";
	if (m_buf)
	{
		lhs = m_buf->data();
	}
	return ::strcmp(lhs, arg);
}
//////////////////////////////////////////////////////////////////////////////
int
String::compareTo(const String& arg) const
{
	return compareTo(arg.c_str());
}
//////////////////////////////////////////////////////////////////////////////
int
String::compareToIgnoreCase(const char* arg) const
{
	const char* lhs = "";
	if (m_buf)
	{
		lhs = m_buf->data();
	}
	return UTF8Utils::compareToIgnoreCase(lhs, arg);
}
//////////////////////////////////////////////////////////////////////////////
int
String::compareToIgnoreCase(const String& arg) const
{
	return compareToIgnoreCase(arg.c_str());
}

//////////////////////////////////////////////////////////////////////////////
String&
String::concat(const char* arg)
{
	if (arg && *arg)
	{
		size_t len = length() + ::strlen(arg);
		AutoPtrVec<char> bfr(new char[len+1]);
		bfr[0] = 0;
		if (m_buf)
		{
			::strcpy(bfr.get(), m_buf->data());
		}
		::strcat(bfr.get(), arg);
		m_buf = new ByteBuf(bfr, len);
	}
	return *this;
}

//////////////////////////////////////////////////////////////////////////////
String&
String::concat(char arg)
{
	size_t newlen = length() + 1;
	AutoPtrVec<char> bfr(new char[newlen+1]);
	bfr[0] = 0;
	if (m_buf)
	{
		::strcpy(bfr.get(), m_buf->data());
	}
	*(bfr.get()+length()) = arg;
	*(bfr.get()+newlen) = 0;
	m_buf = new ByteBuf(bfr, newlen);
	return *this;
}

//////////////////////////////////////////////////////////////////////////////
bool
String::endsWith(char arg) const
{
	return (m_buf
			&& m_buf->length()
			&& m_buf->data()[m_buf->length()-1] == arg);
}

//////////////////////////////////////////////////////////////////////////////
bool
String::endsWith(const char* arg, EIgnoreCaseFlag ignoreCase) const
{
	if (!arg || !*arg)
	{
		return (length() == 0);
	}

	if (!m_buf)
	{
		return false;
	}

	int ndx = static_cast<int>(length() - ::strlen(arg));
	if (ndx < 0)
	{
		return false;
	}

	return (ignoreCase)
		? (UTF8Utils::compareToIgnoreCase(m_buf->data()+ndx, arg) == 0)
		: (::strcmp(m_buf->data()+ndx, arg) == 0);
}
//////////////////////////////////////////////////////////////////////////////
bool
String::equals(const char* arg) const
{
	return(compareTo(arg) == 0);
}
//////////////////////////////////////////////////////////////////////////////
bool
String::equals(const String& arg) const
{
	return equals(arg.c_str());
}
//////////////////////////////////////////////////////////////////////////////
bool
String::equalsIgnoreCase(const char* arg) const
{
	return(compareToIgnoreCase(arg) == 0);
}
//////////////////////////////////////////////////////////////////////////////
bool
String::equalsIgnoreCase(const String& arg) const
{
	return equalsIgnoreCase(arg.c_str());
}
//////////////////////////////////////////////////////////////////////////////
UInt32
String::hashCode() const
{
	UInt32 hash = 0;
	size_t len = length();
	for (size_t i = 0; i < len; i++)
	{
		// Don't need to check if m_buf is null, because if it is, len == 0,
		// and this loop won't be executed.
		const char temp = m_buf->data()[i];
		hash = (hash << 4) + (temp * 13);
		UInt32 g = hash & 0xf0000000;
		if (g)
		{
			hash ^= (g >> 24);
			hash ^= g;
		}
	}
	return hash;
}
//////////////////////////////////////////////////////////////////////////////
size_t
String::indexOf(char ch, size_t fromIndex) const
{
	//if (fromIndex < 0)
	//{
	//	fromIndex = 0;
	//}
	size_t cc = npos;
	if (fromIndex < length())
	{
		// Don't need to check m_buf for NULL, because if length() == 0,
		// this code won't be executed.
		const char* p = String::strchr(m_buf->data()+fromIndex, ch);
		if (p)
		{
			cc = p - m_buf->data();
		}
	}
	return cc;
}
//////////////////////////////////////////////////////////////////////////////
size_t
String::indexOf(const char* arg, size_t fromIndex) const
{
	size_t cc = npos;
	if (fromIndex < length())
	{
		// Don't need to check m_buf for NULL, because if length() == 0,
		// this code won't be executed
		char* p(0);
		if (arg && *arg)
		{
			p = ::strstr(m_buf->data()+fromIndex, arg);
		}
		else
		{
			p = m_buf->data()+fromIndex;
		}

		if (p != NULL)
		{
			cc = static_cast<size_t>(p - m_buf->data());
		}
	}
	return cc;
}
//////////////////////////////////////////////////////////////////////////////
size_t
String::lastIndexOf(char ch, size_t fromIndex) const
{
	if (fromIndex == npos)
	{
		if ((fromIndex = length()-1) == npos)
		{
			return npos;
		}
	}
	size_t cc = npos;
	if (fromIndex < length())
	{
		for (size_t i = fromIndex; i != npos; i--)
		{
			// Don't need to check m_buf for NULL, because if length() == 0,
			// this code won't be executed.
			if (m_buf->data()[i] == ch)
			{
				cc = i;
				break;
			}
		}
	}
	return cc;
}
//////////////////////////////////////////////////////////////////////////////
size_t
String::lastIndexOf(const char* arg, size_t fromIndex) const
{
	if (fromIndex == npos || fromIndex >= length())
	{
		if (static_cast<int>(fromIndex = length()-1) < 0)
		{
			return npos;
		}
	}

	size_t arglen = (arg) ? ::strlen(arg) : 0;
	if (static_cast<int>(fromIndex -= arglen - 1) < 0)
	{
		return npos;
	}
	if (!arg)
	{
		return length() - 1;
	}
	while (fromIndex != npos)
	{
		// Don't need to check m_buf for NULL, because if length() == 0,
		// this code won't be executed.
		if (::strncmp(m_buf->data()+fromIndex, arg, arglen) == 0)
		{
			break;
		}
		fromIndex--;
	}
	return fromIndex;
}
//////////////////////////////////////////////////////////////////////////////
bool
String::startsWith(char arg) const
{
	return (m_buf
			&& m_buf->length()
			&& m_buf->data()[0] == arg);
}

//////////////////////////////////////////////////////////////////////////////
bool
String::startsWith(const char* arg, EIgnoreCaseFlag ignoreCase) const
{
	bool cc = false;
	if (!arg || !*arg) // treat NULL as identical to the empty string
	{
		return true; // the empty string is a prefix of any string
	}

	size_t arglen = ::strlen(arg);
	if (arglen <= length())
	{
		// Don't need to check m_buf for NULL, because if length() == 0,
		// this code won't be executed.
		if (ignoreCase == E_CASE_INSENSITIVE)
		{
			cc = (strncmpi(m_buf->data(), arg, arglen) == 0);
		}
		else
		{
			cc = (::strncmp(m_buf->data(), arg, arglen) == 0);
		}
	}
	return cc;
}
//////////////////////////////////////////////////////////////////////////////
String
String::substring(size_t beginIndex, size_t len) const
{
	String nil;
	size_t count = len;
	size_t l = length();
	if (0 == l)
	{
		return nil;
	}
	if (beginIndex >= l)
	{
		return nil;
	}
	else if (0 == len)
	{
		return nil;
	}
	else if (len == npos)
	{
		count = l - beginIndex;
	}
	if (count + beginIndex > l)
	{
		count = l - beginIndex;
	}
	// Don't need to check m_buf for NULL, because if length() == 0,
	// this code won't be executed.
	return String(static_cast<const char*>(m_buf->data()+beginIndex), count);
}
//////////////////////////////////////////////////////////////////////////////
bool
String::isSpaces() const
{
	if (!m_buf)
	{
		return true;
	}
	char* p = m_buf->data();
	while (isspace(*p) && *p != '\0')
	{
		p++;
	}
	return (*p == '\0');
}
//////////////////////////////////////////////////////////////////////////////
String&
String::ltrim()
{
	if (!m_buf)
	{
		return *this;
	}
	char* s1 = m_buf->data();
	while (isspace(*s1) && *s1 != '\0')
	{
		s1++;
	}
	if (s1 == m_buf->data())
	{
		return *this;
	}
	*this = String(s1);
	return *this;
}
//////////////////////////////////////////////////////////////////////////////
String&
String::rtrim()
{
	if (length() == 0)
	{
		return *this;
	}
	char* s1 = m_buf->data() + (length()-1);
	while (isspace(*s1) && s1 >= m_buf->data())
	{
		s1--;
	}
	if (s1 == (m_buf->data() + (length()-1)))
	{
		return *this;
	}
	if (s1 < m_buf->data())
	{
		*this = String();
		return *this;
	}
	size_t len = (s1 - m_buf->data()) + 1;
	*this = String(m_buf->data(), len);
	return *this;
}
//////////////////////////////////////////////////////////////////////////////
String&
String::trim()
{
	if (length() == 0)
	{
		return *this;
	}
	char* s1 = m_buf->data();
	while (isspace(*s1) && *s1 != '\0')
	{
		s1++;
	}
	if (*s1 == '\0')
	{
		// String is all spaces
		*this = String();
		return *this;
	}
	const char* p2 = String::strchr(s1, '\0');
	const char* s2 = p2 - 1;
	while (isspace(*s2))
	{
		s2--;
	}
	if (s1 == m_buf->data() && s2 == p2)
	{
		// String has no leading or trailing spaces
		return *this;
	}
	size_t len = (s2 - s1) + 1;
	*this = String(s1, len);
	return *this;
}
//////////////////////////////////////////////////////////////////////////////
String&
String::erase()
{
	m_buf = 0;
	return *this;
}
//////////////////////////////////////////////////////////////////////////////
String&
String::erase(size_t idx, size_t len)
{
	if ( idx >= length() )
	{
		return *this;
	}
	if (len == npos)
	{
		*this = substring(0, idx);
	}
	else
	{
		*this = substring(0, idx) + substring(idx + len);
	}
	return *this;
}
//////////////////////////////////////////////////////////////////////////////
String&
String::toLowerCase()
{
	if (m_buf)
	{
		if (!UTF8Utils::toLowerCaseInPlace(m_buf->data()))
		{
			*this = UTF8Utils::toLowerCase(m_buf->data());
		}
	}
	return *this;
}

//////////////////////////////////////////////////////////////////////////////
String&
String::toUpperCase()
{
	if (m_buf)
	{
		if (!UTF8Utils::toUpperCaseInPlace(m_buf->data()))
		{
			*this = UTF8Utils::toUpperCase(m_buf->data());
		}
	}
	return *this;
}
//////////////////////////////////////////////////////////////////////////////
#ifndef BLOCXX_NO_IOSTREAMS
void String::readObject(std::streambuf & istrm)
{
	UInt32 len;
	BinarySerialization::readLen(istrm, len);
	AutoPtrVec<char> bfr(new char[len+1]);
	BinarySerialization::read(istrm, bfr.get(), len);
	bfr[len] = '\0';
	m_buf = new ByteBuf(bfr, len);
}
//////////////////////////////////////////////////////////////////////////////
void
String::writeObject(std::streambuf & ostrm) const
{
	UInt32 len = static_cast<UInt32>(length());
	BinarySerialization::writeLen(ostrm, len);
	if (len)
	{
		BinarySerialization::write(ostrm, m_buf->data(), len);
	}
}
#endif // BLOCXX_NO_IOSTREAMS
//////////////////////////////////////////////////////////////////////////////
String&
String::operator= (const String& arg)
{
	m_buf = arg.m_buf;
	return *this;
}
//////////////////////////////////////////////////////////////////////////////
const char*
String::c_str() const
{
	if (m_buf)
	{
		return m_buf->data();
	}
	else
	{
		return "";
	}
}
//////////////////////////////////////////////////////////////////////////////
static const char cnullChar = '\0';
const char&
String::operator[] (size_t ndx) const
{
#ifdef BLOCXX_DEBUG
	BLOCXX_ASSERT(ndx <= length());
#endif
	if (!m_buf) // then length() == 0
	{
		// Only needed in case ndx == 0, but doesn't hurt if ndx > 0
		const_cast<buf_t &>(m_buf) = new ByteBuf("");
	}
	return *(ndx <= m_buf->length() ? m_buf->data() + ndx : &cnullChar);
}
//////////////////////////////////////////////////////////////////////////////
static char nullChar = '\0';
char&
String::operator[] (size_t ndx)
{
#ifdef BLOCXX_DEBUG
	BLOCXX_ASSERT(ndx <= length());
#endif
	if (!m_buf) // then length() == 0
	{
		// Only needed in case ndx == 0, but doesn't hurt if ndx > 0
		m_buf = new ByteBuf("");
	}
	return (ndx <= m_buf->length() ? m_buf->data()[ndx] : nullChar);
}
//////////////////////////////////////////////////////////////////////////////
String
String::toString() const
{
	return *this;
}
//////////////////////////////////////////////////////////////////////////////
static inline void
throwStringConversion(const String::buf_t& m_buf, const char* type)
{
	BLOCXX_THROW(StringConversionException, Format("Unable to convert \"%1\" into %2", m_buf->data(), type).c_str());
}
//////////////////////////////////////////////////////////////////////////////
static inline void
throwStringConversion(const char* str, const char* type)
{
	BLOCXX_THROW(StringConversionException, Format("Unable to convert \"%1\" into %2", str, type).c_str());
}
//////////////////////////////////////////////////////////////////////////////
Real32
String::toReal32() const
{
	return convertToRealType<Real32>(*this);
}
//////////////////////////////////////////////////////////////////////////////
float
String::toFloat() const
{
	return convertToRealType<float>(*this);
}
//////////////////////////////////////////////////////////////////////////////
Real64
String::toReal64() const
{
	return convertToRealType<Real64>(*this);
}
//////////////////////////////////////////////////////////////////////////////
double
String::toDouble() const
{
	return convertToRealType<double>(*this);
}
//////////////////////////////////////////////////////////////////////////////
bool
String::toBool() const
{
	if (equalsIgnoreCase("true"))
	{
		return true;
	}
	else if (equalsIgnoreCase("false"))
	{
		return false;
	}
	else
	{
		throwStringConversion(c_str(), "bool");
	}
	return false; // to make compiler happy
}
template <typename T, typename FP, typename FPRT>
static inline
T doConvertToIntType(const String::buf_t& m_buf, const char* type, FP fp, int base)
{
	// the error detecting code below won't detect an empty string, so
	// we have to check for it explicitly.
	if (m_buf && m_buf->length() > 0)
	{
		char* endptr(0);
		errno = 0;		// errno is thread local
		FPRT v = fp(m_buf->data(), &endptr, base);
		T rv = static_cast<T>(v);
		if (*endptr != '\0' || errno == ERANGE || FPRT(rv) != v)
		{
			throwStringConversion(m_buf, type);
		}
		return rv;
	}
	else
	{
		throwStringConversion("", type);
	}
	return T(); // to make compiler happy
}
typedef unsigned long int (*strtoulfp_t)(const char *, char **,int);
typedef long int (*strtolfp_t)(const char *, char **,int);
typedef unsigned long long int (*strtoullfp_t)(const char *, char **,int);
typedef long long int (*strtollfp_t)(const char *, char **,int);
template <typename T>
static inline
T convertToUIntType(const String::buf_t& m_buf, const char* msg, int base)
{
	return doConvertToIntType<T, strtoulfp_t, unsigned long int>(m_buf, msg, &strtoul, base);
}
template <typename T>
static inline
T convertToIntType(const String::buf_t& m_buf, const char* msg, int base)
{
	return doConvertToIntType<T, strtolfp_t, long int>(m_buf, msg, &strtol, base);
}
template <typename T>
static inline
T convertToUInt64Type(const String::buf_t& m_buf, const char* msg, int base)
{
	return doConvertToIntType<T, strtoullfp_t, unsigned long long int>(m_buf, msg, &String::strtoull, base);
}
template <typename T>
static inline
T convertToInt64Type(const String::buf_t& m_buf, const char* msg, int base)
{
	return doConvertToIntType<T, strtollfp_t, long long int>(m_buf, msg, &String::strtoll, base);
}
//////////////////////////////////////////////////////////////////////////////
UInt8
String::toUInt8(int base) const
{
	return convertToUIntType<UInt8>(m_buf, "UInt8", base);
}
//////////////////////////////////////////////////////////////////////////////
Int8
String::toInt8(int base) const
{
	return convertToIntType<Int8>(m_buf, "Int8", base);
}
//////////////////////////////////////////////////////////////////////////////
UInt16
String::toUInt16(int base) const
{
	return convertToUIntType<UInt16>(m_buf, "UInt16", base);
}
//////////////////////////////////////////////////////////////////////////////
Int16
String::toInt16(int base) const
{
	return convertToIntType<Int16>(m_buf, "Int16", base);
}
//////////////////////////////////////////////////////////////////////////////
UInt32
String::toUInt32(int base) const
{
	return convertToUIntType<UInt32>(m_buf, "UInt32", base);
}
//////////////////////////////////////////////////////////////////////////////
Int32
String::toInt32(int base) const
{
	return convertToIntType<Int32>(m_buf, "Int32", base);
}
//////////////////////////////////////////////////////////////////////////////
UInt64
String::toUInt64(int base) const
{
	return convertToUInt64Type<UInt64>(m_buf, "UInt64", base);
}
//////////////////////////////////////////////////////////////////////////////
Int64
String::toInt64(int base) const
{
	return convertToInt64Type<Int64>(m_buf, "Int64", base);
}
//////////////////////////////////////////////////////////////////////////////
unsigned int
String::toUnsignedInt(int base) const
{
	return convertToUIntType<unsigned int>(m_buf, "unsigned int", base);
}
//////////////////////////////////////////////////////////////////////////////
int
String::toInt(int base) const
{
	return convertToIntType<int>(m_buf, "int", base);
}
//////////////////////////////////////////////////////////////////////////////
StringArray
String::tokenize(const char* delims, EReturnDelimitersFlag returnDelimitersAsTokens, EEmptyTokenReturnFlag returnEmptyTokens) const
{
	StringArray ra;
	if (empty())
	{
		return ra;
	}
	if (delims == 0)
	{
		ra.append(*this);
		return ra;
	}
	// Don't need to check m_buf for NULL, because if length() == 0,
	// this code won't be executed.
	char* pstr = m_buf->data();
	AutoPtrVec<char> data(new char[m_buf->length()+1]);
	data[0] = 0;
	int i = 0;
  	bool last_was_delim = false;
	while (*pstr)
	{
		if (String::strchr(delims, *pstr))
		{
			if (data[0] != 0)
			{
				ra.append(String(data.get()));
				data[0] = 0;
			}
			if ( (returnEmptyTokens == E_RETURN_EMPTY_TOKENS) && last_was_delim )
			{
				ra.append(String());
			}
			if ( returnDelimitersAsTokens == E_RETURN_DELIMITERS )
			{
				ra.append(String(*pstr));
			}
			i = 0;
			last_was_delim = true;
		}
		else
		{
			last_was_delim = false;
			data[i++] = *pstr;
			data[i] = 0;
		}
		pstr++;
	}
	if (data[0] != 0)
	{
		ra.append(String(data.get()));
	}
	else if( (returnEmptyTokens == E_RETURN_EMPTY_TOKENS) && last_was_delim )
	{
		// It ended with a delimiter so there should be an empty token at the end.
		ra.append(String());
	}
	return ra;
}

//////////////////////////////////////////////////////////////////////////////
#ifdef BLOCXX_HAVE_STRTOLL
long long int
String::strtoll(const char* nptr, char** endptr, int base)
{
	return ::strtoll(nptr, endptr, base);
}
#else
#ifndef LLONG_MAX
#if BLOCXX_SIZEOF_LONG_LONG_INT == 8
#define LLONG_MAX 9223372036854775807LL
#else
#define LLONG_MAX 2147483647LL
#endif
#define LLONG_MIN (-LLONG_MAX - 1LL)
#endif
long long int
String::strtoll(const char* nptr, char** endptr, int base)
{
	const char *s;
	long long acc, cutoff;
	int c;
	int neg, any, cutlim;
	// Skip white space and pick up leading +/- sign if any.
	// If base is 0, allow 0x for hex and 0 for octal, else
	// assume decimal; if base is already 16, allow 0x.
	s = nptr;
	do
	{
		c = (unsigned char) *s++;
	} while (isspace(c));
	if (c == '-')
	{
		neg = 1;
		c = *s++;
	}
	else
	{
		neg = 0;
		if (c == '+')
		{
			c = *s++;
		}
	}
	if ((base == 0 || base == 16)
		&& c == '0'
		&& (*s == 'x' || *s == 'X'))
	{
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
	{
		base = c == '0' ? 8 : 10;
	}
	// Compute the cutoff value between legal numbers and illegal
	// numbers.  That is the largest legal value, divided by the
	// base.  An input number that is greater than this value, if
	// followed by a legal input character, is too big.  One that
	// is equal to this value may be valid or not; the limit
	// between valid and invalid numbers is then based on the last
	// digit.  For instance, if the range for longs is
	// [-2147483648..2147483647] and the input base is 10,
	// cutoff will be set to 214748364 and cutlim to either
	// 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
	// a value > 214748364, or equal but the next digit is > 7 (or 8),
	// the number is too big, and we will return a range error.
	//
	// Set any if any `digits' consumed; make it negative to indicate
	// overflow.
	cutoff = neg ? LLONG_MIN : LLONG_MAX;
	cutlim = static_cast<int>(cutoff % base);
	cutoff /= base;
	if (neg)
	{
		if (cutlim > 0)
		{
			cutlim -= base;
			cutoff += 1;
		}
		cutlim = -cutlim;
	}
	for (acc = 0, any = 0;; c = (unsigned char) *s++)
	{
		if (isdigit(c))
		{
			c -= '0';
		}
		else if (isalpha(c))
		{
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		}
		else
		{
			break;
		}
		if (c >= base)
		{
			break;
		}
		if (any < 0)
		{
			continue;
		}
		if (neg)
		{
			if ((acc < cutoff) || ((acc == cutoff) && (c > cutlim)))
			{
				any = -1;
				acc = LLONG_MIN;
				errno = ERANGE;
			}
			else
			{
				any = 1;
				acc *= base;
				acc -= c;
			}
		}
		else
		{
			if ((acc > cutoff) || ((acc == cutoff) && (c > cutlim)))
			{
				any = -1;
				acc = LLONG_MAX;
				errno = ERANGE;
			}
			else
			{
				any = 1;
				acc *= base;
				acc += c;
			}
		}
	}
	if (endptr != 0)
	{
		*endptr = (char *) (any ? s - 1 : nptr);
	}
	return(acc);
}
#endif	// #ifdef BLOCXX_HAVE_STRTOLL

//////////////////////////////////////////////////////////////////////////////
#ifdef BLOCXX_HAVE_STRTOULL
unsigned long long int
String::strtoull(const char* nptr, char** endptr, int base)
{
	return ::strtoull(nptr, endptr, base);
}
#else
#ifndef ULLONG_MAX
#if BLOCXX_SIZEOF_LONG_LONG_INT == 8
#define ULLONG_MAX 18446744073709551615ULL
#else
#define ULLONG_MAX 4294967295ULL
#endif
#endif
unsigned long long int
String::strtoull(const char* nptr, char** endptr, int base)
{
	const char *s;
	unsigned long long acc, cutoff, cutlim;
	unsigned int c;
	int neg, any;
	s = nptr;
	do
	{
		c = (unsigned char) *s++;
	} while (isspace(c));
	if (c == '-')
	{
		neg = 1;
		c = *s++;
	}
	else
	{
		neg = 0;
		if (c == '+')
		{
			c = *s++;
		}
	}
	if ((base == 0 || base == 16)
		&& c == '0'
		&& (*s == 'x' || *s == 'X'))
	{
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
	{
		base = c == '0' ? 8 : 10;
	}
	cutoff = ULLONG_MAX / (unsigned long long)base;
	cutlim = ULLONG_MAX % (unsigned long long)base;
	for (acc = 0, any = 0;; c = (unsigned char) *s++)
	{
		if (isdigit(c))
		{
			c -= '0';
		}
		else if (isalpha(c))
		{
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		}
		else
		{
			break;
		}
		if (c >= (unsigned int)base)
		{
			break;
		}
		if (any < 0)
		{
			continue;
		}
		if ((acc > cutoff) || ((acc == cutoff) && (c > cutlim)))
		{
			any = -1;
			acc = ULLONG_MAX;
			errno = ERANGE;
		}
		else
		{
			any = 1;
			acc *= (unsigned long)base;
			acc += c;
		}
	}
	if (neg && (any > 0))
	{
#ifdef BLOCXX_WIN32
#pragma warning (push)
#pragma warning (disable: 4146)
#endif

		acc = -acc;

#ifdef BLOCXX_WIN32
#pragma warning (pop)
#endif

	}
	if (endptr != 0)
	{
		*endptr = (char *) (any ? s - 1 : nptr);
	}
	return(acc);
}
#endif	// #ifdef BLOCXX_HAVE_STRTOULL

String repeat(char c, size_t count)
{
	if( count > 0 )
	{
		vector<char> temp(count, c);
		return String(&temp[0], count);
	}
	return String();
}

//////////////////////////////////////////////////////////////////////////////
String padOnLeft(const String& s, size_t width, char fill)
{
	if( s.length() >= width )
	{
		return s;
	}

	size_t difference = width - s.length();

	return repeat(fill, difference) + s;
}

String padOnRight(const String& s, size_t width, char fill)
{
	if( s.length() >= width )
	{
		return s;
	}

	size_t difference = width - s.length();

	return s + repeat(fill, difference);
}


//////////////////////////////////////////////////////////////////////////////
String
operator + (const String& s1, const String& s2)
{
	String rstr(s1);
	rstr += s2;
	return rstr;
}
//////////////////////////////////////////////////////////////////////////////
String
operator + (const char* p, const String& s)
{
	String rstr(p);
	rstr += s;
	return rstr;
}
//////////////////////////////////////////////////////////////////////////////
String
operator + (const String& s, const char* p)
{
	String rstr(s);
	rstr += p;
	return rstr;
}
//////////////////////////////////////////////////////////////////////////////
String
operator + (char c, const String& s)
{
	String rstr(c);
	rstr += s;
	return rstr;
}
//////////////////////////////////////////////////////////////////////////////
String
operator + (const String& s, char c)
{
	String rstr(s);
	rstr += String(c);
	return rstr;
}
//////////////////////////////////////////////////////////////////////////////
#ifndef BLOCXX_NO_IOSTREAMS
std::ostream& operator<< (std::ostream& ostr, const String& arg)
{
	ostr.write(arg.c_str(), arg.length());
	return ostr;
}
//////////////////////////////////////////////////////////////////////////////
// static
String String::getLine(std::istream& is)
{
	StringBuffer rv(80);
	rv.getLine(is);
	return rv.releaseString();
}
#endif // BLOCXX_NO_IOSTREAMS
//////////////////////////////////////////////////////////////////////////////
// STATIC
const char*
String::strchr(const char* theStr, int c)
{
	const char* tmpChar = theStr;
	for (; *tmpChar && *tmpChar != c; tmpChar++)
	{
		// empty
	}
	return ((*tmpChar) == c ? tmpChar : 0);
}



} // end namespace BLOCXX_NAMESPACE

