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

#ifndef BLOCXX_EXCEPTION_HPP_INCLUDE_GUARD_
#define BLOCXX_EXCEPTION_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"

#ifdef BLOCXX_NO_EXCEPTIONS
#include <assert.h>

#define BLOCXX_THROW(e,f) assert(#f == 0)
#define BLOCXX_THROW_SUBEX(exType, msg, subex) assert(#msg == 0)
#define BLOCXX_THROW_ERR(exType, msg, err) assert(#msg == 0)
#define BLOCXX_THROW_ERRNO(exType) assert(#exType == 0)
#define BLOCXX_THROW_ERRNO1(exType, errnum) assert(errnum == 0)
#define BLOCXX_THROW_ERRNO_MSG(exType, msg) assert(#msg == 0)
#define BLOCXX_THROW_ERRNO_MSG1(exType, msg, errnum) assert(#msg == 0)
#define BLOCXX_THROW_ERR_SUBEX(exType, msg, err, subex) assert(#msg == 0)
#define BLOCXX_DECLARE_EXCEPTION2(NAME, BASE)
#define BLOCXX_DECLARE_APIEXCEPTION2(NAME, BASE, LINKAGE_SPEC)
#define BLOCXX_DECLARE_EXCEPTION(NAME)
#define BLOCXX_DECLARE_APIEXCEPTION(NAME, LINKAGE_SPEC)
#define BLOCXX_DEFINE_EXCEPTION2(NAME, BASE)
#define BLOCXX_DEFINE_EXCEPTION_WITH_BASE_AND_ID_AUX(NAME, BASE, SUB_CLASS_ID)
#define BLOCXX_DEFINE_EXCEPTION(NAME)
#define BLOCXX_DEFINE_EXCEPTION_WITH_ID(NAME)
#define BLOCXX_DEFINE_EXCEPTION_WITH_BASE_AND_ID(NAME, BASE)

#else

#include "miniblocxx/ExceptionIds.hpp"
#include "miniblocxx/AutoPtr.hpp"
#if defined(BLOCXX_NON_THREAD_SAFE_EXCEPTION_HANDLING)
#include "miniblocxx/CommonFwd.hpp" // for Mutex
#endif
#include <iosfwd>
#include <exception>
#include <new>

#include <cerrno>

namespace BLOCXX_NAMESPACE
{

/**
 * This class is the base of all exceptions thrown by BloCxx code.
 * It cannot be constructed directly, only subclasses can be instantiated.
 *
 * Creating a correct subclass is facilitated by the macros defined in
 * this file.
 *
 * Exceptions should be thrown by using one of the BLOCXX_THROW_* macros which
 * inserts the file and line number among other helpful things.
 */
class BLOCXX_COMMON_API Exception : public std::exception
{
protected:
	/**
	 * @param file The filename of the code throwing the exception.
	 * @param line The line of the code throwing the exception.
	 * @param msg A human readable message associated with the exception.
	 * @param errorCode A number identifying the specific error that caused
	 *   this exception.
	 * @param otherException A pointer to another exception.  This allows for
	 *   exception chaining, presumably otherException is the underlying cause
	 *   that the current exception is being thrown.  If otherException != 0,
	 *   The new Exception instance will store and delete the result of
	 *   calling otherException->clone(). otherException will not be saved.
	 * @param subClassId Each subclass of Exception may create an id.
	 *   getSubClassId() will return this value.  You may pass
	 *   UNKNOWN_SUBCLASS_ID if no id is required.  blocxx Ids are be registered
	 *   in BLOCXX_ExceptionIds.hpp.
	 */
	Exception(const char* file, int line, const char* msg, int errorCode, const Exception* otherException = 0, int subClassId = UNKNOWN_SUBCLASS_ID);

#ifdef BLOCXX_WIN32
	// Can't seem to catch exceptions by reference if copy CTOR is
	// protected on Windoz
public:
#endif
	Exception(const Exception& e);
	Exception& operator= (const Exception& rhs);
#ifdef BLOCXX_WIN32
protected:
#endif
	void swap(Exception& x);
	void setSubClassId(int subClassId);
	void setErrorCode(int errorCode);

public:
	virtual ~Exception() throw();

