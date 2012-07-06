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

#ifndef BLOCXX_STRING_HPP_INCLUDE_GUARD_
#define BLOCXX_STRING_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/Types.hpp"
#include "miniblocxx/COWIntrusiveReference.hpp"
#include "miniblocxx/CommonFwd.hpp"
#include "miniblocxx/Exception.hpp"
#include <string>

namespace BLOCXX_NAMESPACE
{

BLOCXX_DECLARE_APIEXCEPTION(StringConversion, BLOCXX_COMMON_API);

/**
 * This String class is an abstract data type that represents as NULL
 * terminated string of characters. String objects are ref counted and
 * copy on write.
 * Internal representation is UTF-8.  Conversion to/from Char16 (UCS-2) is
 * performed correctly.
 * All operations that return/take an offset are based on the character array
 * as if this were an ASCII string.
 * The class does not enforce or maintain the integrity of any UTF-8 chars.
 * length() returns the size in bytes of the string.  Use UTF8Length() to
 * get the number of chars in the string.
 */
class BLOCXX_COMMON_API String
{
public:
	class ByteBuf;
	/**
	 * Create a new String object with a length of 0.
	 */
	String();
	/**
	 * Create a new String object that will hold the string representation
	 * of the given Int32 value.
	 * @param val	The 32 bit value this String will hold the string
	 * representation of.
	 */
	explicit String(Int32 val);
	/**
	 * Create a new String object that will hold the string representation
	 * of the given UInt32 value.
	 * @param val	The 32 bit value this String will hold the string
	 * representation of.
	 */
	explicit String(UInt32 val);
// do this check so we fill in the gaps and have int, long & long long constructors if necessary
#if defined(BLOCXX_INT32_IS_INT) && defined(BLOCXX_INT64_IS_LONG_LONG)
	/**
	 * Create a new String object that will hold the string representation
	 * of the given long value.
	 * @param val	The long value this String will hold the string
	 * representation of.
	 */
	explicit String(long val);
	/**
	 * Create a new String object that will hold the string representation
	 * of the given unsigned long value.
	 * @param val	The unsigned long value this String will hold the string
	 * representation of.
	 */
	explicit String(unsigned long val);
#endif
	/**
	 * Create a new String object that will hold the string representation
	 * of the given Int64 value.
	 * @param val	The 64 bit value this String will hold the string
	 * representation of.
	 */
	explicit String(Int64 val);
	/**
	 * Create a new String object that will hold the string representation
	 * of the given UInt64 value.
	 * @param val	The 64 bit value this String will hold the string
	 * representation of.
	 */
	explicit String(UInt64 val);
	/**
	 * Create a new String object that will hold the string representation
	 * of the given Real32 value.
	 * @param val	The real value this String will hold the string
	 * representation of.
	 */
	explicit String(Real32 val);
	/**
	 * Create a new String object that will hold the string representation
	 * of the given Real64 value.
	 * @param val	The real value this String will hold the string
	 * representation of.
	 */
	explicit String(Real64 val);
	/**
	 * Create a new String object that will contain a copy of the given
	 * character string.
	 * @param str	The null terminated string to make a copy of.
	 */
	String(const char* str);

