/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Daniel Erler, Michael Kempf

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
 * \file networkstruct.h
 * \defgroup networkstruct \ Strukturen zur Netzwerk&uuml;bertragung
 * \author Hans Wulf
 * \version 1.0
 * \date 2007-05-31
 * \brief Pakete bei Netzwerk&uuml;bertragung
 * \note Definiert mittels den Strukturen Chatmessage, ClientCommand, ClientItem und ClientWorldObject den Aufbau der Datenpakete, die zwischen Server und Client &uuml;bertragen werden
 */
#ifndef NETWORKSTRUCT_H
#define NETWORKSTRUCT_H

#include "network.h"
#include "action.h"
#include "worldobject.h"
#include "geometry.h"

/**
 * \enum PackageType
 * \brief Zaehlt die verschiedenen Arten von Netzwerkpaketen auf
 */
enum PackageType
{
	/**
	 * \brief Anfrage des Clients an den Server
	 */
	PTYPE_C2S_REQUEST =ID_USER_PACKET_ENUM,

 /**
  * \brief Antwort des Servers auf eine Anfrage des Clients
  */
	PTYPE_S2C_RESPONSE ,

 /**
  * \brief Savegame vom Server an den Client
  */
	PTYPE_S2C_SAVEGAME ,

 	/**
  	* \brief Savegame vom Client an den Server
  	*/
 	PTYPE_C2S_SAVEGAME ,

 /**
  * \brief Normale Daten vom Server zum Client
  */
	PTYPE_S2C_EVENT ,

 /**
  * \brief Kommando vom Client zum Server
  */
	PTYPE_C2S_COMMAND,

 	/**
	 * \brief Anfrage nach Daten die beim Client fehlen / inkorrekt sind
	 */
	PTYPE_C2S_DATA_REQUEST,

	/**
	* \brief Daten zu einer Region (feste Objekte und Untergrund)
	*/
	PTYPE_S2C_REGION ,

	/**
	 * \brief Daten ueber Spieler
	 */
 	PTYPE_S2C_PLAYER,

  	/**
	 * \brief Informationen zur Initialisierung
	 */
	PTYPE_S2C_INITIALISATION,
 
	/**
	 * \brief Information, dass beim Server der Spieler die Region gewechselt hat
	 */
	PTYPE_S2C_REGION_CHANGED,
 
	/**
	* \brief Nachricht vom Server an den Client
	*/
 	PTYPE_S2C_MESSAGE,
 
	/**
	* \brief Nachricht vom Client an den Server
	*/
 	PTYPE_C2S_MESSAGE,
  
	/**
	* \brief Partyinformationen vom Server an den Client
	*/
	PTYPE_S2C_PARTY,
 
 	/**
  	* \brief Lua Code vom Server an den Client
  	*/
 	PTYPE_S2C_LUA_CHUNK,
};

/**
 * \struct ClientDataRequest
 * \brief Beschreibt Anfrage des Client nach Daten vom Server
 */
struct ClientDataRequest
{
	/**
	 * \enum Data
	 * \brief Zaehlt verschiedene Datenanfragen auf
	 */
	enum Data
	{
		REGION_STATIC=1,
		REGION_NONSTATIC=2,
		REGION_ALL=3,
		ITEM = 0x10,
		PLAYERS = 0x20,
		OBJECT = 0x30,
 		PROJECTILE = 0x40,
	};

	/**
	 * \var Data m_data
	 * \brief Art der geforderten Daten
	 */
	Data m_data;

	/**
	 * \var int m_id
	 * \brief ID des geforderten Objektes
	 */
	int m_id;

	/**
	 * \fn void toString(CharConv* cv)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param cv Ausgabepuffer
	 */
	void toString(CharConv* cv)
	{
		cv->toBuffer((char) m_data);
		cv->toBuffer(m_id);
	}


	/**
	 * \fn void fromString(CharConv* cv)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param cv Eingabepuffer
	 */
	void fromString(CharConv* cv)
	{
		char tmp;
		cv->fromBuffer(tmp);
		m_data = (Data) tmp;
		cv->fromBuffer(m_id);
	}
};


/**
 * \enum Button
 * \brief Zählt alle Möglichkeiten auf, wie der Anwender durch Mausklicks oder das Drücken von Buttons Aktionen auslösen kann
 */
enum Button
{
	/**
	 * \brief Eingabe durch Klicken mit linker Maustaste ins Spielfeld
	 */
	LEFT_MOUSE_BUTTON =0,

 	/**
 	 * \brief Eingabe durch Klicken mit linker Maustaste und Shift ins Spielfeld
 	 */
 	LEFT_SHIFT_MOUSE_BUTTON =1,

