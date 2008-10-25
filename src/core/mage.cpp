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

#include <string>
#include "mage.h"

Mage::Mage( int id) : Player( id)
{
	init();
}



Mage::~Mage()
{
}

bool Mage::init ()
{
	// Attribute auf Basiswerte setzen
	getTypeInfo()->m_subtype = "mage";

	CreatureBaseAttr* bas = getBaseAttr();
	CreatureDynAttr* dyn = getDynAttr();


	dyn->m_experience=0;
	dyn->m_health = 100;

	bas->m_max_experience = 100;
	bas->m_level =60;
	bas->m_max_health = 100;
	bas->m_armor = 15;
	bas->m_block=0;
	bas->m_attack = 10;
	bas->m_strength = 15;
	bas->m_dexterity = 15;
	bas->m_magic_power = 45;
	bas->m_willpower = 25;
	bas->m_resistances[0] =0;
	bas->m_resistances[1] =0;
	bas->m_resistances[2] =0;
	bas->m_resistances[3] =0;
	bas->m_resistances_cap[0] =50;
	bas->m_resistances_cap[1] =50;
	bas->m_resistances_cap[2] =50;
	bas->m_resistances_cap[3] =50;
	bas->m_walk_speed = 3000;
	bas->m_attack_speed=1500;
	m_base_action = Action::MAGIC_ATTACK;
	
	Item* si;
	
	si = ItemFactory::createItem(Item::WEAPON,"ice_wand");
	insertItem(si);
	si = ItemFactory::createItem(Item::WEAPON,"ice_staff");
	insertItem(si);

	// Alle Faehigkeiten deaktivieren
	int i;
	for (i=1;i<6;i++)
		bas->m_abilities[i]=0;

	// Debugging
	bas->m_abilities[3] = 0xffffff;
	// entzuenden
	//bas->m_abilities[3] = 0x7f7fff;
	// klirrende Kaelte
	//bas->m_abilities[3] = 0x7fff7f;
	// Ionisierung
	//bas->m_abilities[3] = 0xff7f7f;

	// Basisfaehigkeiten (normaler Angriff etc) erlauben
	bas->m_abilities[0] = 0x3f4f;
	bas->m_attack_range =20;

	m_base_action = Action::MAGIC_ATTACK;
	m_left_action = Action::MAGIC_ATTACK;
	m_right_action = Action::MAGIC_ATTACK;


	bas->m_special_flags=0;

	m_name.assign("Gandalf");
	// Modifizierte Basisattribute erzeugen
	calcBaseAttrMod();

	return true;
}


bool Mage::update(float time)
{
	// Magespezifische update Routine

	// update von User aufrufen
	Player::update(time);
	return true;
}

