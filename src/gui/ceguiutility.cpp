#include "ceguiutility.h"
#include "CEGUI.h"
#include <iostream>

std::string CEGUIUtility::stripColours(const std::string &input)
{
    std::string output = input;

    std::string::size_type pos = output.find("[");
    while (pos != std::string::npos)
    {
        output.erase(pos,pos+19);
        pos = output.find("[");
    }

    return output;
}

std::list< std::string > CEGUIUtility::getTextAsList(const std::string &text)
{
    std::list<std::string> l;
    std::string temp = text;

    std::string::size_type pos = text.find_first_of("\n");
    while (pos != std::string::npos)
    {
        // ignore new line directly at the beginning of the string
        if (pos != 0)
        {
            std::string substr = temp.substr(0, pos-1);
            l.push_back(temp.substr(0, pos-1));
            temp.erase(0, pos+1);
            pos = temp.find("\n");
        }
        else
		{
			temp.erase(0, 1);
			//l.push_back(" ");
			pos = temp.find("\n");
		}
    }

    return l;
}

CEGUI::UVector2 CEGUIUtility::getWindowSizeForText(std::list<std::string> list, CEGUI::Font *font, std::string &added)
{
    using namespace CEGUI;

    float textWidth = 0.0f;
    float textHeight = 0.0f;
    Size screenSize = CEGUI::System::getSingleton().getRenderer()->getDisplaySize();

    added = "";

    std::list<std::string>::iterator iter = list.begin();
    while (iter != list.end())
    {
        std::string s = *iter;
        added += s;
        float tempwidth = font->getTextExtent(stripColours(s).c_str()) ;
        if (tempwidth > textWidth)
            textWidth = tempwidth;

        iter++;
    }

    textHeight = list.size() * font->getFontHeight();

    return UVector2(UDim((textWidth/screenSize.d_width) + 0.05f, 0), UDim(textHeight / screenSize.d_height, 0) );
}

CEGUI::UVector2 CEGUIUtility::getWindowSizeForText(std::string text, CEGUI::Font* font)
{
    std::list<std::string> l = getTextAsList(text);
    std::string s;
    return getWindowSizeForText(l, font, s);
}

std::string CEGUIUtility::getStdColourAsString(int col)
{
	switch(col)
	{
		case Red:
			return "[colour='FFFF0000']";
		case Green:
			return "[colour='FF00FF00']";
		case Blue:
			return "[colour='FF0000FF']";
		case White:
			return "[colour='FFFFFFFF']";
		case Black:
			return "[colour='FF000000']";
		default:
			return "";
	}		
}

std::string CEGUIUtility::getColourizedString(int colour, std::string text, int afterColour)
{
	if(!afterColour > 0)
		return getStdColourAsString(colour) + text;
	else
		return getStdColourAsString(colour) + text + getStdColourAsString(afterColour);
}


