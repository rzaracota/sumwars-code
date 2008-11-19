#ifndef DROP_ITEM_H
#define DROP_ITEM_H

#include "item.h"
#include "itemlist.h"
#include <sstream>
#include <map>
#include "random.h"

/**
 * \struct DropItem
 * \brief Wrapper Struktur fuer Gegenstaende die auf dem Boden liegen
 */
class DropItem
{
	public:
		/**
		* \fn DropItem()
		* \brief Konstruktor
		*/
		DropItem();
		
		/**
		* \var Item* m_item
		* \brief Zeiger auf das Item
		*/
		Item* m_item;
		
		/**
		* \var short m_x
		* \brief Position (x-Richtung) in 0.5x0.5 Feldern
		*/
		short m_x;
	
		/**
		* \var short m_y
		* \brief Position (y-Richtung) in 0.5x0.5 Feldern
		*/
		short m_y;
		
		/**
		* \var float m_height
		* \brief Hoehe in der sich der Gegenstand befindet
		*/
		float m_height;
	
		/**
		* \var float m_angle_z
		* \brief Drehwinkel mit dem der Gegenstand dargestellt wird
		*/
		float m_angle_z;
		
		/**
		 * \var float m_angle_x
		 * \brief Drehwinkel um die x Achse (beim fallen)
		 */	
		float m_angle_x;
		
		/**
		 * \var float m_time
		 * \brief Zeit die der Gegenstand schon auf dem Boden liegt
		 */
		float m_time;
		
		/**
		* \fn void toString(CharConv* cv)
		* \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
		* \param cv Ausgabepuffer
		*/
		void toString(CharConv* cv);
				
				
		/**
		* \fn void fromString(CharConv* cv)
		* \brief Erzeugt das Objekt aus einem String
		* \param cv Eingabepuffer
		*/
		void fromString(CharConv* cv);
		
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
		
		/**
		* \brief aktualisiert das Dropitem, nach eine bestimmte Zeitspanne vergangen ist
		* \param time Zeitspanne
		*/
		void update(float time);
		
	private:
		/**
		* \var float m_speed
		* \brief Fallgeschwindigkeit
		*/
		float m_speed;
	
		
		
};

typedef std::map<int,DropItem*> DropItemMap;

#endif
