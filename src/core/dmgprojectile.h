#ifndef DMGPROJECTILE_H
#define DMGPROJECTILE_H


#include "projectile.h"
#include "damage.h"
#include "worldobject.h"
#include <algorithm>
#include <math.h>
#include "event.h"

class World;
class ServerWObject;

/**
 * \class DmgProjectile
 * \brief Klasse fuer Projektile, die Schaden anrichten
 */
class DmgProjectile : public  Projectile
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
	 * \fn DmgProjectile(ProjectileType type, WorldObject::TypeInfo::Fraction fr, int id)
	 * \brief Konstruktor
	 * \param type Typ des Projektils
	 * \param fr Fraktion des Urhebers
	 */
	DmgProjectile(World* w, ProjectileType type, WorldObject::TypeInfo::Fraction fr, int id);
			
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
	
	
	private:
		
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
	
};


#include "serverwobject.h"
#include "world.h"


#endif
