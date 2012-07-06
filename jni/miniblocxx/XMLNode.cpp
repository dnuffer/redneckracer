#include "XMLNode.hpp"
#include "XMLEscape.hpp"
#include "StringBuffer.hpp"

namespace XML
{
	XMLNodeImpl::XMLNodeImpl(const std::string& name)
		: m_name(name)
	{
	}

	XMLNode::XMLNode()
		: m_impl() // null impl
	{
	}

	XMLNode::XMLNode(const std::string& name)
		: m_impl(new XMLNodeImpl(name))
	{
	}

	XMLNode::XMLNode(const std::string& name, const XMLAttributeArray& attrs)
		: m_impl(new XMLNodeImpl(name))
	{
		m_impl->m_attributes = attrs;
	}

	std::string XMLNode::getName() const
	{
		return m_impl->m_name;
	}

	XMLAttributeArray XMLNode::getAttributes() const
	{
		return m_impl->m_attributes;
	}

	std::string XMLNode::getText() const
	{
		return m_impl->m_text;
	}

	void XMLNode::setText(const std::string& text)
	{
		if( !hasChildren() )
		{
			m_impl->m_text = text;
		}
		else
		{
			// THROW?
		}
	}

	void XMLNode::appendText(const std::string& text) const
	{
		if( !hasChildren() )
		{
			m_impl->m_text += text;
		}
		else
		{
			// THROW?
		}
	}

	bool XMLNode::hasText() const
	{
		return (m_impl->m_children.empty()) && !(m_impl->m_text.empty());
	}
	bool XMLNode::hasChildren() const
	{
		return !m_impl->m_children.empty();
	}

	bool XMLNode::getAtrribute(const std::string& name, XMLAttribute& attr) const
	{
		for( XMLAttributeArray::const_iterator iter = m_impl->m_attributes.begin();
			  iter != m_impl->m_attributes.end();
			  ++iter )
		{
			if( iter->getName() == name )
			{
				attr = *iter;
				return true;
			}
		}
		return false;
	}

	void XMLNode::addChild(const XMLNode& node)
	{
		if( !hasText() )
		{
			m_impl->m_children.push_back(node);
		}
		else
		{
			// THROW?
		}
	}

	void XMLNode::removeChild(const XMLNode& node)
	{
		for( XMLNodeArray::iterator iter = m_impl->m_children.begin();
			  iter != m_impl->m_children.end();
			  ++iter )
		{
			if( iter->m_impl == node.m_impl )
			{
				m_impl->m_children.erase(iter);
				return;
			}
		}
	}

	XMLNode XMLNode::findChild(const std::string& name) const
	{
		for( XMLNodeArray::const_iterator iter = m_impl->m_children.begin(); iter != m_impl->m_children.end(); ++iter )
		{
			if( iter->getName() == name )
			{
				return *iter;
			}
		}
		return XMLNode();
	}

	void XMLNode::addAttribute(const std::string& name, const std::string& value)
	{
		m_impl->m_attributes.push_back(XMLAttribute(name, value));
	}

	void XMLNode::addAttribute(const XMLAttribute& attr)
	{
		m_impl->m_attributes.push_back(attr);
	}

	void XMLNode::removeAttribute(const std::string& name)
	{
		for( XMLAttributeArray::iterator iter = m_impl->m_attributes.begin();
			  iter != m_impl->m_attributes.end();
			  ++iter )
		{
			if( iter->getName() == name )
			{
				m_impl->m_attributes.erase(iter);
				return;
			}
		}
	}

	std::string XMLNode::toString() const
	{
		if( !m_impl )
		{
			return "NULL NODE!!!";
		}

		blocxx::StringBuffer result;

		result += "<";
		result += m_impl->m_name;

		for( XMLAttributeArray::const_iterator iter = m_impl->m_attributes.begin(); iter != m_impl->m_attributes.end(); ++iter )
		{
			result += ' ' + iter->getName() + "=\"" + XMLEscape(iter->getValue()) + '"';
		}

		if( m_impl->m_children.empty() && m_impl->m_text.empty() )
		{
			result += " />\n";
		}
		else
		{
			result += ">";

			if( !m_impl->m_children.empty() )
			{
				result += "\n";
				for( XMLNodeArray::const_iterator iter = m_impl->m_children.begin(); iter != m_impl->m_children.end(); ++iter )
				{
					result += iter->toString();
				}
			}
			else if( !m_impl->m_text.empty() )
			{
				result += XMLEscape(m_impl->m_text);
			}

			result += "</" + m_impl->m_name + ">\n";
		}
		return result.toString();
	}
}
