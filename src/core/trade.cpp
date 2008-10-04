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

#include "trade.h"

//Constructors/Destructors
 Trade::Trade(int id, int trader1_id, int trader2_id) {
 	
	/*m_id=id;
	m_player_id[0]=trader1_id;
	m_player_id[1]=trader2_id;
	m_finished=false;
	m_successful=false;
	
	m_inventory[0]=new ItemList;
	m_inventory[1]=new ItemList;	
		
	m_accept[0]=false;
	m_accept[1]=false;
		
	m_got_items[0]=false;
	m_got_items[1]=false;	
	*/
	
 }
 
 Trade::~Trade()
 {
	 delete m_inventory[0];
	 delete m_inventory[1];	 
 }

//Methods
int Trade::getPlayerIndex(int id) {
	if (id == m_player_id[0])
		return 0;
	
	if (id == m_player_id[1])
		return 1;
	
	return -1;

}

int Trade::getPlayerId(int index){
	return m_player_id[index];
}

void Trade::setPlayerId(int value, int index) {
	m_player_id[index]=value;
}

int Trade::getPlayerType(int index){
	return m_player_type[index];
}

void Trade::setPlayerType(int value, int index) {
	m_player_type[index]=value;
}

ItemList* Trade::getInventory(int index) {
	return m_inventory[index];
}

void  Trade::setInventory(ItemList* item_list, int index) {
	m_inventory[index]=item_list;
}


void  Trade::setId(int id)
{
	m_id = id;
}

int  Trade::getId()
{
	return m_id;
}

bool Trade::getAccept(int index){
	return m_accept[index];
}

void Trade::setAccept(bool value, int index) {
	m_accept[index]=value;
	
	// Wenn ein Spieler aktzeptiert hat: testen ob der handel ausgeführt werden kann
	if (value==true)
	{
		acceptTrade();
	}
}

bool Trade::getGotItems(int index){
	return m_got_items[index];
}

void Trade::setGotItems(bool value, int index) {
	m_got_items[index]=value;
}

bool Trade::getFinished()
{
	return m_finished;
}

bool Trade::getSuccessful()
{
	return m_successful;
}

void Trade::acceptTrade() {
	
	// Testen ob beide Partner die erhandelten Gegenstände tragen können
	// wenn nicht, betreffenden Spieler auf nicht akzeptierend setzen
	
	/*
	Item* item;
	if (m_accept[0]==true)
		DEBUG4("Spieler 1 akzeptiert\n");
	if (m_accept[1]==true)
		DEBUG4("Spieler 2 akzeptiert\n");
	
	DEBUG4("Trade::accept()\n");
	
	
	// Testen ob beide Partner dem Handel zugestimmt haben, wenn nicht abbrechen
	if (m_accept[0]==false || m_accept[1]==false)
	{
		return;
	}
	
	
	// Handel erfolgreich
	
	
	m_successful=true;
	m_finished = true;
	*/
	
}

void Trade::abortTrade() {
	
	// Handel abbrechen
	
	// nicht erfolgreich, Handel beendet

	DEBUG4("Handel abbrechen");
	
	m_successful=false;
	m_finished=true;
	
	// Da keiner Items bekommt kann Itemaustausch als fertig betrachtet werden
	m_got_items[0]=true;
	m_got_items[1]=true;
}


