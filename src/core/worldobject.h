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
#include "netevent.h"
#include <map>
#include <list>

#include "geometry.h"

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
	 * \var m_region
	 * \brief Region in der sich das Objekt befindet
	 */
	short m_region;
	
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
class WorldObject {

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
	
	/**
	 * \enum Layer
	 * \brief TODO
	 */
	enum Layer
	{
		LAYER_BASE =0x01,
		LAYER_AIR = 0x02,
		LAYER_DEAD = 0x04,
		LAYER_ALL = 0x0F,
   
		LAYER_SPECIAL = 0x10,
	};
	
	/**
	 * \struct TypeInfo
	 * \brief Enthaelt alle Informationen zum Typ eines Objektes
	 */
	struct TypeInfo
	{
		/**
		 * \enum ObjectType
		 * \brief Grundtypen von Objekten
		 */
		enum ObjectType
		{
			TYPE_NONE=0,
			TYPE_PLAYER =1,
			TYPE_MONSTER= 2,
			TYPE_TRADER=3,
			TYPE_FIXED_OBJECT=4
		};

		/**
		 * \var ObjectSubtype
		 * \brief genauere Unterteilung von Objekten
		 */
		typedef std::string ObjectSubtype;

		



		/**
		* \var m_type
		* \brief Informationen zum Objekttyp, unter anderem welcher abgeleiteten Klasse das Objekt angeh&ouml;rt
		*/
		ObjectType m_type;

		/**
		* \var m_subtype;
		* \brief Gibt den Untertyp des Objektes an
		*/
		ObjectSubtype m_subtype;

		

	};


	/**
	 * \enum Category
	 * \brief Verschiedene Kategorien von Lebewesen
	 */
	enum Category
	{
		NOCATEGORY =0,
		HUMAN = 0x10,
		DEMON = 0x20,
		UNDEAD = 0x40,
		DWARF = 0x80,
		DRAKE = 0x100,
		FAIRY = 0x200,
		GOBLIN = 0x400,
		ANIMAL = 0x800,
		SUMMONED = 0x1000
	};

	/**
	 * \enum Fraction
	 * \brief Parteien fuer die Lebewesen kaempfen koennen
	*/
	enum Fraction
	{
		NOFRACTION = 0,
		FRAC_HUMAN = 1,
		FRAC_DEMON = 2,
		FRAC_UNDEAD = 3,
		FRAC_DWARF = 4,
		FRAC_SUMMONER = 5,
		FRAC_MONSTER = 6,
		FRAC_HOSTILE_TO_ALL=9,
		FRAC_PLAYER_PARTY = 10,
	};
	
