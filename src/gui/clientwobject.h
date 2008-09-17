#ifndef CLIENTWOBJECT_H
#define CLIENTWOBJECT_H

#include "../shared/worldobject.h"
#include "../shared/action.h"

/**
 * \class ClientWObject
 * \brief Klasse fuer ein Objekt der Spielwelt auf Clientseite
 */
class ClientWObject : public WorldObject
{
	public:
		
	
	/** \fn ClientWObject(int id)
	 * \brief Konstruktor
	 * \param id ID des Objektes
	 */
	ClientWObject(int id);
	
	/**
	 * \fn virtual ~ClientWObject()
	 * \brief Destruktor
	 */
	virtual ~ClientWObject()
	{
	}
	
	/**
	 * \fn Action* getAction()
	 * \brief Gibt Zeiger auf die aktuelle Aktion zurueck
	 * \return Aktion
	 */
	Action* getAction()
	{
		return &m_action;
	}
	
	/**
	 * \fn float getHealthPerc()
	 * \brief Gibt Prozentsatz der Lebenspunkte von den Maximallebenspunkten aus
	 * \return Prozentsatz Lebenspunkte
	 */
	float getHealthPerc()
	{
		return m_health_perc;
	}
		
	/**
	 * \fn char getStatusMods()
	 * \brief Gibt die Statusveraenderungen aus
	 * \return Statusveraenderungen
	 */
	char getStatusMods()
	{
		return m_status_mods;
	}
	
	/**
	 * \fn string getName()
	 * 
	 * \brief Gibt den Name des Objektes aus
	 * \return Name
	 */
	string getName()
	{
		return m_name;
	}
	
	
	/**
	 * \fn void fromString(CharConv* cv)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param buf Objekt als String
	 * \return Zeiger hinter den gelesenen Datenbereich
	 */
	virtual void fromString(CharConv* cv);
	
		/**
	 	* \var Action m_action
		* \brief Aktion die das Objekt gerade ausfuehrt
		*/
		Action m_action;
		
		/**
		 * \var float m_health_perc
		 * \brief Prozentsatz der Lebenpunkte von den maximalen Lebenspunkten
		 */
		float m_health_perc;
		
		/**
		 * \var char m_status_mods
		 * \brief Statusveraenderungen
		 */
		char m_status_mods;
		
		/**
		 * \var char m_effects
		 * \brief visuelle Effekte in Bitkodierung
		 */
		char m_effects;
		
		/**
		 * \var std::string m_name
		 * \brief Name des Objektes
		 */
		std::string m_name;
};

#endif // CLIENTWOBJECT_H