	enum ETakeOwnershipFlag
	{
		E_TAKE_OWNERSHIP
	};
	/**
	 * Create a new string object from a dynamically allocated buffer.
	 * The buffer is assumed to contain a valid c string and be
	 * previously allocated with the new operator. The memory given by the
	 * allocatedMemory parameter will be deallocated by the String class.
	 * @param takeOwnerShipTag This parm is not used. It is here to differentiate
	 *  this constructor from the others.
	 * @param allocatedMemory The dynamically allocated string that will be
	 *  used by this String.  Must not be NULL.
	 * @param len The length of the string allocatedMemory.
	 */
	explicit String(ETakeOwnershipFlag, char* allocatedMemory, size_t len);
	/**
	 * Create s new String object that will contain a copy of the given
	 * character string for the given length.
	 * @param str The character array to copy from. It does not need
	 *            to be null terminated.
	 * @param len The number of bytes to copy from the str param. The
	 *            count should not include a terminating null.
	 */
	explicit String(const char* str, size_t len);
	/**
	 * Create a new String object that is a copy of another String object.
	 * Upon return, both String objects will point to the same underlying
	 * character buffer. This state will remain until one of the 2 String
	 * objects is modified (copy on write)
	 * @param arg	The String object to make a copy of.
	 */
	String(const String& arg);
	/**
	 * Create a new String object that contains a single character.
	 * @param c		The character that this string will contain.
	 */
	explicit String(char c);
	/**
	 * Create a copy of str
	 */
	String(const std::string& str);
	/**
	 * Destroy this String object.
	 */
	~String();
	/**
	 * Swap this instance with another
	 */
	void swap(String& x);
	/**
	 * Create a null terminated string that contains the contents of this
	 * String. The returned pointer MUST be deleted by the caller using free().
	 * @return A pointer to the newly allocated buffer that contains the
	 *		contents of this String object.
	 */
	char* allocateCString() const;
	/**
	 * convert to std::string
	 */
	operator std::string() const;
	/**
	 * @return The number of bytes in this String.
	 */
	size_t length() const;
	/**
	 * @return The number of bytes in this String.
	 */
	size_t size() const { return length(); }
	/**
	 * If the string is not valid UTF-8, then the result of this function
	 * is undefined.
	 * @return The number of UTF-8 chars in this String.
	 */
	size_t UTF8Length() const;
	/**
	 * @return True if empty string, false otherwise
	 */
	bool empty() const { return length() == 0; }
	/**
	 * Format this string according to the given format and variable
	 * argument list (printf style)
	 * @param fmt	The format to use during the operation
	 * @param ...	A variable argument list to be applied to the format
	 * @return The length of the string after the operation on success.
	 * npos on error.
	 */
	int format(const char* fmt, ...);
	enum EReturnDelimitersFlag
	{
		E_DISCARD_DELIMITERS,
		E_RETURN_DELIMITERS
	};
	enum EEmptyTokenReturnFlag
	{
		E_SKIP_EMPTY_TOKENS,
		E_RETURN_EMPTY_TOKENS
	};
	/**
	 * Tokenize this String object using the given delimeters.
	 * @param delims	A pointer to a char array of delimeters that separate
	 * 	the tokens in this String object.
	 * @param returnDelimitersAsTokens If this flag is E_RETURN_DELIMITERS, then the
	 * 	delimiter characters are also returned as tokens, otherwise the
	 * 	delimiters are removed.
	 * @param returnEmptyTokens If this flag is E_RETURN_EMPTY_TOKENS, then
	 * 	two sequential delimiters will result in an empty token being
	 * 	returned.
	 * @return An StringArray that contains the tokens from this String
	 * object. If there are no tokens the StringArray will be empty.
	 */
	StringArray tokenize(const char* delims = " \n\r\t\v",
		EReturnDelimitersFlag returnDelimitersAsTokens = E_DISCARD_DELIMITERS,
		EEmptyTokenReturnFlag returnEmptyTokens = E_SKIP_EMPTY_TOKENS ) const;
	/**
	 * @return The c string representation of this String object. This
	 * will be a null terminated character array.
	 */
	const char* c_str() const;
	/**
	 * Get the character at a specified index.
	 * @param ndx	The index of the character to return within the char array.
	 * @return The character at the given index within the array. If the index
	 * is out of bounds, then a 0 is returned.
	 */
	char charAt(size_t ndx) const;
	/**
	 * Compare another String object with this one.
	 * @param arg	The String object to compare with this one.
	 * @return 0 if this String object is equal to arg. Greater than 0 if
	 * this String object is greater than arg. Less than 0 if this String
	 * object is less than arg.
	 */
	int compareTo(const String& arg) const;
	/**
	 * Compare another string with this one.
	 * @param arg	The string to compare with this one.
	 * @return 0 if this String object is equal to arg. Greater than 0 if
	 * this String object is greater than arg. Less than 0 if this String
	 * object is less than arg.
	 */
	int compareTo(const char* arg) const;
	/**
	 * Compare another string with this one ignoring case.
	 * @param arg	The string to compare with this one.
	 * @return 0 if this String object is equal to arg. Greater than 0 if
	 * this String object is greater than arg. Less than 0 if this String
	 * object is less than arg.
	 */
	int compareToIgnoreCase(const String& arg) const;
	/**
	 * Compare another string with this one ignoring case.
	 * @param arg	The string to compare with this one.
	 * @return 0 if this String object is equal to arg. Greater than 0 if
	 * this String object is greater than arg. Less than 0 if this String
	 * object is less than arg.
	 */
	int compareToIgnoreCase(const char* arg) const;
	/**
	 * Append a C string to this String object.
	 * @param arg	The C string to append to this String object.
	 * @return A reference to this String object.
	 */
	String& concat(const char* arg);

