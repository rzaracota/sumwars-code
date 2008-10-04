/*
	Summoning Wars
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
 * \file itemlist.h
 * \defgroup itemlist \ Gegenstandsliste
 * \brief Verwaltet listenartig Gegenst&auml;nde
 * \author Hans Wulf
 * \version 1.0
 * \date 2007/05/28
 * \note Beinhaltet die Klasse ItemList
 */

#ifndef ITEMLIST_H
#define ITEMLIST_H

#include <string>
#include <cstdlib>
#include <algorithm>
#include "debug.h"

#include "item.h"


/**
 * \class ItemList
 *
 * \brief Klasse fuer eine Liste von Gegenstaenden
 */
class ItemList {
//Public stuff
public:
	//Fields
	//Constructors
   /**
	 * \fn ItemList()
	 * \brief Konstruktor
	 *
	 * Legt ein neues ItemList Objekt an
	 * \param max_small maximale Anzahl kleiner Objekte in der Liste
	 * \param max_medium maximale Anzahl mittelgroßer Objekte in der Liste
	 * \param max_big maximale Anzahl großer Objekte in der Liste
	 */
	ItemList(short max_small, short max_medium, short max_big);

	/**
	 * \fn ItemList(Itemlist* itemlist)
	 * \brief Kopierkonstruktor, legt eine flache Kopie an, die referenzierten Items sind also die gleichen!
	 * \param itemlist Kopiervorlage
	 */
	ItemList(ItemList* itemlist);

   /**
	 * \fn ~ItemList()
	 * \brief Destruktor
	 *
	 * Löscht das ItemList Objekt
	 */
	~ItemList();

	/**
	 * \fn void clear()
	 * \brief Loescht alle enthaltenen Items
	 */
	void clear();


	/**
	 * \fn Item* getItem(char m_size, int index)
	 * \brief Gibt einen Zeiger auf das Item der Groesse m_size an der Position index zurueck. Gibt NULL zurueck, wenn der Index die Listengroesse ueberschreitet.
	 * \param m_size Groesse des Items
	 * \param indes Index
	 */
	Item* getItem(Item::Size m_size, int index);

	/**
	 * \fn int getFreePlace(char m_size)
	 * \brief Gibt den Indes  des ersten freien Platzes fuer ein Item der angegebenen Groesse aus.
	 * \param m_size Groesse des Items
	 * \return Index fuer einen freien Platz
	 */
	int getFreePlace(Item::Size m_size);

	/**
	 * \fn void swapItem(Item* item, int index)
	 * \brief vertauscht den angegebene Gegenstand mit dem gleichgrossen an Position index. Ein Gegenstand wird aus der Liste entfernt, indem man ihn mit einem leeren Gegenstand austauscht. Ein Gegenstand wird in die Liste eingefuegt, indem man einen leeren Platz der richtigen Groesse sucht und damit austauscht.
	 * \param item Zeiger auf den zu tauschenden Gegenstand
	 * \param index Position des Gegenstandes in der Liste, mit dem getauscht werden soll
	 */
	void swapItem(Item* &item,Item::Size size, int index);

	/**
	 * \fn short getMaxSmall()
	 * \brief Gibt maximale Anzahl kleiner Gegenstaende aus
	 */
	short getMaxSmall()
	{
		return m_max_small;
	}

	/**
	 * \fn short getMaxMedium()
	 * \brief Gibt maximale Anzahl mittlere Gegenstaende aus
	 */
	short getMaxMedium()
	{
		return m_max_medium;
	}

	/**
	 * \fn short getMaxBig()
	 * \brief Gibt maximale Anzahl grosser Gegenstaende aus
	 */
	short getMaxBig()
	{
		return m_max_big;
	}

	/**
	 * \fn int getGold()
	 * \brief Gibt die Menge Gold aus
	 * \return Gold
	 */
	int getGold()
	{
		return m_gold;
	}

	/**
	 * \fn void setGold(int gold)
	 * \brief Setzt die Menge Gold
	 * \param gold Gold
	 */
	void setGold(int gold)
	{
		m_gold = gold;
	}


//Private stuff
private:
	//Fields

	/**
	 * \var m_max_small
	 * \brief Gibt die maximale Anzahl kleiner Gegenstaende in der Liste an
	 */
	short m_max_small;

	/**
	 * \var m_max_medium
	 * \brief Gibt die maximale Anzahl mittelgrosser Gegenstaende in der Liste an
	 */
	short m_max_medium;

	/**
	 * \var m_max_big
	 * \brief Gibt die maximale Anzahl grosser Gegenstaende in der Liste an
	 */
	short m_max_big;


	/**
	 * \var m_small_items;
	 * \brief Liste der kleinen Gegenst&auml;nde
	 */
	Item**  m_small_items;

	/**
	 * \var m_medium_items;
	 * \brief Liste der mittelgrossen Gegenst&auml;nde
	 */
	Item**  m_medium_items;

	/**
	 * \var m_big_items;
	 * \brief Liste der kleinen Gegenst&auml;nde
	 */
	Item**  m_big_items;

