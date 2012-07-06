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

#ifndef BLOCXX_BOOL_HPP_INCLUDE_GUARD_
#define BLOCXX_BOOL_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/CommonFwd.hpp"

#ifndef BLOCXX_NO_IOSTREAMS
#include <iosfwd>
#endif

namespace BLOCXX_NAMESPACE
{

/**
 * The Bool class is an abstraction for the boolean data type.  It's not
 * meant to be a replacement for bool.  Use it if you need a type that
 * supports toString(), readObject(), or writeObject().  It's also useful for
 * preventing implicit conversion of char*->bool.  Using it can make overloading
 * functions safer (ie it'll be less likely the compiler will pick the wrong
 * overload.)
 */
class BLOCXX_COMMON_API Bool
{
public:

	/**
	 * Create an Bool object initialized to false.
	 */
	Bool() : m_val(false) {}
	/**
	 * Create an Bool object initialized to a given boolean value.
	 * @param val The boolean value to initialize this Bool to
	 */
	Bool(bool val) : m_val(val) {}
	/**
	 * Copy constructor
	 * @param arg The Bool to make this object a copy of.
	 */
	Bool(const Bool& arg) : m_val(arg.m_val) {}
	/**
	 * Assignment operator
	 * @param arg The Bool to assign to this one.
	 * @return A reference to this object after the assignment has been made.
	 */
	Bool& operator= (const Bool& arg) { m_val = arg.m_val; return *this; }
	/**
	 * Equality operator
	 * @param arg bool value to compare this Bool object against.
	 * @return true If the given bool value is equal to this one. Otherwise
	 * false
	 */
	bool operator== (const bool arg) const { return m_val == arg; }
	/**
	 * Equality operator
	 * @param arg The Bool object to compare this one with.
	 * @return true If the given Bool object is equal to this one. Otherwise
	 * false
	 */
	bool operator== (const Bool& arg) const { return m_val == arg.m_val; }
	/**
	 * Inequality operator
	 * @param arg bool value to compare this Bool object against.
	 * @return true If the given bool value is not equal to this one. Otherwise
	 * false
	 */
	bool operator!= (const bool arg) const { return m_val != arg; }
	/**
	 * Inequality operator
	 * @param arg The Bool object to compare this one with.
	 * @return true If the given Bool object is not equal to this one.
	 * Otherwise false
	 */
	bool operator!= (const Bool& arg) const { return m_val != arg.m_val; }
	/**
	 * @return The bool value of this Bool object.
	 */
	operator bool() const { return m_val; }
	/**
	 * Negation operator
	 * @return true if this Bool operator has a value of false.
	 */
	bool operator !() const { return !m_val; }
	/**
	 * @return The string representation of this object. If this object contains
	 * a true value, the "true" is returned. Otherwise "false" is returned.
	 */
	String toString() const;
#ifndef BLOCXX_NO_IOSTREAMS
	/**
	 * Write this object to an output stream.
	 * @param ostrm The output stream to write this object to.
	 */
	void writeObject(std::streambuf & ostrm) const;
	/**
	 * Read this object from an input stream.
	 * @param istrm The input stream to read this object from.
	 */
	void readObject(std::streambuf & istrm);
#endif
	/**
	 * Less than operator (friend function)
	 * @param b1 The first Bool object to use in the comparison.
	 * @param b2 The second Bool object to use in the comparison.
	 * @return true if b1 is less than b2. Otherwise false.
	 */
	friend bool operator< (const Bool& b1, const Bool& b2)
	{
		return b1.m_val < b2.m_val;
	}
	/**
	 * Not equal operator (friend function)
	 * @param b1 The first Bool object to use in the comparison.
	 * @param b2 The second Bool object to use in the comparison.
	 * @return true if b1 is not equal to b2. Otherwise false.
	 */
	friend bool operator!=(bool b1, Bool b2)
	{
		return b1 != b2.m_val;
	}
private:
	// These are private/unimplemented to help prevent unintended errors of
	// passing a pointer to the constructor.
	Bool(const void*);
	Bool(void*);
	Bool(volatile const void*);
	Bool(volatile void*);
	bool m_val;
};
BLOCXX_EXPORT_TEMPLATE(BLOCXX_COMMON_API, Array, Bool);

#ifndef BLOCXX_NO_IOSTREAMS
/**
 * Insert the string representation of a Bool object into a given stream.
 * @param ostrm The stream the Bool object will be inserted into.
 * @param arg A reference to a bool object to insert into the given stream.
 * @return A reference to the given output stream (ostrm)
 */
BLOCXX_COMMON_API std::ostream& operator << (std::ostream& ostrm, const Bool& arg);
#endif // no iostreams

} // end namespace BLOCXX_NAMESPACE

#endif
