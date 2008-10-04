#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "debug.h"
#include "charconv.h"
#include "worldobject.h"
#include "damage.h"
#include <algorithm>
#include <math.h>
#include "event.h"
#include <list>

class World;

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
	 * \enum Flags
	 * \brief Zaehlt Sondereigenschaften von Geschossen auf
	 */
	enum Flags
	{
		EXPLODES = 1,
		MULTI_EXPLODES = 2,
		PIERCING = 4,
		BOUNCING = 8,
		PROB_BOUNCING = 16
		
	};
	
	/**
	 * \fn Projectile(World* w, ProjectileType type, WorldObject::TypeInfo::Fraction fr, int id)
	 * \param type Typ des Projektils
	 * \brief Konstruktor
	 */
	Projectile(World* w, ProjectileType type, WorldObject::TypeInfo::Fraction fr, int id);
	
/**
	 * \fn virtual update ( float time)
	 * \brief Aktualisiert das Projektil, nachdem eine bestimmte Zeit vergangen ist. Alle Aktionen des Objekts werden auf diesem Weg ausgeloest. 
	 * \param time Menge der vergangenen Zeit in Millisekunden
	 * \return bool, der angibt, ob die Aktualisierung fehlerfrei verlaufen ist
 */
	virtual  bool  update ( float time);

	
	/**
	 * \fn void setSpeedX(float sx)
	 * \brief Setzt die Geschwindigkeit in x-Richtung
	 * \param sx Geschwindigkeit in x-Richtung
	 */
	void setSpeedX(float sx)
	{
		m_speed_x = sx;
		if (m_speed_y!=0 || m_speed_x !=0)
		{
			m_geometry.m_angle = atan2(m_speed_y,m_speed_x);
		}
	}
	
	/**
	 * \fn void setSpeedY(float sy)
	 * \brief Setzt die Geschwindigkeit in Y-Richtung
	 * \param sy Geschwindigkeit in y-Richtung
	 */
	void setSpeedY(float sy)
	{
		m_speed_y = sy;
		if (m_speed_y!=0 || m_speed_x !=0)
		{
			m_geometry.m_angle = atan2(m_speed_y,m_speed_x);
		}
	}
	
	/**
	 * \fn short getRegion()
	 * \brief Gibt die Region aus, in der das Projektil sich befindet
	 * \return Region
	 */
	short getRegion()
	{
		return m_region;
	}
	
	/**
	 * \fn void setRegion(short r)
	 * \brief setzt die Region
	 * \param r Region
	 */
	void setRegion(short r)
	{
		m_region=r;
	}
	
	/**
	 * \fn void setCounter(short cnt)
	 * \brief Setzt den internen Zaehler
	 * \param cnt Zaehler
	 */
	void setCounter(short cnt)
	{
		m_counter = cnt;
	}
	
	/**
	 * \fn void setMaxRadius(float mr)
	 * \brief setzt den maximalen Radius
	 * \param mr maximaler Radius
	 */
	void setMaxRadius(float mr)
	{
		m_max_radius = mr;
	}
		
		
	
	/**
	 * \fn void setFlags(char f)
	 * \brief Setzt die Flags
	 * \param f Flags
	 */
	void setFlags(char f)
	{
		m_flags = f;
	}
	
	/**
	 * \fn Damage* getDamage()
	 * \brief Gibt Struktur mit Schadensinformationen aus
	 * \return Damage Objekt
	 * 
	 */
	Damage* getDamage()
	{
		return &m_damage;
	}
	
	/**
	 * \fn void setGoalObject(int id)
	 * \brief Setzt das ZielObjekt
	 */
	void setGoalObject(int id)
	{
		m_goal_object = id;
	}
	
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
	 * \fn virtual void writeEvent(Event* event, CharConv* cv)
	 * \brief Schreibt die Daten zu einem Event in den Bitstream
	 * \param event Event das beschrieben wird
	 * \param cv Bitstream
	 */
	virtual void writeEvent(Event* event, CharConv* cv);
	
	/**
	 * \fn virtual void processEvent(Event* event, CharConv* cv)
	 * \brief Fuehrt die Wirkung eines Events auf das Objekt aus. Weitere Daten werden aus dem Bitstream gelesen
	 */
	virtual void processEvent(Event* event, CharConv* cv);
	
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
	 * \fn void handleFlying(float dtime)
	 * \brief Fuehrt update fuer Objekt im Zustand FLYING aus
	 */
	void handleFlying(float dtime);
		
	/**
	 * \fn void handleFlying(float dtime)
	 * \brief Fuehrt update fuer Objekt im Zustand GROWING aus
	 */
		void handleGrowing(float dtime);
	
	/**
	 * \fn void handleFlying(float dtime)
	 * \brief Fuehrt update fuer Objekt im Zustand STABLE aus
	 */
	void handleStable(float dtime);
	
	/**
	 * \var float m_speed_x
	 * \brief Geschwindigkeit in x-Richtung
	 */
	float m_speed_x;
	
	/**
	 * \var float m_speed_y
	 * \brief Geschwindigkeit in y-Richtung
	 */
	float m_speed_y;
	
	/**
	 * \var short m_region
	 * \brief Region in der sich das Projektil befindet
	 */
	short m_region;
	
	/**
	 * \var Damage m_damage
	 * \brief Schaden den das Projektil anrichtet
	 */
	Damage m_damage;
	
	/**
	 * \var World* m_world
	 * \brief Zeiger auf die Welt
	 */
		World* m_world;
	
	/**
	 * \var int m_last_hit_object_id
	 * \brief Zeiger auf das zuletzt getroffene Objekt
	 */
	int m_last_hit_object_id;
	
	/**
	 * \var int m_creator
	 * \brief Erzeuger des Geschosses (wird nicht getroffen)
	 */
	WorldObject::TypeInfo::Fraction m_creator_fraction;
	
	/**
	 * \var char m_flags
	 * \brief einige Einstellungen
	 */
	char m_flags;
	
	/**
	 * \var float m_max_radius
	 * \brief maximaler Radius den das Objekt erreicht
	 */
	float m_max_radius;
	
	/**
	 * \var int m_goal_object
	 * \brief Zielobjekt
	 */
	int m_goal_object;
		
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

typedef std::map<int,Projectile*> ProjectileMap;

#include "world.h"


#endif
