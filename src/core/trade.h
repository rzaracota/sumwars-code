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
 * \file trade.h
 * \defgroup Trade \Handel
 * \brief Objekt, das einen Handelsvorgang verwaltet.
 * \author Hans Wulf
 * \version 1.0
 * \date 2007/05/28
 * \note Beinhaltet die Klasse Trade
 */

#ifndef TRADE_H
#define TRADE_H

#include "itemlist.h"
#include "item.h"
#include "networkstruct.h"

/**
 * \class Trade
 * \brief Verwaltet das Verhalten des Handelsmodus gegen&uuml;ber allen Spielerobjekten. Ein Objekt dieser Klasse repr&auml;sentiert einen Handel zwischen zwei Handelspartnern.
 */

class Trade {
public:
	//Fields
	//Constructors
	/**
	 * \fn Trade(int id, int trader1_id, int trader2_id)
	 * \brief Konstruktor
	 * \param id ID des Handels
	 * \param trader1_id ID des ersten Handelspartners
	 * \param trader2_id ID des zweiten Handelsparters
	 * 
	 * Legt ein ein neues Trade Objekt an, welches einen Handel zwischen den zwei angegebenen Spielern verwaltet
	 */
	Trade(int id, int trader1_id, int trader2_id);
	/**
	 *\fn ~Trade
	 *\brief Destruktor
	 * Löscht das Trade Objekt und gibt allokierten Speicher frei
	 */
	 ~Trade();
	//Accessor Methods
	/**
	 * \fn int getPlayerIndex(int id)
	 * \brief Die Klasse Trade verf&uuml;gt &uuml;ber zwei Positionen f&uuml;r die Handelspartner. Die Funktion liefert den Index zur&uuml;ck, auf welcher der beiden Positionen sich der Spieler mit der angegebenen id befindet. Das Ergebnis ist entweder 0 oder 1, wenn die Anfrage  erfolgreich war, wenn ein Fehler auftritt so ist das Ergebnis -1. F&uuml;r alle Set und Get Funktionen muss der Index mit angegeben werden.
	 * \param id Spielerid
	 * \return int interner Index des Spielers
	 */
	int getPlayerIndex(int id);
	
	
	/**
	 * \fn getPlayerId(int index)
	 * \brief Gibt ID des Spielers zur&uuml;ck
	 * \param index Index des Spielers
	 * \return int, ID des Spielers
	 *
	 * Die Funktion liefert die Id des handelnden Spielers auf der Position index zur&uuml;ck. Der Index muss 0 oder 1 sein.
	 */
	int getPlayerId(int index);
	
	
	/**
	 * \fn void setPlayerId(int id, int index)
	 * \brief Setzt die ID des Spieler
	 * \param index Index des Spielers
	 * \param id ID des Spielers
	 *
	 * Die Funktion setzt Id des handelnden Spielers auf der Position index. Der Index muss 0 oder 1 sein.
	 */
	void  setPlayerId(int id, int index);
	
	
	/**
	 * \fn getId();
	 * \brief Gibt die ID des Handels zur&uuml;ck
	 * \return ID des Handels
	 *
	 * Die Funktion liefert die ID des Handels zur&uuml;ck.
	 */
	int getId();
	
	
	/**
	 * \fn setId(int id)
	 * \brief Setzt die ID des Handels
	 * \param id ID des Handels
	 *
	 * Die Funktion setzt die ID des Handels.
	 */
	void  setId(int id);
	
	
	/**
	 * \fn getPlayerType(int index)
	 * \brief Gibt den Typ des Spielers zur&uuml;ck
	 * \param index Typ des Spielers
	 * \return int, kodiert den Typ des Spielers
	 *
	 * Die Funktion liefert den Typ des handelnden Spielers auf der Position index zur&uuml;ck. Der Index muss 0 oder 1 sein.
	 */
	int getPlayerType(int index);
	
	
	/**
	 * \fn void setPlayerType(int type, int index)
	 * \brief Setzt die ID des Spieler
	 * \param type Typ des Spielers
	 * \param index ID des Spielers
	 *
	 * Die Funktion setzt den Typ des handelnden Spielers auf der Position index. Der Index muss 0 oder 1 sein.
	 */
	void  setPlayerType(int type, int index);
	

	/**
	 * \fn ItemList* getInventory(int index)
	 * \brief Gibt Inventar des Spielers zur&uuml;ck
	 * \param index Index des Spielers
	 * \return Itemlist*, Inventarliste des Spielers
	 *
	 * Die Funktion liefert die Inventarliste des handelnden Spielers auf der Position index zur&uuml;ck. Der Index muss 0 oder 1 sein.
	 */
	ItemList* getInventory(int index);
	
	/**
	 * \fn void setInventory(ItemList* item_list, int index)
	 * \brief Setzt Inventar des Spielers
	 * \param index Index des Spielers
	 * \param item_list Inventarliste des Spielers
	 *
	 * Die Funktion setzt die Inventarliste des handelnden Spielers auf der Position index. Der Index muss 0 oder 1 sein.
	 */
	void  setInventory(ItemList* item_list, int index);
	
