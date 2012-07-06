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
 * @author Jon Carey
 * @author Dan Nuffer
 */

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/StringBuffer.hpp"
#include "miniblocxx/Char16.hpp"

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "miniblocxx/stl/algorithm" // for std::swap

#ifndef BLCOXXX_NO_STREAMS
#include <iostream>
#endif

namespace BLOCXX_NAMESPACE
{

#if defined(BLOCXX_AIX)
const size_t StringBuffer::BLOCXX_DEFAULT_ALLOCATION_UNIT = 128;
#endif // BLOCXX_AIX
//////////////////////////////////////////////////////////////////////////////
StringBuffer::StringBuffer(size_t allocSize) :
	m_len(0),
	m_allocated(allocSize > 0 ? allocSize : BLOCXX_DEFAULT_ALLOCATION_UNIT),
	m_bfr(new char[m_allocated])
{
	m_bfr[0] = 0;
}
//////////////////////////////////////////////////////////////////////////////
StringBuffer::StringBuffer(const char* arg) :
	m_len(strlen(arg)),
	m_allocated(m_len + BLOCXX_DEFAULT_ALLOCATION_UNIT),
	m_bfr(new char[m_allocated])
{
	::strcpy(m_bfr, arg);
}
//////////////////////////////////////////////////////////////////////////////
StringBuffer::StringBuffer(const String& arg) :
	m_len(arg.length()),
	m_allocated(m_len + BLOCXX_DEFAULT_ALLOCATION_UNIT),
	m_bfr(new char[m_allocated])
{
	::strcpy(m_bfr, arg.c_str());
}
//////////////////////////////////////////////////////////////////////////////
StringBuffer::StringBuffer(const StringBuffer& arg) :
	m_len(arg.m_len), m_allocated(arg.m_allocated),
	m_bfr(new char[arg.m_allocated])
{
	::memmove(m_bfr, arg.m_bfr, arg.m_len + 1);
}
//////////////////////////////////////////////////////////////////////////////
StringBuffer&
StringBuffer::operator= (const String& arg)
{
	StringBuffer(arg).swap(*this);
	return *this;
}
//////////////////////////////////////////////////////////////////////////////
StringBuffer&
StringBuffer::operator= (const char* str)
{
	StringBuffer(str).swap(*this);
	return *this;
}
//////////////////////////////////////////////////////////////////////////////
StringBuffer&
StringBuffer::operator =(const StringBuffer& arg)
{
	StringBuffer(arg).swap(*this);
	return *this;
}
//////////////////////////////////////////////////////////////////////////////
void
StringBuffer::swap(StringBuffer& x)
{
	std::swap(m_len, x.m_len);
	std::swap(m_allocated, x.m_allocated);
	std::swap(m_bfr, x.m_bfr);
}
//////////////////////////////////////////////////////////////////////////////
void
StringBuffer::reset()
{
	m_len = 0;
	m_bfr[0] = '\0';
}

//////////////////////////////////////////////////////////////////////////////
void
StringBuffer::truncate(size_t index)
{
	if (index < m_len)
	{
		m_bfr[index] = '\0';
		m_len = index;
	}
}

//////////////////////////////////////////////////////////////////////////////
char
StringBuffer::operator[] (size_t ndx) const
{
	return (ndx > m_len) ? 0 : m_bfr[ndx];
}
//////////////////////////////////////////////////////////////////////////////
// This operator must write "TRUE"/"FALSE" to StringBuffer
StringBuffer&
StringBuffer::operator += (Bool v)
{
	return append(v.toString());
}
#if defined(BLOCXX_WIN32)
#define snprintf _snprintf // stupid windoze...
#endif
//////////////////////////////////////////////////////////////////////////////
StringBuffer&
StringBuffer::operator += (UInt8 v)
{
	char bfr[6];
	::snprintf(bfr, sizeof(bfr), "%u", UInt32(v));
	return append(bfr);
}
//////////////////////////////////////////////////////////////////////////////
StringBuffer&
StringBuffer::operator += (Int8 v)
{
	char bfr[6];
	::snprintf(bfr, sizeof(bfr), "%d", Int32(v));
	return append(bfr);
}
//////////////////////////////////////////////////////////////////////////////
StringBuffer&
StringBuffer::operator += (UInt16 v)
{
	char bfr[16];
	::snprintf(bfr, sizeof(bfr), "%u", UInt32(v));
	return append(bfr);
}
//////////////////////////////////////////////////////////////////////////////
StringBuffer&
StringBuffer::operator += (Int16 v)
{
	char bfr[16];
	::snprintf(bfr, sizeof(bfr), "%d", Int32(v));
	return append(bfr);
}
//////////////////////////////////////////////////////////////////////////////
StringBuffer&
StringBuffer::operator += (UInt32 v)
{
	char bfr[16];
	::snprintf(bfr, sizeof(bfr), "%u", v);
	return append(bfr);
}
//////////////////////////////////////////////////////////////////////////////
StringBuffer&
StringBuffer::operator += (Int32 v)
{
	char bfr[16];
	::snprintf(bfr, sizeof(bfr), "%d", v);
	return append(bfr);
}
#if defined(BLOCXX_INT32_IS_INT) && defined(BLOCXX_INT64_IS_LONG_LONG)
//////////////////////////////////////////////////////////////////////////////
StringBuffer&
StringBuffer::operator += (unsigned long v)
{
	char bfr[28];
	::snprintf(bfr, sizeof(bfr), "%lu", v);
	return append(bfr);
}
//////////////////////////////////////////////////////////////////////////////
StringBuffer&
StringBuffer::operator += (long v)
{
	char bfr[28];
	::snprintf(bfr, sizeof(bfr), "%ld", v);
	return append(bfr);
}
#endif
//////////////////////////////////////////////////////////////////////////////
StringBuffer&
StringBuffer::operator += (UInt64 v)
{
	char bfr[28];
#if BLOCXX_SIZEOF_LONG_INT == 8
	::snprintf(bfr, sizeof(bfr), "%lu", v);
#else
	::snprintf(bfr, sizeof(bfr), "%llu", v);
#endif
	return append(bfr);
}
//////////////////////////////////////////////////////////////////////////////
StringBuffer&
StringBuffer::operator += (Int64 v)
{
	char bfr[28];
#if BLOCXX_SIZEOF_LONG_INT == 8
	::snprintf(bfr, sizeof(bfr), "%ld", v);
#else
	::snprintf(bfr, sizeof(bfr), "%lld", v);
#endif
	return append(bfr);
}
//////////////////////////////////////////////////////////////////////////////
// decimal digits = ceiling((bits)*ln(2)/ln(10))
StringBuffer&
StringBuffer::operator += (Real32 v)
{
	return append(String(v));
}
//////////////////////////////////////////////////////////////////////////////
StringBuffer&
StringBuffer::operator += (Real64 v)
{
	return append(String(v));
}
#if defined(BLOCXX_WIN32)
#undef snprintf
#endif
//////////////////////////////////////////////////////////////////////////////
StringBuffer&
StringBuffer::append(const char* str, const size_t len)
{
	checkAvail(len+1);
	::strncpy(m_bfr+m_len, str, len);
	m_len += len;
	m_bfr[m_len] = '\0';
	return *this;
}
//////////////////////////////////////////////////////////////////////////////
bool
StringBuffer::equals(const char* arg) const
{
	return ::strcmp(arg, m_bfr) == 0;
}

//////////////////////////////////////////////////////////////////////////////
bool
StringBuffer::equals(const StringBuffer& arg) const
{
	return ::strcmp(arg.m_bfr, m_bfr) == 0;
}

//////////////////////////////////////////////////////////////////////////////
bool
StringBuffer::endsWith(char ch) const
{
	return (m_len && m_bfr[m_len-1] == ch);
}

//////////////////////////////////////////////////////////////////////////////
bool
StringBuffer::startsWith(char ch) const
{
	return (m_len && m_bfr[0] == ch);
}

//////////////////////////////////////////////////////////////////////////////
void
StringBuffer::chop()
{
	if (m_len)
	{
		truncate(m_len-1);
	}
}

//////////////////////////////////////////////////////////////////////////////
void
StringBuffer::trim()
{
	if (m_len)
	{
		while (m_len && isspace(m_bfr[m_len-1]))
		{
			m_bfr[--m_len] = 0;
		}

		if (m_len)
		{
			char *p = m_bfr;
			while (*p && isspace(*p))
			{
				++p;
			}

			if (*p && p > m_bfr)
			{
				m_len -= (p - m_bfr);
				memmove(m_bfr, p, m_len+1);
			}
		}
	}
}

#ifndef BLOCXX_NO_IOSTREAMS
//////////////////////////////////////////////////////////////////////////////
// Get one line from an input stream. This StringBuffer object will be
// reset (cleared) before an attempt is made to retrieve the line.
const char*
StringBuffer::getLine(std::istream& is, bool resetBuffer)
{
	if (resetBuffer)
	{
		reset();
	}

	if (is)
	{
		size_t count = 0;
		std::streambuf *sb = is.rdbuf();

		while (1)
		{
			int ch = sb->sbumpc();
			if (ch == EOF)
			{
				is.setstate(count == 0
					? (std::ios::failbit | std::ios::eofbit) : std::ios::eofbit);
				break;
			}

			++count;

			if (ch == '\n')
			{
				break;
			}

			append(static_cast<char>(ch));
		}
	}

	const char* p = ::strchr(m_bfr, '\r');
	if (p)
	{
		truncate(size_t(p-m_bfr));
	}

	return m_bfr;
}

//////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& ostr, const StringBuffer& b)
{
	ostr.write(b.c_str(), b.length());
	return ostr;
}
#endif // BLCOXXX_SUPPORT_STREAMS

//////////////////////////////////////////////////////////////////////////////
bool operator==(const StringBuffer& x, const StringBuffer& y)
{
	return x.equals(y);
}

//////////////////////////////////////////////////////////////////////////////
bool operator!=(const StringBuffer& x, const StringBuffer& y)
{
	return !(x == y);
}

//////////////////////////////////////////////////////////////////////////////
bool operator==(const StringBuffer& x, const String& y)
{
	return x.equals(y.c_str());
}

//////////////////////////////////////////////////////////////////////////////
bool operator!=(const StringBuffer& x, const String& y)
{
	return !(x == y);
}

//////////////////////////////////////////////////////////////////////////////
bool operator==(const String& x, const StringBuffer& y)
{
	return x.equals(y.c_str());
}

//////////////////////////////////////////////////////////////////////////////
bool operator!=(const String& x, const StringBuffer& y)
{
	return !(x == y);
}

} // end namespace BLOCXX_NAMESPACE

