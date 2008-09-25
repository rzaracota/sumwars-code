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

int ItemList::getFreePlace(Item::Size m_size)
{
	Item** arr = m_small_items;
	short k = m_max_small;
	
	if (m_size == Item::MEDIUM)
	{
		arr = m_medium_items;
		k = m_max_medium;
	}
	
	if (m_size == Item::BIG)
	{
		arr = m_big_items;
		k = m_max_big;
	}
	
	int i;
	for (i=0;i<k;i++)
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
		short k = m_max_medium;
	}
	
	if (size == Item::BIG)
	{
		arr = m_big_items;
		short k = m_max_big;
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
	m_helmet =0;
	m_armor =0;
	m_gloves =0;
	m_weapon=0;
	m_weapon2=0;
	m_shield=0;
	m_shield2=0;
	m_ring_left=0;
	m_ring_right=0;
	m_amulet=0;
	m_cursor_item=0;
};

Equipement::~Equipement()
{
	clear();
}

void Equipement::clear()
{
	if (m_helmet !=0) 
	{
		delete m_helmet;
		m_helmet=0;
	}
	if (m_armor !=0)
	{
		delete m_armor;
		m_armor=0;
	}
	if (m_gloves !=0)
	{
		delete m_gloves;
		m_gloves=0;
	}
	
	if (m_weapon!=0)
	{
		delete m_weapon;
		m_weapon=0;
	}
	
	if (m_weapon2!=0)
	{
		delete m_weapon2;
		m_weapon=0;
	}
	
	
	if (m_shield!=0)
	{
		delete m_shield;
		m_shield=0;
	}
	
	if (m_shield2!=0)
	{
		delete m_shield2;
		m_shield=0;
	}

	
	if (m_ring_left!=0)
	{
		delete m_ring_left;
		m_ring_left =0;
	}
	if (m_ring_right!=0)
	{
		delete m_ring_right;
		m_ring_right =0;
	}
	if (m_amulet!=0)
	{
		delete m_amulet;
		m_amulet =0;
	}
	if (m_cursor_item!=0)
	{
		delete m_cursor_item;
		m_cursor_item=0;
	}
	DEBUG5("clearing equipement");
	m_inventory.clear();
	DEBUG5("done");
}

