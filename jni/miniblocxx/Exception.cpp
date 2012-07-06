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

#ifndef BLOCXX_NO_EXCEPTIONS
#include "miniblocxx/Exception.hpp"
#include "miniblocxx/Format.hpp"
#include <string.h>
// Not <cstring>, because strerror_r is not part of C or C++ standard lib,
// but is a POSIX function defined to be in <string.h>.
#include <cstdlib>
#if defined(BLOCXX_HAVE_ISTREAM) && defined(BLOCXX_HAVE_OSTREAM)
#include <istream>
#include <ostream>
#else
#include <iostream>
#endif
#include <algorithm> // for std::swap


namespace BLOCXX_NAMESPACE
{
	namespace
	{
		const char* const COMPONENT_NAME = "blocxx.Exception";
	}

	void logErroneousException(const char* description)
	{
		// FIXME!
		// trace("%s", Format("Unable to handle exception: %1", description).c_str());
	}
	void logErroneousException(const char* description, const std::exception& e)
	{
		// FIXME!
		// trace("%s", Format("Unable to handle std::exception: %1: %2", description, e.what()).c_str());
	}
	void logErroneousException(const char* description, const Exception& e)
	{
		// FIXME!
		// trace("%s", Format("Unable to handle exception: %1: %2", description, e).c_str());
	}

#if defined(BLOCXX_NON_THREAD_SAFE_EXCEPTION_HANDLING)
Mutex* Exception::m_mutex = new Mutex();
#endif
//////////////////////////////////////////////////////////////////////////////
static void freeBuf(char** ptr)
{
	delete [] *ptr;
	*ptr = NULL;
}
//////////////////////////////////////////////////////////////////////////////
char* Exception::dupString(const char* str)
{
	if (!str)
	{
		return 0;
	}
	char* rv = new (std::nothrow) char[strlen(str)+1];
	if (!rv)
	{
		return 0;
	}
	strcpy(rv, str);
	return rv;
}
//////////////////////////////////////////////////////////////////////////////
Exception::Exception(const char* file, int line, const char* msg, int errorCode, const Exception* subException, int subClassId)
	: std::exception()
	, m_file(dupString(file))
	, m_line(line)
	, m_msg(dupString(msg))
	, m_subClassId(subClassId)
	, m_subException(subException ? subException->clone() : 0)
	, m_errorCode(errorCode)
{
#if defined(BLOCXX_NON_THREAD_SAFE_EXCEPTION_HANDLING)
	m_mutex->acquire();
#endif
}
//////////////////////////////////////////////////////////////////////////////
Exception::Exception( const Exception& e )
    : std::exception(e)
    , m_file(dupString(e.m_file))
    , m_line(e.m_line)
    , m_msg(dupString(e.m_msg))
	, m_subClassId(e.m_subClassId)
    , m_subException(e.m_subException ? e.m_subException->clone() : 0)
	, m_errorCode(e.m_errorCode)
{
#if defined(BLOCXX_NON_THREAD_SAFE_EXCEPTION_HANDLING)
    m_mutex->acquire();
#endif
}
//////////////////////////////////////////////////////////////////////////////
Exception::~Exception() throw()
{
	try
	{
		delete m_subException;
		freeBuf(&m_file);
		freeBuf(&m_msg);
#if defined(BLOCXX_NON_THREAD_SAFE_EXCEPTION_HANDLING)
		m_mutex->release();
#endif
	}
	catch (...)
	{
		// don't let exceptions escape
	}
}
//////////////////////////////////////////////////////////////////////////////
Exception&
Exception::operator=(const Exception& rhs)
{
    Exception(rhs).swap(*this);
    return *this;
}
//////////////////////////////////////////////////////////////////////////////
void
Exception::swap(Exception& rhs)
{
	std::swap(static_cast<std::exception&>(*this), static_cast<std::exception&>(rhs));
	std::swap(m_file, rhs.m_file);
	std::swap(m_line, rhs.m_line);
	std::swap(m_msg, rhs.m_msg);
	std::swap(m_subClassId, rhs.m_subClassId);
	std::swap(m_subException, rhs.m_subException);
	std::swap(m_errorCode, rhs.m_errorCode);
}

//////////////////////////////////////////////////////////////////////////////
const char*
Exception::type() const
{
	return "Exception";
}

//////////////////////////////////////////////////////////////////////////////
int
Exception::getLine() const
{
	return m_line;
}

//////////////////////////////////////////////////////////////////////////////
const char*
Exception::getMessage() const
{
	return (m_msg != NULL) ? m_msg : "";
}
//////////////////////////////////////////////////////////////////////////////
const char*
Exception::getFile() const
{
	return (m_file != NULL) ? m_file : "";
}
//////////////////////////////////////////////////////////////////////////////
std::ostream&
operator<<(std::ostream& os, const Exception& e)
{
	if (*e.getFile() == '\0')
	{
		os << "[no file]: ";
	}
	else
	{
		os << e.getFile() << ": ";
	}

	if (e.getLine() == 0)
	{
		os << "[no line] ";
	}
	else
	{
		os << e.getLine() << ' ';
	}

	os << e.type() << ": ";

	if (e.getErrorCode() != Exception::UNKNOWN_ERROR_CODE)
	{
		os << e.getErrorCode() << ": ";
	}

	if (*e.getMessage() == '\0')
	{
		os << "[no message]";
	}
	else
	{
		os << e.getMessage();
	}

	const Exception* subEx = e.getSubException();
	if (subEx)
	{
		os << " <" << *subEx << '>';
	}
	return os;
}
//////////////////////////////////////////////////////////////////////////////
const char*
Exception::what() const throw()
{
	return getMessage();
}

//////////////////////////////////////////////////////////////////////////////
int
Exception::getSubClassId() const
{
	return m_subClassId;
}

//////////////////////////////////////////////////////////////////////////////
void
Exception::setSubClassId(int subClassId)
{
	m_subClassId = subClassId;
}

//////////////////////////////////////////////////////////////////////////////
Exception*
Exception::clone() const
{
	return new(std::nothrow) Exception(*this);
}

//////////////////////////////////////////////////////////////////////////////
void Exception::rethrow() const
{
	throw *this;
}

//////////////////////////////////////////////////////////////////////////////
const Exception*
Exception::getSubException() const
{
	return m_subException;
}

//////////////////////////////////////////////////////////////////////////////
int
Exception::getErrorCode() const
{
	return m_errorCode;
}

//////////////////////////////////////////////////////////////////////////////
void
Exception::setErrorCode(int errorCode)
{
	m_errorCode = errorCode;
}

namespace ExceptionDetail
{

// HPUX, solaris have a thread safe strerror(), windows doesn't have strerror_r(), and doesn't document whether strerror() is thread safe or not.
#if defined(BLOCXX_HPUX) || defined(BLOCXX_SOLARIS) || defined(BLOCXX_WIN32) || defined(BLOCXX_NCR)

