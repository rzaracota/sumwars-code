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

#ifndef SERVERWOBJECT_H
#define SERVERWOBJECT_H

#include <string>
#include "worldobject.h"
#include "damage.h"
#include "event.h"

class World;
class Region;

/**
 * \file serverwobject.h
 * \defgroup serverwobject \Weltobjekt
 * \ingroup world
 * \brief Deklaration des ServerWObject
 * \author Hans Wulf
 * \version 1.0
 * \date 2007/05/28
 * \note Bietet der Welt das ServerWObject an
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
};


/**
 * \class ServerWObject
 * \brief ein Objekt, welches sich in der Spielwelt befindet
 */
class ServerWObject : public WorldObject
{
//Public stuff
public:
	
		
	//Fields
	//Constructors
	/**
 	 * \fn ServerWObject(World* world, unsigned int id)
	 * \brief Konstruktor
	 * \param world World Objekt, in dem sich das neue ServerWObject befindet
	 * \param id ID des WorlObject
	 *
	 * Erstellt ein neues ServerWObject
	 */
	ServerWObject(World* world, int id);
	

	//Accessor Methods
	/**
	 * \fn getWorld()
	 * \brief Gibt einen Zeiger auf die Welt zurueck
	 * \return Zeiger auf die Welt
	 */
	World* getWorld()
	{
		return m_world;
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
	
	/**
	 * \fn MoveInfo* getMoveInfo()
	 * \brief Gibt Zeiger auf aktuellen Bewegungszustand zurueck
	 * \return aktueller Bewegungszustand
	 */
	MoveInfo* getMoveInfo()
	{
		return &m_move_info;
	}
	
	//Operations
	/**
	 * \fn virtual bool init ()
	 * \brief Initialisiert das ServerWObject. Die Funktion ist virtuell und wird durch die abgeleiteten Klassen &uuml;berschrieben.
	 * \return bool, der angibt ob die initialisierung erfolgreich war
	 */
	virtual  bool  init ();
	
	
	/**
	 * \fn virtual bool update ( float time)
	 * \brief Aktualisiert das ServerWObject, nachdem eine bestimmte Zeit vergangen ist. Alle Aktionen des Objekts werden auf diesem Weg ausgel&ouml;st. Die Funktion ist virtuell und wird von den abgeleiteten Klassen &uuml;berschrieben
	 * \param time Menge der vergangenen Zeit in Millisekunden
	 * \return bool, der angibt, ob die Aktualisierung fehlerfrei verlaufen ist
	 */
	virtual  bool  update ( float time) 
	{
		return true;
	}
	
	
	/**
	 * \fn virtual bool destroy()
	 * \brief Zerst&ouml;rt das ServerWObject.  Die Funktion ist virtuell und wird von den abgeleiteten Klassen &uuml;berschrieben
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
	 * \fn void moveTo(float x, float y)
	 * \brief setzt die x- und die y-Koordinate
	 * \param x x-Koordinate
	 * \param y y-Koordinate
	 * 
	 * Diese Funktion verschiebt das Objekt an einen anderen Ort. Wenn das Objekt in einem Quadtree eingeordnet ist, so muss bei jeder &Auml;nderung der Koordinaten getestet werden, ob das Objekt im Quadtree neu angeordnet werden muss. Diese Funktion teilt dem Quadtree die &Auml;nderung der Koordinanten mit und l&ouml;st damit ebentuell eine Reorganisation aus. Daher sollte ausserhalb des Quadtree diese Funktion zum ver&auml;ndern der Koordinaten verwendet werden.
	 */
	bool moveTo(float x, float y);
	
	/**
	 * \fn void takeDamage(Damage* damage)
	 * \brief Das Objekt nimmt Schaden in der in damage angegebenen Art und Hoehe.
	 * \param damage Schadensart und -hoehe
	 */
	virtual void takeDamage(Damage* damage)
	{
	}
	
	/**
	 * \fn virtual void writeEvent(Event* event, CharConv* cv)
	 * \brief Schreibt die Daten zu einem Event in den Bitstream
	 * \param event Event das beschrieben wird
	 * \param cv Bitstream
	 */
	virtual void writeEvent(Event* event, CharConv* cv)
	{
	}
	
	/**
	 * \fn virtual void processEvent(Event* event, CharConv* cv)
	 * \brief Fuehrt die Wirkung eines Events auf das Objekt aus. Weitere Daten werden aus dem Bitstream gelesen
	 */
	virtual void processEvent(Event* event, CharConv* cv)
	{
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

	protected:
		
		/**
		* \var int m_event_mask
		* \brief Bitmaske mit den Events die das Objekt seit dem letzten Update erlebt hat
		*/
		int m_event_mask;

//Private stuff
private:
	
	
	/**
 	* \var GridLocation m_grid_location
	 * \brief Ort des Objektes in der internen Struktur
	 */
	
	 GridLocation m_grid_location;
	 /**
	  * \var m_move_info
	  * \brief Information ueber den Bewegungszustand
	  **/
	 MoveInfo m_move_info;
	
	/**
 	* \var m_destroyed
 	* \brief Information, ob das Objekt zerst&ouml;rt ist
	 */
	bool m_destroyed;
	
	/**
	 * \var m_world
	 * \brief Zeiger auf das World Objekt, in dem das aktuelle Objekt eingeordnet ist
	 */
	World* m_world;
	
	
};

#include "world.h"

#endif //SERVERWOBJECT_H
