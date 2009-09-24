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

#include "itemlist.h"

#include "itemfactory.h"

//Constructors/Destructors

ItemList::ItemList(short max_small, short max_medium, short max_big)
{
	m_max_small = max_small;
	m_max_medium = max_medium;
	m_max_big = max_big;

	m_small_items = new Item*[max_small];
	m_medium_items = new Item*[max_medium];
	m_big_items = new Item*[max_big];

	 int i;
	 for (i=0; i<max_small;i++)
	 {
		 m_small_items[i]=0;
	 }

	 for (i=0; i<max_medium;i++)
	 {
		 m_medium_items[i]=0;
	 }

	 for (i=0; i<max_big;i++)
	 {
		 m_big_items[i]=0;
	 }
	 m_gold =0;
}

ItemList::ItemList(ItemList* itemlist)
{
	m_max_small = itemlist->m_max_small;
	m_max_medium = itemlist->m_max_medium;
	m_max_big = itemlist->m_max_big;

	m_small_items = new Item*[m_max_small];

	m_medium_items = new Item*[m_max_medium];

	m_big_items = new Item*[m_max_big];

	int i;
	for (i=0; i<m_max_small;i++)
	{
		m_small_items[i]=itemlist->m_small_items[i];
	}

	for (i=0; i<m_max_medium;i++)
	{
		m_medium_items[i]=itemlist->m_medium_items[i];
	}

	for (i=0; i<m_max_big;i++)
	{
		m_big_items[i]=itemlist->m_big_items[i];
	}
	m_gold =itemlist->m_gold;
}


ItemList::~ItemList()
{
	delete[] m_small_items;
	delete[] m_medium_items;
	delete[] m_big_items;
}

void ItemList::clear()
{
	int i;
	for (i=0; i<m_max_small;i++)
	{
		if (m_small_items[i]!=0)
			delete m_small_items[i];
		m_small_items[i]=0;
	}

	for (i=0; i<m_max_medium;i++)
	{
		if (m_medium_items[i])
		{
			delete m_medium_items[i];
		}
		m_medium_items[i]=0;
	}

	for (i=0; i<m_max_big;i++)
	{
		if (m_big_items[i])
			delete m_big_items[i];
		m_big_items[i]=0;
	}
	m_gold =0;
}


Item* ItemList::getItem(Item::Size m_size, int index)
{
	if (m_size == Item::SMALL && index < m_max_small)
		return m_small_items[index];

	if (m_size == Item::MEDIUM && index < m_max_medium)
		return m_medium_items[index];

	if (m_size == Item::BIG && index < m_max_big)
		return m_big_items[index];

	return 0;
}

int ItemList::getFreePlace(Item::Size size, bool useup_possible)
{
	Item** arr = m_small_items;
	short k = m_max_small;

	if (size == Item::MEDIUM)
	{
		arr = m_medium_items;
		k = m_max_medium;
	}

	if (size == Item::BIG)
	{
		arr = m_big_items;
		k = m_max_big;
	}

	int i=0;
	// im ersten drittel der kleinen Items duerfen nur verbrauchbare Gegenstaende sein
	if (size == Item::SMALL && !useup_possible)
	{
		i = k/3;
	}
	
	for (;i<k;i++)
	{
		if (arr[i]==0)
			return i;
	}
	return -1;
}

void ItemList::swapItem(Item* &item,Item::Size size, int index)
{
	DEBUG5("swapping %p of size %i at %i",item,size,index);
	Item** arr = m_small_items;
	short k = m_max_small;

	if (size == Item::MEDIUM)
	{
		arr = m_medium_items;
		k = m_max_medium;
	}

	if (size == Item::BIG)
	{
		arr = m_big_items;
		k = m_max_big;
	}

	if (index >=k)
		return;

	Item* t;
	t=arr[index];
	arr[index]=item;
	item = t;
}


Equipement::Equipement(short max_small, short max_medium, short max_big)
	: m_inventory(max_big,max_medium,max_small)
{
	for (int i=0; i<12; i++)
	{
		m_body_items[i] =0;
	}
	
	m_gold = 0;
};

Equipement::~Equipement()
{
	clear();
}

void Equipement::clear()
{
	for (int i=0; i<NR_BODY_ITEMS; i++)
	{
		if (m_body_items[i] !=0)
		{
			delete m_body_items[i];
			m_body_items[i] =0;
		}
	}
	
	DEBUG5("clearing equipement");
	m_inventory.clear();
	DEBUG5("done");
}

