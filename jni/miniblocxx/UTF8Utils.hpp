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
 * @author Dan Nuffer
 */

#ifndef BLOCXX_UTF8_UTILS_HPP_INCLUDE_GUARD_
#define BLOCXX_UTF8_UTILS_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/Types.hpp"
#include "miniblocxx/Exception.hpp"
#include "miniblocxx/CommonFwd.hpp"

namespace BLOCXX_NAMESPACE
{

BLOCXX_DECLARE_APIEXCEPTION(InvalidUTF8, BLOCXX_COMMON_API);

namespace UTF8Utils
{
/**
 * Count the number of UTF-8 chars in the string.  This may be different than
 * the number of bytes (as would be returned by strlen()).
 * If utf8str is not a valid UTF-8 string, then the result is undefined.
 * @param utf8str string in UTF-8 encoding.
 * @return Number of chars in the string.
 */
BLOCXX_COMMON_API size_t charCount(const char* utf8str);
/**
 * Convert one UTF-8 char (possibly multiple bytes) into a UCS2 16-bit char
 * @param utf8char pointer to the UTF-8 char to convert
 * @return The corresponding UCS2 char.  0xFFFF if utf8char points to an
 *  invalid UTF-8 sequence.  Not all UTF-8 chars are handled. UTF-8 chars
 *  outside the range of a UCS2 char will produce 0xFFFF.
 */
BLOCXX_COMMON_API UInt16 UTF8toUCS2(const char* utf8char);
/**
 * Convert one UCS2 16-bit char into a UTF-8 char (possibly multiple bytes)
 * @param ucs2char UCS2 char to convert.
 * @return The corresponding UTF-8 char.
 */
BLOCXX_COMMON_API String UCS2toUTF8(UInt16 ucs2char);
/**
 * Convert one UTF-8 char (possibly multiple bytes) into a UCS4 32-bit char
 * @param utf8char pointer to the UTF-8 char to convert
 * @return The corresponding UCS4 char.  0xFFFFFFFF if utf8char points to an
 *  invalid UTF-8 sequence.
 */
BLOCXX_COMMON_API UInt32 UTF8toUCS4(const char* utf8char);
/**
 * Convert one UCS4 32-bit char into a UTF-8 char (possibly multiple bytes)
 * @param ucs4char UCS4 char to convert.
 * @return The corresponding UTF-8 char.
 */
BLOCXX_COMMON_API String UCS4toUTF8(UInt32 ucs4char);
/**
 * Convert one UCS4 32-bit char into a UTF-8 char (possibly multiple bytes)
 * This version is faster to use in a loop than the version which returns a
 * String.
 * @param ucs4char UCS4 char to convert.
 * @param sb The corresponding UTF-8 char will be appended to the end of sb.
 */
BLOCXX_COMMON_API void UCS4toUTF8(UInt32 ucs4char, StringBuffer& sb);

/**
 * Compares 2 UTF-8 strings, ignoring any case differences as defined by the
 * Unicode spec CaseFolding.txt file.
 * @param str1 first string
 * @param str2 second string
 * @return a value less than, equal to, or greater than 0 if str1 is found to
 * be less than, equal to, or greater than str2
 */
BLOCXX_COMMON_API int compareToIgnoreCase(const char* str1, const char* str2);
/**
 * Convert a UTF-8 (or ASCII) string into a UCS2 string
 * @param input The UTF-8 string
 * @return An Array of UCS2 characters
 * @throws InvalidUTF8Exception if input contains invalid UTF-8 characters.
 */
BLOCXX_COMMON_API Array<UInt16> StringToUCS2(const String& input);

/**
 * Convert a UTF-8 (or ASCII) string into a UCS2 string. Invalid characters
 * will be changed to U+FFFD (the Unicode Replacement character)
 * @param input The UTF-8 string
 * @return An Array of UCS2 characters
 */
BLOCXX_COMMON_API Array<UInt16> StringToUCS2ReplaceInvalid(const String& input);

/**
 * Convert a UCS2 string into a UTF-8 (or ASCII) string
 * @param input An Array of UCS2 characters
 * @return The UTF-8 string
 */
BLOCXX_COMMON_API String UCS2ToString(const Array<UInt16>& input);
/**
 * Convert a UCS2 string into a UTF-8 (or ASCII) string
 * @param input An Array of UCS2 characters
 * @return The UTF-8 string
 */
BLOCXX_COMMON_API String UCS2ToString(const Array<char>& input);
/**
 * Convert a UCS2 string into a UTF-8 (or ASCII) string
 * @param input An Array of UCS2 characters
 * @param inputLength The size (in bytes) of input.
 * @return The UTF-8 string
 */
BLOCXX_COMMON_API String UCS2ToString(const void* input, size_t inputLength);

/**
 * Convert the UTF-8 string to upper case. The string is modified in place.
 * If a character is encountered whose replacement occupies a greater number
 * of bytes than the original, processing will cease and false will be returned.
 * The current implementation does not handle any of the special cases
 * as defined in the Unicode SpecialCasing.txt file, and thus characters will
 * not grow, so currently false will never be returned.
 * @return true if successful. false if the upper-cased replacement would be
 * larger than the original.
 */
BLOCXX_COMMON_API bool toUpperCaseInPlace(char* input);

/**
 * Convert the UTF-8 string to upper case and return the result.
 */
BLOCXX_COMMON_API String toUpperCase(const char* input);

/**
 * Convert the UTF-8 string to lower case. The string is modified in place.
 * If a character is encountered whose replacement occupies a greater number
 * of bytes than the original, processing will cease and false will be returned.
 * The current implementation does not handle any of the special cases
 * as defined in the Unicode SpecialCasing.txt file, and thus characters will
 * not grow, so currently false will never be returned.
 * @return true if successful. false if the lower-cased replacement would be
 * larger than the original.
 */
BLOCXX_COMMON_API bool toLowerCaseInPlace(char* input);

/**
 * Convert the UTF-8 string to lower case and return the result.
 */
BLOCXX_COMMON_API String toLowerCase(const char* input);

} // end namespace UTF8Utils

} // end namespace BLOCXX_NAMESPACE

#endif
