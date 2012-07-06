/*******************************************************************************
* Copyright (C) 2010, Quest Software, Inc. All rights reserved.
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

#ifndef BLOCXX_CHAR16_HPP_INCLUDE_GUARD_
#define BLOCXX_CHAR16_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/Types.hpp"
#include "miniblocxx/Bool.hpp"
#include "miniblocxx/CommonFwd.hpp"
#include "miniblocxx/SafeBool.hpp"

namespace BLOCXX_NAMESPACE
{

class String;
/**
 * The Char16 class is an abstraction for a double byte character.
 */
class BLOCXX_COMMON_API Char16
{
public:
	/**
	 * Create a new Char16 object with a value of zero.
	 */
	Char16() : m_value(0) {}
	/**
	 * Copy constructor
	 * @param arg The Char16 object this object will be a copy of.
	 */
	Char16(const Char16& arg) : m_value(arg.m_value) {}
	/**
	 * Create a new Char16 object from a single byte character.
	 * @param c The character this Char16 object will contain.
	 */
	explicit Char16(char c) : m_value(c) {}
	/**
	 * Create a new Char16 object from the 1st byte of a string.
	 * @param x The string to take the character from that will be contained by
	 *		this Char16 object.
	 */
	explicit Char16(const String& x);
	/**
	 * Create a new Char16 object of an unsigned 16 bit value.
	 * Note that this constructor is intentionally non-explicit,
	 * because implicit conversion from UInt16 is desireable for this class.
	 * @param val The unsigned 16 bit value this object will contain.
	 */
	Char16(UInt16 val) : m_value(val) {}
	/**
	 * Create a new Char16 object of an signed 16 bit value.
	 * @param val The signed 16 bit value this object will contain.
	 */
	explicit Char16(Int16 val) : m_value(val) {}
	/**
	 * Create a new Char16 object of an unsigned 8 bit value.
	 * @param val The unsigned 8 bit value this object will contain.
	 */
	explicit Char16(UInt8 val) : m_value(val) {}
	/**
	 * Create a new Char16 object of an signed 8 bit value.
	 * @param val The signed 8 bit value this object will contain.
	 */
	explicit Char16(Int8 val) : m_value(val) {}
	/**
	 * Create a new Char16 object of an unsigned 32 bit value.
	 * @param val The unsigned 32 bit value this object will contain.
	 */
	explicit Char16(UInt32 val) : m_value(UInt16(val)) {}
	/**
	 * Create a new Char16 object of an signed 32 bit value.
	 * @param val The signed 32 bit value this object will contain.
	 */
	explicit Char16(Int32 val) : m_value(UInt16(val)) {}
	/**
	 * Create a new Char16 object of an unsigned 64 bit value.
	 * @param val The unsigned 64 bit value this object will contain.
	 */
	explicit Char16(UInt64 val) : m_value(static_cast<UInt16>(val)) {}
	/**
	 * Create a new Char16 object of an signed 64 bit value.
	 * @param val The signed 64 bit value this object will contain.
	 */
	explicit Char16(Int64 val) : m_value(static_cast<UInt16>(val)) {}
	/**
	 * Create a new Char16 object from a real 32 value.
	 * @param val The real 32 value this object will contain.
	 */
	explicit Char16(Real32 val) : m_value(static_cast<UInt16>(val)) {}
	/**
	 * Create a new Char16 object from a real 64 value.
	 * @param val The real 64 value this object will contain.
	 */
	explicit Char16(Real64 val) : m_value(static_cast<UInt16>(val)) {}
	/**
	 * Create a new Char16 object from a boolean value.
	 * @param val The boolean value this object will contain.
	 */
	explicit Char16(Bool val) : m_value(val) {}
	/**
	 * @return The value of this object as a unsigned 16 bit value.
	 */
	UInt16 getValue() const { return m_value; }
	/**
	 * @return The value of this object as a unsigned 16 bit value.
	 */
	operator UInt16() const { return getValue(); }
	/**
	 * Assignment operator
	 * @param arg The Char16 to assign to this object.
	 * @return A reference to this object after the assignment has been made.
	 */
	Char16& operator= (const Char16& arg)
	{
		m_value = arg.m_value;
		return *this;
	}
	/**
	 * Equality operator
	 * @param arg The Char16 object to compare this object with.
	 * @return true if the given Char16 object is equal to this one.
	 */
	bool operator== (const Char16& arg) const
	{
		return m_value == arg.m_value;
	}
	/**
	 * Inequality operator
	 * @param arg The Char16 object to compare this object with.
	 * @return true if the given Char16 object is not equal to this one.
	 */
	bool operator!= (const Char16& arg) const
	{
		return m_value != arg.m_value;
	}
	/**
	 * Less than operator
	 * @param arg The Char16 object to compare this object with.
	 * @return true if this object is less than the given Char16 object.
	 */
	bool operator< (const Char16& arg) const
	{
		return m_value < arg.m_value;
	}
	/**
	 * Less than or equal operator
	 * @param arg The Char16 object to compare this object with.
	 * @return true if this object is less than or equal to the given Char16
	 * object.
	 */
	bool operator<= (const Char16& arg) const
	{
		return m_value <= arg.m_value;
	}
	/**
	 * Greater than operator
	 * @param arg The Char16 object to compare this object with.
	 * @return true if this object is greater than the given Char16 object.
	 */
	bool operator> (const Char16& arg) const
	{
		return m_value > arg.m_value;
	}
	/**
	 * Greater than or equal operator
	 * @param arg The Char16 object to compare this object with.
	 * @return true if this object is greater than or equal to the given
	 * Char16 object.
	 */
	bool operator>= (const Char16& arg) const
	{
		return m_value >= arg.m_value;
	}
	/**
	 * Add another Char16 object to this one.
	 * @param arg The Char16 object to add to this one.
	 * @return A reference to this object.
	 */
	Char16& operator+= (const Char16& arg)
	{
		m_value += arg.m_value;
		return *this;
	}
	/**
	 * Subtract another Char16 object from this one.
	 * @param arg The Char16 object to subtract from this one.
	 * @return A reference to this object.
	 */
	Char16& operator-= (const Char16& arg)
	{
		m_value -= arg.m_value;
		return *this;
	}
	/**
	 * Multiply this Char16 object by another.
	 * @param arg The Char16 object to multiply this one by.
	 * @return A reference to this object.
	 */
	Char16& operator*= (const Char16& arg)
	{
		m_value *= arg.m_value;
		return *this;
	}
	/**
	 * Divide this Char16 object by another.
	 * @param arg The Char16 object to divide this one by.
	 * @return A reference to this object.
	 */
	Char16& operator/= (const Char16& arg)
	{
		m_value /= arg.m_value;
		return *this;
	}

