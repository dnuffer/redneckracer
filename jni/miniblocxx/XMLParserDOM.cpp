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
#include "miniblocxx/XMLParserDOM.hpp"
#include "miniblocxx/XMLParserSAX.hpp"
#include "miniblocxx/XMLNode.hpp"
#include "miniblocxx/XMLParserCore.hpp" // for XMLToken
#include "miniblocxx/XMLParserError.hpp"

namespace XML
{
	using std::istream;
	using namespace blocxx;

	namespace
	{

		class DOMDocumentBuilder : public ParserSAX::SAXDocumentHandler
		{
		public:
			virtual void endDocument()
			{
			}
			virtual void startElement(const XMLToken &entry)
			{
				XMLAttributeArray newAttrArray;
				unsigned int len = entry.attributes.size();
				for (unsigned int index = 0; index < len; index++)
				{
					std::string nodeName = entry.attributes[index].name;
					std::string nodeValue = entry.attributes[index].value;

					XMLAttribute newAttribute(nodeName, nodeValue);
					newAttrArray.push_back(newAttribute);
				}

				XMLNode newNode(entry.text, newAttrArray);

				if (newNode)
				{
					if (!m_topNode)
					{
						m_topNode = newNode;
					}

					// If there is anything in the array, this is the child of that last guy
					if (m_nodeArray.size() > 0)
					{
						XMLNode parent = m_nodeArray[m_nodeArray.size() - 1];
						parent.addChild(newNode);
					}

					m_nodeArray.push_back(newNode);
				}
			}

			// name is not unescaped (implementation has to do it if necessary)
			virtual void endElement(const std::string &name)
			{
				// at the end of the element, we just need to pop a node
				// off the stack
				m_nodeArray.pop_back();
			}
			// chars is not unescaped (implementation has to do it if necessary)
			virtual void characters(const std::string &chars)
			{
				if (m_nodeArray.size() > 0)
				{
					XMLNode curNode = m_nodeArray[m_nodeArray.size() - 1];
					if (curNode != 0)
					{
						curNode.appendText(chars);
					}
				}
			}
			virtual void startDocument()
			{
			}

			XMLNode getDocument() const
			{
				return m_topNode;
			}

		private :

			XMLNode m_currentNode;
			XMLNode m_topNode;
			XMLNodeArray m_nodeArray;

		};

		class DOMErrorHandler : public ParserSAX::SAXErrorHandler
		{
		public:
			virtual void warning(const XMLParserError &exception)
			{
			}
			virtual void error(const XMLParserError &exception)
			{
				XML::setParserError(exception);
			}
			virtual void fatalError(const XMLParserError &exception)
			{
				XML::setParserError(exception);
			}
		};

	} // end unnamed namespace

	namespace ParserDOM
	{

		XMLNode
		parse(const std::string& xmlData)
		{
			DOMDocumentBuilder docHandler;
			DOMErrorHandler errHandler;
			ParserSAX::parse(xmlData, docHandler, errHandler);
			XMLNode rv = docHandler.getDocument();
			if (!rv)
			{
				std::string message;
				if( haveParserError() )
				{
					message = ": " + getParserError().getText();
				}
				XML_ERROR(XMLParserError::BAD_START_TAG, "No document found" + message);
			}
			return rv;
		}

		XMLNode
		parse(istream& data)
		{
			DOMDocumentBuilder docHandler;
			DOMErrorHandler errHandler;
			ParserSAX::parse(data, docHandler, errHandler);
			XMLNode rv = docHandler.getDocument();
			if (!rv)
			{
				XML_ERROR(XMLParserError::BAD_START_TAG, "No document found");
			}
			return rv;
		}

	} // end namespace XMLParserDOM
} // end namespace XML_NAMESPACE


