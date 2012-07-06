/******************************************************************************
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
#include "miniblocxx/XMLParserCore.hpp"
#include "miniblocxx/XMLParserError.hpp"
#include "miniblocxx/IstreamBufIterator.hpp"

#include <cctype>

namespace XML
{
	// Note that we don't use the OW_THROW macro in this file to throw
	// an XMLParseException.  This is because it needs extra information not
	// available with OW_THROW.

	////////////////////////////////////////////////////////////////////////////////
	//
	// XMLParserCore
	//
	////////////////////////////////////////////////////////////////////////////////
	bool XMLParserCore::next(XMLToken& entry)
	{
		// if the last tag was a <.../> then set the next token to END_TAG so that
		// the caller doesn't need to worry about <.../>, it will look like
		// <...></...>
		if (m_tagIsEmpty)
		{
			m_tagIsEmpty = false;
			entry.type = XMLToken::END_TAG;
			entry.attributes.clear();
			return true;
		}
		blocxx::IstreamBufIterator iterEOF;
		if (m_current == iterEOF || *m_current == 0)
		{
			if (!m_stack.empty())
			{
				XML_ERROR_LINE_MSG(XMLParserError::UNCLOSED_TAGS, m_line, stringPrint("tag <%s> is not closed", m_stack.top().c_str()));
				return false;
			}
			return false;
		}
		// Either a "<...>" or content begins next:
		if (*m_current == '<')
		{
			// Skip over any whitespace:
			skipWhitespace();
			m_current++;
			getElement(entry);
			if (entry.type == XMLToken::START_TAG)
			{
				if (m_stack.empty() && m_foundRoot)
				{
					XML_ERROR_LINE_MSG(XMLParserError::MULTIPLE_ROOTS, m_line, stringPrint("second root is \"%s\"", entry.text.c_str()));
					return false;
				}
				m_foundRoot = true;
				if (!m_tagIsEmpty)
				{
					m_stack.push(entry.text);
				}
			}
			else if (entry.type == XMLToken::END_TAG)
			{
				if (m_stack.empty())
				{
					XML_ERROR_LINE_MSG(XMLParserError::START_END_MISMATCH, m_line, stringPrint("end tag \"</%s>\" appears without start tag", entry.text.c_str()));
				}
				if (m_stack.top() != entry.text)
				{
					XML_ERROR_LINE_MSG(XMLParserError::START_END_MISMATCH, m_line, stringPrint("expected \"</%s>\", found \"</%s>\"", m_stack.top().c_str(), entry.text.c_str()));
				}
				m_stack.pop();
			}
			return true;
		}
		else
		{
			entry.type = XMLToken::CONTENT;
			//bool isSpaces;
			//getContent(entry, isSpaces);
			getContent(entry);
			//if (isSpaces)
			//{
			// content is entirely white space, so just skip it.
			//	return next(entry);
			//}
			return true;
		}
	}
	/*
	  void XMLParserCore::putBack(XMLToken& entry)
	  {
	  _putBackStack.push(entry);
	  }
	*/
	void XMLParserCore::skipWhitespace()
	{
		while (isspace(*m_current))
		{
			if (*m_current == '\n')
			{
				++m_line;
			}
			++m_current;
		}
	}

	inline bool isNameChar(char c)
	{
		return isalnum(c) || c == '_' || c == '-' ||
			c == ':' || c == '.';
	}

	bool XMLParserCore::getElementName(XMLToken& entry, XMLParserError::Code errorCode)
	{
		if (!isalpha(*m_current) && *m_current != '_')
		{
			XML_ERROR_LINE(errorCode, m_line);
			return false;
		}
		entry.text.erase();
		while (isNameChar(*m_current))
		{
			entry.text += *m_current++;
		}
		// The next character might be a space:
		skipWhitespace();

		if (*m_current == '>')
		{
			++m_current;
			return true;
		}
		return false;
	}

	bool XMLParserCore::getOpenElementName(XMLToken& entry, bool& openCloseElement)
	{
		openCloseElement = false;
		if (getElementName(entry, XMLParserError::BAD_START_TAG))
		{
			return true;
		}
		if (*m_current == '/')
		{
			++m_current;
			if (*m_current == '>')
			{
				openCloseElement = true;
				++m_current;
				return true;
			}
		}
		return false;
	}
	void XMLParserCore::getAttributeNameAndEqual(XMLToken::Attribute& att)
	{
		if (!isalpha(*m_current) && *m_current != '_')
		{
			XML_ERROR_LINE_MSG(XMLParserError::BAD_ATTRIBUTE_NAME,
				m_line,
				stringPrint("Expected alpha or _; got %c", *m_current));
		}
		att.name.erase();
		while (isalnum(*m_current) || *m_current == '_' || *m_current == '-' ||
			*m_current == ':' || *m_current == '.')
		{
			att.name += *m_current++;
		}
		skipWhitespace();
		if (*m_current != '=')
		{
			XML_ERROR_LINE_MSG(XMLParserError::BAD_ATTRIBUTE_NAME,
				m_line, stringPrint("Expected =; got %c", *m_current));
		}
		m_current++;
		skipWhitespace();
	}
	void XMLParserCore::getAttributeValue(XMLToken::Attribute& att)
	{
		// ATTN-B: handle values contained in semiquotes:
		if (*m_current != '"' && *m_current != '\'')
		{
			XML_ERROR_LINE_MSG(XMLParserError::BAD_ATTRIBUTE_VALUE,
				m_line, stringPrint("Expecting \" or '; got %c", *m_current));
		}
		char startChar = *m_current++;
		att.value.erase();
		while (*m_current && *m_current != startChar)
		{
			att.value += *m_current++;
		}

		if (*m_current != startChar)
		{
			XML_ERROR_LINE_MSG(XMLParserError::BAD_ATTRIBUTE_VALUE,
				m_line, stringPrint("Expecting %c; Got %c", startChar, static_cast<int>(*m_current)));
		}
		++m_current;
	}
	void XMLParserCore::getComment()
	{
		// Now p points to first non-whitespace character beyond "<--" sequence:
		for (; *m_current; m_current++)
		{
			if (*m_current == '-')
			{
				++m_current;
				if (*m_current == '-')
				{
					++m_current;
					if (*m_current == '>')
					{
						++m_current;
						return;
					}
					else
					{
						XML_ERROR_LINE(
							XMLParserError::MINUS_MINUS_IN_COMMENT, m_line);
					}
				}
			}
		}
		// If it got this far, then the comment is unterminated:
		XML_ERROR_LINE(XMLParserError::UNTERMINATED_COMMENT, m_line);
	}
	void XMLParserCore::getCData(XMLToken& entry)
	{
		// At this point m_current points one past "<![CDATA[" sequence:
		entry.text.erase();
		for (; *m_current; m_current++)
		{
			if (*m_current == ']')
			{
				++m_current;
				if (*m_current == ']')
				{
					++m_current;
					if (*m_current == '>')
					{
						++m_current;
						return;
					}
					else
					{
						entry.text += ']';
						entry.text += ']';
					}
				}
				else
				{
					entry.text += ']';
				}
			}
			if (*m_current == '\n')
			{
				++m_line;
			}
			entry.text += *m_current;
		}
		// If it got this far, then the cdata is unterminated:
		XML_ERROR_LINE(XMLParserError::UNTERMINATED_CDATA, m_line);
	}
	void XMLParserCore::getDocType()
	{
		// Just ignore the DOCTYPE command for now.
		// !DOCTYPE can have nested !ELEMENTs and !ATTLISTs.  These need to be
		// skipped when searching for the closing '>'.
		int closing_symbols_required = 1;
		for (; *m_current; ++m_current)
		{
			if (*m_current == '\n')
			{
				++m_line;
			}
			else if (*m_current == '<')
			{
				++closing_symbols_required;
			}
			else if (*m_current == '>')
			{
				--closing_symbols_required;
				if (closing_symbols_required == 0)
				{
					break;
				}
			}
		}
		if (*m_current != '>')
		{
			XML_ERROR_LINE(XMLParserError::UNTERMINATED_DOCTYPE, m_line);
		}
		m_current++;
	}
	//void XMLParserCore::getContent(XMLToken& entry, bool& isWhiteSpace)
	void XMLParserCore::getContent(XMLToken& entry)
	{
		entry.text.erase();
		//isWhiteSpace = true;
		blocxx::IstreamBufIterator end;
		while (m_current != end && *m_current && *m_current != '<')
		{
			if (*m_current == '\n')
			{
				++m_line;
			}
			//isWhiteSpace &= isspace(*m_current);
			//if (isWhiteSpace)
			//{
			//	isWhiteSpace = isspace(*m_current);
			//}
			entry.text += *m_current++;
		}
	}
	void XMLParserCore::getElement(XMLToken& entry)
	{
		entry.attributes.clear();
		entry.text.erase();
		//--------------------------------------------------------------------------
		// Get the element name (expect one of these: '?', '!', [A-Za-z_])
		//--------------------------------------------------------------------------
		if (*m_current == '?')
		{
			entry.type = XMLToken::XML_DECLARATION;
			++m_current;
			if (getElementName(entry, XMLParserError::BAD_XML_DECLARATION))
			{
				return;
			}
		}
		else if (*m_current == '!')
		{
			m_current++;
			// Expect a comment or CDATA:
			if (*m_current == '-')
			{
				++m_current;
				if (*m_current == '-')
				{
					++m_current;
					entry.type = XMLToken::COMMENT;
					getComment();
					return;
				}
			}
			else if (*m_current == '[')
			{
				char string[] = "CDATA[";
				char *curChar = string;
				m_current++;
				while (*curChar)
				{
					if (*curChar++ != *m_current++)
					{
						XML_ERROR_LINE(XMLParserError::EXPECTED_COMMENT_OR_CDATA, m_line);
					}
				}
				entry.type = XMLToken::CDATA;
				getCData(entry);
				return;
			}
			else if (*m_current == 'D')
			{
				char string[] = "OCTYPE";
				char *curChar = string;
				m_current++;
				while (*curChar)
				{
					if (*curChar++ != *m_current++)
					{
						XML_ERROR_LINE(XMLParserError::EXPECTED_COMMENT_OR_CDATA, m_line);
					}
				}
				entry.type = XMLToken::DOCTYPE;
				getDocType();
				return;
			}
			XML_ERROR_LINE(XMLParserError::EXPECTED_COMMENT_OR_CDATA, m_line);
		}
		else if (*m_current == '/')
		{
			entry.type = XMLToken::END_TAG;
			++m_current;
			if (!getElementName(entry, XMLParserError::BAD_END_TAG))
			{
				XML_ERROR_LINE(XMLParserError::BAD_END_TAG, m_line);
			}
			return;
		}
		else if (isalpha(*m_current) || *m_current == '_')
		{
			entry.type = XMLToken::START_TAG;
			bool openCloseElement;
			if (getOpenElementName(entry, openCloseElement))
			{
				if (openCloseElement)
				{
					entry.type = XMLToken::START_TAG;
					m_tagIsEmpty = true;
				}
				return;
			}
		}
		else
			XML_ERROR_LINE(XMLParserError::BAD_START_TAG, m_line);
		//--------------------------------------------------------------------------
		// Grab all the attributes:
		//--------------------------------------------------------------------------
		for (;;)
		{
			skipWhitespace();
			if (entry.type == XMLToken::XML_DECLARATION)
			{
				if (*m_current == '?')
				{
					++m_current;
					if (*m_current == '>')
					{
						++m_current;
						return;
					}
					else
					{
						XML_ERROR_LINE_MSG(
							XMLParserError::BAD_ATTRIBUTE_VALUE, m_line,
							stringPrint("Expecting >; Got %c", *m_current));
					}
				}
			}
			else if (entry.type == XMLToken::START_TAG && *m_current == '/')
			{
				++m_current;
				if (*m_current =='>')
				{
					entry.type = XMLToken::START_TAG;
					m_tagIsEmpty = true;
					++m_current;
					return;
				}
				else
				{
					XML_ERROR_LINE_MSG(XMLParserError::BAD_ATTRIBUTE_VALUE,
						m_line, stringPrint("Expecting >; Got %c", *m_current));
				}
			}
			else if (*m_current == '>')
			{
				++m_current;
				return;
			}
			entry.attributes.push_back(XMLToken::Attribute());
			XMLToken::Attribute& attr = *entry.attributes.rbegin();
			getAttributeNameAndEqual(attr);
			getAttributeValue(attr);
		}
	}

} // end namespace XML_NAMESPACE