Item* Equipement::getItem(int pos)
{
	if (pos>0 && pos <NR_BODY_ITEMS)
	{
		return m_body_items[pos];
	}

	if (pos>=BIG_ITEMS && pos < MEDIUM_ITEMS)
	{
		return m_inventory.getItem(Item::BIG,pos-BIG_ITEMS);
	}

	if (pos>=MEDIUM_ITEMS && pos < SMALL_ITEMS)
	{
		return m_inventory.getItem(Item::MEDIUM,pos-MEDIUM_ITEMS);
	}

	if (pos>=SMALL_ITEMS)
	{
		return m_inventory.getItem(Item::SMALL,pos-SMALL_ITEMS);
	}

	return 0;

}



bool Equipement::swapItem(Item* &item,int pos)
{
	if (pos>= ARMOR && pos<=CURSOR_ITEM )
	{
		// Tausch mit einem Ausruestungsgegenstand
		std::swap(item, m_body_items[pos]);
		return true;
	}
	else
	{
		// Index berechnen bei dem getauscht werden soll
		int idx = pos-BIG_ITEMS;
		Item::Size size= Item::BIG;

		if (pos>= MEDIUM_ITEMS)
		{
			idx = pos - MEDIUM_ITEMS;
			size = Item::MEDIUM;
		}
		if (pos>=SMALL_ITEMS)
		{
			idx = pos - SMALL_ITEMS;
			size = Item::SMALL;
		}

		// Tausch mit Gegenstand im Inventar
		m_inventory.swapItem(item,size,idx);
		DEBUG5("cursor item %p %p",getItem(Equipement::CURSOR_ITEM), &(m_body_items[CURSOR_ITEM ]));
		
		return true;
	}
}

bool Equipement::swapCursorItem(int pos)
{
	bool ret = swapItem(m_body_items[CURSOR_ITEM],pos);
	return ret;
}

short  Equipement::insertItem(Item* item, bool check_useup,bool use_equip, bool use_secondary)
{
	if (item ==0)
		return NONE;

	if (item->m_size == Item::GOLD)
	{
		m_gold += item->m_price;
		delete item;
		return GOLD;
	}
	
	if (use_equip)
	{
		if (item->m_type == Item::WEAPON && !use_secondary && m_body_items[WEAPON] == 0 && (item->m_weapon_attr->m_two_handed == false || m_body_items[SHIELD] == 0))
		{
			swapItem(item,WEAPON);
			return WEAPON;
		}
		else if (item->m_type == Item::WEAPON && use_secondary && m_body_items[WEAPON2] == 0  && (item->m_weapon_attr->m_two_handed == false || m_body_items[SHIELD2] == 0))
		{
			swapItem(item,WEAPON2);
			return WEAPON2;
		}
		else if (item->m_type == Item::SHIELD && !use_secondary && m_body_items[SHIELD] == 0 && (m_body_items[WEAPON] == 0 || m_body_items[WEAPON]->m_weapon_attr->m_two_handed == false))
		{
			swapItem(item,SHIELD);
			return SHIELD;
		}
		else if (item->m_type == Item::SHIELD && use_secondary && m_body_items[SHIELD2] == 0 && (m_body_items[WEAPON2] == 0 || m_body_items[WEAPON2]->m_weapon_attr->m_two_handed == false))
		{
			swapItem(item,SHIELD2);
			return SHIELD2;
		}
		else if (item->m_type == Item::ARMOR && m_body_items[ARMOR] == 0)
		{
			swapItem(item,ARMOR);
			return ARMOR;
		}
		else if (item->m_type == Item::GLOVES && m_body_items[GLOVES] == 0)
		{
			swapItem(item,GLOVES);
			return GLOVES;
		}
		else if (item->m_type == Item::HELMET && m_body_items[HELMET] == 0)
		{
			swapItem(item,HELMET);
			return HELMET;
		}
		else if (item->m_type == Item::RING && m_body_items[RING_LEFT] == 0)
		{
			swapItem(item,RING_LEFT);
			return RING_LEFT;
		}
		else if (item->m_type == Item::RING && m_body_items[RING_RIGHT] == 0)
		{
			swapItem(item,RING_RIGHT);
			return RING_RIGHT;
		}
		else if (item->m_type == Item::AMULET && m_body_items[AMULET] == 0)
		{
			swapItem(item,AMULET);
			return AMULET;
		}
	}
	
	Item* itm = item;
	int pos;
	bool useup = (item->m_useup_effect != 0);
	if (!check_useup)
		useup = true;
	pos = m_inventory.getFreePlace(item->m_size,useup);
	DEBUG5("free pos: %i",pos);


	if (pos ==-1)
	{
		return NONE;
	}

	if (item->m_size == Item::SMALL)
	{
		swapItem(itm,pos+SMALL_ITEMS);
		return pos+SMALL_ITEMS;
	}
	if (item->m_size == Item::MEDIUM)
	{
		swapItem(itm,pos+MEDIUM_ITEMS);
		return pos+MEDIUM_ITEMS;
	}
	if (item->m_size == Item::BIG)
	{
		swapItem(itm,pos+BIG_ITEMS);
		return pos+BIG_ITEMS;
	}

	ERRORMSG("unknown item size item type %i subtype %s",item->m_type,item->m_subtype.c_str());
	return NONE;

}

