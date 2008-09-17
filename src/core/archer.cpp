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
#include "archer.h"

Archer::Archer(World* world, int id) : Player(world, id)
{
	init();
}



Archer::~Archer()
{
}

bool Archer::init ()
{
	getTypeInfo()->m_subtype = "archer";

	CreatureBaseAttr* bas = getBaseAttr();
	CreatureDynAttr* dyn = getDynAttr();

	// Attribute auf Basiswerte setzen
	dyn->m_experience=0;
	dyn->m_health = 150;

	bas->m_max_experience = 100;
	bas->m_level =1;
	bas->m_max_health = 150;
	bas->m_armor = 15;
	bas->m_block=0;
	bas->m_attack = 30;
	bas->m_strength = 25;
	bas->m_dexterity = 45;
	bas->m_magic_power = 15;
	bas->m_willpower = 15;
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
	m_base_action = Action::RANGE_ATTACK;

	// Alle Faehigkeiten deaktivieren
	int i;
	for (i=1;i<6;i++)
		bas->m_abilities[i]=0;

	// Debugging
	bas->m_abilities[2] = 0xffffff;


	// Basisfaehigkeiten (normaler Angriff etc) erlauben
	bas->m_abilities[0] = 0x3f2f;
	bas->m_attack_range =20;

	m_base_action = Action::RANGE_ATTACK;
	m_left_action = Action::RANGE_ATTACK;
	m_right_action = Action::RANGE_ATTACK;

	bas->m_special_flags=0;

	m_name.assign("Legolas");

	// Modifizierte Basisattribute erzeugen
	calcBaseAttrMod();

	return true;
}


bool Archer::update(float time)
{
	// spezifische update Routine

	// update von Player aufrufen
	Player::update(time);
	return true;
}

