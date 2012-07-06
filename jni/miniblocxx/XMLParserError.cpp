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

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/XMLParserError.hpp"
#include <stdarg.h>
#include <stdio.h>
#include <vector>

namespace XML
{
	namespace
	{
		XMLParserError G_PARSER_ERROR = XMLParserError("", 0, XMLParserError::NO_ERROR, "", 0);
	}

	bool haveParserError()
	{
		return G_PARSER_ERROR.getCode() != XMLParserError::NO_ERROR;
	}

	void clearParserError()
	{
		G_PARSER_ERROR = XMLParserError("", 0, XMLParserError::NO_ERROR, "", 0);
	}

	XMLParserError getParserError()
	{
		return G_PARSER_ERROR;
	}

	void setParserError(const XMLParserError& error)
	{
		G_PARSER_ERROR = error;
	}


	////////////////////////////////////////////////////////////////////////////////
	//
	// XMLParserError
	//
	////////////////////////////////////////////////////////////////////////////////
	static const char* _xmlMessages[] =
		{
			"Bad opening element",
			"Bad closing element",
			"Bad XML declaration",
			"Bad attribute name",
			"Exepected equal sign",
			"Bad attribute value",
			"A \"--\" sequence found within comment",
			"Unterminated comment",
			"Unterminated CDATA block",
			"Unterminated DOCTYPE",
			"Too many attributes: parser only handles 10",
			"Malformed reference",
			"Expected a comment or CDATA following \"<!\" sequence",
			"Closing element does not match opening element",
			"One or more tags are still open",
			"More than one root element was encountered",
			"Validation error",
			"Semantic error"
		};
	////////////////////////////////////////////////////////////////////////////////
	XMLParserError::XMLParserError(
		const char* file,
		unsigned int line,
		Code code,
		const char* msg,
		unsigned int xmlline)
		: _text(stringPrint("%s:%d: Line %d: %s: %s", file, line, xmlline, (code != XMLParserError::NO_ERROR) ? _xmlMessages[code - 1] : "no error", (msg != 0 ? msg : "")))
		, _code(code)
		, _msg(msg ? msg : "")
	{
	}

	////////////////////////////////////////////////////////////////////////////////
	XMLParserError::XMLParserError(
		const char* file,
		unsigned int line,
		Code code,
		const std::string& msg,
		unsigned int xmlline)
		: _text(stringPrint("%s:%d: Line %d: %s: %s", file, line, xmlline, (code != XMLParserError::NO_ERROR) ? _xmlMessages[code - 1] : "no error", msg.c_str()))
		, _code(code)
		, _msg(msg)
	{
	}

	std::string stringPrint(const char* fmt, ...)
	{
		std::vector<char> buffer(64);
		int n = 0;
		while(true)
		{
			va_list ap;
			va_start(ap, fmt);
			n = vsnprintf(&buffer[0], buffer.size(), fmt, ap);
			va_end(ap); // If that worked, return the string.

			if( (n > -1) && (size_t(n) < buffer.size()) )
			{
				break;
			}
			buffer.resize(buffer.size() * 2);
		}
		return std::string(&buffer[0], n);
	}
} // end namespace XML_NAMESPACE

