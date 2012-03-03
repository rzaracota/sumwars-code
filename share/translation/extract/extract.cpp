/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Tool for extracting translateable strings from Summoning Wars content files.
 * Copyright 2012 Michael 'Protogenes' Kunz
 * 
 * extract <prefix> [input files]
 * <prefix>: path that will prefix the destination files. Append / to place files in a folder!
 * [input files]: list of files that shall be processed, if missing the list is read from stdin.
 * 
 */


#include <cctype>
#include <functional>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <Poco/DirectoryIterator.h>
#include <Poco/FileStream.h>
#include <Poco/NumberFormatter.h>
#include <Poco/Path.h>
#include <Poco/SAX/Attributes.h>
#include <Poco/SAX/ContentHandler.h>
#include <Poco/SAX/InputSource.h>
#include <Poco/SAX/SAXParser.h>

const std::string cDomain = "sumwars";
typedef std::map<std::string, std::map<std::string, std::vector<std::string> > > TStrings; // domain -> strings -> locations

void addStrings(const std::string & Dom, std::string Str, const std::string & Loc, TStrings & Strings)
{
	if (!Str.empty())
	{
		for (auto i = Str.begin(); i != Str.end(); ++i)
		{ // escape characters
			if (*i == '"' && (i == Str.begin() || *(i-1) != '\\'))
				i = Str.insert(i, '\\');
		}

		Strings[Dom][Str].push_back(Loc);
	}
}

class CXMLHandler : public Poco::XML::ContentHandler
{
	public:
		CXMLHandler(const std::string & Path, std::function<void(const std::string &, const std::string &, const std::string &)> Callback)
			:	m_Path(Path),
				m_Callback(Callback)
		{	
			m_Domains.push(cDomain);
		};

		void startElement(const Poco::XML::XMLString & uri, const Poco::XML::XMLString & localName, const Poco::XML::XMLString & qname, const Poco::XML::Attributes & attrList)
		{
			m_Content.push(std::string());
			if (attrList.getValue("", "domain").empty())
				m_Domains.push(m_Domains.top());
			else
				m_Domains.push(attrList.getValue("", "domain"));
			
			if (qname == "Item"
				|| qname == "Monster"
				|| qname == "Quest"
				|| qname == "Region"
				|| qname == "PlayerClass"
				|| qname == "Ability")
			{
				m_Callback(m_Domains.top(), attrList.getValue("", "name"), getLoc());
			}
			if (qname == "Ability")
			{
				m_Callback(m_Domains.top(), attrList.getValue("", "description"), getLoc());
			}
			if (qname == "NPC")
			{
				m_Callback(m_Domains.top(), attrList.getValue("", "refname"), getLoc());
			}
			if (qname == "Topic")
			{
				m_Callback(m_Domains.top(), attrList.getValue("", "start_option"), getLoc());
			}
		};

		void endElement(const Poco::XML::XMLString & uri, const Poco::XML::XMLString & localName, const Poco::XML::XMLString & qname)
		{
			if (!m_Content.top().empty())
				handleString(m_Domains.top(), m_Content.top(), getLoc());
			m_Content.pop();
			m_Domains.pop();
		};
		void characters(const Poco::XML::XMLChar ch[], int start, int length)
		{
			m_Content.top().append(ch+start, length);
		};
		void ignorableWhitespace(const Poco::XML::XMLChar ch[], int start, int length)
		{
			m_Content.top().append(ch+start, length);
		};

		virtual void processingInstruction(const Poco::XML::XMLString& target, const Poco::XML::XMLString& data)
		{ };
		virtual void startPrefixMapping(const Poco::XML::XMLString& prefix, const Poco::XML::XMLString& uri)
		{ };
		virtual void endPrefixMapping(const Poco::XML::XMLString& prefix)
		{ };
		virtual void skippedEntity(const Poco::XML::XMLString& name)
		{ };
		virtual void startDocument()
		{ };
		virtual void endDocument()
		{ };
		virtual void setDocumentLocator(const Poco::XML::Locator* loc)
		{ m_Locator = loc; };
	private:
		const Poco::XML::Locator * m_Locator;
		std::string m_Path;
		std::function<void(const std::string &, const std::string &, const std::string &)> m_Callback;
		std::stack<std::string> m_Content;
		std::stack<std::string> m_Domains;

