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
#include "warrior.h"

Warrior::Warrior(World* world, int id) : Player(world, id)
{
	init();
}



Warrior::~Warrior()
{
}

bool Warrior::init ()
{
	getTypeInfo()->m_subtype = "warrior";

	CreatureBaseAttr* bas = getBaseAttr();
	CreatureDynAttr* dyn = getDynAttr();

	// Basiswerte setzen
	dyn->m_experience=0;
	dyn->m_health = 200;

	bas->m_max_experience = 100;
	bas->m_level =1;
	bas->m_max_health = 200;
	bas->m_armor = 20;
	bas->m_block=0;
	bas->m_attack = 40;
	bas->m_strength = 50;
	bas->m_dexterity = 20;
	bas->m_magic_power = 10;
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
	bas->m_attack_speed=2000;

	m_base_action = Action::ATTACK;

	// Faehigkeiten setzen
	int i;
	for (i=1;i<6;i++)
		bas->m_abilities[i]=0;

	bas->m_abilities[0] = 0xffffff;
	bas->m_attack_range =1;

	bas->m_special_flags=0;


	m_name.assign("Boromir");

	m_base_action = Action::ATTACK;
	m_left_action = Action::ATTACK;
	m_right_action = Action::ATTACK;

	// Debugging

	bas->m_abilities[1] = 0xffffff;
	// monsterjaeger
	//bas->m_abilities[1] = 0x4ff;
	//bas->m_abilities[1] = 0xfffcff;
	// Ausdauer
	//bas->m_abilities[1]=0x10ff;


	// Inventar belegen
	Item* si;
	Equipement* equ = getEquipement();
	si = ItemFactory::createItem(Item::WEAPON,"short_sw");
	equ->swapItem(si,Equipement::MEDIUM_ITEMS);


	si = ItemFactory::createItem(Item::ARMOR,"leath_arm");
	equ->swapItem(si,Equipement::ARMOR);

	si = ItemFactory::createItem(Item::ARMOR,"heavy_arm");
	equ->swapItem(si,Equipement::BIG_ITEMS+2);

	si = ItemFactory::createItem(Item::HELMET,"steel_hlm");
	equ->swapItem(si,Equipement::HELMET);

	si = ItemFactory::createItem(Item::GLOVES,"leath_gl");
	equ->swapItem(si,Equipement::GLOVES);

	si = ItemFactory::createItem(Item::SHIELD,"wood_sh");
	equ->swapItem(si,Equipement::SHIELD);

	si = ItemFactory::createItem(Item::SHIELD,"iron_sh");
	equ->swapItem(si,Equipement::BIG_ITEMS+1);

	si = ItemFactory::createItem(Item::ARMOR,"leath_arm");
	equ->swapItem(si,Equipement::ARMOR);

	si = ItemFactory::createItem(Item::WEAPON,"holy_flail");
	equ->swapItem(si,Equipement::MEDIUM_ITEMS+1);

	si = ItemFactory::createItem(Item::WEAPON,"battle_axe");
	equ->swapItem(si,Equipement::BIG_ITEMS);

	si = ItemFactory::createItem(Item::POTION,"heal_1");
	equ->swapItem(si,Equipement::SMALL_ITEMS);

	si = ItemFactory::createItem(Item::POTION,"heal_1");
	equ->swapItem(si,Equipement::SMALL_ITEMS+5);

	si = ItemFactory::createItem(Item::POTION,"heal_1");
	equ->swapItem(si,Equipement::SMALL_ITEMS+10);

	si = ItemFactory::createItem(Item::POTION,"heal_1");
	equ->swapItem(si,Equipement::SMALL_ITEMS+11);

	si = ItemFactory::createItem(Item::POTION,"heal_bl");
	equ->swapItem(si,Equipement::SMALL_ITEMS+12);

	si = ItemFactory::createItem(Item::POTION,"heal_fr");
	equ->swapItem(si,Equipement::SMALL_ITEMS+13);


	si = ItemFactory::createItem(Item::POTION,"heal_2");
	equ->swapItem(si,Equipement::SMALL_ITEMS+1);

	si = ItemFactory::createItem(Item::POTION,"heal_2");
	equ->swapItem(si,Equipement::SMALL_ITEMS+21);


	calcBaseAttrMod();

	return true;
}


bool Warrior::update(float time)
{
	// Warriorspezifische update Routine

	// update von User aufrufen
	Player::update(time);
	return true;
}
