/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Hans Wulf, Daniel Erler

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
 * \file item.h
 * \defgroup Item \ Gegenstand
 * \brief Beschreibt den Aufbau von Gegenst&auml;nden, die ein Spieler haben kann.
 * \author Hans Wulf
 * \version 1.0
 * \date 2007/05/28
 * \note Beinhaltet die Klasse Item
 */
#ifndef ITEM_H
#define ITEM_H


#include <string>
#include <cstring>
#ifdef WIN32

#else
    #include <libintl.h>
#endif
#include <locale.h>
#include "charconv.h"
#include "debug.h"

using namespace std;


/**
 * \struct Item
 * \brief Dieses Objekt ist ein Gegenstand und hat als solches Eigenschaften, wie Preis, Anzahl oder Gewicht.
 */
struct Item {
	/**
	 * \enum Size
	 * \brief TODO
	 */
	enum Size
	{
		GOLD = 3,
		SMALL = 2,
		MEDIUM = 1,
		BIG = 0,
	};


	/**
	 * \enum Type
	 * \brief TODO
	 */
	enum Type
	{
		NOITEM = 0,
		ARMOR  = 1,
		HELMET = 2,
		GLOVES = 6,
		WEAPON = 8,
		SHIELD = 5,
		POTION = 4,
		RING = 3,
		AMULET= 7,
 		GOLD_TYPE = 10
	};

	/**
	 * \var  Subtype
	 */
	typedef std::string Subtype;
/*
	enum Subtype
	{
		NOSUBTYPE = 0,

		SHORT_SWORD = 1,
		LONG_SWORD = 2,
		WOODEN_BOW = 3,
		LONG_BOW = 4,
		BURNING_BOW = 5,
		BATTLE_AXE = 6,
		HOLY_FLAIL = 7,
		FROZEN_MAGIC_WAND = 14,


		LEATHER_ARMOR = 1000,
		TILED_ARMOR = 1001,
		HEAVY_ARMOR = 1002,

		STEEL_HELMET = 2000,
		DIVINE_HELMET =2001,

		MAGIC_RING = 3000,

		SMALL_HEAL_POTION = 4000,
		BIG_HEAL_POTION = 4001,
		HEAL_BLIND_POTION = 4002,
		UNFREEZE_POTION = 4003,


		WOODEN_SHIELD = 5000,
		IRON_SHIELD=5001,

		LEATHER_GLOVES = 6000,

		MAGIC_AMULET = 7000,
	};
*/
	/**
	 * \enum CharRequirement
	 * \brief Aufzaehlung der Charakterklassen als Vorraussetzung fuer das Verwendung eines Gegenstandes
	 */
	enum CharRequirement
	{
		REQ_WARRIOR=1,
		REQ_MAGE=2,
		REQ_ARCHER=4,
		REQ_PRIEST=8,
		REQ_NONE=15,
	};

	/**
	 * \fn virtual ~Item()
	 * \brief Destruktor
	 */
	virtual ~Item()
	{
	}

	/**
	 * \fn String getName()
	 * \brief Gibt den lesbaren Name des Objektes aus
	 */
	virtual string getName();

	/**
	 * \fn String getString()
	 * \brief Gibt den Name des Objektes aus
	 */
	virtual string getString();

	/**
	 * \fn void toString(CharConv* cv, short pos)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param buf Ausgabepuffer
	 * \return Zeiger hinter den beschriebenen Datenbereich
	 */
	virtual void toString(CharConv* cv, short pos);


	/**
	 * \fn void fromString(void CharConv* cv, short& pos)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param buf Objekt als String
	 * \return Zeiger hinter den gelesenen Datenbereich
	 */
	virtual void fromString(CharConv* cv,short& pos);


	/**
	* \var m_subtype;
	* \brief Gibt die Unterart eines Gegenstands an ((Langschwert, Dolch usw)
	*/
	Subtype m_subtype;


	/**
	 * \var m_type;
	 * \brief Gibt Typ des Gegenstands an (Waffe, Helm usw...)
	 */
	Type m_type;

	/**
	* \var m_size
	\brief Gibt die Groesse des Gegenstandes (klein,mittel, gross) an
	*/
	Size m_size;




};

/*

*/



#endif //ITEM_H

