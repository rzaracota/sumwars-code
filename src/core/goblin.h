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

/**
 * \file goblin.h
 * \defgroup Goblin \ Goblin
 * \ingroup Monster
 * \brief Stellt das Objekt Goblin zur Verf&uuml;gung
 * \author Hans Wulf
 * \version 1.0
 * \date 2007/06/05
 * \note Beinhaltet die Klasse Goblin und spezifiziert sein Verhalten
 */
#ifndef GOBLIN_H
#define GOBLIN_H
#include <string>
#include "monster.h"
#include "world.h"

/**
 * \class Goblin
 * \brief Der Goblin ist ein schnelles und gef&auml;hrliches Monster.
 */
class Goblin : public Monster {
//Public stuff
public:
	//Fields
	//Constructors
	/**
	 * \fn Goblin(World* world, int id)
	 * \brief Konstruktor
	 * \param world Welt
	 * \param id Objekt-ID
	 *
	 * Legt ein neues Goblin Objekt an.
	 */
	Goblin(World* world,  int id);

	/**
	 * \fn ~Goblin()
	 * \brief Destruktor
	 *
	 * Gibt den Allokierten Speicher wieder frei
	 */
	~Goblin();
	//Accessor Methods
	//Operations
	/**
	 * \fn virtual bool init ()
	 * \brief Initialisiert den Goblin. 
	 * \return bool, der angibt ob die initialisierung erfolgreich war
	 */
	virtual bool init ();
	
	
	/**
	 * \fn virtual bool destroy ()
	 * \brief Zerstoert das Objekt. Die Funktion ist virtuell und wird durch die abgeleiteten Klassen &uuml;berschrieben.
	 * \return bool, der angibt ob die Zerstoerung erfolgreich war
	 */
	virtual bool destroy()
	{
		Monster::destroy();
	}
	
	/**
	 * \fn virtual void updateCommand()
	 * \brief aktualisiert das aktuelle Kommando
	 */
	virtual void updateCommand()
	{
		Monster::updateCommand();
	}
	//bool update(float time);
//Private stuff
private:
	//Fields
	//Constructors
	//Accessor Methods
	//Operations

};
#endif //GOBLIN_H
