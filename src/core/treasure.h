#ifndef TREASURE_H
#define TREASURE_H

#include "treasure.h"
#include "treasurebase.h"
#include "fixedobject.h"

/**
 * \class Treasure
 * \brief Klasse fuer Schaetze (Truhen, Feasser etc) aller Art
 */
class Treasure : public FixedObject
{
	public:
		/**
		* \fn Treasure(int id)
		* \brief Konstruktor
		* \param ID des Objektes
		*/
		Treasure(int id);
		
		/**
		 * \fn Treasure(int id, TreasureBasicData& data)
		 * \brief Initialisiert das Objekt mit den angegebenen Daten
		 * \param ID des Objektes
		 * \param data Basisdaten fuer die Initialisierung
		 */
		Treasure(int id, TreasureBasicData& data);
	
		/**
		 * \fn virtual bool init()
		 * \brief Initialisierung von internen Daten
		 * \return bool Gibt an, ob die Initialisierung erfolgreich war
		 */
		virtual bool init ();
	
		/**
		 * \fn virtual bool  update (float time)
		 * \brief aktualisiert das Objekt nachdem time Millisekunden vergangen sind
		 * \param time vergangene Zeit in Millisekunden
	 	*/
		virtual bool  update (float time);
	
		/**
		 * \fn reactOnUse(int id)
		 * \brief Beschreibt die Reaktion, wenn ein Lebewesen auf es die Aktion benutzen ausführt.  
		 * \param id ist die ID des Lebewesens, welche die Aktion benutzen ausgeführt hat
	 	*/
		virtual bool reactOnUse(int id);
		
		/**
		 * \fn virtual void toString(CharConv* cv)
		 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
		 * \param cv Ausgabepuffer
		 * \return Zeiger hinter den beschriebenen Datenbereich
		 */
		virtual void toString(CharConv* cv);
	
		/**
		 * \fn virtual void fromString(CharConv* cv)
		 * \brief Erzeugt das Objekt aus einem String
		 * \param cv Eingabepuffer
		 * \return Zeiger hinter den gelesenen Datenbereich
		 */
		virtual void fromString(CharConv* cv);
		
		/**
		 * \fn virtual void getFlags(std::set<std::string>& flags)
		 * \brief Gibt den Status der bekannten Flags aus
		 * \param flags Ausgabeparameter: Flags
		 */
		virtual void getFlags(std::set<std::string>& flags);
		
	private:
		/**
	 	* \var DropSlot m_drop_slots[4]
		* \brief Items, die das Objekt droppen kann
		*/
		DropSlot m_drop_slots[4];
		
		/**
		 * \var float m_open_timer
		 * \brief zaehlt die Zeit nach dem oeffnen
		 */
		float m_open_timer;
		
		/**
		 * \var bool m_open
		 * \brief Gibt an, ob der Schatz schon gepluendert wurde
		 */
		bool m_open;
};


#endif