short Equipement::findItem(Item::Subtype subtype,short startpos)
{
	int start = startpos;
	for (int i=std::max(0,start); i<CURSOR_ITEM; i++)
	{
		if (m_body_items[i] !=0 && m_body_items[i]->m_subtype == subtype)
			return i;
	}
	
	Item* item;
	for (int i= std::max(int(BIG_ITEMS),start); i< BIG_ITEMS+m_inventory.getMaxBig(); i++)
	{
		item = m_inventory.getItem(Item::BIG, i - BIG_ITEMS);
		if (item != 0 && item->m_subtype == subtype)
			return i;
	}
	
	for (int i= std::max(int(BIG_ITEMS),start); i< BIG_ITEMS+m_inventory.getMaxBig(); i++)
	{
		item = m_inventory.getItem(Item::BIG, i - BIG_ITEMS);
		if (item != 0 && item->m_subtype == subtype)
			return i;
	}
	
	for (int i= std::max(int(MEDIUM_ITEMS),start); i< MEDIUM_ITEMS+m_inventory.getMaxMedium(); i++)
	{
		item = m_inventory.getItem(Item::MEDIUM, i - MEDIUM_ITEMS);
		if (item != 0 && item->m_subtype == subtype)
			return i;
	}
	
	for (int i= std::max(int(SMALL_ITEMS),start); i< SMALL_ITEMS+m_inventory.getMaxSmall(); i++)
	{
		item = m_inventory.getItem(Item::SMALL, i - SMALL_ITEMS);
		if (item != 0 && item->m_subtype == subtype)
			return i;
	}
	
	return NONE;
}

short Equipement::stringToPosition(std::string posstr, bool secondary)
{
	if (posstr == "armor")
		return ARMOR;
	else if (posstr == "helmet")
		return HELMET;
	else if (posstr == "gloves")
		return GLOVES;
	else if (posstr == "shield")
	{
		if (!secondary)
			return SHIELD;
		else
			return SHIELD2;
	}
	else if (posstr == "weapon")
	{
		if (!secondary)
			return WEAPON;
		else
			return WEAPON2;
	}
	else if (posstr == "ring_left")
		return RING_LEFT;
	else if (posstr == "ring_right")
		return RING_RIGHT;
	else if (posstr == "amulet")
		return AMULET;
	else if (posstr == "big_items")
		return BIG_ITEMS;
	else if (posstr == "medium_items")
		return MEDIUM_ITEMS;
	else if (posstr == "small_items")
		return SMALL_ITEMS;

 	return NONE;
}

int Equipement::getNumberItems()
{
	int nr =0;
	
	for (int i=0; i<NR_BODY_ITEMS; i++)
	{
		if (m_body_items[i] !=0)
		{
			nr ++;
		}
	}
	
	int i;
	for (i=0;i<m_inventory.getMaxBig();i++)
	{
		if (m_inventory.getItem(Item::BIG,i))
			nr++;
	}

	for (i=0;i<m_inventory.getMaxMedium();i++)
	{
		if (m_inventory.getItem(Item::MEDIUM,i))
			nr++;
	}

	for (i=0;i<m_inventory.getMaxSmall();i++)
	{
		if (m_inventory.getItem(Item::SMALL,i))
			nr++;
	}



	return nr;
}

int Equipement::getMaxItemNumber(Item::Size size)
{
	int pos =0;
	if (size == Item::BIG)
	{
		int i;
		for (i=0;i<m_inventory.getMaxBig();i++)
		{
			if (m_inventory.getItem(Item::BIG,i))
				pos = i;
		}
	}
	else if(size == Item::MEDIUM)
	{
		int i;
		for (i=0;i<m_inventory.getMaxMedium();i++)
		{
			if (m_inventory.getItem(Item::MEDIUM,i))
				pos = i;
		}
	}
	else if(size == Item::SMALL)
	{
		int i;
		for (i=0;i<m_inventory.getMaxSmall();i++)
		{
			if (m_inventory.getItem(Item::SMALL,i))
				pos = i;
		}
	}
	return pos;
}

