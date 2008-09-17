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
 * \file trader.h
 * \defgroup Trader \ H&auml;ndler
 * \ingroup Player
 * \brief Steuerung des H&auml;ndlerverhalten
 * \author Hans Wulf
 * \version 1.0
 * \date 2007/05/28
 * \note Beinhaltet die Klasse Trader
 */
#ifndef TRADER_H
#define TRADER_H
#include <string>
#include "player.h"
#include "world.h"

/**
 * \class Trader
 * \brief Dieses Objekt stellt einen H&auml;ndler dar. Der H&auml;ndler wird unter andrem durch NichtAngreifbarkeit charakterisiert.
 */
class Trader : public Player {
//Public stuff
public:
	//Fields
	//Constructors
	/**
	 * \fn Trader(World* world, unsigned int id)
	 * \brief Konstruktor
	 * \param world 
	 * \param id 
	 *
	 * Legt ein neues Trader Objekt an.
	 */
	Trader(World* world, unsigned int id);
	/**
	 * \fn Trader(World* world, float coordinate_x, float coordinate_y, unsigned int id)
	 * \brief Konstruktor
	 * \param world 
	 * \param coordinate_x 
	 * \param coordinate_y 
	 * \param id 
	 *
	 * Legt ein neues User Objekt an und initialisiert es mit den eingegebenen Werte.
	 */
	Trader(World* world, float coordinate_x, float coordinate_y, unsigned int id);
	/**
	 * \fn Trader(World* world, float coordinate_x, float coordinate_y, unsigned int id, int object_subtype)
	 * \brief Konstruktor
	 * \param world Zeiger auf die Welt
	 * \param coordinate_x x-Koordinate des Objekts
	 * \param coordinate_y y-Koordinate des Objekts
	 * \param id ID des Objekts
	 * \param object_subtype Gibt Subtyp des Haendlers an
	 *
	 * Legt ein neues User Objekt an und initialisiert es mit den eingegebenen Werte.
	 */
	Trader(World* world, float coordinate_x, float coordinate_y, unsigned int id, int object_subtype);
	/**
	 * \fn ~Trader()
	 * \brief Destruktor
	 *
	 * Gibt den Allokierten Speicher wieder frei
	 */
	~Trader();

	//Operations
	/**
	 * \fn virtual bool init()
	 * \brief Initialisierung des Händlers
	 * \return bool Gibt an ob die Initialisierung erfolgreich war
	 */
	virtual bool init();
	/**
	 * \fn virtual bool update (float time)
	 * \brief Updatefunktion des Händlers
	 * \param time Verstrichene Zeit
	 * \return bool, Erfolgreiches Update?
	 *
	 * Die Updatefunktion des H&auml;ndlers wird in regelm&auml;ssigen Abst&auml;nden von der World aufgerufen und ben&ouml;tigt die Angabe, wie viel Zeit f&uuml;r das Update zur Verf&uuml;gung gestellt wird. Der R&uuml;ckgabewert gibt an ob die Funktion erfolgreich ausgef&uuml;hrt wurde. Da die Funktion hier virtuell definiert wird, wird sie erst in den abgeleiteten Klassen implementiert.
	 */
	virtual bool update(float time);
};
#endif //TRADER_H
