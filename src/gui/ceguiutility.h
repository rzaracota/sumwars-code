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

#ifndef __SUMWARS_GUI_CEGUIUTILITY_H__
#define __SUMWARS_GUI_CEGUIUTILITY_H__

// Define the CEGUI version. TODO: move to a global definition.
#define CEGUI_07

#ifdef CEGUI_07
#include "CEGUI.h"
#else
#include "cegui/CEGUI.h"
#endif

#include "CEGUI/CEGUIUDim.h"
#include "CEGUI/CEGUIFont.h"
#include <list>
#include <string>
#include <OIS.h>

struct FormatedText
{
public:
	CEGUI::String text;
	size_t lines;
};

#define LINE_ENDING "\n"
#define ERASE_CNT 1

/**
	A class to be used as a namespace placeholder for most CEGUI functionality.
	This is to alleviate some of the issues caused by a change in the API syntax between various versions of CEGUI.

*/

class CEGUIUtility
{
	public:

		/*!
		\brief
		Enumerated type that contains valid formatting types that can be specified when rendering text into a Rect area (the formatting Rect).
		*/
		enum TextFormatting
		{
			/// All text is printed on a single line.  The left-most character is aligned with the left edge of the formatting Rect.
			LeftAligned,
			/// All text is printed on a single line.  The right-most character is aligned with the right edge of the formatting Rect.
			RightAligned,
			/// All text is printed on a single line.  The text is centred horizontally in the formatting Rect.
			Centred,
			/// All text is printed on a single line.  The left-most and right-most characters are aligned with the edges of the formatting Rect.
			Justified,
			/// Text is broken into multiple lines no wider than the formatting Rect.  The left-most character of each line is aligned with the left edge of the formatting Rect.
			WordWrapLeftAligned,
			/// Text is broken into multiple lines no wider than the formatting Rect.  The right-most character of each line is aligned with the right edge of the formatting Rect.
			WordWrapRightAligned,
			/// Text is broken into multiple lines no wider than the formatting Rect.  Each line is centred horizontally in the formatting Rect.
			WordWrapCentred,
			/// Text is broken into multiple lines no wider than the formatting Rect.  The left-most and right-most characters of each line are aligned with the edges of the formatting Rect.
			WordWrapJustified
		};

		enum Colours
		{
			Red = 1,
			Green,
			Blue,
			White,
			Black,
		};
		
		/**
		* \fn static CEGUI::UVector2 getWindowSizeForText(std::list<std::string> list, CEGUI::Font *font, std::string &added)
		* \brief Calculates the onscreen size of a text
		* \param list A std::list with each line of the text separated
		* \param font the CEGUI::Fonts thats used
		* \param added The complete list joined together as one string is writen into this argument
		* \returns The size of the text as a CEGUI::UVector2
		*/
		static CEGUI::UVector2 getWindowSizeForText(std::list<std::string> list, CEGUI::Font *font, std::string &added);

		/**
		* \fn static CEGUI::UVector2 getWindowSizeForText(std::string text, CEGUI::Font *font)
		* \brief Calculates the size of a text
		* \param list The text as a std::string
		* \param font the CEGUI::Fonts thats used
		* \returns The size of the text as a CEGUI::UVector2
		*/
		static CEGUI::UVector2 getWindowSizeForText(std::string text, CEGUI::Font *font);

		/**
		* \fn static std::list<std::string> getTextAsList(std::string text)
		* \brief Returns the input text as a std::list<std::string> separated by line breaks
		* \param list The text as a std::string
		* \returns The input text as a stringlist
		*/
		static std::list<std::string> getTextAsList(const std::string &text);
		
		/**
		* \fn static std::string stripColours(const std::string &input)
		* \brief Removes CEGUI colour tags from the string
		* \param Input text
		* \returns Input text without the colour information
		*/
		static std::string stripColours(const std::string &input);

		/**
		* \fn static std::string getStdColourAsString(std::string &col)
		* \brief Returns a string with the colour formated for use with CEGUI
		* \param Input Colour name
		* \returns A string with the colour formated for use with CEGUI
		*/
		static std::string getStdColourAsString(int col);

		/**
		* \fn static std::string getStdColourAsString(std::string &col)
		* \brief Colourizes a text string with the given colours
		* \param colour The colour
		* \param text The text to colourise
		* \param colourAfter colour after the string
		* \returns the input string formated with the colour for use with CEGUI
		*/
		static std::string getColourizedString(int colour, std::string text, int colourAfter);

		/**
		* \fn static size_t fitTextToWindow (const CEGUI::String& text, const CEGUI::Rect& format_area,TextFormatting fmt, CEGUI::Font *font, float x_scale = 1.0f, CEGUI::String& out_string = "")
		* \brief Returns the number of lines the text will have within a given CEGUI window
		* \param text Fits the text to a window
                * \param maxWidth The maximum width
		* \param fmt The text format
		* \param font The CEGUI::Font
		* \param x_scale Optional text scale
		* \returns the formated text and the line count
		*/
		static FormatedText fitTextToWindow (const CEGUI::String& text, float maxWidth, TextFormatting fmt, CEGUI::Font *font, float x_scale = 1.0f);
		

		static CEGUI::Window* getWindow (const CEGUI::String& name);
		static CEGUI::Window* getWindowForSystem (CEGUI::System* sys, const CEGUI::String& name);

		static void injectKeyDown (OIS::KeyCode myKey);
		static void injectKeyDown (CEGUI::System* sys, OIS::KeyCode myKey);

		static void injectKeyUp (OIS::KeyCode myKey);
		static void injectKeyUp (CEGUI::System* sys, OIS::KeyCode myKey);

		static void injectChar (int myKey);
		static void injectChar (CEGUI::System* sys, int myKey);
		
		static void injectMousePosition (float x, float y);
		static void injectMousePosition (CEGUI::System* sys, float x, float y);

		static void injectMouseWheelChange (float pos);
		static void injectMouseWheelChange (CEGUI::System* sys, float pos);

		static void injectMouseButtonDown (OIS::MouseButtonID btn);
		static void injectMouseButtonDown (CEGUI::System* sys, OIS::MouseButtonID btn);

		static void injectMouseButtonUp (OIS::MouseButtonID btn);
		static void injectMouseButtonUp (CEGUI::System* sys, OIS::MouseButtonID btn);

		static CEGUI::MouseButton convertOISButtonToCegui (int buttonID);

		// Make sure that the FULL widget name (including path) can be used easily in CEGUI_07 as well.
		static std::string getNameForWidget (const std::string& name);

		// Retrieve the widget with a given skin.
		static std::string getWidgetWithSkin (const std::string& skin, const std::string& name);

		static void setScrollPositionForWidget (const CEGUI::String& widgetName, float newScrollPosition);


		static void setDefaultFont (const CEGUI::String& fontName);

		static void setDefaultMouseCursor (CEGUI::System* sys, const std::string& skinName, const std::string& cursorName);
		static void setDefaultTooltip (CEGUI::System* sys, const std::string& skinName, const std::string& cursorName);

private:
		/**
		* \fn static const size_t getNextWord(const CEGUI::String& in_string, size_t start_idx, CEGUI::String& out_string)
		* \brief 
		* \param in_string String to search
		* \param start_idx The start index
		* \param out_string The word get written into this string
		* \returns The new index
		*/
		static const size_t getNextWord(const CEGUI::String& in_string, size_t start_idx, CEGUI::String& out_string);
		
};

#endif // __SUMWARS_GUI_CEGUIUTILITY_H__
