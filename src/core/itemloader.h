/*
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef ITEMLOADER_H
#define ITEMLOADER_H

#include "item.h"
#include "itemfactory.h"
#include <map>
#include "dropslot.h"
#include <list>

#include "../tinyxml/tinyxml.h"
#include <string>
#include <cstring>



/**
 * \class ItemLoader
 * \brief Laedt Items aus einer XML-Datei
 */
class ItemLoader
{
	public:
	
		/**
		* \fn static bool loadItemData(const char* pFilename)
		* \brief Liest die Daten zu Items aus einer XML Datei
		* \param pFilename Name der XML Datei
		*/
		static bool loadItemData(const char* pFilename);
		
		/**
		 * \fn static void loadQuest(TiXmlNode* node)
		 * \brief Laedt die Daten zu einem Item
		 * \param node Knoten, der die Daten enthaelt
		 * \param silent_replace if set to true, existing data is replaced without error message
		 * \return subtype of the created Item
		 */
		static std::string loadItem(TiXmlNode* node, bool silent_replace= false);
	
	private:
	
		/**
		 * \fn static void loadItems(TiXmlNode* node)
		 * \brief Liest die Daten zu Items aus einer XML Datei
		 * \param node XML Knoten
		 */
		static void loadItems(TiXmlNode* node);
	


	
	
};


#endif

