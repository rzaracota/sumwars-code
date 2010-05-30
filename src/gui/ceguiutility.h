#ifndef CEGUIUTILITY_H
#define CEGUIUTILITY_H

#include "CEGUIUDim.h"
#include "CEGUIFont.h"
#include <list>
#include <string>

class CEGUIUtility
{

	
	public:

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
};

#endif // CEGUIUTILITY_H