Item* Equipement::getItem(int pos)
{
	switch (pos)
	{
		case ARMOR:
			return m_armor;
			
		case WEAPON:
			return m_weapon;
			
		case WEAPON2:
			return m_weapon2;
			
		case SHIELD:
			return m_shield;
			
		case SHIELD2:
			return m_shield2;
			
		case HELMET:
			return m_helmet;
			
		case GLOVES:
			return m_gloves;
			
		case RING_LEFT:
			return m_ring_left;
			
		case RING_RIGHT:
			return m_ring_right;
			
		case AMULET:
			return m_amulet;
			
		case CURSOR_ITEM:
			return m_cursor_item;
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


Item* &  Equipement::getItemRef(int pos)
{
	switch (pos)
	{
		case ARMOR:
			return m_armor;
			
		case WEAPON:
			return m_weapon;
			
		case WEAPON2:
			return m_weapon2;
			
		case SHIELD:
			return m_shield;
			
		case SHIELD2:
			return m_shield2;

			
		case HELMET:
			return m_helmet;
			
		case GLOVES:
			return m_gloves;
			
		case RING_LEFT:
			return m_ring_left;
			
		case RING_RIGHT:
			return m_ring_right;
			
		case AMULET:
			return m_amulet;
	}	
	
	return m_cursor_item;
}

bool Equipement::swapItem(Item* &item,int pos)
{
	DEBUG5("swapping %p at %i",item,pos);
	if (pos>= ARMOR && pos<=CURSOR_ITEM )
	{
		// Tausch mit einem Ausruestungsgegenstand
		std::swap(item, getItemRef(pos));	
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
		return true;
	}		
}

bool Equipement::swapCursorItem(int pos)
{
	return swapItem(m_cursor_item,pos);
}
	
bool  Equipement::insertItem(Item* item)
{
	if (item ==0)
		return true;
	
	Item* itm = item;
	int pos;
	pos = m_inventory.getFreePlace(item->m_size);
	DEBUG5("free pos: %i",pos);
	
	
	if (pos ==-1)
	{
		return false;
	}
	
	if (item->m_size == Item::SMALL)
	{
		swapItem(itm,pos+SMALL_ITEMS);
	}
	if (item->m_size == Item::MEDIUM)
	{
		swapItem(itm,pos+MEDIUM_ITEMS);
	}
	if (item->m_size == Item::BIG)
	{
		swapItem(itm,pos+BIG_ITEMS);
	}
	
	return true;
}
	
int Equipement::getNumberItems(bool secondary_equip)
{
	int nr =0;
	if (!secondary_equip)
	{
		if (m_weapon)
			nr++;
		if (m_shield)
			nr++;
	}
	else
	{
		if (m_weapon2)
			nr++;
		if (m_shield2)
			nr++;
	}
	if (m_helmet)
		nr++;
	if (m_armor)
		nr++;
	if (m_amulet)
		nr++;
	if (m_ring_left)
		nr++;
	if (m_ring_right)
		nr++;
	if (m_gloves)
		nr++;
	if (m_cursor_item)
		nr++;
	
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
	
void Equipement::toString(CharConv* cv, int & nr, bool secondary_equip)
{
/*
	nr=0;
	// Ausruestungsgegenstaende
	if (m_armor)
	{
		m_armor->toString(cv,ARMOR);
		nr++;
	}
	
	Item* weapon = m_weapon;
	if (secondary_equip)
	{
		weapon = m_weapon2;
	}
	if (weapon)
	{
		weapon->toString(cv,WEAPON);
		nr++;
	}
	
	if (m_helmet)
	{
		 m_helmet->toString(cv,HELMET);
		nr++;
	}
	
	Item* shield = m_shield;
	if (secondary_equip)
	{
		shield = m_shield2;
	}
	if (shield)
	{
		shield->toString(cv,SHIELD);
		nr++;
	}
	
	if (m_gloves)
	{
		m_gloves->toString(cv,GLOVES);
		nr++;
	}
	
	if (m_ring_left)
	{
		m_ring_left->toString(cv,RING_LEFT);
		nr++;
	}
	
	if (m_ring_right)
	{
		 m_ring_right->toString(cv,RING_RIGHT);
		nr++;
	}
	
	if (m_amulet)
	{
		 m_amulet->toString(cv,AMULET);
		nr++;
	}
	
	if (m_cursor_item)
	{
		m_cursor_item->toString(cv,CURSOR_ITEM);
		nr++;
	}
	
	
	int i;
	
	// grosse Items
	Item* it;
	for (i=0;i<m_inventory.getMaxBig();i++)
	{
		it = m_inventory.getItem(Item::BIG,i);
		if (it!=0)
		{
			it->toString(cv,BIG_ITEMS+i);
			nr++;
		}
	}
	
	// mittlere Items
	for (i=0;i<m_inventory.getMaxMedium();i++)
	{
		it = m_inventory.getItem(Item::MEDIUM,i);
		if (it!=0)
		{
			it->toString(cv,MEDIUM_ITEMS+i);
			nr++;
		}
	}
	
	// grosse Items
	for (i=0;i<m_inventory.getMaxSmall();i++)
	{
		it = m_inventory.getItem(Item::SMALL,i);
		if (it!=0)
		{
			it->toString(cv,SMALL_ITEMS+i);
			nr++;
		}
	}
*/
	

}

void Equipement::fromString(CharConv* cv, int nr)
{	

/*
	int i;
	short pos;
	Item* it;
	for (i=0;i<nr;i++)
	{
		it = new Item;
		// Datenfelder des Items belegen
		it->fromString(cv,pos);
		
		DEBUG5("creating item %p",it);
		
		// Item ins Inventar tauschen
		swapItem(it,pos);
	}
	
	*/

}