	BLOCXX_SAFE_BOOL_IMPL(Char16, UInt16, Char16::m_value, m_value)

	/**
	 * Convert this to UTF8
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
#endif //BLOCXX_NO_IOSTREAMS
private:
	UInt16 m_value;
};
BLOCXX_EXPORT_TEMPLATE(BLOCXX_COMMON_API, Array, Char16);

inline bool operator== (char c, const Char16& arg)
{
	return Char16(c) == arg;
}
inline bool operator== (const Char16& arg, int v)
{
	return (arg.getValue() == v);
}
inline bool operator== (int v, const Char16& arg)
{
	return (arg.getValue() == v);
}
inline bool operator!= (const Char16& arg, int v)
{
	return (arg.getValue() != v);
}
inline bool operator!= (int v, const Char16& arg)
{
	return (arg.getValue() != v);
}
inline bool operator!= (char c, const Char16& arg)
{
	return Char16(c) != arg;
}
inline bool operator< (char c, const Char16& arg)
{
	return Char16(c) < arg;
}
inline bool operator<= (char c, const Char16& arg)
{
	return Char16(c) <= arg;
}
inline bool operator> (char c, const Char16& arg)
{
	return Char16(c) > arg;
}
inline bool operator>= (char c, const Char16& arg)
{
	return Char16(c) >= arg;
}
inline Char16 operator+ (const Char16& arg1, const Char16& arg2)
{
	return Char16(UInt16(arg1.getValue() + arg2.getValue()));
}
inline Char16 operator- (const Char16& arg1, const Char16& arg2)
{
	return Char16(UInt16(arg1.getValue() - arg2.getValue()));
}
inline Char16 operator* (const Char16& arg1, const Char16& arg2)
{
	return Char16(UInt16(arg1.getValue() * arg2.getValue()));
}
inline Char16 operator/ (const Char16& arg1, const Char16& arg2)
{
	return Char16(UInt16(arg1.getValue() / arg2.getValue()));
}
#ifndef BLOCXX_NO_IOSTREAMS
BLOCXX_COMMON_API std::ostream& operator<< (std::ostream& ostrm, const Char16& arg);
#endif

} // end namespace BLOCXX_NAMESPACE

#endif
