/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Hans Wulf

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <string>
#include <cstring>
#include <sstream>
#include "debug.h"
#include "charconv.h"
#include <map>
#include <list>

#include "geometry.h"
#include "gameobject.h"

class World;
class Region;
struct Damage;


/**
 * \file worldobject.h
 * \defgroup worldobject \Weltobjekt
 * \ingroup world
 * \brief Deklaration des WorldObject
 * \author Hans Wulf
 * \version 1.0
 * \date 2007/05/28
 * \note Bietet der Welt das WorldObject an
 */

/**
 * \struct GridLocation
 * \brief Informationen zur Position eines Objektes in der Grid-Datenstruktur der Welt
 */
struct GridLocation
{
	
	/**
	 * \var m_grid_x
	 * \brief Index des Objektes im Grid in x-Richtung
	 */
	int m_grid_x;
	
	/**
	 * \var m_grid_y
	 * \brief Index des Objektes im Grid in y-Richtung
	 */
	int m_grid_y;
	
	/**
	 * \var int m_index
	 * \brief Index Stelle an der das Objekt zu finden ist
	 */
	int m_index;
};



/**
 * \class WorldObject
 * \brief ein Objekt, welches sich in der Spielwelt befindet
 */
class WorldObject : public GameObject
{

	public:

	/**
	 * \enum Relation
	 * \brief Listet die verschiedenen Beziehungen zwischen Parties auf
	 */
	enum Relation
	{
		NEUTRAL = 0,
		ALLIED = 1,
 		HOSTILE =2
	};

	
	/**
	 * \enum Group
	 * \brief Listet verschiedene Grundtypen von Objekten auf
	 */
	enum Group
	{
		CREATURE_ONLY = 1,
		PLAYER=2,
		CREATURE = 3,
  		FIXED = 4,
  		DEAD = 8,
  		GROUP_ALL = 15
	};
	

	typedef std::string Race;
	
	/**
	 * \enum State
	 * \brief zaehlt Zustaende von Objekten auf
	 */
	

	/**
	 * \enum SpecialFlags
	 * \brief zaehlt Spezialzustaende und -eigenschaften von Objekten auf
	 */
	enum SpecialFlags
	{
		FIRESWORD = 0x1,
		FLAMESWORD = 0x2,
		FLAMEARMOR = 0x4,
		DECOY = 0x8,
		SCARE = 0x10,
		CRIT_HITS = 0x20,
		ICE_ARROWS = 0x40,
		FROST_ARROWS = 0x80,
		WIND_ARROWS = 0x100,
		STORM_ARROWS = 0x200,
		WIND_WALK = 0x400,
		BURNING_RAGE = 0x800,
		STATIC_SHIELD = 0x1000

	};




	//Fields
	//Constructors
	/**
	 * \fn WorldObject(int id)
	 * \brief Konstruktor
	  * \param id ID des WorlObject
	 *
	 * Erstellt ein neues WorldObject
	 */
	WorldObject( int id);

	/**
	 * \fn virtual ~WorldObject()
	 * \brief Destruktor
	 */
	virtual ~WorldObject()
	{
	}
	
	
	/**
	 * \fn GridLocation* getGridLocation()
	 * \brief Gibt den Ort des Objektes im Grid aus
	 * \return Ort des Objektes im Grid
	 */
	GridLocation* getGridLocation()
	{
		return &m_grid_location;
	}
	
	
	//Operations
	/**
	 * \fn virtual bool init ()
	 * \brief Initialisiert das WorldObject. Die Funktion ist virtuell und wird durch die abgeleiteten Klassen &uuml;berschrieben.
	 * \return bool, der angibt ob die initialisierung erfolgreich war
	 */
	virtual  bool  init ()
	{
		return true;
	}
	
	
	/**
	 * \fn virtual bool destroy()
	 * \brief Zerstoert das WorldObject.  Die Funktion ist virtuell und wird von den abgeleiteten Klassen &uuml;berschrieben
	 * \return bool der angibt, ob die Zerstoerung erfolgreich war
	 */
	virtual  bool  destroy ();
	
	/**
	 * \fn bool moveTo(Vector newpos )
	 * \brief Verschiebt das Objekt an einen neuen Ort
	 * \param newpos neue Position
	 * 
	 */
	bool moveTo(Vector newpos);
	
	/**
	 * \fn bool takeDamage(Damage* damage)
	 * \brief Das Objekt nimmt Schaden in der in damage angegebenen Art und Hoehe.
	 * \param damage Schadensart und -hoehe
	 */
	virtual bool takeDamage(Damage* damage);
	
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
	 * \fn virtual bool reactOnUse (int id)
	 * \brief Diese Funktion behandelt die Reaktion eines Worldobject, wenn auf es eine Aktion ausgefuehrt wird.
	 * \param id Id des Spielers, der die Aktion ausfuehrt
	 * \return bool, der angibt, ob die Behandlung der Aktion erfolgreich war
	 */
	virtual  bool  reactOnUse (int id);
	
	/**
	 * \fn virtual bool isLarge()
	 * \brief Gibt an, ob es sich um ein grosses Objekt handelt. Diese werden gesondert behandelt
	 */
	virtual bool isLarge();
	
	

	//Accessor Methods


	
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
	void setFraction(Fraction fr);
	
	/**
	 * \fn Race getRace()
	 * \brief Gibt die Rasse aus
	 */
	Race getRace()
	{
		return m_race;
	}
	
	/**
	 * \fn void setRace(Race race)
	 * \brief Setzt die Kategorie
	 * \param cat neue Kategorie
	 */
	void setRace(Race race);
	
	
	/**
	 * \fn WorldObject::Group getGroup()
	 * \brief Gibt aus zu welcher Gruppierung ein Objekt gehoert
	 */
	WorldObject::Group getGroup()
	{
		if (getType() == "FIXED_OBJECT")
			return WorldObject::FIXED;
		if (getState() == STATE_DEAD || getState() == STATE_DIEING )
			return WorldObject::DEAD;
		return WorldObject::CREATURE;
			
	}

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

	
	
	/**
	 * \fn virtual int getValue(std::string valname)
	 * \brief Schiebt den gewuenschten Attributwert eines Objektes auf den Lua Stack
	 * \param valname Name des Wertes
	 */
	virtual int getValue(std::string valname);
	
	/**
	 * \fn virtual bool setValue(std::string valname)
	 * \brief Setzt den gewuenschten Attributwert eines Objektes
	 * \param valname Name des Wertes
	 */
	virtual bool setValue(std::string valname);
	
	/**
	 * \fn virtual bool isCreature()
	 * \brief Gibt aus, ob es sich um eine Kreatur handelt
	 */
	virtual bool isCreature();

	protected:
		

	/**
	 * \var Race m_race
	 * \brief Kategorie des Objektes
	 */
	Race m_race;

	/**
	 * \var Fraction m_fraction
	 * \brief Fraktion des Objektes
	 */
	Fraction m_fraction;
		
//Private stuff
private:


	/**
	 * \var GridLocation m_grid_location
	 * \brief Ort des Objektes in der internen Struktur
	 */
	
	GridLocation m_grid_location;
	
	
	

	
	
};

#include "damage.h"


/**
 * Liste von WorldObject Zeigern sortiert nach ihrer ID
 */
typedef std::map<int,WorldObject*> WorldObjectMap;
	
/**
 * Liste von WorldObject Zeigern
 */
typedef std::list<WorldObject*> WorldObjectList;
				 
#endif //WORLDOBJECT_H
