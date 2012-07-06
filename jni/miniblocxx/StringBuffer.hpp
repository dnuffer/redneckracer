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

#ifndef BLOCXX_STRING_BUFFER_HPP_INCLUDE_GUARD_
#define BLOCXX_STRING_BUFFER_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/String.hpp"
#include "miniblocxx/Char16.hpp"
#include "miniblocxx/Bool.hpp"
#include <string.h>

namespace BLOCXX_NAMESPACE
{

class BLOCXX_COMMON_API StringBuffer
{
public:
#if defined(BLOCXX_AIX)
	static const size_t BLOCXX_DEFAULT_ALLOCATION_UNIT;
#else
	static const size_t BLOCXX_DEFAULT_ALLOCATION_UNIT = 128;
#endif // BLOCXX_AIX
	StringBuffer(size_t allocSize = BLOCXX_DEFAULT_ALLOCATION_UNIT);
	StringBuffer(const char* arg);
	StringBuffer(const String& arg);
	StringBuffer(const StringBuffer& arg);
	~StringBuffer() { delete [] m_bfr; }
	StringBuffer& operator= (const StringBuffer& arg);
	StringBuffer& operator= (const String& arg);
	StringBuffer& operator= (const char* str);
	void swap(StringBuffer& x);
	StringBuffer& append(char c)
	{
		checkAvail();
		m_bfr[m_len++] = c;
		m_bfr[m_len] = '\0';
		return *this;
	}
	StringBuffer& append(const char* str)
	{
		size_t len = ::strlen(str);
		checkAvail(len+1);
		::strcpy(m_bfr+m_len, str);
		m_len += len;
		return *this;
	}
	StringBuffer& append(const char* str, const size_t len);
	StringBuffer& append(const String& arg)
		{ return append(arg.c_str(), arg.length()); }
	StringBuffer& append(const StringBuffer& arg)
	{
		return append(arg.c_str(), arg.length());
	}
	StringBuffer& operator += (char c)
		{ return append(c); }
	StringBuffer& operator += (Char16 c)
		{ return append(c.toString()); }
	StringBuffer& operator += (const char* str)
		{ return append(str); }
	StringBuffer& operator += (const String& arg)
		{ return append(arg); }
	StringBuffer& operator += (Bool v);
	StringBuffer& operator += (UInt8 v);
	StringBuffer& operator += (Int8 v);
	StringBuffer& operator += (UInt16 v);
	StringBuffer& operator += (Int16 v);
	StringBuffer& operator += (UInt32 v);
	StringBuffer& operator += (Int32 v);
	StringBuffer& operator += (UInt64 v);
	StringBuffer& operator += (Int64 v);
// do this check so we fill in the gaps and have int, long & long long constructors if necessary
#if defined(BLOCXX_INT32_IS_INT) && defined(BLOCXX_INT64_IS_LONG_LONG)
	StringBuffer& operator += (long v);
	StringBuffer& operator += (unsigned long v);
#endif
	StringBuffer& operator += (Real32 v);
	StringBuffer& operator += (Real64 v);
	StringBuffer& operator += (const StringBuffer& arg)
	{
		return append(arg);
	}
	char operator[] (size_t ndx) const;
	String toString() const
			{ return String(m_bfr); }
	// After calling this function, the StringBuffer is unusable
	String releaseString()
	{
		char * bfr = m_bfr;
		m_bfr = 0;
		return String(String::E_TAKE_OWNERSHIP, bfr, m_len);
	}
	size_t length() const {  return m_len; }

	/**
	 * Truncate the string at the given index. This doesn't change anything
	 * before the given index and does not free up any memory.
	 * @param index The index to truncate the string buffer at. If this value
	 * is greater than the current length of the StringBuffer, then nothing
	 * is done.
	 */
	void truncate(size_t index);

#ifndef BLOCXX_NO_IOSTREAMS
	/**
	 * Fill this StringBuffer object with the next line from the given
	 * input stream.
	 * @param is The input stream to retrieve the next line from
	 * @return const char* pointer to the line of text just read.
	 */
	const char* getLine(std::istream& is, bool resetBuffer=true);
#endif

	bool endsWith(char ch) const;
	bool startsWith(char ch) const;
	void chop();
	void trim();

	size_t allocated() const {  return m_allocated; }
	void reset();
	const char* c_str() const {  return m_bfr; }
	bool equals(const char* arg) const;
	bool equals(const StringBuffer& arg) const;
private:
	void checkAvail(size_t len=1)
	{
		size_t freeSpace = m_allocated - (m_len+1);

		if (len > freeSpace)
		{
			size_t toalloc = m_allocated * 2 + len;
			char* bfr = new char[toalloc];
			::memmove(bfr, m_bfr, m_len);
			delete [] m_bfr;
			m_allocated = toalloc;
			m_bfr = bfr;
		}
	}
	size_t m_len;
	size_t m_allocated;
	char* m_bfr;
};

BLOCXX_COMMON_API bool operator==(const StringBuffer& x, const StringBuffer& y);
BLOCXX_COMMON_API bool operator!=(const StringBuffer& x, const StringBuffer& y);
BLOCXX_COMMON_API bool operator==(const StringBuffer& x, const String& y);
BLOCXX_COMMON_API bool operator!=(const StringBuffer& x, const String& y);
BLOCXX_COMMON_API bool operator==(const String& x, const StringBuffer& y);
BLOCXX_COMMON_API bool operator!=(const String& x, const StringBuffer& y);

} // end namespace BLOCXX_NAMESPACE

#endif
