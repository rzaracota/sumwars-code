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

#include "trader.h"

//Constructors
Trader::Trader(World* world, unsigned int id) : Player( world,id)
{
	bool tmp=Trader::init();
	if (!tmp)
	{
		DEBUG("Initialisierung des Händlers fehlgeschlagen!");
	}
}

Trader::Trader(World* world, float coordinate_x, float coordinate_y, unsigned int id) : Player(world, coordinate_x, coordinate_y,  id)
{
	getObjectType().setObjectSubtype(SUBTYPE_UNIVERSAL_TRADER);
	bool tmp=Trader::init();
	if (!tmp)
	{
		DEBUG("Initialisierung des Händlers fehlgeschlagen!");
	}
}

Trader::Trader(World* world, float coordinate_x, float coordinate_y, unsigned int id, int object_subtype) : Player(world, coordinate_x, coordinate_y,  id)
{
	getObjectType().setObjectSubtype(object_subtype);
	bool tmp=Trader::init();
	if (!tmp)
	{
		DEBUG("Initialisierung des Händlers fehlgeschlagen!");
	}
}

Trader::~Trader()
{
	// allokierten Speicher freigeben
}
//Methods
bool Trader::init()
{
	//eigene Initialisierung
	getObjectType().setObjectType(OBJECTTYPE_TRADER);

	setHealth(10);
	setMaxHealth(10);
	
	Item * item = new Item(ITEM_GOLD,1);
	setMaxBaggage(10000000);
	int typ = getObjectType().getObjectSubtype();
		
	if (typ == SUBTYPE_UNIVERSAL_TRADER || typ ==SUBTYPE_WEAPON_TRADER)
	{
		item->init(ITEM_BROADSWORD,10);
		getItems()->addItem(item);
		
		item->init(ITEM_LONGSWORD,10);
		getItems()->addItem(item);
		
		item->init(ITEM_GREATSWORD,10);
		getItems()->addItem(item);
		
		item->init(ITEM_GIANTSWORD,10);
		getItems()->addItem(item);
	}
	
	if (typ == SUBTYPE_UNIVERSAL_TRADER || typ ==SUBTYPE_ARMOR_TRADER)
	{
		item->init(ITEM_RINGMAIL,10);
		getItems()->addItem(item);
		item->init(ITEM_PLATEARMOR,10);
		getItems()->addItem(item);
		item->init(ITEM_ELVENMAIL,10);
		getItems()->addItem(item);
		item->init(ITEM_MITHRILMAIL,10);
		getItems()->addItem(item);
		
		item->init(ITEM_IRONSHIELD,10);
		getItems()->addItem(item);
		item->init(ITEM_STEELSHIELD,10);
		getItems()->addItem(item);
		item->init(ITEM_ELVENSHIELD,10);
		getItems()->addItem(item);
		item->init(ITEM_MITHRILSHIELD,10);
		getItems()->addItem(item);
		
	}
	
	if (typ == SUBTYPE_UNIVERSAL_TRADER || typ == SUBTYPE_POTION_TRADER)
	{
		item->init(ITEM_HEALINGPOTION,100);
		getItems()->addItem(item);
		item->init(ITEM_BIGHEALINGPOTION,100);
		getItems()->addItem(item);
		item->init(ITEM_FULLHEALINGPOTION,100);
		getItems()->addItem(item);

	}
	delete item;
	strcpy(getName(),"Haendler");
	
	return true;
}