	/**
	 * Append another String object to this String object.
	 * @param arg	The String object to append to this String object.
	 * @return A reference to this String object.
	 */
	String& concat(const String& arg)
	{
		return concat(arg.c_str());
	}

	/**
	 * Append a char to this String object.
	 * @param arg	The char to append to this String object.
	 * @return A reference to this String object.
	 */
	String& concat(char arg);
	enum EIgnoreCaseFlag
	{
		E_CASE_SENSITIVE,
		E_CASE_INSENSITIVE
	};
	/**
	 * Determine if this String object ends with given C string.
	 * @param arg	The C string object to search the end of this String
	 * 	object for.
	 * @param ignoreCase If true, character case will be ignored.
	 * @return true if this String ends with the given String. Otherwise
	 * return false.
	 */
	bool endsWith(const char* arg, EIgnoreCaseFlag ignoreCase = E_CASE_SENSITIVE) const;

	/**
	 * Determine if this String object ends with the same string
	 * represented by another String object.
	 * @param arg	The String object to search the end of this String
	 * 	object for.
	 * @param ignoreCase If true, character case will be ignored.
	 * @return true if this String ends with the given String. Otherwise
	 * return false.
	 */
	bool endsWith(const String& arg, EIgnoreCaseFlag ignoreCase = E_CASE_SENSITIVE) const
	{
		return endsWith(arg.c_str(), ignoreCase);
	}

	/**
	 * Determine if this String object ends with a given character.
	 * @param arg The character to test for.
	 * @return true if this String ends with the given character. Otherwise
	 * return false.
	 */
	bool endsWith(char arg) const;

	/**
	 * Determine if another String object is equal to this String object.
	 * @param arg The String object to check this String object against
	 * 	for equality.
	 * @return true if this String object is equal to the given String
	 * object. Otherwise return false.
	 */
	bool equals(const String& arg) const;
	/**
	 * Determine if another string is equal to this String object.
	 * @param arg The string to check this String object against
	 * 	for equality.
	 * @return true if this String object is equal to the given String
	 * object. Otherwise return false.
	 */
	bool equals(const char* arg) const;
	/**
	 * Determine if another String object is equal to this String object,
	 * ignoring case in the comparision.
	 * @param arg	The String object to check this String object against
	 * 	for equality.
	 * @return true if this String object is equal to the given String
	 * object. Otherwise return false.
	 */
	bool equalsIgnoreCase(const String& arg) const;
	/**
	 * Determine if another string is equal to this String object,
	 * ignoring case in the comparision.
	 * @param arg	The string to check this String object against
	 * 	for equality.
	 * @return true if this String object is equal to the given String
	 * object. Otherwise return false.
	 */
	bool equalsIgnoreCase(const char* arg) const;
	/**
	 * @return a 32 bit hashcode of this String object.
	 */
	UInt32 hashCode() const;
	/**
	 * Find the first occurence of a given character in this String object.
	 * @param ch The character to search for.
	 * @param fromIndex The index to start the search from.
	 * @return The index of the given character in the underlying array if it
	 * was found. npos if the character was not found at or after the given
	 * fromIndex.
	 */
	size_t indexOf(char ch, size_t fromIndex=0) const;
	/**
	 * Find the first occurence of a C string in this String object.
	 * @param arg			The C string to search for.
	 * @param fromIndex	The index to start the search from.
	 * @return The index of the given string in the underlying array if it
	 * was found. npos if the string was not found at or after the fromIndex.
	 */
	size_t indexOf(const char* arg, size_t fromIndex=0) const;
	/**
	 * Find the first occurence of a string in this String object.
	 * @param arg			The string to search for.
	 * @param fromIndex	The index to start the search from.
	 * @return The index of the given string in the underlying array if it
	 * was found. npos if the string was not found at or after the fromIndex.
	 */
	size_t indexOf(const String& arg, size_t fromIndex=0) const
	{
		return indexOf(arg.c_str(), fromIndex);
	}