	/**
	 * \fn bool getAccept(int index)
	 * \brief Gibt Information &uuml;ber den Akzeptanzstatus des Spielers zur&uuml;ck
	 * \param index Index des Spielers
	 * \return bool Information ob der Spieler den Handel anzeptiert hat
	 *
	 * Die Funktion liefert true zur&uuml;ck, wenn der Spieler auf der Position index den Handel akzeptiert hat, sonst false. Der Index muss 0 oder 1 sein.
	 */
	bool getAccept(int index);
	
	/**
	 * \fn void setAccept(bool accept, int index)
	 * \brief Setzt den Akzeptanzstatus des Spielers
	 * \param accept Information, ob der Handel akzeptiert werden soll
	 * \param index Index des Spielers
	 *
	 * Die Funktion setzt die Information, ob der Spieler auf der Position index den Handel akzeptiert hat. Der Index muss 0 oder 1 sein.
	 */
	void  setAccept(bool accept, int index);
	
	
	/**
	 * \fn bool getGotItems(int index)
	 * \brief Gibt an, ob der Spieler an Position index die erhandelten Gegenstaende schon erhalten hat
	 * \param index Index des Spielers
	 * \return bool Information ob der Spieler an Position index die erhandelten Gegenstaende schon erhalten hat
	 *
	 * Die Funktion liefert true zur&uuml;ck, wenn der Spieler auf der Position index bereits die erhandelten Gegenstaende erhalten hat, sonst false. Der Index muss 0 oder 1 sein.
	 */
	bool getGotItems(int index);
	
	
	/**
	 * \fn void setGotItems(bool got_items, int index)
	 * \brief Setzt die Information, ob der Spieler die erhandelten Gegenstaende schon erhalten hat
	 * \param got_items Information, ob der Spieler die erhandelten Gegenstaende schon erhalten hat
	 * \param index Index des Spielers
	 *
	 * Die Funktion setzt die Information, ob der Spieler auf der Position index die erhandelten Gegenstaende schon erhalten hat. Der Index muss 0 oder 1 sein.
	 */
	void  setGotItems(bool got_items, int index);
	
	
	/**
	 * \fn bool getFinished()
	 * \brief gibt an, ob der Handel beendet ist
	 * \return Information, ob der Handel beendet ist
	 * 
	 * Die Funktion gibt true aus, wenn der Handel abgeschlossen ist, sonst false
	 */
	bool getFinished();
	
	/**
	 * \fn bool getSuccessful()
	 * \brief gibt an, ob der Handel erfolgreich war
	 * \return Information, ob der Handel erfolgreich war
	 * 
	 * Die Funktion gibt true aus, wenn der Handel abgeschlossen ist und erfolgreich war, sonst false
	 */
	bool getSuccessful();
	
	//Operations
	/**
	 * \fn void acceptTrade()
	 * \brief Pr&uuml;ft ob beide Parteien einverstanden sind und schliesst den Handel ab
	 *
	 * Die Funktion pr&uuml;ft ob beide Parteien dem Handel zugestimmt haben und der Handel vollzogen werden kann. Wenn das der Fall ist, so werden die Gegenst&auml;nde ausgetauscht und der Handel ist abgeschlossen.
	 */
	void acceptTrade();
	
	
	/**
	 * \fn void abortTrade()
	 * \brief bricht den Handel ab
	 * 
	 * Die Funktion bricht den Handel ab
	 */
	void abortTrade();
	
	
	/**
	 * \fn void onTrade(int index, ClientCommand* trade_command)
	 * \brief F&uuml;hrt einen Handelsauftrag aus
	 * \param index Index des Spielers
	 * \param trade_command Auftrag des Spielers
	 *
	 * Die Funktion f&uuml;hrt einen Auftrag, etwas an der aktuellen Handelssituation zu &auml;ndern, aus.
	 */		       
	void onTrade(int index, ClientCommand* trade_command);       

//Private stuff
private:
	//Fields
	/**
	 * \var m_id
	 * \brief Speichert die ID des Handels
	 */
	 int m_id;
	/**
	 * \var m_player_id[2]
	 * \brief Speichert Playerids der handelnden Spieler
	 */
    	int m_player_id[2];
	/**
	 * \var m_player_type[2]
	 * \brief Speichert den Typ der handelnden Spieler
	 */
    	int m_player_type[2];
	/**
	 * \var m_inventory[2]
	 * \brief Speichert Inventare der handelnden Spieler
	 */
	ItemList* m_inventory[2];
	
	/**
	 * \var m_accept[2]
	 * \brief Speichert f&uuml;r beide Handelspartner, ob sie dem Handel zugestimmt haben
	 */
	bool m_accept[2];
	/**
	 * \var m_finished
	 * \brief gibt an, ob der Handel beendet ist
	 */
	bool m_finished;
	/**
	 * \var m_got_items[2]
	 * \brief Speichert f&uuml;r beide Handelspartner, ob sie die erhandelten Gegenstaende erhaltenhaben
	 */
	bool m_got_items[2];
	/**
	 * \var m_successful
	 * \brief gibt an, ob der Handel erfolgreich war
	 */
	bool m_successful;
	
};
#endif //TRADE_H