bool  Trader::update(float time)
{
	// Trade spezifisches Update
	DEBUG5("Update des Händlerobjekts [%i] wird gestartet", getId());
	// Wenn ein Handel existiert
	if (getTradeId() !=0)
	{
		DEBUG5("Suche Handelsobjekt des Händlers");
		Trade* trade = getWorld()->getTrade(getTradeId());
		DEBUG5("Ergebnis: %p",trade);
		
		// Wenn der Handel nicht existiert Fehler ausgeben
		if (trade == 0)
		{
			setTradeId(0);
			return false;
			
		}
		else
		{
			// eigene Position bei dem Handel bestimmen
			int idx = trade->getPlayerIndex(getId());
		
			DEBUG5("Händlerobjekt [%i] hat Handelsobjekt [%i] als seinen Handel erkannt", getId(), getTradeId());
			// Abfragen ob der Handel bereits abgeschlossen ist.
			if (idx == -1)
			{
				setTradeId(0);
			}
			else if (trade->getFinished()==true)
			{
				// Abfragen ob der Handel erfolgreich war
				if (trade->getSuccessful()==true)
				{
					
					// Items auf erhalten setzen, aber NICHT austauschen
					trade->setGotItems(true, idx);
				}
				
				// Handel ist erledigt, id auf 0 setzen
				setTradeId(0);
			}
			else
			{
				// Handel ist noch nicht abgeschlossen
				
				// Testen Handel aktuell akzeptiert wird
				if (trade->getAccept(idx)==false)
				{
					// Handel steht aktuell auf nicht akzeptierend
					// Prüfen ob Handel akzeptiert werden könnte
					
					int value_own=0, value_pl=0;
					
					map< int, Item >::iterator iter;
					map< int, Item >* itemmap = trade->getOffer(idx)->getItems();
					Item* item;
					
					// Wert des eigenen Angebots aufsummieren
					for (iter = itemmap->begin(); iter != itemmap->end(); iter++)
					{
						item = &(iter->second);
						value_own += item->getNumber()* item-> getPrice();
					}
					DEBUG5("Haendlerangebot ist Wert: %i", value_own);
					// Wert des fremden Angebots aufsummieren
					itemmap = trade->getOffer(1-idx)->getItems();
					
					for (iter = itemmap->begin(); iter != itemmap->end(); iter++)
					{
						item = &(iter->second);
						value_pl += item->getNumber()* item-> getPrice();
					}
					DEBUG5("Spielerangebot ist Wert: %i", value_pl);
					if (value_own == value_pl)
					{
						// Handel geht genau auf, akzeptieren
						DEBUG5("Haendler ist einverstanden so wie es ist.");
						trade->setAccept(true,idx);
					}
					
					if (value_own < value_pl)
					{
						// Haendler bietet weniger als der Spieler, mit Gold auffüllen
						
						item = new Item(ITEM_GOLD,value_pl-value_own);
						trade->getOffer(idx)->addItem(item);
						
						// Handel akzeptieren
						DEBUG5("Haendler ist einverstanden, gibt %i Gold mehr an Wechselgeld", value_pl-value_own);
						trade->setAccept(true,idx);
						// Item Objekt löschen
						delete item;

					}
					
					if (value_own > value_pl)
					{
						// Haendler bietet mehr als der Spieler
						
						// testen ob der Haendler Gold im Angebot hat
						item = trade->getOffer(idx)->getItem(ITEM_GOLD);
						
						// Wenn ja
						if (item !=0)
						{
							// Wenn mehr Gold im Angebot ist als zum Ausgleichen nötig
							if (item->getNumber() >= value_own- value_pl)
							{
								// Gold aus dem Angebot entfernen
								item->setNumber(value_own- value_pl);
								trade->getOffer(idx)->removeItem(item);
								
								// Handel akzeptieren
								DEBUG5("Haendler gibt %i Gold weniger an Wechselgeld ist aber einverstanden\n", value_own-value_pl);
								trade->setAccept(true,idx);
								
							}
							else
							{
								// alles Gold aus dem Angebot entfernen
								DEBUG5("Haendler entfernt das gesamte Wechselgeld");
								trade->getOffer(idx)->removeItem(item);
							}
							
						}
						if (item!=0)
							delete item;
					}
				}
				else
				{
					//Haendler akzeptierte bereits vorher, lag wohl am Spieler...
					//printf("Haendler akzeptierte bereits vor diesem Update\n");
					//trade->acceptTrade();
				}
			}
		}
	
		if (getCommand()->getType() == COMMAND_TRADE && getTradeId()==0)
		{
			getCommand()->setType(COMMAND_NONE);
			getObjectType().setActionType(ACTION_NONE);
			getAction()->setType(ACTION_NONE);

		}

	}
	
	// Update von Player aufrufen
	Player::update(time);
}
