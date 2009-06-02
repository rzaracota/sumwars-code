#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "debug.h"
#include "charconv.h"
#include "worldobject.h"
#include "damage.h"
#include <algorithm>
#include <math.h>
#include "networkstruct.h"
#include <list>
#include "gameobject.h"

class World;

/**
 * \class Projectile
 * \brief Klasse fuer bewegliche Gegenstaende und Erscheinungen
 */
class Projectile : public GameObject
{
	public:

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
	 * \fn Projectile( ProjectileType type, Damage* dmg,  int id=0)
	 * \param subtype Typ des Projektils
	 * \param dmg Schaden den das Projektil anrichtet
	 * \param id ID des Projektils
	 * \brief Konstruktor
	 */
	Projectile( Subtype subtype, Damage* dmg, int id=0);
	
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
	 * \fn Fraction getFraction()
	* \brief Gibt die Fraktion aus
	*/
	Fraction getFraction()
	{
		return m_fraction;
	}
	
	/**
	 * \fn void setFraction(Fraction fr)
	 * \brief setzt die Fraktion
	 * \param fr Fraktion
	 */
	void setFraction(Fraction fr)
	{
		m_fraction = fr;
	}
	
	/**
	 * \fn bool virtual getDestroyed()
	 * \brief Gibt zurueck ob das Objekt zerstoert ist
	 */
	virtual bool getDestroyed()
	{
		return getState() == STATE_DESTROYED;
	}
	
		/**
	 * \fn std::string getActionString()
	 * \brief Gibt die aktuelle Aktion als String aus
		 */
	virtual std::string getActionString();
	
		
		/**
	 * \fn virtual float getActionPercent()
	 * \brief Gibt den Prozentsatz, zu dem die aktuelle Aktion fortgeschritten ist aus
		 */
	virtual float getActionPercent();
	
	/**
	 * \fn virtual void getFlags(std::set<std::string>& flags)
	 * \brief Gibt den Status der bekannten Flags aus
	 * \param flags Ausgabeparameter: Flags
	 */
	virtual void getFlags(std::set<std::string>& flags);
	
	
	protected:
		
		/**
		 * \fn void setTimerLimit(float limit)
		 * \brief Setzt das Timer Limit
		 * \param limit neues Limit
		 */
		void setTimerLimit(float limit)
		{
			m_timer_limit = limit;
			addToNetEventMask(NetEvent::DATA_TIMER);
		}
		
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
		 * \var Fraction m_fraction
		 * \brief Fraktion des Objektes
		 */
		Fraction m_fraction;

};

/**
 * Liste von Projektilen sortiert nach ihrer ID
 */
typedef std::map<int,Projectile*> ProjectileMap;



#endif
