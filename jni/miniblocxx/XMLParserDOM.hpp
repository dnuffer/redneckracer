/*******************************************************************************
 * Copyright (C) 2004 Quest Software, Inc. All rights reserved.
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


#ifndef OW_XMLPARSERDOM_HPP_INCLUDE_GUARD_
#define OW_XMLPARSERDOM_HPP_INCLUDE_GUARD_

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/XMLNode.hpp"

namespace XML
{
	namespace ParserDOM
	{
		/**
		 * Parse the XML document contained in the string xmlData
		 *
		 * @param xmlData The std::string containing the XML document to parse
		 * @return OW_XMLNode which is the root of the XML document parsed.
		 * The return value is NULL if the parse failed.
		 */
		BLOCXX_COMMON_API XMLNode parse(const std::string& xmlData);

		/**
		 * Parse the XML document to be read from the std::istream data
		 *
		 * @param data   std::istream to read the XML document to be parsed
		 * @return XMLNode which is the root of the XML document parsed
		 * The return value is NULL if the parse failed.
		 */
		BLOCXX_COMMON_API XMLNode parse(std::istream& data);
	} // end namespace ParserDOM
} // end namespace XML_NAMESPACE


#endif