	/**
	 * Find the last occurence of a character in this String object.
	 * @param ch			The character to search for.
	 * @param fromIndex	The index to start the search from.
	 * @return The index of the last occurence of the given character in the
	 * underlying array if it was found. npos if this String object does not
	 * contain the character before the given fromIndex.
	 */
	size_t lastIndexOf(char ch, size_t fromIndex=npos) const;
	/**
	 * Find the last occurence of a C string in this String object.
	 * @param arg			The C string to search for.
	 * @param fromIndex	The index to start the search from.
	 * @return The index of the last occurence of the string in the underlying
	 * array if it was found. npos if the this String does not contain the
	 * given substring before the fromIndex.
	 */
	size_t lastIndexOf(const char* arg, size_t fromIndex=npos) const;
	/**
	 * Find the last occurence of a string in this String object.
	 * @param arg			The string to search for.
	 * @param fromIndex	The index to start the search from.
	 * @return The index of the last occurence of the string in the underlying
	 * array if it was found. npos if the this String does not contain the
	 * given substring before the fromIndex.
	 */
	size_t lastIndexOf(const String& arg, size_t fromIndex=npos) const
	{
		return lastIndexOf(arg.c_str(), fromIndex);
	}

	/**
	 * Determine if this String object starts with a given substring.
	 * @param arg The substring to test for.  NULL is considered an empty string.
	 * @param ignoreCase	If true, case of the characters will be ignored.
	 * @return true if this String object starts with @a arg, i.e., @a arg is
	 * a prefix of *this.  Note that the empty string is a prefix of all strings.
	 * Otherwise false.
	 */
	bool startsWith(const char* arg, EIgnoreCaseFlag ignoreCase = E_CASE_SENSITIVE) const;
	/**
	 * Determine if this String object starts with a given substring.
	 * @param arg The substring to test for.
	 * @param ignoreCase	If true, case of the characters will be ignored.
	 * @return true if this String object starts with @a arg, i.e., @a arg is
	 * a prefix of *this.  Note that the empty string is a prefix of all strings.
	 * Otherwise false.
	 */
	bool startsWith(const String& arg, EIgnoreCaseFlag ignoreCase = E_CASE_SENSITIVE) const
	{
		return startsWith(arg.c_str(), ignoreCase);
	}
	/**
	 * Determine if this String object starts with a given character.
	 * @param arg The character to test for.
	 * @return true if this String object starts with the given character.
	 * Otherwise false.
	 */
	bool startsWith(char arg) const;

	/**
	 * Create another String object that is comprised of a substring of this
	 * String object.
	 * @param beginIndex		The start index of the substring.
	 * @param length			The length of the substring to use.
	 * @return A new String object representing the substring of this
	 * String object.
	 */
	String substring(size_t beginIndex,
		size_t length=npos) const;
	/**
	 * @return true if this String object contains nothing but space
	 * characters.
	 */
	bool isSpaces() const;
	/**
	 * Convert this String object to lower case characters.
	 * @return A reference to this String object after the conversion has
	 * taken place.
	 */
	String& toLowerCase();
	/**
	 * Convert this String object to upper case characters.
	 * @return A reference to this String object after the conversion has
	 * taken place.
	 */
	String& toUpperCase();
	/**
	 * Strip all leading space characters (as defined by
	 * the C function isspace()) from this String object.
	 * @return A reference to this String object after the operation has
	 * taken place.
	 */
	String& ltrim();
	/**
	 * Strip all trailing space characters (as defined by
	 * the C function isspace()) from this String object.
	 * @return A reference to this String object after the operation has
	 * taken place.
	 */
	String& rtrim();
	/**
	 * Strip all leading and trailing space characters (as defined by
	 * the C function isspace()) from this String object.
	 * @return A reference to this String object after the operation has
	 * taken place.
	 */
	String& trim();
	/**
	 * Delete all the characters of the string.  Thus, the string is empty
	 * after the call.
	 * @return A referernce to this object.
	 */
	String& erase();

