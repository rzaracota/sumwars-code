#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "fixedobject.h"
#include "geometry.h"

/**
 * \struct WaypointInfo
 * \brief Struktur mit Informationen zu einem Wegpunkt
 */
struct WaypointInfo
{
	
	/**
	 * \var std::string m_name
	 * \brief Name des Wegpunktes
	 */
	std::string m_name;
	
	/**
	 * \var short m_id
	 * \brief ID des Wegpunktes (entspricht der RegionId)
	 */
	short m_id;
	
	/**
	 * \var Vector m_world_coord
	 * \brief Ort auf der Weltkarte (in relative Koordinaten)
	 */
	Vector m_world_coord;
	
};

/**
 * \class Waypoint
 * \brief Dieses Objekt stellt einen Ort dar, an dem sich Spieler teleportieren koennen
 */
class Waypoint : public FixedObject
{
	public:
	/**
	 * \fn Waypoint(int id)
	 * \brief Konstruktor
	 * \param ID des Objektes
	 */
	Waypoint(int id=0);
	
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
	 * \fn virtual void getFlags(std::set<std::string>& flags)
	 * \brief Gibt den Status der bekannten Flags aus
	 * \param flags Ausgabeparameter: Flags
	 */
	virtual void getFlags(std::set<std::string>& flags);
	
	/**
	 * \fn virtual void writeNetEvent(NetEvent* event, CharConv* cv)
	 * \brief Schreibt die Daten zu einem NetEvent in den Bitstream
	 * \param event NetEvent das beschrieben wird
	 * \param cv Bitstream
	 */
	virtual void writeNetEvent(NetEvent* event, CharConv* cv);
	
	
		/**
	 * \fn virtual void processNetEvent(NetEvent* event, CharConv* cv)
	 * \brief Fuehrt die Wirkung eines NetEvents auf das Objekt aus. Weitere Daten werden aus dem Bitstream gelesen
		 */
	virtual void processNetEvent(NetEvent* event, CharConv* cv);
	
	/**
	 * \fn void toString(CharConv* cv)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param cv Ausgabepuffer
	 */
	virtual void toString(CharConv* cv);


	/**
	 * \fn virtual void fromString(CharConv* cv)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param cv Eingabepuffer
	 */
	virtual void fromString(CharConv* cv);
	
	private:
		/**
		 * \var m_active 
		 * \brief Legt fest, ob der Wegpunkt als aktiv dargestellt wird
		 */
		bool m_active;
};

#endif
