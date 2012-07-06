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


#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/XMLParserSAX.hpp"
#include "miniblocxx/XMLParserCore.hpp"
#include "miniblocxx/XMLParserError.hpp"
#include "miniblocxx/XMLUnescape.hpp"
#include <iostream>
#include <sstream>

namespace XML
{
	namespace ParserSAX
	{

		/////////////////////////////////////////////////////////////////////////////
		SAXDocumentHandler::~SAXDocumentHandler()
		{
		}

		/////////////////////////////////////////////////////////////////////////////
		SAXErrorHandler::~SAXErrorHandler()
		{
		}

		/////////////////////////////////////////////////////////////////////////////
		void parse(std::istream& istr, SAXDocumentHandler& docHandler, SAXErrorHandler& errHandler)
		{
			XMLParserCore parser(istr);

			clearParserError();

			XMLToken entry;
			docHandler.startDocument();
			while (parser.next(entry) && !haveParserError())
			{
				switch (entry.type)
				{
				case XMLToken::INVALID:
					break;
				case XMLToken::XML_DECLARATION:
					break;
				case XMLToken::START_TAG:
					for (size_t i = 0; i < entry.attributes.size(); ++i)
					{
						entry.attributes[i].value = XMLUnescape(entry.attributes[i].value.c_str(), entry.attributes[i].value.length());
						if( haveParserError() )
							break;
					}
					docHandler.startElement(entry);
					break;
				case XMLToken::END_TAG:
					docHandler.endElement(XMLUnescape(entry.text.c_str(), entry.text.length()));
					break;
				case XMLToken::COMMENT:
					break;
				case XMLToken::CDATA:
					docHandler.characters(entry.text); // cdata isn't escaped
					break;
				case XMLToken::DOCTYPE:
					break;
				case XMLToken::CONTENT:
					docHandler.characters(XMLUnescape(entry.text.c_str(), entry.text.length()));
					break;
				default:
					break;
				}
			}

			if( haveParserError() )
			{
				errHandler.fatalError(getParserError());
			}

			docHandler.endDocument();

		}

		/////////////////////////////////////////////////////////////////////////////
		void
		parse(const std::string& xmlData, SAXDocumentHandler& docHandler, SAXErrorHandler& errHandler)
		{
			std::istringstream tfs(xmlData);
			parse(tfs, docHandler, errHandler);
		}

	} // end namespace XMLParser

} // end namespace XML_NAMESPACE



