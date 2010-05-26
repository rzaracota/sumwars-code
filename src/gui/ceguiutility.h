#ifndef CEGUIUTILITY_H
#define CEGUIUTILITY_H

#include "CEGUIUDim.h"
#include "CEGUIFont.h"
#include <list>
#include <string>

class CEGUIUtility
{
	public:
		/**
		* \fn void updateItemWindow(CEGUI::Window* img, Item* item, Player* player, int gold=-1, )
		* \brief Calculates the size of a text
		* \param list A std::list with each line of the text separated
		* \param font the CEGUI::Fonts thats used
		* \param added The complete list joined together as one string is writen into this argument
		* \returns The size of the window as CEGUI::UVector2
		*/
		static CEGUI::UVector2 getWindowSizeForText(std::list<std::string> list, CEGUI::Font *font, std::string &added);

		/**
		* \fn static std::string stripColors(const std::string &input)
		* \brief Removes CEGUI color tags from the string
		* \param Input text
		* \returns Input text without the color information
		*/
		static std::string stripColors(const std::string &input);
};

#endif // CEGUIUTILITY_H
