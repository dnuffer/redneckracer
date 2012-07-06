/*******************************************************************************
* Copyright (C) 2009, Quest Software, Inc. All rights reserved.
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
 * @author Richard Holden
 */

#include "blocxx/BLOCXX_config.h"
#include "blocxx/StringConversionImpl.hpp"
#include "blocxx/Format.hpp"

#include <cmath> // for HUGE_VAL
#include <cfloat> // for DBL_MANT_DIG
#include <limits>
#include <cstdlib>

namespace BLOCXX_NAMESPACE
{
	using std::numeric_limits;

	namespace // anonymous
	{
		template <typename T, typename FP>
		static inline
		T convertToRealTypeHelper(const String& str, const char* type, FP fp)
		{
			if (!str.empty())
			{
				const char* begin = str.c_str();
				const char* end = str.c_str() + str.length();
				const char* current = begin;
				const char* marker = begin + 1;

			start:
				if (current >= end)
				{
					goto finish;
				}
				/*!re2c
				  re2c:define:YYCTYPE = "unsigned char";
				  re2c:define:YYCURSOR = current;
				  re2c:define:YYLIMIT = end;
				  re2c:define:YYMARKER = marker;
				  re2c:yyfill:enable = 0;
				  re2c:yych:conversion = 1;
				  re2c:indent:top = 2;

				  WS = [ \t]+ ;
				  NAN = [+-]? ("nan"|"NAN") ("(" [^)]+ ")")? ;
				  INFINITY = [+]? ( "infinity"|"INFINITY"| "inf" | "INF" );
				  NEG_INFINITY = [-] ( "infinity" | "INFINITY" | "inf" | "INF" );
				  ANY = [^];

				  WS  { marker = current + 1; goto start; }
				  NAN { return numeric_limits<T>::quiet_NaN(); }
				  INFINITY { return numeric_limits<T>::infinity(); }
				  NEG_INFINITY { return -numeric_limits<T>::infinity(); }
				  ANY { goto finish; }
				*/

			finish:
				char* endptr(0);
				errno = 0;		// errno is thread local
				T rv = fp(str.c_str(), &endptr);
				if (*endptr != '\0' || errno == ERANGE)
				{
					BLOCXX_THROW(StringConversionException, Format("Unable to convert \"%1\" into %2", str, type).c_str());
				}
				return rv;
			}
			else
			{
				BLOCXX_THROW(StringConversionException, Format("Unable to convert \"%1\" into %2", str, type).c_str());
			}
			return T(); // to make compiler happy
		}
	}

	template<> Real32 convertToRealType<Real32>(const String& str)
	{
#if defined(BLOCXX_REAL32_IS_FLOAT) && defined(BLOCXX_HAVE_STRTOF)
		return convertToRealTypeHelper<Real32>(str, "Real32", &strtof);
#elif defined(BLOCXX_REAL32_IS_DOUBLE) || (defined(BLOCXX_REAL32_IS_FLOAT) && !defined(BLOCXX_HAVE_STRTOF))
		return convertToRealTypeHelper<Real32>(str, "Real32", &strtod);
#endif
	}

	template<> Real64 convertToRealType<Real64>(const String& str)
	{
#if defined(BLOCXX_REAL64_IS_DOUBLE)
		return convertToRealTypeHelper<Real64>(str, "Real64", &strtod);
#elif defined(BLOCXX_REAL64_IS_LONG_DOUBLE)
		return convertToRealTypeHelper<Real64>(str, "Real64", &strtold);
#endif
	}
}