void Equipement::toString(CharConv* cv)
{

	short nr = getNumberItems();
	cv->toBuffer<short>(nr);
	
	for (int i=0; i<NR_BODY_ITEMS; i++)
	{
		if (m_body_items[i] !=0)
		{
			cv->printNewline();
			cv->toBuffer(i);
			m_body_items[i]->toString(cv);
		}
	}


	// grosse Items
	Item* it;
	for (int i=0;i<m_inventory.getMaxBig();i++)
	{
		it = m_inventory.getItem(Item::BIG,i);
		if (it!=0)
		{
			cv->printNewline();
			cv->toBuffer<short>(BIG_ITEMS+i);
			it->toString(cv);
			nr++;
		}
	}

	// mittlere Items
	for (int i=0;i<m_inventory.getMaxMedium();i++)
	{
		it = m_inventory.getItem(Item::MEDIUM,i);
		if (it!=0)
		{
			cv->printNewline();
			cv->toBuffer<short>(MEDIUM_ITEMS+i);
			it->toString(cv);
			nr++;
		}
	}

	// kleine Items
	for (int i=0;i<m_inventory.getMaxSmall();i++)
	{
		it = m_inventory.getItem(Item::SMALL,i);
		if (it!=0)
		{
			cv->printNewline();
			cv->toBuffer<short>(SMALL_ITEMS+i);
			it->toString(cv);
			nr++;
		}
	}



}

void Equipement::fromString(CharConv* cv)
{
	clear();
	short nr;
	cv->fromBuffer<short>(nr);
	
	int i;
	short pos;
	Item* it;
	
	std::string subtype;
	char type;
	int id;
	
	for (i=0;i<nr;i++)
	{
		cv->fromBuffer(pos);
		cv->fromBuffer(type);
		cv->fromBuffer(subtype);
		cv->fromBuffer(id);
		
		it = ItemFactory::createItem((Item::Type) type, std::string(subtype),id);
		
		// Datenfelder des Items belegen
		it->fromString(cv);

		DEBUG5("creating item %p",it);

		// Item ins Inventar tauschen
		swapItem(it,pos);
	}

}

void Equipement::toStringComplete(CharConv* cv)
{

	short nr = getNumberItems();
	cv->toBuffer<short>(nr);
	
	for (int i=0; i<NR_BODY_ITEMS; i++)
	{
		if (m_body_items[i] !=0)
		{
			cv->printNewline();
			cv->toBuffer<short>(i);
			m_body_items[i]->toStringComplete(cv);
		}
	}


	// grosse Items
	Item* it;
	for (int i=0;i<m_inventory.getMaxBig();i++)
	{
		it = m_inventory.getItem(Item::BIG,i);
		if (it!=0)
		{
			cv->printNewline();
			cv->toBuffer<short>(BIG_ITEMS+i);
			it->toStringComplete(cv);
			nr++;
		}
	}

	// mittlere Items
	for (int i=0;i<m_inventory.getMaxMedium();i++)
	{
		it = m_inventory.getItem(Item::MEDIUM,i);
		if (it!=0)
		{
			cv->printNewline();
			cv->toBuffer<short>(MEDIUM_ITEMS+i);
			it->toStringComplete(cv);
			nr++;
		}
	}

	// kleine Items
	for (int i=0;i<m_inventory.getMaxSmall();i++)
	{
		it = m_inventory.getItem(Item::SMALL,i);
		if (it!=0)
		{
			cv->printNewline();
			cv->toBuffer<short>(SMALL_ITEMS+i);
			it->toStringComplete(cv);
			nr++;
		}
	}
	cv->printNewline();
	cv->toBuffer(m_gold);

}

void Equipement::fromStringComplete(CharConv* cv)
{
	clear();
	short nr;
	cv->fromBuffer<short>(nr);
	
	int i;
	short pos;
	Item* it;
	std::string subtype;
	char type;
	int id;
	
	for (i=0;i<nr;i++)
	{
		cv->fromBuffer(pos);
		cv->fromBuffer(type);
		cv->fromBuffer(subtype);
		cv->fromBuffer(id);
		if (World::getWorld() !=0 && World::getWorld()->isServer())
			id =0;
		DEBUG5("pos %i type %i subtype %s",pos,type, subtype.c_str());
		
		it = ItemFactory::createItem((Item::Type) type, std::string(subtype),id);
		
		// Datenfelder des Items belegen
		it->fromStringComplete(cv);

		DEBUG5("creating item %p",it);

		// Item ins Inventar tauschen
		swapItem(it,pos);
	}
	
	cv->fromBuffer(m_gold);

}