	/**
	 * \var m_gold
	 * \brief Menge des Goldes
	 */
	int m_gold;


};


/**
 * \class Equipement
 * \brief Klasse fuer die Ausruestung eines Spielers
 */
class Equipement
{
	public:

	/**
	 * \enum Position
	 * \brief Zaehlt Positionen auf, an denen sich ein Gegenstand im Inventar eines Spielers befinden kann
	 */
	enum Position
	{
		NONE = 0,
		ARMOR  =1,
		HELMET =2,
		GLOVES =3,
		WEAPON =4,
		SHIELD =5,
		RING_LEFT=6,
		RING_RIGHT =7,
		AMULET= 8,
		WEAPON2=9,
		SHIELD2=10,
		CURSOR_ITEM=11,
		BIG_ITEMS=20,
		MEDIUM_ITEMS=30,
		SMALL_ITEMS = 60
	};

	/**
	 * \fn Equipement()
	 * \brief Konstruktor
	 */
	Equipement(short max_small, short max_medium, short max_big);

	/**
	 * \fn ~Equipement()
	 * Destruktor
	 */
	virtual ~Equipement();

	/**
	 * \fn Item* getItem(int pos)
	 * \param pos Position des Items
	 * \brief Gibt Item an der angegebenen Position aus
	 */
	Item* getItem(int pos);

	/**
	 * \fn short insertItem(Item* item)
	 * \brief Fuegt Gegenstand ins Inventar ein
	 * \param item Zeiger auf den Gegenstand
	 * \return Gibt an, wo das item eingefuegt wurde
	 */
	short insertItem(Item* item);
	
	/**
	 * \fn bool swapCursorItem(int pos)
	 * \param pos Position des Items
	 * \brief Tauscht das aktuelle Cursoritem mit dem an der angegebenen Position
	 */
	bool swapCursorItem(int pos);

	/**
	 * \fn bool swapItem(Item* &item, int pos)
	 * \param pos Position des Items
	 * \param Item Zeiger auf das Item das getauscht wird. Nach beenden der Funktion Zeiger auf das Item das aus dem Inventar herrausgetauscht wurde
	 * \brief Tauscht das angegebene Item mit dem an der angegebenen Position
	 */
	bool swapItem(Item* &item, int pos);

	
	/**
	 * \fn int getNumberItems(bool secondary_equip)
	 * \brief Gibt aus die Anzahl der Gegenstaende in der Ausruestung zurueck
	 */
	int getNumberItems(bool secondary_equip);
	
	/**
	 * \fn virtual void toString(CharConv* cv,int& nr, bool secondary_equip)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param buf Ausgabepuffer
	 * \param nr Anzahl der Items
	 * \param secondary_equip Wenn true, wird die zweite Ausruestung geschrieben
	 * \return Zeiger hinter den beschriebenen Datenbereich
	 */
	virtual void toString(CharConv* cv,int& nr, bool secondary_equip);


	/**
	 * \fn virtual void fromString(CharConv* cv,int nr)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param buf Objekt als String
	 * \param nr Anzahl der Items
	 * \return Zeiger hinter den gelesenen Datenbereich
	 */
	virtual void fromString(CharConv* cv,int nr);

	/**
	 * \fn void clear()
	 * \brief Loescht alle enthaltenen Item
	 */
	void clear();

	private:

	/**
	 * \var Item* m_helmet
	 * \brief Helm
	 */
	Item* m_helmet;

	/**
	 * \var Item* m_armor
	 * \brief Verwendete Ruestung
	 */
	Item* m_armor;

	/**
	 * \var Item* m_weapon
	 * \brief Verwendete Waffe
	 */
	Item* m_weapon;

	/**
	 * \var Item* m_weapon2
	 * \brief sekundaere Waffe
	 */
	Item* m_weapon2;

	/**
	 * \var Item* m_shield
	 * \brief Verwendeter Schild
	 */
	Item* m_shield;

	/**
	 * \var Item* m_shield2
	 * \brief Sekundaerer Schild
	 */
	Item* m_shield2;

	/**
	 * \var Item* m_gloves
	 * \brief Verwendete Handschuhe
	 */
	Item* m_gloves;

	/**
	 * \var Item* m_ring_left
	 * \brief Ring linke Hand
	 */
	Item* m_ring_left;

	/**
	 * \var Item* m_ring_right
	 * \brief Ring rechte Hand
	 */
	Item* m_ring_right;

	/**
	 * \var Item* m_amulet
	 * \brief verwendetes Amulett
	 */
	Item* m_amulet;

	/**
	 * \var  Item* m_cursor_item
	 * \brief Item, das aktuell mit dem Cursor gehalten wird
	 */
	Item* m_cursor_item;

	/**
	 * \var ItemList m_inventory
	 * \brief das Inventar
	 */
	ItemList m_inventory;


	/**
	 * \fn Item* getItemRef(int pos)
	 * \param pos Position des Items
	 * \brief Gibt Item an der angegebenen Position aus
	 */
		Item* & getItemRef(int pos);
};

#endif //ITEMLIST_H