	static const int UNKNOWN_SUBCLASS_ID = -1;
	static const int UNKNOWN_ERROR_CODE = -1;

	/**
	 * Returns a string representing the concrete type.  e.g. "SocketException".  Will not return 0.
	 * This function will not throw.  Derived class implementations must not throw.
	 */
	virtual const char* type() const;
	/**
	 * Returns the message.  May return 0.
	 * This function will not throw.  Derived class implementations must not throw.
	 */
	virtual const char* getMessage() const;
	/**
	 * Returns the file.  May return 0.
	 */
	const char* getFile() const;
	int getLine() const;
	int getSubClassId() const;
	/**
	 * Returns the sub exception if available, otherwise 0.
	 */
	const Exception* getSubException() const;
	/**
	 * Returns the error code representing the error which occurred.
	 * Code are unique only in the scope of the derived exception class.
	 * May return UNKNONWN_ERROR_CODE if the error is unavailable.
	 */
	int getErrorCode() const;

	/**
	 * Returns getMessage()
	 */
	virtual const char* what() const throw();

	/**
	 * Make a copy of this exception object. If allocation fails, return 0.
	 * Subclasses need to override this function.  This function must not
	 * throw.  Here is an example of how to implement this in a derived class:
	 *
	 * virtual DerivedException* clone() const
	 * {
	 *     return new(std::nothrow) DerivedException(*this);
	 * }
	 *
	 */
	virtual Exception* clone() const;

	/**
	 * Re-throws the exception. All derived classes should override this which makes it
	 * possible to catch a base class Exception and still re-thrown the derived class.
	 */
	virtual void rethrow() const;

	/**
	 * Utility function to copy a string. This function will not throw--if allocation fails, 0 is returned.
	 * The result must be free[]'d.
	 * if str == 0, 0 is returned.
	 * @todo This function doesn't really belong on this class, but so far only this class and subclasses have a use for it.
	 */
	char* dupString(const char* str);

private:
	char* m_file;
	int m_line;
	char* m_msg;
	int m_subClassId;
	const Exception* m_subException;
	int m_errorCode;

#if defined(BLOCXX_NON_THREAD_SAFE_EXCEPTION_HANDLING)
	static Mutex* m_mutex;
#endif

};

// Log error messages for some exceptions we cannot handle.
void logErroneousException(const char* description);
void logErroneousException(const char* description, const std::exception& e);
void logErroneousException(const char* description, const Exception& e);

namespace ExceptionDetail
{

	// Reconciles GNU strerror_r and POSIX strerror_r.
	//
	BLOCXX_COMMON_API void portable_strerror_r(int errnum, char * buf, unsigned n);

	struct BLOCXX_COMMON_API FormatMsgImpl;

	class BLOCXX_COMMON_API FormatMsg
	{

#ifdef BLOCXX_WIN32
#pragma warning (push)
#pragma warning (disable: 4251)
#endif

		AutoPtr<FormatMsgImpl> pImpl;

#ifdef BLOCXX_WIN32
#pragma warning (pop)
#endif

	public:
		FormatMsg(char const * msg, int errnum);
		~FormatMsg();
		char const * get() const;
	private:
		FormatMsg(const FormatMsg&);
		FormatMsg& operator=(const FormatMsg&);
	};

	unsigned const BUFSZ = 1024;

	template <typename exType>
	struct Errno
	{
		static exType simple(char const * file, int line, int errnum)
		{
			char buf[BUFSZ];
			portable_strerror_r(errnum, buf, BUFSZ);
			return exType(file, line, buf, errnum);
		}

