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
 */

#include "ceguiutility.h"
#include "CEGUI/CEGUI.h"
#include <iostream>
#include <algorithm>
#include "debug.h"

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
	
    std::string::size_type pos = text.find_first_of(LINE_ENDING);
    while (pos != std::string::npos)
    {
        // ignore new line directly at the beginning of the string
        if (pos != 0)
        {
            l.push_back(temp.substr(0, pos));
            temp.erase(0, pos+ERASE_CNT);
            pos = temp.find(LINE_ENDING);
        }
        else
		{
			temp.erase(0, ERASE_CNT);
			//l.push_back(" ");
			pos = temp.find(LINE_ENDING);
		}
    }
    
    if (temp != "")
	{
		l.push_back(temp);
	}
	
    return l;
}

CEGUI::UVector2 CEGUIUtility::getWindowSizeForText(std::list<std::string> list, CEGUI::Font *font, std::string &added)
{
    using namespace CEGUI;

    float textWidth = 0.0f;
    float textHeight = 0.0f;
    CEGUI::Size screenSize = CEGUI::System::getSingleton().getRenderer()->getDisplaySize();

    added.clear();
	size_t count = 0;
	
    for (std::list<std::string>::iterator iter = list.begin();
		iter != list.end();
		++iter)
    {
		std::string & s = *iter;
		
		if (!s.empty())
		{
			std::string::size_type word_pos = 0;
			while ( word_pos!=std::string::npos )
			{
				word_pos = s.find ( LINE_ENDING, word_pos );
				if ( word_pos != std::string::npos )
				{
					++count;

					// start next search after this word
					word_pos += 2;
				}
			}
			
			added += s + '\n';

			float tempwidth = font->getTextExtent(stripColours(s).c_str()) ;
			if (tempwidth > textWidth)
				textWidth = tempwidth;
		}
    }

	// increase the counter intil we have at least one line break more than we have text
	if (count <= list.size())  
		count = list.size() + 1;

	textHeight = count * (font->getLineSpacing());

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

const size_t CEGUIUtility::getNextWord(const CEGUI::String& in_string, size_t start_idx, CEGUI::String& out_string) 
{
	out_string = CEGUI::TextUtils::getNextWord(in_string, start_idx, CEGUI::TextUtils::DefaultWrapDelimiters);
	
	return out_string.length();
}

FormatedText CEGUIUtility::fitTextToWindow(const CEGUI::String& text, float maxWidth, TextFormatting fmt, CEGUI::Font *font, float x_scale)
{
/*	if ((fmt == LeftAligned) || (fmt == Centred) || (fmt == RightAligned) || (fmt == Justified))
	{
		return std::count(text.begin(), text.end(), static_cast<CEGUI::utf8>('\n')) + 1;
	}
	*/

	CEGUI::String newText((CEGUI::utf8*)text.c_str());
	
	// handle wraping cases
	size_t lineStart = 0, lineEnd = 0;
	CEGUI::String	sourceLine;

	CEGUI::String  whitespace = CEGUI::TextUtils::DefaultWhitespace;
	CEGUI::String	thisLine, thisWord;
	size_t	line_count = 0, currpos = 0;
	
	size_t fullPosCtr = 0;
	
	while (lineEnd < text.length())
	{
		if ((lineEnd = text.find_first_of(LINE_ENDING, lineStart)) == CEGUI::String::npos)
		{
			lineEnd = text.length();
		}
		
		sourceLine = text.substr(lineStart, lineEnd - lineStart);
		lineStart = lineEnd + 1;
		
		// get first word.
		currpos = getNextWord(sourceLine, 0, thisLine);
		
		// while there are words left in the string...
		while (CEGUI::String::npos != sourceLine.find_first_not_of(whitespace, currpos))
		{
			// get next word of the string...
			currpos += getNextWord(sourceLine, currpos, thisWord);
			
			//float davor = (font->getTextExtent(thisLine, x_scale));
			//float danach = (font->getTextExtent(thisLine, x_scale) + font->getTextExtent(thisWord, x_scale));
			
			// if the new word would make the string too long
			if ((font->getTextExtent(thisLine, x_scale) + font->getTextExtent(thisWord, x_scale)) > maxWidth)
			{
				// too long, so that's another line of text
				fullPosCtr += thisLine.size();
				line_count++;
				
				// remove whitespace from next word - it will form start of next line
				thisWord = thisWord.substr(thisWord.find_first_not_of(whitespace));
				
				newText.insert(fullPosCtr+1, LINE_ENDING);
				fullPosCtr += 2;
				
				// reset for a new line.
				thisLine.clear();
			}
			
			// add the next word to the line
			thisLine += thisWord;
		}
		
		fullPosCtr += thisLine.size();
		// plus one for final line
		line_count++;
	}
	FormatedText formt;
	formt.text = newText;
	formt.lines = line_count;
	return formt;
}


//
//	Other utility static functions
//

CEGUI::Window* CEGUIUtility::getWindow (const CEGUI::String& name)
{
	CEGUI::System* ceguiSysPtr_ = CEGUI::System::getSingletonPtr ();
	return getWindowForSystem (ceguiSysPtr_, name);
}

// static
CEGUI::Window* CEGUIUtility::getWindowForSystem (CEGUI::System* sys, const CEGUI::String& name)
{
#ifdef CEGUI_07
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton ();
	if (win_mgr.isWindowPresent (name))
	{
		return win_mgr.getWindow (name);
	}
#else
	CEGUI::Window* root = sys->getDefaultGUIContext().getRootWindow();
	if (root->isChild (name))
	{
		return root->getChild (name);
	}
	else
	{
	}
#endif
	return 0;
}


void CEGUIUtility::injectKeyDown (OIS::KeyCode myKey)
{
	injectKeyDown (CEGUI::System::getSingletonPtr(), myKey);
}

void CEGUIUtility::injectKeyDown (CEGUI::System* sys, OIS::KeyCode myKey)
{
	if (!sys)
	{
		return;
	}
#ifdef CEGUI_07
sys->injectKeyDown (myKey);
#else
sys->getDefaultGUIContext ().injectKeyDown (static_cast<CEGUI::Key::Scan> (myKey));
#endif
}

void CEGUIUtility::injectKeyUp (OIS::KeyCode myKey)
{
	injectKeyUp (CEGUI::System::getSingletonPtr(), myKey);
}


void CEGUIUtility::injectKeyUp (CEGUI::System* sys, OIS::KeyCode myKey)
{
	if (!sys)
	{
		return;
	}
#ifdef CEGUI_07
sys->injectKeyUp (myKey);
#else
sys->getDefaultGUIContext ().injectKeyUp (static_cast<CEGUI::Key::Scan> (myKey));
#endif
}


void CEGUIUtility::injectChar (int myKey)
{
	injectChar (CEGUI::System::getSingletonPtr(), myKey);
}


void CEGUIUtility::injectChar (CEGUI::System* sys, int myKey)
{
	if (!sys)
	{
		return;
	}
#ifdef CEGUI_07
sys->injectChar (myKey);
#else
sys->getDefaultGUIContext ().injectChar (static_cast<CEGUI::Key::Scan> (myKey));
#endif
}


void CEGUIUtility::injectMousePosition (float x, float y)
{
	injectMousePosition (CEGUI::System::getSingletonPtr(), x, y);
}

void CEGUIUtility::injectMousePosition (CEGUI::System* sys, float x, float y)
{
	if (!sys)
	{
		return;
	}
#ifdef CEGUI_07
sys->injectMousePosition (x, y);
#else
sys->getDefaultGUIContext ().injectMousePosition (x, y);
#endif
}

void CEGUIUtility::injectMouseWheelChange (float pos)
{
	injectMouseWheelChange (CEGUI::System::getSingletonPtr(), pos);
}

void CEGUIUtility::injectMouseWheelChange (CEGUI::System* sys, float pos)
{
	if (!sys)
	{
		return;
	}
#ifdef CEGUI_07
sys->injectMouseWheelChange (pos);
#else
sys->getDefaultGUIContext ().injectMouseWheelChange (pos);
#endif
}

void CEGUIUtility::injectMouseButtonDown (OIS::MouseButtonID btn)
{
	injectMouseButtonDown (CEGUI::System::getSingletonPtr(), btn);
}

void CEGUIUtility::injectMouseButtonDown (CEGUI::System* sys, OIS::MouseButtonID btn)
{
	if (!sys)
	{
		return;
	}
#ifdef CEGUI_07
sys->injectMouseButtonDown (convertOISButtonToCegui (btn));
#else
sys->getDefaultGUIContext ().injectMouseButtonDown (convertOISButtonToCegui (btn));
#endif
}

void CEGUIUtility::injectMouseButtonUp (OIS::MouseButtonID btn)
{
	injectMouseButtonUp (CEGUI::System::getSingletonPtr(), btn);
}

void CEGUIUtility::injectMouseButtonUp (CEGUI::System* sys, OIS::MouseButtonID btn)
{
	if (!sys)
	{
		return;
	}
#ifdef CEGUI_07
sys->injectMouseButtonUp (convertOISButtonToCegui (btn));
#else
sys->getDefaultGUIContext ().injectMouseButtonUp (convertOISButtonToCegui (btn));
#endif
}

CEGUI::MouseButton CEGUIUtility::convertOISButtonToCegui (int buttonID)
{
	switch (buttonID)
	{
	case OIS::MB_Left:
		return CEGUI::LeftButton;
	case OIS::MB_Right:
		return CEGUI::RightButton;
	case OIS::MB_Middle:
		return CEGUI::MiddleButton;
	}
	// default:
	return CEGUI::LeftButton;
}


std::string CEGUIUtility::getNameForWidget (const std::string& name)
{
#ifdef CEGUI_07
	int nPos = name.rfind('/');
	if (nPos != CEGUI::String::npos)
	{
		return name.substr (nPos + 1);
	}
#endif
	return name;
}


// Retrieve the widget with a given skin.
std::string CEGUIUtility::getWidgetWithSkin (const std::string& skin, const std::string& name)
{
	std::stringstream ss;
	ss << skin << "/" << name;
	return ss.str ();
}


void CEGUIUtility::setScrollPositionForWidget (const CEGUI::String& widgetName, float newScrollPosition)
{
	CEGUI::Scrollbar* scroller;
	scroller = static_cast<CEGUI::Scrollbar*> (CEGUIUtility::getWindow (widgetName));
	if (scroller)
	{
#ifdef CEGUI_07
		scroller->setScrollPosition (0.5f);
#else
		scroller->setUnitIntervalScrollPosition (0.5f);
#endif
	}
}



void CEGUIUtility::setDefaultFont (const CEGUI::String& fontName)
{
	if (CEGUI::FontManager::getSingleton ().isDefined (fontName))
	{
#ifdef CEGUI_07
		CEGUI::System::getSingletonPtr ()->setDefaultFont ((CEGUI::utf8*)fontName.c_str ());
#else
		CEGUI::System::getSingletonPtr ()->getDefaultGUIContext ().setDefaultFont ((CEGUI::utf8*)fontName.c_str ());
#endif
	}
}


void CEGUIUtility::setDefaultMouseCursor (CEGUI::System* sys, const std::string& skinName, const std::string& cursorName)
{
	sys->setDefaultMouseCursor ((CEGUI::utf8*)skinName.c_str (), (CEGUI::utf8*)cursorName.c_str ());
}

void CEGUIUtility::setDefaultTooltip (CEGUI::System* sys, const std::string& skinName, const std::string& cursorName)
{
	std::stringstream ss;
	ss << skinName << "/" << cursorName;
	sys->setDefaultTooltip (ss.str ().c_str ());
}