	void portable_strerror_r(int errnum, char * buf, unsigned n)
	{
		::strncpy(buf, strerror(errnum), n);
		buf[n-1] = '\0'; // just in case...
	}

#else
	typedef int (*posix_fct)(int, char *, ::std::size_t);
	typedef char * (*gnu_fct)(int, char *, ::std::size_t);
	typedef int (*aix_fct)(int, char *, int);

	struct dummy
	{
	};

	// We make the strerror_r_wrap functions into templates so that
	// code is generated only for the one that gets used.

	template <typename Dummy>
	inline int
	strerror_r_wrap(posix_fct strerror_r, int errnum, char * buf, unsigned n,
	                Dummy)
	{
		return strerror_r(errnum, buf, n);
	}

	template <typename Dummy>
	inline int
	strerror_r_wrap(aix_fct strerror_r, int errnum, char * buf, unsigned n,
	                Dummy)
	{
		return strerror_r(errnum, buf, n);
	}

	template <typename Dummy>
	inline int
	strerror_r_wrap(gnu_fct strerror_r, int errnum, char * buf, unsigned n,
	                Dummy)
	{
		char * errstr = strerror_r(errnum, buf, n);
		if (errstr != buf)
		{
			if (errstr)
			{
				::strncpy(buf, errstr, n);
			}
			else
			{
				return -1;
			}
		}
		return 0;
	}

	void portable_strerror_r(int errnum, char * buf, unsigned n)
	{
		int errc = strerror_r_wrap(&::strerror_r, errnum, buf, n, dummy());
		if (errc != 0)
		{
			::strncpy(buf, "[Could not create error message for error code]", n);
		}
		buf[n-1] = '\0'; // just in case...
	}
#endif

	struct BLOCXX_COMMON_API FormatMsgImpl
	{
		String fm;
	};

	FormatMsg::FormatMsg(char const * msg, int errnum)
		: pImpl(new FormatMsgImpl)
	{
		char arr[BUFSZ];
		portable_strerror_r(errnum, arr, BUFSZ);
		char const * sarr = static_cast<char const *>(arr);
		pImpl->fm = Format("%1: %2(%3)", msg, errnum, sarr).toString();
	}

	FormatMsg::~FormatMsg()
	{
	}

	char const * FormatMsg::get() const
	{
		return pImpl->fm.c_str();
	}

} // namespace ExceptionDetail

} // end namespace BLOCXX_NAMESPACE

#endif // BLOCXX_NO_EXCEPTIONS