 	 /**
	* \brief Eingabe durch Klicken mit rechter Maustaste und Shift ins Spielfeld
	*/
  	RIGHT_MOUSE_BUTTON =2,


	/**
	 * \brief Eingabe durch Speichern+Beenden Schaltfläche oder Schliessen-Kreuz
	 */
	BUTTON_SAVE_QUIT =3,

	BUTTON_SET_LEFT_ACTION =6,

	BUTTON_SET_RIGHT_ACTION=7,

	BUTTON_ITEM_LEFT=8,

	BUTTON_ITEM_RIGHT=9,

	BUTTON_SWAP_EQUIP = 10,

	REQUEST_DETAILED_ITEM = 11,

	REQUEST_ABILITY_DAMAGE=12,

	BUTTON_INCREASE_ATTRIBUTE=13,

	BUTTON_LEARN_ABILITY = 14,

	DROP_ITEM = 20,
 
	BUTTON_ANSWER=21,
 
	BUTTON_MEMBER_ACCEPT = 30,
 
 	BUTTON_MEMBER_REJECT = 31,
  
	BUTTON_APPLY = 32,

	BUTTON_KICK = 33,

	BUTTON_PEACE = 34,
 
	BUTTON_WAR = 35,
 
	BUTTON_LEAVE = 36,
 
	BUTTON_TRADE_ITEM_LEFT=37,

	BUTTON_TRADE_ITEM_RIGHT=38,
 
	BUTTON_TRADE_END = 39,

	DEBUG_SIGNAL=100,
};


/**
 * \struct ClientCommand
 * \brief Datenpaket, das ein Kommando des Anwenders enthält (Struktur)
 *
 * Sämtliche Anweisungen des Nutzers werden auf diese Weise vom Client zum Server übertragen.
 */
struct ClientCommand
{
	/**
	 * \var m_button;
	 * \brief Gibt den angelickten Button an
	 */
	Button m_button;

	/**
	 * \var Action::ActionType m_action
	 * \brief Action die ausgefuehrt werden soll
	 */

	Action::ActionType m_action;

	/**
	 * \var Vector m_goal
	 * \brief Zielpunkt der Aktion
	 */
	Vector m_goal;

	/**
	 * \var m_id;
	 * \brief Gibt Typ des Gegenstands an, falls die Aktionen einen Gegenstand betrifft Gibt Id des ZielObjekts an, wenn die Aktion ein Objekt betrifft
	 */
	int m_id;
	/**
	 * \var m_number;
		 * \brief Gibt Anzahl der Gegenst&auml;nde an, falls die Aktionen einen Gegenstand betrifft
	 */
	int m_number;
	/**
	 * \var m_direction;
	 * \brief Gibt an, in welche Richtung ein Gegenstand verschoben wird, wenn die Aktion einen Handel betrifft
	 */
	short m_direction;

	/**
	 * \fn void toString(CharConv* cv)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param cv Ausgabepuffer
	 */
	void toString(CharConv* cv);


	/**
	 * \fn void fromString(CharConv*  buf)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param cv Eingabepuffer
	 */
	 void fromString(CharConv*  cv);
};

/**
 * \enum ItemLocation
 * \brief Zählt alle möglichen Positionen auf, an denen sich ein beim Client angezeigter Gegenstand befinden kann: Im Inventar des Spielers, im Angebot des Spielers für einen Handel, im Inventar des Handelspartners, im Angebot des Handelspartner, sowie in der Hand beziehungsweise am Körper, wenn der Gegenstand gerade benutzt wird.
 */
enum ItemLocation
{
	LOCATION_INVENTORY=1,
	LOCATION_OFFER=2,
	LOCATION_TRADER_INVENTORY=3,
	LOCATION_TRADER_OFFER=4,
	LOCATION_RIGHT_HAND=5,
	LOCATION_BODY_LOCATION=6,
	LOCATION_LEFT_HAND=7
};



/**
 * \struct PackageHeader
 * \brief Struktur fuer den Header jedes gesendeten Paketes
 */
struct PackageHeader
{
	/**
	 * \var PackageType m_content
	 * \brief Art der Nachricht
	 */
	PackageType m_content;

	/**
	 * \var short m_number
	 * \brief Zaehler fuer die Art der gleichartigen Objekte
	 */
	short m_number;

	/**
	 * \fn void toString(CharConv* cv)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param cv Ausgabepuffer
	 */
	void toString(CharConv* cv);


	/**
	 * \fn void fromString(CharConv* cv)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param cv Eingabepuffer
	 */
	void fromString(CharConv* cv);
};

#endif //NETWORKSTRUCT_H