	/**
	 * Erases, at most, len characters of *this, starting at index idx.
	 * if len == npos, erase to the end of the string.
	 * @return A referernce to this object.
	 */
	String& erase( size_t idx, size_t len = npos );
	/**
	 * Assignment operator.
	 * @param arg		The String object to assign to this String.
	 * @return A reference to this String object after the assignment has
	 * taken place.
	 */
	String& operator= (const String & arg);
	/**
	 * Operator [].
	 * @param ndx		The index of the character to retrieve from the underlying
	 * 					character array.
	 * @return The character at the given index within the underlying character
	 * array. If ndx is out of bounds, 0 is returned.
	 */
	const char& operator[] (size_t ndx) const;
	char& operator[] (size_t ndx);
	/**
	 * Concatination operator.
	 * @param arg	The String to append to the end of this String.
	 * @return A reference to this String object after the concatination has
	 * taken place.
	 */
	String& operator+= (const String& arg) { return concat(arg); }
	/**
	 * Concatination operator.
	 * @param arg	The C string to append to the end of this String.
	 * @return A reference to this String object after the concatination has
	 * taken place.
	 */
	String& operator+= (const char* arg) { return concat(arg); }
	/**
	 * Character concatination operator.
	 * @param arg	The character to append to the end of this String.
	 * @return A reference to this String object after the concatination has
	 * taken place.
	 */
	String& operator+= (char arg) { return concat(arg); }
#ifndef BLOCXX_NO_IOSTREAMS
	/**
	 * Read this String object from the given istream. An String must have
	 * been previously written to this location of the stream.
	 * @param istrm	The input stream to read this String from.
	 * @exception IOException
	 */
	void readObject(std::streambuf & istrm);
	/**
	 * Write this String object to the given ostream.
	 * @param ostrm	The output stream to write this String to.
	 * @exception IOException
	 */
	void writeObject(std::streambuf & ostrm) const;
#endif // BLOCXX_NO_IOSTREAMS
	/**
	 * @return A copy of this String object.
	 */
	String toString() const;
	/**
	 * @return The Real32 value of this String object.
	 * @throws StringConversionException if the conversion is impossible.
	 */
	Real32 toReal32() const;
	/**
	 * @return The float value of this String object.
	 * @throws StringConversionException if the conversion is impossible.
	 */
	float toFloat() const;
	/**
	 * @return The Real64 value of this String object.
	 * @throws StringConversionException if the conversion is impossible.
	 */
	Real64 toReal64() const;
	/**
	 * @return The double value of this String object.
	 * @throws StringConversionException if the conversion is impossible.
	 */
	double toDouble() const;
	/**
	 * @return The boolean value of this String object. The string is
	 * assumed to be the value of "TRUE"/"FALSE" ignoring case.
	 * @throws StringConversionException if the conversion is impossible.
	 */
	bool toBool() const;
	/**
	 * @return The UInt8 value of this String object.
	 * @throws StringConversionException if the conversion is impossible.
	 */
	UInt8 toUInt8(int base=10) const;
	/**
	 * @return The Int8 value of this String object.
	 * @throws StringConversionException if the conversion is impossible.
	 */
	Int8 toInt8(int base=10) const;
	/**
	 * @return The UInt16 value of this String object.
	 * @throws StringConversionException if the conversion is impossible.
	 */
	UInt16 toUInt16(int base=10) const;
	/**
	 * @return The Int16 value of this String object.
	 * @throws StringConversionException if the conversion is impossible.
	 */
	Int16 toInt16(int base=10) const;
	/**
	 * @return The UInt32 value of this String object.
	 * @throws StringConversionException if the conversion is impossible.
	 */
	UInt32 toUInt32(int base=10) const;
	/**
	 * @return The Int32 value of this String object.
	 * @throws StringConversionException if the conversion is impossible.
	 */
	Int32 toInt32(int base=10) const;
	/**
	 * @return The UInt64 value of this String object.
	 * @throws StringConversionException if the conversion is impossible.
	 */
	UInt64 toUInt64(int base=10) const;
	/**
	 * @return The Int64 value of this String object.
	 * @throws StringConversionException if the conversion is impossible.
	 */
	Int64 toInt64(int base=10) const;
	/**
	 * @return The unsigned int value of this String object.
	 * @throws StringConversionException if the conversion is impossible.
	 */
	unsigned int toUnsignedInt(int base=10) const;
	/**
	 * @return The int value of this String object.
	 * @throws StringConversionException if the conversion is impossible.
	 */
	int toInt(int base=10) const;
	/**
	 * Convert a null terminated string to an unsigned 64 bit value.
	 * @param nptr A pointer to beginning of string to convert.
	 * @param endptr If not NULL, this function stores the address of the 1st
	 *		invalid character in *endptr.
	 * @param base A number that specifies the base for the number pointed to
	 *		by nptr.
	 * @return The unsigned 64 bit value of the number represented in string
	 * form pointed to by nptr.
	 */
	static unsigned long long int strtoull(const char* nptr, char** endptr,
		int base);
	/**
	 * Convert a null terminated string to an signed 64 bit value.
	 * @param nptr A pointer to beginning of string to convert.
	 * @param endptr If not NULL, this function stores the address of the 1st
	 *		invalid character in *endptr.
	 * @param base A number that specifies the base for the number pointed to
	 *		by nptr.
	 * @return The signed 64 bit value of the number represented in string
	 * form pointed to by nptr.
	 */
	static long long int strtoll(const char* nptr, char** endptr, int base);
	/**
	 * Find the first occurrence of a character in a null terminated string.
	 * @param theStr The string to search for the given character.
	 * @param c The character to to search for.
	 * @return a  pointer to the first occurrence of the character c in the
	 * string theStr if found. Otherwise a NULL pointer.
	 */
	static const char* strchr(const char* theStr, int c);
#ifndef BLOCXX_NO_IOSTREAMS
	/**
	 * Reads from in input stream until a newline is encountered.
	 * This consumes the newline character (but doesn't put the newline in
	 * the return value), and leaves the file pointer at the char past
	 * the newline.
	 *
	 * @return an String containing the line.
	 */
	static String getLine(std::istream& istr);
#endif // BLOCXX_NO_IOSTREAMS

#if defined(BLOCXX_AIX)
	static const size_t npos;
#else
	static const size_t npos = size_t(~0);
#endif // BLOCXX_AIX

#ifdef BLOCXX_WIN32
#pragma warning (push)
#pragma warning (disable: 4251)
#endif

