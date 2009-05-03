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
		
	
	
	private:
	
		/**
		 * \fn static void loadItems(TiXmlNode* node)
		 * \brief Liest die Daten zu Items aus einer XML Datei
		 * \param node XML Knoten
		 */
		static void loadItems(TiXmlNode* node);
	
		/**
		 * \fn static void loadQuest(TiXmlNode* node)
		 * \brief Laedt die Daten zu einem Item
		 * \param node Knoten, der die Daten enthaelt
		 */
		static void loadItem(TiXmlNode* node);

	
	
};


#endif

