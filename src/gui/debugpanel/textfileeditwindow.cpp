#include "textfileeditwindow.h"
#include "CEGUI/CEGUI.h"
#include <iostream>
#include <fstream>
#include <string>

#define TAB "\267\267\267"

using namespace CEGUI;

CEGUI::String TextFileEditWindow::WidgetTypeName = "TextFileEditWindow";

TextFileEditWindow::TextFileEditWindow(const CEGUI::String& type, const CEGUI::String& name): Window(type, name)
{
	m_textEditBox = static_cast<MultiLineEditbox*>(WindowManager::getSingleton().createWindow("TaharezLook/MultiLineEditbox", name + "EditBox"));
	m_textEditBox->setPosition(UVector2(UDim(0.0f, 0.0f), UDim(0.0f, 0.0f)));
	m_textEditBox->setSize(UVector2(UDim(1.0f, 0.0f), UDim(1.0f, 0.0f)));
	addChildWindow(m_textEditBox);
	m_isDirty = false;
	m_spaceCounter = "";
	m_filePath = "";
	m_fb = 0;
	
	m_textEditBox->subscribeEvent(MultiLineEditbox::EventCharacterKey, Event::Subscriber(&TextFileEditWindow::handleCharacterKey, this));
}

void TextFileEditWindow::close()
{
	
	save();
	
}

void TextFileEditWindow::getNewFileNameForName()
{

}


bool TextFileEditWindow::load(const String &fileName)
{
	m_filePath = fileName;
	std::string s;
	std::string line;
	std::ifstream myfile (fileName.c_str());
	if (myfile.is_open())
	{
		s += '\t';
		while (! myfile.eof() )
		{
			std::getline (myfile,line);
			
			int pos =  line.find_last_of('\t');
			while(pos != line.npos)
			{
				line.erase(pos, 1);
				line.insert(pos, TAB);
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
	m_handleTextChangedConnection = m_textEditBox->subscribeEvent(MultiLineEditbox::EventTextChanged, CEGUI::Event::Subscriber(&TextFileEditWindow::handleTextChanged, this));

	return true;
}

void TextFileEditWindow::save()
{
	if(m_filePath = "")
	{
		getNewFileNameForName();
		return;
	}
	
	if(!m_isDirty)
		return;
	else
	{
		CEGUI::String s = m_textEditBox->getText();
		std::ofstream myfile (m_filePath.c_str());
		if (myfile.is_open())
		{
			int pos =  s.find(TAB);
			while(pos != s.npos)
			{
				s.erase(pos, 3);
				s.insert(pos, "\t");
				pos =  s.find(TAB);
			}
			myfile << s.c_str();
			myfile.close();
		}
		else
		{
			myfile.close();
			return;
		}

		s = getText().c_str();
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

	return true;
}

bool TextFileEditWindow::handleCharacterKey(const CEGUI::EventArgs& ee)
{
	KeyEventArgs e = static_cast<const KeyEventArgs&>(ee);
	
	if(m_spaceCounter.length() == 3)
	{
		// replace spaces here
		replaceSpacesWithMidpoints();
		
		m_spaceCounter = "";
	}
	
	if(e.codepoint == 32)
		m_spaceCounter += " ";
	else
		m_spaceCounter = "";

	return true;
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

void TextFileEditWindow::replaceSpacesWithMidpoints()
{
	CEGUI::String s = m_textEditBox->getText();
	
	size_t pos = s.find("   ");
	s = s.erase(pos, 3);
	s = s.insert(pos, TAB);
	m_textEditBox->setText(s);
}

bool TextFileEditWindow::handleFileBrowserAcceptClicked(const CEGUI::EventArgs& e)
{
	setFilepath(m_fb->getCurrentSelected());
	save();
	m_fb->destroy();
	delete m_fb;
	m_fb = 0;
	return true;
}
