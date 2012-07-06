#if !defined(XML_NODE_H_INCLUDED)
#define XML_NODE_H_INCLUDED

#include <string>
#include <vector>

#include "XMLAttribute.h"
#include "Reference.hpp"

namespace XML
{
	class XMLNode;
	typedef std::vector<XMLAttribute> XMLAttributeArray;
	typedef std::vector<XMLNode> XMLNodeArray;

	struct XMLNodeImpl
	{
		XMLNodeImpl(const std::string& name);

		std::string m_name;
		XMLNodeArray m_children;
		XMLAttributeArray m_attributes;
		std::string m_text;
	};

	class XMLNode
	{
	public:
		XMLNode();
		XMLNode(const std::string& name);
		XMLNode(const std::string& name, const XMLAttributeArray& attrs);

		std::string getName() const;
		XMLAttributeArray getAttributes() const;
		std::string getText() const;

		// Cannot set text on nodes that have children.
		void setText(const std::string& text);
		void appendText(const std::string& text) const;

		bool hasText() const;
		bool hasChildren() const;
		bool getAtrribute(const std::string& name, XMLAttribute& attr) const;
		XMLNode findChild(const std::string& name) const;

		// Cannot add children on nodes that have text.
		void addChild(const XMLNode& node);
		void removeChild(const XMLNode& node);

		void addAttribute(const std::string& name, const std::string& value);
		void addAttribute(const XMLAttribute& attr);
		void removeAttribute(const std::string& name);

		std::string toString() const;

		typedef XMLNodeArray::const_iterator const_iterator;
		typedef XMLNodeArray::iterator iterator;
		XMLNodeArray getChildren() const { return m_impl->m_children; }
		const_iterator begin() const { return m_impl->m_children.begin(); }
		const_iterator end() const { return m_impl->m_children.end(); }
		iterator begin() { return m_impl->m_children.begin(); }
		iterator end() { return m_impl->m_children.end(); }

		BLOCXX_SAFE_BOOL_IMPL(XMLNode, blocxx::Reference<XMLNodeImpl>, XMLNode::m_impl, m_impl);

	private:
		blocxx::Reference<XMLNodeImpl> m_impl;
	};
}

#endif // !defined(XML_NODE_H_INCLUDED)
