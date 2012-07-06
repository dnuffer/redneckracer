/*******************************************************************************
 * Copyright (C) 2001-2008 Quest Software, Inc. All rights reserved.
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
 * @author Kevin Harris
 */


/* a simple lexical scanner to escape xml */
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/XMLEscape.hpp"
#include "miniblocxx/StringBuffer.hpp"

namespace XML
{
	std::string XMLEscape(const char* unescapedText, unsigned len)
	{
		blocxx::StringBuffer rval;

		const char* begin = unescapedText;
		const char* end = unescapedText + len;
		const char* current = begin;
		// This is reset to be one-after the start for each "invalid" character
		// sequence (multi-byte or not).
		const char* marker = begin + 1;
	start:
		if( current >= end )
		{
			goto finish;
		}

		// 0000-0008 is invalid
		// 000b is invalid
		// 000e-001f is invalid
		// d800-dfff is invalid
		// fffe-ffff is invalid

		/*!re2c
		  re2c:define:YYCTYPE = "unsigned char";
		  re2c:define:YYCURSOR = current;
		  re2c:define:YYLIMIT = end;
		  re2c:define:YYMARKER = marker;
		  re2c:yyfill:enable = 0;
		  re2c:yych:conversion = 1;
		  re2c:indent:top = 1;

		  d800dfff = ([\355](([\240][\200-\377])|([\241-\276].)|([\277][\000-\277])));
		  fffeffff = ([\357][\277][\276-\277]);
		  invalid = ([\000-\011]|[\013]|[\016-\037]|d800dfff|fffeffff);
		  any = [\000-\377];

		  ">" { rval += "&gt;"; goto start; }
		  "<" { rval += "&lt;"; goto start; }
		  "&" { rval += "&amp;"; goto start; }
		  "\"" { rval += "&quot;"; goto start; }
		  "'" { rval += "&apos;"; goto start; }
		  "\r" { rval += "&#13;"; goto start; }
		  "\n" { rval += "&#10;"; goto start; }
		  "\t" { rval += "&#9;"; goto start; }
		  invalid
		  {
		  // This replaces the character sequence from (marker - 1) to (current)
		  // with the UTF-8 replacement sequence.
		  rval += "\xef\xbf\xbd"; // UTF-8 replacement sequence.
		  goto start;
		  }
		  any
		  {
		  // Copy straight across.
		  rval += *(current-1);
		  marker = current + 1;
		  goto start;
		  }
		*/
	finish:

		return rval.toString();
	}

} // end namespace OW_NAMESPACE

