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

#ifndef BLOCXX_STRING_STREAM_HPP_INCLUDE_GUARD_
#define BLOCXX_STRING_STREAM_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"

#ifndef BLOCXX_NO_IOSTREAMS

#include "miniblocxx/StringBuffer.hpp"
#include "miniblocxx/BaseStreamBuffer.hpp"
#if defined(BLOCXX_HAVE_OSTREAM) && defined(BLOCXX_HAVE_ISTREAM)
#include <istream>
#include <ostream>
#else
#include <iostream>
#endif
#ifdef BLOCXX_HAVE_STREAMBUF
#include <streambuf>
#else
#include <streambuf.h>
#endif
#include <cstdio>

namespace BLOCXX_NAMESPACE
{

///////////////////////////////////////////////////////////////////////////////
class IStringStreamBuf : public std::streambuf
{
public:
	IStringStreamBuf(const String& s)
		: std::streambuf()
		, m_buf(s)
	{
		setg(const_cast<char*>(reinterpret_cast<const char*>(m_buf.c_str())),
			const_cast<char*>(reinterpret_cast<const char*>(m_buf.c_str())),
			const_cast<char*>(reinterpret_cast<const char*>(m_buf.c_str()+m_buf.length())));
	}
	virtual ~IStringStreamBuf();
protected:
	int underflow()
	{
		return (gptr() < egptr()) ? static_cast<unsigned char>(*gptr()) : EOF; 	// need a static_cast so a -1 doesn't turn into an EOF
	}
private:
	String m_buf;
};
///////////////////////////////////////////////////////////////////////////////
class IStringStreamBase
{
public:
	IStringStreamBase(const String& s) : m_buf(s) {}
	mutable IStringStreamBuf m_buf;
};

#ifdef BLOCXX_WIN32
#pragma warning(push)
#pragma warning(disable:4275)
#endif

///////////////////////////////////////////////////////////////////////////////
class BLOCXX_COMMON_API IStringStream : private IStringStreamBase, public std::istream
{
public:
	IStringStream(const String& s);
	~IStringStream();
private:
	IStringStream(const IStringStream&) BLOCXX_FUNCTION_NOT_IMPLEMENTED;
	IStringStream& operator=(const IStringStream&) BLOCXX_FUNCTION_NOT_IMPLEMENTED;
};

#ifdef BLOCXX_WIN32
#pragma warning(pop)
#endif


///////////////////////////////////////////////////////////////////////////////
class BLOCXX_COMMON_API OStringStreamBuf : public BaseStreamBuffer
{
public:
	OStringStreamBuf(size_t size);
	virtual ~OStringStreamBuf();
	String toString() const;
	// After calling releaseString(), this OStringStream is unusable
	String releaseString();
	size_t length() const;
	const char* c_str() const;
	void reset();
protected:
	virtual int buffer_to_device(const char *c, int n);
private:
	StringBuffer m_buf;
	friend class OStringStream;
};
///////////////////////////////////////////////////////////////////////////////
class BLOCXX_COMMON_API OStringStreamBase
{
public:
	OStringStreamBase(size_t sz);
	mutable OStringStreamBuf m_buf;
};
///////////////////////////////////////////////////////////////////////////////
class BLOCXX_COMMON_API OStringStream : private OStringStreamBase, public std::ostream
{
public:
	OStringStream(size_t size = 256);
	~OStringStream();
	OStringStream(const OStringStream&);
	OStringStream& operator=(const OStringStream&);
	String toString() const;
	// After calling releaseString(), this OStringStream is unusable
	String releaseString();
	size_t length() const;
	const char* c_str() const;
	void reset();
};

} // end namespace BLOCXX_NAMESPACE

#endif // BLOCXX_NO_IOSTREAMS
#endif
