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

#ifndef OW_XMLPARSE_EXCEPTION_HPP_INCLUDE_GUARD_
#define OW_XMLPARSE_EXCEPTION_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include <string>

namespace XML
{
	class BLOCXX_COMMON_API XMLParserError
	{
	public:
		enum Code
		{
			NO_ERROR = 0,
			BAD_START_TAG = 1,
			BAD_END_TAG,
			BAD_XML_DECLARATION,
			BAD_ATTRIBUTE_NAME,
			EXPECTED_EQUAL_SIGN,
			BAD_ATTRIBUTE_VALUE,
			MINUS_MINUS_IN_COMMENT,
			UNTERMINATED_COMMENT,
			UNTERMINATED_CDATA,
			UNTERMINATED_DOCTYPE,
			TOO_MANY_ATTRIBUTES,
			MALFORMED_REFERENCE,
			EXPECTED_COMMENT_OR_CDATA,
			START_END_MISMATCH,
			UNCLOSED_TAGS,
			MULTIPLE_ROOTS,
			VALIDATION_ERROR,
			SEMANTIC_ERROR
		};

		XMLParserError(
			const char* file,
			unsigned int line,
			Code code,
			const char* msg,
			unsigned int xmlline = 0);

		XMLParserError(
			const char* file,
			unsigned int line,
			Code code,
			const std::string& msg,
			unsigned int xmlline = 0);

		XMLParserError::Code getCode() const
		{
			return _code;
		}

		std::string getMessage() const
		{
			return _msg;
		}

		std::string getText() const
		{
			return _text;
		}

	private:
		std::string _text;
		Code _code;
		std::string _msg;
	};

	bool haveParserError();
	void clearParserError();
	XMLParserError getParserError();
	void setParserError(const XMLParserError& error);

	std::string stringPrint(const char* fmt, ...);

#define XML_ERROR(code, message) ::XML::setParserError(XMLParserError(__FILE__, __LINE__, (code), (message)));

#define XML_ERROR_LINE(code, line) ::XML::setParserError(XMLParserError(__FILE__, __LINE__, (code), 0, (line)));

#define XML_ERROR_LINE_MSG(code, line, message) ::XML::setParserError(XMLParserError(__FILE__, __LINE__, (code), (message), (line)));

} // end namespace XML


#endif