		std::string getLoc()
		{
			if (m_Locator)
				return m_Path + ':' + Poco::NumberFormatter::format(m_Locator->getLineNumber());
			else
				return m_Path;
		};

		void handleString(const std::string & Dom, std::string Str, const std::string & Loc)
		{
			auto e = Str.end();
			enum state_t {outside, underscore, parenthesis, inside} state = outside; // doesn't check for closing parenthesis
			char marks = 0;
			std::string buf;
			buf.reserve(Str.size());

			for (auto i = Str.begin(); i < e; ++i)
			{
				if (state == inside || !isspace(*i))
				{
					if (state == outside && *i == '_')
						state = underscore;
					else if (state == underscore && *i == '(')
						state = parenthesis;
					else if (state == parenthesis && (*i == '"' || *i == '\''))
					{
						marks = *i;
						state = inside;
					} else if (state == inside)
					{
						if (*i == marks && *(i-1) != '\\')
						{
							m_Callback(Dom, buf, Loc);
							state = outside;
							buf.clear();
						} else {

							switch (*i)
							{ // unescape characters
								case '\'':  if (marks == '"' && *(i-1) == '\\') *buf.rbegin() = '"'; else buf.push_back(*i); break;
								case '"':   if (marks == '\'' && *(i-1) == '\\') *buf.rbegin() = '\''; else buf.push_back(*i); break;
								default: buf.push_back(*i); break;
							}
						}
					}
				}
			}
		};
};

void handleXML(const Poco::Path & Path, TStrings & Strings)
{
	Poco::FileInputStream file(Path.toString());
	Poco::XML::InputSource source(file);
	CXMLHandler handler(Path.getFileName(), [&](const std::string & Dom, const std::string & Str, const std::string & Loc){
		addStrings(Dom, Str, Loc, Strings);
	});
	Poco::XML::SAXParser parser;
	parser.setContentHandler(&handler);
	parser.parse(&source);
}


void handleFile(const Poco::Path & Path, TStrings & Strings)
{
	try {
		if (Path.getExtension() == "xml")
			handleXML(Path, Strings);
		else
			std::cerr << "Ignored " << Path.toString() << std::endl;
	} catch (std::exception & e)
	{
		std::cerr << Path.toString() << " " << e.what() << std::endl;
	}
}

void handlePath(const Poco::Path & Path, TStrings & Strings)
{
	if (Path.isDirectory())
	{
		Poco::DirectoryIterator e;
		for (auto i = Poco::DirectoryIterator(Path); i != e; ++ i)
		{
			handlePath(i->path(), Strings);
		}

	} else {
		handleFile(Path, Strings);
	}
}

int main(int argc, char ** argv)
{
	if (argc < 2)
	{
		std::cerr << argv[0] << " needs first argument for output directory!" << std::endl;
		return -1;
	}

	TStrings str;
	if (argc > 2)
	{
		for (int i = 2; i < argc; ++i)
			handlePath(argv[i], str);
	} else {
		std::string s;
		do {
			std::getline(std::cin, s);
			handlePath(s, str);
		} while (!s.empty());
	}

	auto de = str.cend();
	for (auto d = str.cbegin(); d != de; ++d)
	{
		std::clog << "Writing to " << argv[1] << d->first << ".pot\n";
		Poco::FileOutputStream fo(argv[1] + d->first + ".pot", std::ios::out | std::ios::app); // append to the files
		auto se = d->second.cend();
		for (auto s = d->second.cbegin(); s != se; ++s)
		{
			auto le = s->second.cend();
			for (auto l = s->second.cbegin(); l != le; ++l)
			{
				fo << "#: " << *l << '\n';
			}
			fo << "msgid  \"" << s->first << "\"\n";
			fo << "msgstr \"\"\n\n";
		}
	}

	return 0;
}
