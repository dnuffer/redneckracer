#if !defined(XML_ATTRIBUTE_H_INCLUDED)
#define XML_ATTRIBUTE_H_INCLUDED

#include <string>
#include <vector>

namespace XML
{
	class XMLAttribute
	{
	public:
		XMLAttribute()
		{
		}
		XMLAttribute(const std::string& name, const std::string& value)
			: m_name(name)
			, m_value(value)
		{
		}
		std::string getName() const
		{
			return m_name;
		}
		std::string getValue() const
		{
			return m_value;
		}
		bool operator==(const XMLAttribute& attr) const
		{
			return (m_name == attr.m_name) && (m_value == attr.m_value);
		}
	private:
		std::string m_name;
		std::string m_value;
	};

	typedef std::vector<XMLAttribute> XMLAttributeArray;
}

#endif // !defined(XML_ATTRIBUTE_H_INCLUDED)