void Trade::onTrade(int index, ClientCommand* trade_command) {
	
	if (index!=0 && index!=1)
		return;
				
	/*	
	// Wenn Nutzer auf Handel abbrechen geklickt hat
	if (trade_command->m_button == BUTTON_TRADE_ABORT)
	{
		// Handel abbrechen
		abortTrade();
	}
	
	// Wenn Nutzer auf Handel akzeptieren geklickt hat
	if (trade_command->m_button == BUTTON_TRADE_ACCEPT)
	{
		// Handel für diesen Nutzer akzeptieren
		m_accept[index]=true;
		
		// Testen ob Handel komplett abgeschlossen werden kann
		acceptTrade();
	}
	
	Item* item;
	
	// Nutzer Verschiebt alle Items zwischen zwei Positionen
	if (trade_command->m_button == BUTTON_TRADE_ITEM_MOVE_ALL)
	{
		// Nach ändern der Handelssituation beide Partner auf nicht akzeptierend setzen
		m_accept[0]=false;
		m_accept[1]=false;
		std::map< int, Item >::iterator iter;
		std::map< int, Item >* itemmap = m_inventory[0]->getItems();
		// Verschieben vom eigenen Inventar zum Angebot
		if (trade_command->m_direction== INVENTORY_TO_OFFER)
		{
			DEBUG("Move all INVENTORY_TO_OFFER");
			if (trade_command->m_direction== INVENTORY_TO_OFFER)
			{
				itemmap = m_inventory[index]->getItems();
				for (iter = itemmap->begin(); iter != itemmap->end(); iter++)
				{
					item = &(iter->second);
					m_offer[index]->addItem(item);
					m_inventory[index]->removeItem(item);
				}
			}
		}
		
		// Verschieben vom eigenen Angebot zum Inventar
		if (trade_command->m_direction== OFFER_TO_INVENTORY)
		{
			DEBUG("Move all OFFER_TO_INVENTORY");
			if (trade_command->m_direction== OFFER_TO_INVENTORY)
			{
				itemmap = m_offer[index]->getItems();
				for (iter = itemmap->begin(); iter != itemmap->end(); iter++)
				{
					item = &(iter->second);
					m_inventory[index]->addItem(item);
					m_offer[index]->removeItem(item);
				}
			}
		}
		
		// Verschieben vom fremden Inventar zum Angebot
		if (trade_command->m_direction == TRADER_INVENTORY_TO_OFFER && m_player_type[1-index]!=OBJECTTYPE_USER)
		{
			DEBUG("Move all TRADER_INVENTORY_TO_OFFER");
			if (trade_command->m_direction == TRADER_INVENTORY_TO_OFFER && m_player_type[1-index]!=OBJECTTYPE_USER)
			{
				itemmap = m_inventory[1-index]->getItems();
				for (iter = itemmap->begin(); iter != itemmap->end(); iter++)
				{
					item = &(iter->second);
					m_offer[1-index]->addItem(item);
					m_inventory[1-index]->removeItem(item);
				}
			}
		}
		
		// Verschieben vom efremden Angebot ins Inventar
		if (trade_command->m_direction== TRADER_OFFER_TO_INVENTORY && m_player_type[1-index]!=OBJECTTYPE_USER)
		{
			DEBUG("Move all TRADER_OFFER_TO_INVENTORY");
			if (trade_command->m_direction== TRADER_OFFER_TO_INVENTORY && m_player_type[1-index]!=OBJECTTYPE_USER)
			{
				itemmap = m_offer[1-index]->getItems();
				for (iter = itemmap->begin(); iter != itemmap->end(); iter++)
				{
					item = &(iter->second);
					m_inventory[1-index]->addItem(item);
					m_offer[1-index]->removeItem(item);
				}
			}
		}
		
	}
	
	// Wenn Nutzer Item innerhalb des Handels verschiebt
	if (trade_command->m_button == BUTTON_TRADE_ITEM_MOVE)
	{
		// Nach ändern der Handelssituation beide Partner auf nicht akzeptierend setzen
		m_accept[0]=false;
		m_accept[1]=false;
				
		// Abbild des zu verschiebenden Items anlegen
		//item = new Item(trade_command->m_id, trade_command->m_number);
		
		
		// Verschieben vom eigenen Inventar zum Angebot
		if (trade_command->m_direction== INVENTORY_TO_OFFER)
		{
			item = m_inventory[index]->getItem(trade_command->m_id);
			if (item !=0)
			{
				if (item->getNumber()>=trade_command->m_number)
				{
					item->setNumber(trade_command->m_number);
					m_inventory[index]->removeItem(item);
					m_offer[index]->addItem(item);
				}
			}
			else
			{
				delete item;
			}
		}
		
		// Verschieben vom eigenen Inventar zum Angebot
		if (trade_command->m_direction== OFFER_TO_INVENTORY)
		{
			item = m_offer[index]->getItem(trade_command->m_id);
			if (item !=0)
			{
				if (item->getNumber()>=trade_command->m_number)
				{
					item->setNumber(trade_command->m_number);
					m_offer[index]->removeItem(item);
					m_inventory[index]->addItem(item);
				}
			}
			else
			{
				delete item;
			}
		}
		
		
		// Verschieben vom fremden Inventar zum Angebot
		if (trade_command->m_direction == TRADER_INVENTORY_TO_OFFER && m_player_type[1-index]!=OBJECTTYPE_USER)
		{
			item = m_inventory[1-index]->getItem(trade_command->m_id);
			if (item !=0)
			{
				if (item->getNumber()>=trade_command->m_number)
				{
					item->setNumber(trade_command->m_number);
					m_inventory[1-index]->removeItem(item);
					m_offer[1-index]->addItem(item);
				}
			}
			else
			{
				delete item;
			}
		}
		
		// Verschieben vom efremden Angebot ins Inventar
		if (trade_command->m_direction== TRADER_OFFER_TO_INVENTORY && m_player_type[1-index]!=OBJECTTYPE_USER)
		{
			item = m_offer[1-index]->getItem(trade_command->m_id);
			if (item !=0)
			{
				if (item->getNumber()>=trade_command->m_number)
				{
					item->setNumber(trade_command->m_number);
					m_offer[1-index]->removeItem(item);
					m_inventory[1-index]->addItem(item);
				}
			}
			else
			{
				delete item;
			}
		}
		
	}
	*/
}
