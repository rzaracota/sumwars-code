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
	Waypoint(int id);
	
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
};

#endif
