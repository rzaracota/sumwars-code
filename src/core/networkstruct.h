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
 * \file clientstructures.h
 * \defgroup clientstructures \ Strukturen zur Netzwerk&uuml;bertragung
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
	PTYPE_S2C_DATA ,
 
 /**
  * \brief Normale daten vom Client zum Server
  */
	PTYPE_C2S_DATA,
 
 /**
  * \brief Detailierte Daten zu einem Item vom Server zum Client
  */
	PTYPE_S2C_ITEM ,
 
  /**
  * \brief Detailierte Daten zum Schaden einer Faehigkeit vom Server zum Client
	*/
 PTYPE_S2C_DAMAGE ,
 
 /**
  * \brief Daten zu einer Region (feste Objekte und Untergrund)
  */
 PTYPE_S2C_REGION ,
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
	
	BUTTON_PARTY_APPLY =4,
 
	BUTTON_PARTY_ACCEPT =5,
		
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
	 * \var m_coordinate_x;
	 * \brief Gibt x-Koordinate an, auf die der Spieler gelickt hat
	 */
	float m_coordinate_x;
	/**
	 * \var m_coordinate_y;
	 * \brief Gibt y-Koordinate an, auf die der Spieler gelickt hat
	 */
	float m_coordinate_y;
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
	 * \fn void toString(char* buf)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param buf Ausgabepuffer
	 * \return Zeiger hinter den beschriebenen Datenbereich
	 */
	virtual void toString(CharConv* cv);
			
			
	/**
	 * \fn void fromString(char* buf)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param buf Objekt als String
	 * \return Zeiger hinter den gelesenen Datenbereich
	 */
	virtual void fromString(CharConv* cv);
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



struct ServerHeader
{
	/**
	 * \var PackageType m_content
	 * \brief Art der Nachricht
	 */
	PackageType m_content;
	
	/**
	 * \var short m_objects
	 * \brief Anzahl der enthaltenen Objekte
	 */
	short m_objects;
	
	/**
	 * \var short m_projectiles
	 * \brief Anzahl der Geschosse
	 */
	short m_projectiles;
	
	/**
	 * \var short m_items
	 * \brief Anzahl der enthaltenen Gegenstaende
	 */
	short m_items;
	
	/**
	 * \var short m_drop_items
	 * \brief Anzahl am Boden liegender Gegenstaende
	 */
	short m_drop_items;
	
	/**
	 * \var bool m_chatmessage
	 * \brief true, wenn die Nachricht eine Chatnachricht ist
	 */
	bool m_chatmessage;
	
	/**
	 * \var bool m_trade
	 * \brief true, wenn Handelsinformationen enthalten sind
	 */
	bool m_trade;
	
	/**
	 * \var short m_detailed_item
	 * \brief ID des mit detaillierten Informationen uebertragenen Items (wenn kein Item detailliert gesendet wird gleich 0 )
	 */
	short m_detailed_item;
	
	/**
	 * \fn void toString(CharConv* cv)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param buf Ausgabepuffer
	 * \return Zeiger hinter den beschriebenen Datenbereich
	 */
	virtual void toString(CharConv* cv);
			
			
	/**
	 * \fn void fromString(CharConv* cv)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param buf Objekt als String
	 * \return Zeiger hinter den gelesenen Datenbereich
	 */
	virtual void fromString(CharConv* cv);
};
	
struct ClientHeader
{
	/**
	 * \var PackageType m_content
	 * \brief Art der Nachricht
	 */
	PackageType m_content;
	
	/**
	 * \var bool m_chatmessage
	 * \brief true, wenn die Nachricht eine Chatnachricht ist
	 */
	bool m_chatmessage;
	
	/**
	 * \fn void toString(CharConv* cv)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param buf Ausgabepuffer
	 * \return Zeiger hinter den beschriebenen Datenbereich
	 */
	virtual void toString(CharConv* cv);
			
			
	/**
	 * \fn void fromString(CharConv* cv)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param buf Objekt als String
	 * \return Zeiger hinter den gelesenen Datenbereich
	 */
	virtual void fromString(CharConv* cv);
};
	
#endif //NETWORKSTRUCT_H