	/**
	 * \enum State
	 * \brief zaehlt Zustaende von Objekten auf
	 */
	enum State
	{
			STATE_INACTIVE=0,
			STATE_ACTIVE =1,
			STATE_DIEING =2,
			STATE_DEAD =3,
			STATE_REGION_DATA_REQUEST=6,
			STATE_REGION_DATA_WAITING=4,
			STATE_ENTER_REGION =5,
			STATE_QUIT=9,
			STATE_STATIC = 10,
   			STATE_NONE = 30,
	  		STATE_AUTO = 31,
	};

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
	 * \fn Region* getRegion()
	 * \brief Gibt die Region aus, in der sich das Objekt befindet
	 */
	Region* getRegion();
	
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
		m_destroyed=false;
		return true;
	}
	
	
	/**
	 * \fn virtual bool update ( float time)
	 * \brief Aktualisiert das WorldObject, nachdem eine bestimmte Zeit vergangen ist. Alle Aktionen des Objekts werden auf diesem Weg ausgel&ouml;st. Die Funktion ist virtuell und wird von den abgeleiteten Klassen &uuml;berschrieben
	 * \param time Menge der vergangenen Zeit in Millisekunden
	 * \return bool, der angibt, ob die Aktualisierung fehlerfrei verlaufen ist
	 */
	virtual  bool  update ( float time) 
	{
		return true;
	}
	
	
	/**
	 * \fn virtual bool destroy()
	 * \brief Zerst&ouml;rt das WorldObject.  Die Funktion ist virtuell und wird von den abgeleiteten Klassen &uuml;berschrieben
	 * \return bool der angibt, ob die Zerst&ouml;rung erfolgreich war
	 */
	virtual  bool  destroy ();
	
	/**
	 * \fn bool getDestroyed()
	 * \brief Gibt zurueck ob das Objekt zerstoert ist
	 */
	bool getDestroyed()
	{
		return m_destroyed;
	}
	
	/**
	 * \fn void setDestroyed(bool d)
	 * \brief Setzt den Status zerstoert
	 * \param d neuer Status
	 */
	void setDestroyed(bool d)
	{
		m_destroyed =d;
	}
	
	/**
	 * \fn bool moveTo(Vector newpos )
	 * \brief Verschiebt das Objekt an einen neuen Ort
	 * \param newpos neue Position
	 * 
	 */
	bool moveTo(Vector newpos);
	
	/**
	 * \fn void takeDamage(Damage* damage)
	 * \brief Das Objekt nimmt Schaden in der in damage angegebenen Art und Hoehe.
	 * \param damage Schadensart und -hoehe
	 */
	virtual void takeDamage(Damage* damage)
	{
	}
	
	/**
	 * \fn virtual void writeNetEvent(NetEvent* event, CharConv* cv)
	 * \brief Schreibt die Daten zu einem NetEvent in den Bitstream
	 * \param event NetEvent das beschrieben wird
	 * \param cv Bitstream
	 */
	virtual void writeNetEvent(NetEvent* event, CharConv* cv)
	{
	}
	
	/**
	 * \fn virtual void processNetEvent(NetEvent* event, CharConv* cv)
	 * \brief Fuehrt die Wirkung eines NetEvents auf das Objekt aus. Weitere Daten werden aus dem Bitstream gelesen
	 */
	virtual void processNetEvent(NetEvent* event, CharConv* cv)
	{
	}
	
	/**
	 * \fn virtual bool reactOnUse (int id)
	 * \brief Diese Funktion behandelt die Reaktion eines Worldobject, wenn auf es eine Aktion ausgef&uuml;hrt wird.
	 * \param id Id des Spielers, der die Aktion ausf&uuml;hrt
	 * \return bool, der angibt, ob die Behandlung der Aktion erfolgreich war
	 */
	virtual  bool  reactOnUse (int id)
	{
		return true;
	}
	
	/**
	 * \fn int getNetEventMask()
	 * \brief Gibt die Bitmaske der NetEvents aus
	 */
	int getNetEventMask()
	{
		return m_event_mask;
	}
	
	/**
	 * \fn void clearNetEventMask()
	 * \brief Setzt die Bitmaske der NetEvents auf 0
	 */
	void clearNetEventMask()
	{
		m_event_mask =0;
	}

	//Accessor Methods
    /**
	 * \fn void setSubType(TypeInfo::ObjectSubtype t)
	 * \brief Setzt den Subtyp des Objektes
	 * \param t neuer Subtyp
	 */
    void setSubType(TypeInfo::ObjectSubtype t)
    {
        m_type_info.m_subtype =t;
    }

	/**
	 * \fn Shape* getShape()
	 * \brief Gibt die Form des Objektes aus
	 */
	Shape* getShape()
	{
		return &m_shape;
	}
	
	/**
	 * \fn short getLayer()
	 * \brief Gibt die Ebene des Objektes aus
	 */
	short getLayer()
	{
		return m_layer;
	}
	
	/**
	 * \fn void setLayer(short layer)
	 * \brief Setzt die Ebene des Objektes
	 * \param layer Ebene
	 */
	void setLayer(short layer)
	{
		m_layer = layer;
	}

	/**
	 * \fn TypeInfo* getTypeInfo()
	 * \brief Gibt einen Zeiger auf die Struktur mit den Typinformationen zurueck
	 * \return Zeiger auf die Struktur
	 */
	TypeInfo* getTypeInfo()
	{
		return &m_type_info;
	}

	/**
	 * \fn State getState()
	 * \brief Gibt den aktuellen Status des Objektes aus
	 * \return Status des Objektes
	 */
	State getState()
	{
		return m_state;
	}

	/**
	 * \fn void setState(State s)
	 * \brief Setzt den Status
	 * \param s Status
	 */
	void setState(State s)
	{
		m_state = s;
	}

	/**
	 * \fn int getId()
	 * \brief Gibt die ID des objektes zurueck
	 * \return ID
	 */
	int getId()
	{
		return m_id;
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
	 * \fn Category getCategory()
	 * \brief Gibt die Kategorie aus
	 */
	Category getCategory()
	{
		return m_category;
	}
	
	
	/**
	 * \fn void setId(int id)
	 * \brief Setzt die ID eines Objektes
	 */
	void setId(int id)
	{
		m_id = id;
	}
	
	/**
	 * \fn Vector& getSpeed()
	 * \brief Gibt die Geschwindigkeit des Objektes aus
	 */
	Vector& getSpeed()
	{
		return m_speed;
	}
	
	/**
	 * \fn WorldObject::Group getGroup()
	 * \brief Gibt aus zu welcher Gruppierung ein Objekt gehoert
	 */
	WorldObject::Group getGroup()
	{
		if (getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_FIXED_OBJECT)
			return WorldObject::FIXED;
		if (getState() == WorldObject::STATE_DEAD || getState() == WorldObject::STATE_DIEING )
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
	 * \fn std::string getName()
	 * \brief Gibt den Name des Objektes aus
	 */
	virtual std::string getName();


	/**
	 * \fn std::string getNameId()
	 * \brief Gibt Name mit angehaengter ID aus
	 */
	std::string getNameId();
	
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

	protected:
		
	/**
	 * \var int m_event_mask
	 * \brief Bitmaske mit den NetEvents die das Objekt seit dem letzten Update erlebt hat
	 */
	int m_event_mask;
	
	/**
	 * \var short m_layer
	 * \brief Gibt die Ebene an, in der sich das Objekt befindet
	 */
	short m_layer;
	
	/**
	 * \var Vector m_speed
	 * \brief Gibt die Geschwindigkeit des Objektes an
	 */
	Vector m_speed;
	
	/**
	 * \var Category m_category
	 * \brief Kategorie des Objektes
	 */
	Category m_category;

	/**
	 * \var Fraction m_fraction
	 * \brief Fraktion des Objektes
	 */
	Fraction m_fraction;
		
//Private stuff
private:

	/**
	 * \var Shape m_shape
	 * \brief Gibt die Form des Objektes an
	 */
	Shape m_shape;

	
	
	/**
	 * \var TypeInfo m_type_info
	 * \brief Beeinhaltet alle Informationen zum Typ des Objektes
	 */
	TypeInfo m_type_info;

	/**
	 * \var m_state;
	 * \brief Gibt den Status des Objekts an
	 */
	State m_state;

	/**
	 * \var m_id
	 * \brief ID des Objekts
	 */
	int m_id;

	/**
	 * \var GridLocation m_grid_location
	 * \brief Ort des Objektes in der internen Struktur
	 */
	
	GridLocation m_grid_location;
	
	
	
	/**
	 * \var m_destroyed
	 * \brief Information, ob das Objekt zerst&ouml;rt ist
	 */
	bool m_destroyed;
	
	
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