	typedef COWIntrusiveReference<ByteBuf> buf_t;
private:
	buf_t m_buf;

#ifdef BLOCXX_WIN32
#pragma warning (pop)
#endif

};
BLOCXX_EXPORT_TEMPLATE(BLOCXX_COMMON_API, Array, String);
BLOCXX_EXPORT_TEMPLATE(BLOCXX_COMMON_API, Enumeration, String);

/**
 * Pad the supplied string to be at least width characters larger by
 * inserting the fill character on the left.  If the string is longer
 * than the supplied width, nothing is added.
 */
String padOnLeft(const String& s, size_t width, char fill);

/**
 * Pad the supplied string to be at least width characters larger by
 * inserting the fill character on the right.  If the string is longer
 * than the supplied width, nothing is added.
 */
String padOnRight(const String& s, size_t width, char fill);

String repeat(char c, size_t count);

#ifndef BLOCXX_NO_IOSTREAMS
BLOCXX_COMMON_API std::ostream& operator<< (std::ostream& ostr, const String& arg);
#endif

BLOCXX_COMMON_API String operator + (const String& s1, const String& s2);
BLOCXX_COMMON_API String operator + (const char* p, const String& s);
BLOCXX_COMMON_API String operator + (const String& s, const char* p);
BLOCXX_COMMON_API String operator + (char c, const String& s);
BLOCXX_COMMON_API String operator + (const String& s, char c);
inline bool
operator == (const String& s1, const String& s2)
{
	return (s1.compareTo(s2) == 0);
}
inline bool
operator == (const String& s, const char* p)
{
	return (s.compareTo(p) == 0);
}
inline bool
operator == (const char* p, const String& s)
{
	return (s.compareTo(p) == 0);
}
inline bool
operator != (const String& s1, const String& s2)
{
	return (s1.compareTo(s2) != 0);
}
inline bool
operator != (const String& s, const char* p)
{
	return (s.compareTo(p) != 0);
}
inline bool
operator != (const char* p, const String& s)
{
	return (s.compareTo(p) != 0);
}
inline bool
operator < (const String& s1, const String& s2)
{
	return (s1.compareTo(s2) < 0);
}
inline bool
operator < (const String& s, const char* p)
{
	return (s.compareTo(p) < 0);
}
inline bool
operator < (const char* p, const String& s)
{
	return (String(p).compareTo(s) < 0);
}
inline bool
operator <= (const String& s1, const String& s2)
{
	return (s1.compareTo(s2) <= 0);
}
inline bool
operator <= (const String& s, const char* p)
{
	return (s.compareTo(p) <= 0);
}
inline bool
operator <= (const char* p, const String& s)
{
	return (String(p).compareTo(s) <= 0);
}
inline bool
operator > (const String& s1, const String& s2)
{
	return (s1.compareTo(s2) > 0);
}
inline bool
operator > (const String& s, const char* p)
{
	return (s.compareTo(p) > 0);
}
inline bool
operator > (const char* p, const String& s)
{
	return (String(p).compareTo(s) > 0);
}
inline bool
operator >= (const String& s1, const String& s2)
{
	return (s1.compareTo(s2) >= 0);
}
inline bool
operator >= (const String& s, const char* p)
{
	return (s.compareTo(p) >= 0);
}
inline bool
operator >= (const char* p, const String& s)
{
	return (String(p).compareTo(s) >= 0);
}

} // end namespace BLOCXX_NAMESPACE

#endif
