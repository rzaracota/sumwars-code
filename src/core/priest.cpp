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
#include "priest.h"

Priest::Priest(World* world, int id) : Player(world, id)
{
	init();
}



Priest::~Priest()
{
}

bool Priest::init ()
{
	getTypeInfo()->m_subtype = "priest";

	CreatureBaseAttr* bas = getBaseAttr();
	CreatureDynAttr* dyn = getDynAttr();


	dyn->m_experience=0;
	dyn->m_health = 200;
	dyn->m_health = 200;

	bas->m_max_experience = 100;
//	bas->m_level =1;
	bas->m_level =61;
	bas->m_max_health = 200;
	bas->m_armor = 20;
	bas->m_block=0;
	bas->m_attack = 10;
	bas->m_strength = 25;
	bas->m_dexterity = 15;
	bas->m_magic_power = 15;
	//bas->m_willpower = 45;
	bas->m_willpower = 450;
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

	m_base_action = Action::HOLY_ATTACK;
	m_left_action = Action::HOLY_ATTACK;
	m_right_action = Action::HOLY_ATTACK;



	int i;
	for (i=1;i<6;i++)
		bas->m_abilities[i]=0;

	// Debugging
	bas->m_abilities[4] = 0xffffff;


	bas->m_abilities[0] = 0x3f8f;
	bas->m_attack_range =1;

	bas->m_special_flags=0;

	m_name.assign("Elrond");
	calcBaseAttrMod();

	return true;
}


bool Priest::update(float time)
{
	// Priestspezifische update Routine

	// update von User aufrufen
	Player::update(time);
	return true;
}

