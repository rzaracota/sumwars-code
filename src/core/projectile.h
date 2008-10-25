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
	 * \fn Projectile( ProjectileType type, WorldObject::Fraction fr, int id)
	 * \param type Typ des Projektils
	 * \param fr Fraktion des Spielers der das Projektil erzeugt hat
	 * \param id ID des Projektils
	 * \brief Konstruktor
	 */
	Projectile( ProjectileType type, WorldObject::Fraction fr, int id);
	
	/**
	 * \fn virtual ~Projectile()
	 * \brief Destruktor
	 */
	virtual ~Projectile()
	{
		
	}
	
/**
	 * \fn virtual update ( float time)
	 * \brief Aktualisiert das Projektil, nachdem eine bestimmte Zeit vergangen ist. Alle Aktionen des Objekts werden auf diesem Weg ausgeloest. 
	 * \param time Menge der vergangenen Zeit in Millisekunden
	 * \return bool, der angibt, ob die Aktualisierung fehlerfrei verlaufen ist
 */
	virtual  bool  update ( float time);

	
	/**
	 * \fn void setSpeed(Vector speed)
	 * \brief Setzt die Geschwindigkeit des Projektils
	 * \param speed Geschwindigkeit
	 */
	void setSpeed(Vector speed)
	{
		m_speed = speed;
		getShape()->m_angle = m_speed.angle();
	}
	
	/**
	 * \fn Vector getSpeed()
	 * \brief Gibt die Geschwindigkeit aus
	 */
	Vector getSpeed()
	{
		return m_speed;
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
	
	Region* getRegionPtr();
	
	
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
	 * \fn virtual void toString(CharConv* cv)
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
	 * \fn Shape* getShape()
	 * \brief Gibt die Form des Projektils aus
	 */
	Shape* getShape()
	{
		return &m_shape;
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
	 * \brief Gibt die Bitmaske der Events aus
	 */
	int getEventMask()
	{
		return m_event_mask;
	}
	
	/**
	 * \fn void clearEventMask()
	 * \brief Setzt die Bitmaske der Events auf 0 
	 */
	void clearEventMask()
	{
		m_event_mask =0;
	}
	
	
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
		 * \var Vector m_speed
		 * \brief Geschwindigkeit, mit der sich das Projektil bewegt
		 */
		Vector m_speed;
		
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
		* \var int m_last_hit_object_id
		* \brief Zeiger auf das zuletzt getroffene Objekt
		*/
		int m_last_hit_object_id;
		
		/**
		* \var WorldObject::Fraction m_creator_fraction
		* \brief Fraktion des Erzeugers des Geschosses (wird nicht getroffen)
		*/
		WorldObject::Fraction m_creator_fraction;
		
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
		 * \var Shape m_shape
		 * \brief Form des Projektils
		 */
		Shape m_shape;
		
		/**
		 * \var short m_layer
		 * \brief Ebene
		 */
		short m_layer;
		
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

/**
 * Liste von Projektilen sortiert nach ihrer ID
 */
typedef std::map<int,Projectile*> ProjectileMap;



#endif