		template <typename Mtype>
		static exType format(char const * file, int line,
		                     Mtype const & msg, int errnum)
		{
			return format(file, line, msg.c_str(), errnum);
		}

		static exType format(char const * file, int line,
		                     char const * msg, int errnum)
		{
			FormatMsg fm(msg, errnum);
			return exType(file, line, fm.get(), errnum);
		}
	}; // struct Errno

} // namespace ExceptionDetail

/**
 * Writes the exception object to the stream in the form:
 *  \<file\>: \<line\> \<type\>: \<message\>
 */
BLOCXX_COMMON_API std::ostream& operator<< (std::ostream& os, const Exception& e);

/**
 * Throw an exception using __FILE__ and __LINE__.  If applicable,
 * BLOCXX_THROW_ERR should be used instead of this macro.
 *
 * @param exType The type of the exception
 * @param msg The exception message.  A string that will be copied.
 */
#define BLOCXX_THROW(exType, msg) throw exType(__FILE__, __LINE__, (msg))

/**
 * Throw an exception using __FILE__ and __LINE__.
 * @param exType The type of the exception
 * @param msg The exception message.  A string that will be copied.
 * @param subex A sub-exception. A pointer to it will be passed to the
 *   exception constructor, which should clone() it.
 */
#define BLOCXX_THROW_SUBEX(exType, msg, subex) \
throw exType(__FILE__, __LINE__, (msg), \
             ::BLOCXX_NAMESPACE::Exception::UNKNOWN_ERROR_CODE, &(subex))

/**
 * Throw an exception using __FILE__ and __LINE__.
 * @param exType The type of the exception
 * @param msg The exception message.  A string that will be copied.
 * @param err The error code.
 */
#define BLOCXX_THROW_ERR(exType, msg, err) \
throw exType(__FILE__, __LINE__, (msg), (err))

/**
 * Throw an exception using __FILE__, __LINE__, errno and strerror(errno)
 * @param exType The type of the exception; ctor must take file, line,
 *               message, and error code.
 */
#define BLOCXX_THROW_ERRNO(exType) BLOCXX_THROW_ERRNO1(exType, errno)

/**
 * Throw an exception using __FILE__, __LINE__, errnum and strerror(errnum)
 * @param exType The type of the exception; ctor must take file, line,
 *               message, and error code.
 * @param errnum The errno value.
 */
#define BLOCXX_THROW_ERRNO1(exType, errnum) \
throw ::BLOCXX_NAMESPACE::ExceptionDetail::Errno< exType >::simple(__FILE__, __LINE__, (errnum))

/**
 * Throw an exception using __FILE__, __LINE__, errno and strerror(errno)
 * @param exType The type of the exception; ctor must take file, line,
 *               message, and error code.
 * @param msg The exception message to use.
 */
#define BLOCXX_THROW_ERRNO_MSG(exType, msg) \
BLOCXX_THROW_ERRNO_MSG1(exType, (msg), errno)

/**
 * Throw an exception using __FILE__, __LINE__, errnum and strerror(errnum)
 * @param exType The type of the exception; ctor must take file, line,
 *               message, and error code.
 * @param msg The exception message to use.
 * @param errnum The errno value.
 */
#define BLOCXX_THROW_ERRNO_MSG1(exType, msg, errnum) \
throw ::BLOCXX_NAMESPACE::ExceptionDetail::Errno< exType >:: \
      format(__FILE__, __LINE__, (msg), (errnum))

/**
 * Throw an exception using __FILE__ and __LINE__.
 * @param exType The type of the exception
 * @param msg The exception message.  A string that will be copied.
 * @param err The error code.
 * @param subex A sub-exception. A point to it will be passed to the
 *   exception constructor, which should clone() it.
 */
#define BLOCXX_THROW_ERR_SUBEX(exType, msg, err, subex) \
throw exType(__FILE__, __LINE__, (msg), (err), &(subex))

/**
 * Declare a new exception class named \<NAME\>Exception that derives from \<BASE\>.
 * This macro is typically used in a header file.
 *
 * @param NAME The name of the new class (Exception will be postfixed)
 * @param BASE The base class.
 */
#define BLOCXX_DECLARE_EXCEPTION2(NAME, BASE) \
class NAME##Exception : public BASE \
{ \
public: \
	NAME##Exception(const char* file, int line, const char* msg, int errorCode = ::BLOCXX_NAMESPACE::Exception::UNKNOWN_ERROR_CODE, const ::BLOCXX_NAMESPACE::Exception* otherException = 0, int subClassId = ::BLOCXX_NAMESPACE::Exception::UNKNOWN_SUBCLASS_ID); \
	virtual ~NAME##Exception() throw(); \
	virtual const char* type() const; \
	virtual NAME##Exception* clone() const; \
	virtual void rethrow() const; \
};

/**
 * Declare a new exception class named \<NAME\>Exception that derives from \<BASE\>.
 * This macro is typically used in a header file. The exception class
 * declaration will be prefaced with the linkage_spec parm. This allows
 * the use of BLOCXX_COMMON_API when declaring exceptions. Example:
 * 		BLOCXX_DECLARE_APIEXCEPTION(Bogus, MYException, BLOCXX_COMMON_API)
 *
 * @param NAME The name of the new class (Exception will be postfixed)
 * @param BASE The base class.
 * @param LINKAGE_SPEC The linkage specifier for the exception class.
 */
#define BLOCXX_DECLARE_APIEXCEPTION2(NAME, BASE, LINKAGE_SPEC) \
class LINKAGE_SPEC NAME##Exception : public BASE \
{ \
public: \
	NAME##Exception(const char* file, int line, const char* msg, int errorCode = ::BLOCXX_NAMESPACE::Exception::UNKNOWN_ERROR_CODE, const ::BLOCXX_NAMESPACE::Exception* otherException = 0, int subClassId = ::BLOCXX_NAMESPACE::Exception::UNKNOWN_SUBCLASS_ID); \
	virtual ~NAME##Exception() throw(); \
	virtual const char* type() const; \
	virtual NAME##Exception* clone() const; \
	virtual void rethrow() const; \
};




/**
 * Declare a new exception class named \<NAME\>Exception that derives from Exception
 * This macro is typically used in a header file.
 *
 * @param NAME The name of the new class (Exception will be postfixed)
 */
#define BLOCXX_DECLARE_EXCEPTION(NAME) BLOCXX_DECLARE_EXCEPTION2(NAME, ::BLOCXX_NAMESPACE::Exception)

/**
 * Declare a new exception class named \<NAME\>Exception that derives from Exception
 * This macro is typically used in a header file.
 *
 * @param NAME The name of the new class (Exception will be postfixed)
 * @param LINKAGE_SPEC the linkage specifier. If the BLOCXX_DEFINE_EXCEPTION is part
 *		of libblocxx this would BLOCXX_COMMON_API...
 */
#define BLOCXX_DECLARE_APIEXCEPTION(NAME, LINKAGE_SPEC) BLOCXX_DECLARE_APIEXCEPTION2(NAME, ::BLOCXX_NAMESPACE::Exception, LINKAGE_SPEC)

/**
 * Define a new exception class named \<NAME\>Exception that derives from \<BASE\>.
 * The new class will use UNKNOWN_SUBCLASS_ID for the subclass id.
 * This macro is typically used in a cpp file.
 *
 * @param NAME The name of the new class (Exception will be postfixed)
 * @param BASE The base class.
 */
#define BLOCXX_DEFINE_EXCEPTION2(NAME, BASE) \
NAME##Exception::NAME##Exception(const char* file, int line, const char* msg, int errorCode, const ::BLOCXX_NAMESPACE::Exception* otherException, int subClassId) \
	: BASE(file, line, msg, errorCode, otherException, subClassId) {} \
NAME##Exception::~NAME##Exception() throw() { } \
NAME##Exception* NAME##Exception::clone() const { return new(std::nothrow) NAME##Exception(*this); } \
const char* NAME##Exception::type() const { return #NAME "Exception"; }\
void NAME##Exception::rethrow() const { throw *this; }

/**
 * Define a new exception class named \<NAME\>Exception that derives from \<BASE\>.
 * The new class will use SUB_CLASS_ID for the subclass id.
 * This macro is typically used in a cpp file.
 *
 * @param NAME The name of the new class (Exception will be postfixed)
 * @param BASE The base class.
 * @param SUB_CLASS_ID The subclass id.
 */
#define BLOCXX_DEFINE_EXCEPTION_WITH_BASE_AND_ID_AUX(NAME, BASE, SUB_CLASS_ID) \
NAME##Exception::NAME##Exception(const char* file, int line, const char* msg, int errorCode, const ::BLOCXX_NAMESPACE::Exception* otherException, int subClassId) \
	: BASE(file, line, msg, errorCode, otherException, subClassId == ::BLOCXX_NAMESPACE::Exception::UNKNOWN_SUBCLASS_ID ? (SUB_CLASS_ID) : subClassId) {} \
NAME##Exception::~NAME##Exception() throw() { } \
NAME##Exception* NAME##Exception::clone() const { return new(std::nothrow) NAME##Exception(*this); } \
const char* NAME##Exception::type() const { return #NAME "Exception"; } \
void NAME##Exception::rethrow() const { throw *this; }

/**
 * Define a new exception class named \<NAME\>Exception that derives from Exception.
 * The new class will use UNKNOWN_SUBCLASS_ID for the subclass id.
 * Use this macro for internal implementation exceptions that don't have an id.
 * This macro is typically used in a cpp file.
 *
 * @param NAME The name of the new class (Exception will be postfixed)
 */
#define BLOCXX_DEFINE_EXCEPTION(NAME) BLOCXX_DEFINE_EXCEPTION_WITH_BASE_AND_ID_AUX(NAME, ::BLOCXX_NAMESPACE::Exception, ::BLOCXX_NAMESPACE::Exception::UNKNOWN_SUBCLASS_ID)

/**
 * Define a new exception class named \<NAME\>Exception that derives from Exception.
 * The new class will use ExceptionIds::\<NAME\>ExceptionId for the subclass id.
 * Use this macro to create public exceptions that have an id in the BLOCXX_NAMESPACE::ExceptionIds namespace that derive from Exception
 * This macro is typically used in a cpp file.
 *
 * @param NAME The name of the new class (Exception will be postfixed)
 */
#define BLOCXX_DEFINE_EXCEPTION_WITH_ID(NAME) BLOCXX_DEFINE_EXCEPTION_WITH_BASE_AND_ID_AUX(NAME, ::BLOCXX_NAMESPACE::Exception, ::BLOCXX_NAMESPACE::ExceptionIds::NAME##ExceptionId)

/**
 * Define a new exception class named \<NAME\>Exception that derives from \<BASE\>.
 * The new class will use ExceptionIds::\<NAME\>ExceptionId for the subclass id.
 * Use this macro to create public exceptions that have an id in the BLOCXX_NAMESPACE::ExceptionIds namespace that will derive from BASE
 * This macro is typically used in a cpp file.
 *
 * @param NAME The name of the new class (Exception will be postfixed)
 * @param BASE The base class.
 */
#define BLOCXX_DEFINE_EXCEPTION_WITH_BASE_AND_ID(NAME, BASE) BLOCXX_DEFINE_EXCEPTION_WITH_BASE_AND_ID_AUX(NAME, BASE, ::BLOCXX_NAMESPACE::ExceptionIds::NAME##ExceptionId)

} // end namespace BLOCXX_NAMESPACE

#endif // BLOCXX_NO_EXCEPTIONS

#endif
