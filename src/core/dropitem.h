#ifndef DROP_ITEM_H
#define DROP_ITEM_H

#include "item.h"
#include "itemlist.h"
#include <sstream>
#include <map>

/**
 * \struct DropItem
 * \brief Wrapper Struktur fuer Gegenstaende die auf dem Boden liegen
 */
struct DropItem
{
	/**
	 * \var Item* m_item
	 * \brief Zeiger auf das Item
	 */
	Item* m_item;
	
	/**
	 * \brief short m_x
	 * \brief Position (x-Richtung) in 0.5x0.5 Feldern
	 */
	short m_x;

	/**
	 * \brief short m_y
	 * \brief Position (y-Richtung) in 0.5x0.5 Feldern
	 */
	short m_y;
	
	/**
	 * \var float m_time
	 * \brief Zeit die der Gegenstand schon auf dem Boden liegt
	 */
	float m_time;
	
	/**
	 * \fn void toString(CharConv* cv)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param buf Ausgabepuffer
	 * \return Zeiger hinter den beschriebenen Datenbereich
	 */
	virtual void toString(CharConv* cv);
			
			
	/**
	 * \fn void fromString(CharConv* cv)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param buf Objekt als String
	 * \return Zeiger hinter den gelesenen Datenbereich
	 */
	virtual void fromString(CharConv* cv);
	
	/**
	 * \fn int getId()
	 * \brief Gibt ID des Gegenstandes aus
	 */
	int getId();
	
	/**
	 * \fn std::string getNameId()
	 * \brief Gibt Name es Items mit angehaengter ID aus
	 */
	std::string getNameId();
	
};

typedef std::map<int,DropItem*> DropItemMap;

#endif
