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
 * \file warrior.h
 * \defgroup Warrior \ Krieger
 * \ingroup User
 * \brief Spezifiziert den nutzergesteuerten Krieger
 * \author Hans Wulf
 * \version 1.0
 * \date 2007/05/30
 * \note Beinhaltet die Klasse Warrior
 */
#ifndef WARRIOR_H
#define WARRIOR_H
#include <string>
#include "player.h"
#include "world.h"
/**
 * \class Warrior
 * \brief Dieses Objekt stellt einen Krieger dar.
 */
class Warrior : public Player {
//Public stuff
public:
	//Fields
	//Constructors
	/**
	 * \fn Warrior(World* world, unsigned int id)
	 * \brief Konstruktor
	 * \param world Welt
	 * \param id Objekt-ID
	 *
	 * Legt ein neues Warrior Objekt an.
	 */
	Warrior(World* world, int id);
	
	
	/**
	 * \fn ~Warrior()
	 * \brief Destruktor
	 *
	 * Gibt den Allokierten Speicher wieder frei
	 */
	~Warrior();
	//Accessor Methods
	//Operations
	/**
	 * \fn virtual bool init ()
	 * \brief Initialisiert den Krieger. 
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
		Player::destroy();
	}
	
	/**
	 * \fn virtual void calcDamage(ActionType type,Damage& dmg)
	 * \brief Berechnet den Basisschaden einer Aktion
	 */
	virtual void calcBaseDamage(Action::ActionType act,Damage& dmg)
	{
		Player::calcBaseDamage(act,dmg);
	}
	
	/**
	 * \fn virtual void calcBaseAttrMod()
	 * \brief Die modifizierten Basisattribute werden neu berechnet. Verwendet die Basisattribute, verwendete Items, sowie temporaere Modifikationen.
	 */
	virtual void calcBaseAttrMod()
	{
		Player::calcBaseAttrMod();
	}
	
	
	/**
	 * \fn bool update (float time)
	 * \brief Updatefunktion des Kriegers
	 * \param time Verstrichene Zeit
	 * \return bool, Erfolgreiches Update?
	 *
	 * Die Updatefunktion des Kriegers wird in regelm&auml;ssigen Abst&auml;nden von der World aufgerufen und ben&ouml;tigt die Angabe, wie viel Zeit f&uuml;r das Update zur Verf&uuml;gung gestellt wird. Der R&uuml;ckgabewert gibt an ob die Funktion erfolgreich ausgef&uuml;hrt wurde.
	 */
	bool update(float time);
	
	protected:
	
	/**
	 * \fn virtual void gainLevel()
	 * \brief Wird aufgerufen, wenn das Lebewesen ein Levelup erhält. Passt Erfahrung und alle Basisattribute an.
	 */
	virtual void gainLevel()
	{
		Player::gainLevel();
	}
			
};
#endif //WARRIOR_H
