#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "debug.h"
#include "charconv.h"
#include "worldobject.h"

/**
 * \class Projectile
 * \brief Klasse fuer bewegliche Gegenstaende und Erscheinungen
 */
class Projectile
{
	public:
		
	/**
	 * \struct Geometry
	 * \brief Struktur zum Speichern des Ortes bzw der Lage eines Projektils
	 */
	struct Geometry
	{
		/**
		 * \var m_coordinate_x
		 * \brief x-Koordinate des Objekts (Mittelpunkt)
		 */
		float m_coordinate_x;
	
	/**
		 * \var m_coordinate_y
		 * \brief x-Koordinate des Objekts (Mittelpunkt)
	 */
		float m_coordinate_y;
		
		/**
		 * \var float m_radius
		 * \brief Radius in dem der Effekt sichtbar ist
		 */
		float m_radius;
		
		/**
		 * \var m_angle
		 * \brief Winkel um den das Objekt gegenueber dem Koordinatensystem gedreht ist
		 */
		float m_angle;
		
		/**
		 * \var m_layer
		 * \brief Ebene in der sich das Objekt befindet
		 */
		short m_layer;
	};
	
	/**
	 * \enum ProjectileType
	 * \brief Aufzaehlung verschiedener Typen von Projektilen
	 */
	enum ProjectileType
	{
		ARROW=1,
		MAGIC_ARROW=2,
		FIRE_BOLT=3,
		FIRE_BALL=4,
		FIRE_WALL=5,
  		FIRE_WAVE=6,
		ICE_BOLT = 10,
		BLIZZARD = 11,
		ICE_RING= 12,
		FREEZE =13,
		LIGHTNING = 20,
		THUNDERSTORM = 21,
		CHAIN_LIGHTNING = 23,
		STATIC_SHIELD,
		FIRE_ARROW = 30,
		ICE_ARROW = 31,
		WIND_ARROW = 32,
  		GUIDED_ARROW=34,
		EXPLOSION = 40,
		FIRE_EXPLOSION = 41,
		ICE_EXPLOSION = 42,
		WIND_EXPLOSION = 43,
		LIGHT_BEAM = 50,
		ELEM_EXPLOSION = 51,
		ACID = 52,
		DIVINE_BEAM = 53,
		HYPNOSIS = 54

	};
	 
	/**
	 * \enum ProjectileState
	 * \brief Aufzaehlung der Zustaende die ein Projektil annehmen kann
	 */
	enum ProjectileState
	{
		FLYING = 1,
		EXPLODING = 2,
		GROWING = 3,
		STABLE = 4,
		VANISHING =5,
		DESTROYED= 6
	};

	
	/**
	 * \fn Projectile(ProjectileType type, int id =0)
	 * \param type Typ des Projektils
	 * \brief Konstruktor
	 */
	Projectile( ProjectileType type, int id =0);
	
	/**
	 * \fn Projectile(int id=0)
	 * \brief Konstruktor
	 */
	Projectile(int id=0);

	
	/**
	 * \fn Geometry* getGeometry()
	 * \brief gibt Informationen zur Geometrie aus
	 */
	Geometry* getGeometry()
	{
		return &m_geometry;
	}
	
	/**
	 * \fn ProjectileType getType()
	 * \brief Gibt Typ des Projektils aus
	 */
	ProjectileType getType()
	{
		return m_type;
	}
	
	/**
	 * \fn ProjectileState getState()
	 * \brief Gibt Status des Projektils aus
	 */
	ProjectileState getState()
	{	
		return m_state;
	}
	
	/**
	 * \fn float getTimer()
	 * \brief Gibt Wert des Timers aus
	 */
	float getTimer()
	{
		return m_timer;
	}
	
	/**
	 * \fn float getTimerLimit()
	 * \brief Gibt Wert des Timerlimits aus
	 */
	float getTimerLimit()
	{
		return m_timer_limit;
	}
	
	
	
	
	/**
	 * \fn int getId()
	 * \brief Gibt die ID aus
	 */
	int getId()
	{
		return m_id;
	}
	
	/**
	 * \fn int getEventMask()
	 * \brief Gibt die Bitmaske der Events aus#
	 */
	int getEventMask()
	{
		return m_event_mask;
	}
	
	/**
	 * \fn void clearEventMask()
	 */
	void clearEventMask()
	{
		m_event_mask =0;
	}
	
	/**
	 * \fn virtual void update()
	 * \brief Berechnet die Veraenderungen am Object waerend der letzten time Millisekunden
	 * \param time vergangene Zeit in Millisekunden
	 */
	//virtual void update(float& time) =0;
	
	protected:
		
	/**
	 * \fn Geometry m_geometry
	 * \brief Beschreibung der Geometrie
	 */	
	Geometry m_geometry;
	
	/**
	 * \fn ProjectileType m_type
	 * \brief Typ des Projektils
	 */
	ProjectileType m_type;
			
	/**
	 * \fn ProjectileState m_state
	 * \brief Status des Projektils
	 */
	ProjectileState m_state;
			 
	/**
	 * \fn float m_timer
	 * \brief Timer, Verwendung je nach Art des Projektils und aktuellem Zustand
	 */
	float m_timer;
	
	/**
	 * \fn float m_timer_limit
	 * \brief Wert, bei dessen Erreichen durch den Timer eine Statusveraenderung eintritt
	 */
	float m_timer_limit;
	
	/**
	 * \fn m_counter
	 * \brief Zaehler, Verwendung je Art des Projektils und aktuellem Zustand
	 */
	 int m_counter;
	 
	 /**
	  * \var int m_id
	  * \brief ID des Projektils
	  */
	 int m_id;
	 
	 /**
	  * \var int m_event_mask
	  * \brief Bitmaske mit den beim aktuellen updaten aufgetretenen Events
	  */
	 int m_event_mask;
	 

};





#endif
