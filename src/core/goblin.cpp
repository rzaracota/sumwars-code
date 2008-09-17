/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Hans Wulf, Daniel Erler

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

#include "goblin.h"

Goblin::Goblin(World* world, int id) : Monster( world,  id)
{			
	bool tmp = init();
	
}



Goblin::~Goblin()
{
	// allokierten Speicher freigeben
}

//Methods
bool Goblin::init ()
{
	getTypeInfo()->m_subtype = TypeInfo::SUBTYPE_GOBLIN;
	getTypeInfo()->m_category = TypeInfo::GOBLIN;
	getTypeInfo()->m_fraction = TypeInfo::FRAC_MONSTER;
			
	CreatureBaseAttr* bas = getBaseAttr();
	CreatureDynAttr* dyn = getDynAttr();
	
	float p0[4] ={0.1,0.2,0.2,0.2};
	m_drop_slots[0].init(p0, 0,20, 0.3, 500);
	m_drop_slots[1].init(p0,0, 10, 0.3, 1000);
	
	dyn->m_experience=0;
	dyn->m_health = 150;
	
	bas->m_max_experience = 100000;
	bas->m_level =1;
	bas->m_max_health = 150;
	bas->m_armor = 10;
	bas->m_block=0;
	bas->m_attack = 10;
	bas->m_strength = 15;
	bas->m_dexterity = 10;
	bas->m_magic_power = 5;
	bas->m_willpower = 10;
	bas->m_resistances[0] =0;
	bas->m_resistances[1] =0;
	bas->m_resistances[2] =0;
	bas->m_resistances[3] =0;
	bas->m_resistances_cap[0] =50;
	bas->m_resistances_cap[1] =50;
	bas->m_resistances_cap[2] =50;
	bas->m_resistances_cap[3] =50;
	bas->m_walk_speed = 2000;
	bas->m_attack_speed=1500;
	bas->m_step_length = 0.5;
	m_base_action = Action::ATTACK;
	
	
	int i;
	for (i=1;i<6;i++)
		bas->m_abilities[i]=0;
	
	// Debugging
	//bas->m_abilities[0] = 0xf1f;
	bas->m_abilities[0] = 0xf2f;	
	//bas->m_abilities[1]=0x10;
	//bas->m_abilities[2] = 0x2;
	bas->m_abilities[3] = 0x2;
	//bas->m_abilities[3] = 0x14;
	
	bas->m_attack_range =1;
	
	bas->m_special_flags=0;

	calcBaseAttrMod();
	
	return true;
}

