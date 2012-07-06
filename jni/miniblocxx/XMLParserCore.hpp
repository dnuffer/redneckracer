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

#ifndef OW_XMLPARSERCORE_HPP_INCLUDE_GUARD_
#define OW_XMLPARSERCORE_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/XMLParserError.hpp"
#include "miniblocxx/IstreamBufIterator.hpp"
#include <string>
#include <vector>
#include <stack>
#include <iosfwd>

namespace XML
{

	/////////////////////////////////////////////////////////////////////////////
	struct BLOCXX_COMMON_API XMLToken
	{
		XMLToken() : type(INVALID), text()
		{}
		enum XMLType
		{
			INVALID,
			XML_DECLARATION,
			START_TAG,
			END_TAG,
			COMMENT,
			CDATA,
			DOCTYPE,
			CONTENT
		};
		struct BLOCXX_COMMON_API Attribute
		{
			Attribute(): name(), value()
			{
			}
			std::string name;
			std::string value;
		};
		XMLType type;
		std::string text;
		std::vector<Attribute> attributes;
	};

	/////////////////////////////////////////////////////////////////////////////
	class BLOCXX_COMMON_API XMLParserCore
	{
	public:
		XMLParserCore(std::istream& input) : m_line(1), m_current(), m_foundRoot(false), m_tagIsEmpty(false)
		{
			setInput(input);
		}
		XMLParserCore(): m_line(1), m_current(), m_foundRoot(false), m_tagIsEmpty(false)
		{
		}
		~XMLParserCore()
		{
		}
		void setInput(std::istream& input)
		{
			m_current = blocxx::IstreamBufIterator(input);
		}
		bool next(XMLToken& entry);
		unsigned int getLine() const
		{
			return m_line;
		}
	private:
		void skipWhitespace();
		bool getElementName(XMLToken& entry, XMLParserError::Code errorCode);
		bool getOpenElementName(XMLToken& entry, bool& openCloseElement);
		void getAttributeNameAndEqual(XMLToken::Attribute& att);
		void getAttributeValue(XMLToken::Attribute& att);
		void getComment();
		void getCData(XMLToken& entry);
		void getDocType();
		void getContent(XMLToken& entry);
		void getElement(XMLToken& entry);

#ifdef OW_WIN32
#pragma warning (push)
#pragma warning (disable: 4251)
#endif

		unsigned int m_line;
		blocxx::IstreamBufIterator m_current;


		// used to verify elements' begin and end tags match.
		std::stack<std::string> m_stack;
		bool m_foundRoot;
		bool m_tagIsEmpty;

#ifdef OW_WIN32
#pragma warning (pop)
#endif

	};

} // end namespace XML_NAMESPACE

#endif
