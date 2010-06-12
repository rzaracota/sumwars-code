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
}

void TextFileEditWindow::load(String fileName)

{
	std::string s;
	std::string line;
	std::ifstream myfile (fileName.c_str());
	if (myfile.is_open())
	{
		while (! myfile.eof() )
		{
			std::getline (myfile,line);
			s += line;
		}
	}
	myfile.close();

	m_textEditBox->setText(s.c_str());
}

void TextFileEditWindow::save()
{

}
