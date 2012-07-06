/*******************************************************************************
 * Copyright (C) 2001-2004 Quest Software, Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  - Neither the name of Quest Software, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL Quest Software, Inc. OR THE CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
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

/* a simple lexical scanner to escape xml */
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/XMLUnescape.hpp"
#include "miniblocxx/StringBuffer.hpp"
#include "miniblocxx/XMLParserError.hpp"
#include <limits.h> // for CHAR_MAX
#include <stdlib.h> // for strtol

namespace XML
{

	std::string XMLUnescape(const char* escapedText, unsigned len)
	{
		blocxx::StringBuffer rval(len);
		const char* begin = escapedText;
		const char* q;
		const char* thisTokStart = escapedText;
#define YYCTYPE char
#define YYCURSOR        begin
#define YYLIMIT         begin
#define YYMARKER        q
#define YYFILL(n) (void) true
	start:
		/*!re2c
		END = [\000];
		any = [\001-\377];
		DIGIT = [0-9];
		HEXDIGIT = [0-9a-fA-F];

		"&gt;" { rval += '>'; thisTokStart = YYCURSOR; goto start; }
		"&lt;" { rval += '<'; thisTokStart = YYCURSOR; goto start; }
		"&amp;" { rval += '&'; thisTokStart = YYCURSOR; goto start; }
		"&quot;" { rval += '\"'; thisTokStart = YYCURSOR; goto start; }
		"&apos;" { rval += '\''; thisTokStart = YYCURSOR; goto start; }
		"&#x" HEXDIGIT+ ";"
		{
			unsigned long lval = strtoul( thisTokStart + 3, NULL, 16 );
			if (lval > CHAR_MAX)
			{
				XML_ERROR(XMLParserError::MALFORMED_REFERENCE, stringPrint("XML escape code in unsupported range: %d", YYCURSOR - 1));
				return std::string();
			}
			char val = lval;
			rval += val;
			thisTokStart = YYCURSOR; goto start;
		}
		"&#" DIGIT+ ";"
		{
			unsigned long lval = strtoul( thisTokStart + 2, NULL, 10 );
			if (lval > CHAR_MAX)
			{
				XML_ERROR(XMLParserError::MALFORMED_REFERENCE, stringPrint("XML escape code in unsupported range: %d", YYCURSOR - 1));
				return std::string();
			}
			char val = lval;
			rval += val;
			thisTokStart = YYCURSOR; goto start;
		}
		any { rval += *(YYCURSOR-1); thisTokStart = YYCURSOR; goto start; }
		END { return rval.toString(); }
		*/

		return rval.toString();
	}

} // end namespace OpenWBEM
