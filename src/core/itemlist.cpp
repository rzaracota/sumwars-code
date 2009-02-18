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
	switch (pos >0 && pos <NR_BODY_ITEMS)
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
	DEBUG5("swapping %p at %i",item,pos);
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
		return true;
	}
}

bool Equipement::swapCursorItem(int pos)
{
	return swapItem(m_body_items[CURSOR_ITEM],pos);
}

short  Equipement::insertItem(Item* item)
{
	if (item ==0)
		return NONE;

	if (item->m_size == Item::GOLD)
	{
		m_gold += item->m_price;
		delete item;
		return GOLD;
	}
	
	Item* itm = item;
	int pos;
	bool useup = (item->m_useup_effect != 0);
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


void Equipement::toString(CharConv* cv)
{

	short nr = getNumberItems();
	cv->toBuffer<short>(nr);
	
	for (int i=0; i<NR_BODY_ITEMS; i++)
	{
		if (m_body_items[i] !=0)
		{
			cv->toBuffer<char>(i);
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
			cv->toBuffer<char>(BIG_ITEMS+i);
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
			cv->toBuffer<char>(MEDIUM_ITEMS+i);
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
			cv->toBuffer<char>(SMALL_ITEMS+i);
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
	char pos;
	Item* it;
	for (i=0;i<nr;i++)
	{
		cv->fromBuffer<char>(pos);
		it = new Item;
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
			cv->toBuffer<char>(i);
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
			cv->toBuffer<char>(BIG_ITEMS+i);
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
			cv->toBuffer<char>(MEDIUM_ITEMS+i);
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
			cv->toBuffer<char>(SMALL_ITEMS+i);
			it->toStringComplete(cv);
			nr++;
		}
	}



}

void Equipement::fromStringComplete(CharConv* cv)
{
	clear();
	short nr;
	cv->fromBuffer<short>(nr);
	
	int i;
	char pos;
	Item* it;
	for (i=0;i<nr;i++)
	{
		cv->fromBuffer<char>(pos);
		it = new Item;
		// Datenfelder des Items belegen
		it->fromStringComplete(cv);

		DEBUG5("creating item %p",it);

		// Item ins Inventar tauschen
		swapItem(it,pos);
	}

}










