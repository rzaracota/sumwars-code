#include "textfileeditwindow.h"
#include "CEGUI.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace CEGUI;

CEGUI::String TextFileEditWindow::WidgetTypeName = "TextFileEditWindow";

TextFileEditWindow::TextFileEditWindow(const CEGUI::String& type, const CEGUI::String& name): Window(type, name)
{
	m_textEditBox = static_cast<MultiLineEditbox*>(WindowManager::getSingleton().createWindow("TaharezLook/MultiLineEditbox", name + "EditBox"));
	m_textEditBox->setPosition(UVector2(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f)));
	m_textEditBox->setSize(UVector2(UDim(1.0f, 0.0f), UDim(1.0f, 0.0f)));
	addChildWindow(m_textEditBox);
	m_isDirty = false;

}

void TextFileEditWindow::close()
{
	save();
	
}


bool TextFileEditWindow::load(const String &fileName)
{
	m_filePath = fileName;
	std::string s;
	std::string line;
	std::ifstream myfile (fileName.c_str());
	if (myfile.is_open())
	{
		while (! myfile.eof() )
		{
			std::getline (myfile,line);
			
			int pos =  line.find_last_of('\t');
			while(pos != line.npos)
			{
				line.erase(pos, 1);
				line.insert(pos, "    ");
				pos =  line.find_last_of('\t');
			}
			
			s += (line + "\n");
		}
	}
	else
	{
		myfile.close();
		return false;
	}
		
	

	#ifdef WIN32
		String::size_type pos = fileName.find_last_of("\\");
	#else
		String::size_type pos = fileName.find_last_of("/");
	#endif

	String name = fileName.substr(pos+1);
	setText(name);
	m_textEditBox->setText(s.c_str());

	// Not in constructor to avoid the first event when initialy seting the Tab text
	m_textEditBox->subscribeEvent(MultiLineEditbox::EventTextChanged, CEGUI::Event::Subscriber(&TextFileEditWindow::handleTextChanged, this));

	return true;
}

void TextFileEditWindow::save()
{
	if(!m_isDirty)
		return;
	else
	{
		String s = getText();
		size_t length = s.length();
		s.erase(length-2, 2);
		setText(s);
	}
}

bool TextFileEditWindow::handleTextChanged(const CEGUI::EventArgs& e)
{
	m_isDirty = true;
	String s = getText();
	
	if(s.find(" *") == s.npos)
		setText(s + " *");
}

void TextFileEditWindow::setFilepath(String path)
{
	m_filePath = path;
	
	#ifdef WIN32
	String::size_type pos = m_filePath.find_last_of("\\");
	#else
	String::size_type pos = m_filePath.find_last_of("/");
	#endif
	
	String name = m_filePath.substr(pos+1);
	setText(name);
}
