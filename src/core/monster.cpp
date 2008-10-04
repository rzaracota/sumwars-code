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
#include "monster.h"

Monster::Monster(World* world, unsigned int id) : Creature( world,  id)
{
	bool tmp=Monster::init();
	if(!tmp)
	{
		DEBUG("Initialisierung des Monsters fehlgeschlagen!");
	}
}


Monster::Monster(World* world, unsigned int id,MonsterBasicData& data)
	: Creature( world,  id)
{
	memcpy(getBaseAttr(),&data.m_base_attr, sizeof(data.m_base_attr));

	getTypeInfo()->m_type = data.m_type_info.m_type;
	getTypeInfo()->m_subtype = data.m_type_info.m_subtype;
	getTypeInfo()->m_category = data.m_type_info.m_category;
	getTypeInfo()->m_fraction = data.m_type_info.m_fraction;
	

	memcpy(m_drop_slots,&data.m_drop_slots, 4*sizeof(DropSlot));

	getDynAttr()->m_experience=0;
	getDynAttr()->m_health = getBaseAttr()->m_max_health;

	// Standardform setzen
	getGeometry()->m_shape.m_type = Shape::CIRCLE;
	getGeometry()->m_shape.m_radius = data.m_radius;
	getGeometry()->m_layer = data.m_layer;
	getGeometry()->m_angle =0;

	m_ai.m_goals = new list<pair<WorldObject*,float> >;
	m_ai.m_visible_goals = new list<pair<WorldObject*,float> >;
	m_ai.m_state = Ai::INACTIVE;
	calcBaseAttrMod();
}

Monster::~Monster()
{
	// allokierten Speicher freigeben
	delete m_ai.m_goals;
	delete m_ai.m_visible_goals;
}

bool Monster::destroy()
{
	Creature::destroy();
}

bool Monster::init()
{
	//eigene Initialisierung
	m_ai.m_goals = new list<pair<WorldObject*,float> >;
	m_ai.m_visible_goals = new list<pair<WorldObject*,float> >;

	// Basistyp setzen
	getTypeInfo()->m_type = TypeInfo::TYPE_MONSTER;

	// Standardform setzen
	getGeometry()->m_shape.m_type = Shape::CIRCLE;
	getGeometry()->m_shape.m_radius = 0.5;
	getGeometry()->m_layer = (Geometry::LAYER_BASE | Geometry::LAYER_AIR);
	getGeometry()->m_angle =0;

	getBaseAttr()->m_step_length=0.5;

	m_ai.m_sight_range=8;


	return true;
}

bool Monster::update(float time)
{
	DEBUG5("Update des Monsters [%i]", getId());

	// AI abhandeln



	// Update Funktion der Oberklasse aufrufen
	bool result;
	result = Creature::update(time);
	return result;
}

void Monster::updateCommand()
{
	m_ai.m_goals->clear();
	m_ai.m_visible_goals->clear();

	// eigene Koordinaten
	float x = getGeometry()->m_shape.m_coordinate_x;
	float y = getGeometry()->m_shape.m_coordinate_y;
	// Nummer der Region
	short rid = getGridLocation()->m_region;

	// Koordinaten eines zieles
	float goal_x,goal_y;

	// moegliche Ziele ermitteln

	// Liste der Spieler
	map<int,WorldObject*>* players = getRegion()->getPlayers();
	list<WorldObject*> ret;

	WorldObject* pl;

	// Entfernungen und Sichtbarkeit der Ziele ermitteln
	float dist;
	for (map<int,WorldObject*>::iterator it = players->begin(); it!=players->end(); ++it)
	{
		pl = it->second;
		dist = World::getDistance(getGeometry()->m_shape, pl->getGeometry()->m_shape);
		if ( dist< m_ai.m_sight_range)
		{
			// Spieler ist in Sichtweite
			m_ai.m_goals->push_back(make_pair(pl,dist));

			goal_x = pl->getGeometry()->m_shape.m_coordinate_x;
			goal_y = pl->getGeometry()->m_shape.m_coordinate_y;

			// Testen, ob der Weg zum Spieler frei ist
			ret.clear();
			getWorld()->getSWObjectsOnLine(x,y,goal_x,goal_y,rid,&ret,Geometry::LAYER_AIR, CREATURE | FIXED,0);

			if (ret.empty())
			{
				// Keine Objekte auf der Linie vom Monster zum Ziel
				m_ai.m_visible_goals->push_back(make_pair(pl,dist));

			}
		}
	}

	// Kommando ermitteln
	m_ai.m_command.m_type = Action::NOACTION;
	m_ai.m_command_value =0;

	calcBestCommand();

	// Wenn Ai aktiv
	if ((m_ai.m_state & Ai::ACTIVE) && m_ai.m_command_value>0)
	{
		// berechnetes Kommando uebernehmen
		DEBUG5("calculated command %i",m_ai.m_command.m_type);
		Command* cmd = getCommand();
		cmd-> m_type = m_ai.m_command.m_type;
		cmd->m_goal_coordinate_x = m_ai.m_command.m_goal_coordinate_x;
		cmd->m_goal_coordinate_y = m_ai.m_command.m_goal_coordinate_y;
		cmd->m_goal_object_id = m_ai.m_command.m_goal_object_id;
		cmd->m_range = m_ai.m_command.m_range;
		
		m_event_mask |= Event::DATA_COMMAND;

	}
	else
	{
		DEBUG5("AI state %i ai value %f",m_ai.m_state,m_ai.m_command_value);
	}

	// Kommando ausrechnen das evtl aus einem Statusmod resultiert
	calcStatusModCommand();

}

void Monster::calcBestCommand()
{
	int i,act;



	// Basisaktionen bewerten
	for (act = Action::ATTACK;act<=Action::HOLY_ATTACK;act++)
	{
		if (!checkAbility((Action::ActionType) act))
		{
			// Faehigkeit steht nicht zur Verfuegung
			continue;
		}


		// Kommando evaluieren
		evalCommand((Action::ActionType) act);


	}

	// Faehigkeiten bewerten
	// Schleife ueber Faehigkeitengruppen
	for (i=1;i<6;i++)
	{
		if (getBaseAttrMod()->m_abilities[i]!=0)
		{
			// Monster besitzt eine Faehigkeit aus der Gruppe

			// Schleife ueber die Faehigkeiten
			for (act = i*32;act <(i+1)*32;act++)
			{
				if (!checkAbility((Action::ActionType) act))
				{
					// Faehigkeit steht nicht zur Verfuegung
					continue;
				}


				// Kommando evaluieren
				evalCommand((Action::ActionType) act);
			}
		}
	}
}

void Monster::evalCommand(Action::ActionType act)
{
	list<pair<WorldObject*,float> >::iterator it;
	list<pair<WorldObject*,float> >* goal_list;
	Creature* cgoal=0;

	float dist;
	float value;

	// true, wenn sich Monster bewegen muss um Fernangriff auszufuehren
	bool ranged_move = false;;

	Action::ActionInfo* aci = Action::getActionInfo(act);

	// Liste der Ziele festlegen
	if (aci->m_distance == Action::MELEE)
	{
		// Nahkampfaktionen, Ziele sind alle Objekte im Sichtradius
		goal_list =m_ai.m_goals;
	}
	else if (aci->m_distance == Action::RANGED)
	{
		// Fernkamfangriff, Ziele sind alle Objekte die direkt Sichtbar sind
		goal_list = m_ai.m_visible_goals;

		if (goal_list->empty() && !m_ai.m_goals->empty())
		{
			ranged_move = true;
			goal_list =m_ai.m_goals;
		}
	}
	else if (aci->m_distance == Action::PASSIVE)
	{
		// Passive Faehigkeiten werden nicht betrachtet
		return;
	}
	else if (aci->m_distance == Action::SELF)
	{
		// Faehigkeit auf selbst, es gibt keine ziele
		goal_list =0;
	}
	else if (aci->m_distance == Action::PARTY || aci->m_distance == Action::PARTY_MULTI)
	{
		// Faehigkeiten auf Verbuendete
		// TODO Liste anlegen
		return;
	}

	if (aci->m_timer_nr==1 && m_timer1>0 || aci->m_timer_nr==2 && m_timer2>0)
	{
		// Aktion erfordert einen Timer, der nicht frei ist
		return;
	}

	// Schaden der Aktion ausrechnen
	Damage dmg;
	calcDamage(act,dmg);


	if (goal_list)
	{
		// alle potentiellen Ziele durchgehen
		for (it = goal_list->begin(); it !=goal_list->end(); ++it)
		{
			// moegliches Ziel
			cgoal = (Creature*) it->first;
			dist = it->second;

			// Bewertung:
			value = (dmg.getSumMinDamage()+dmg.getSumMaxDamage())/dist;

			if (aci->m_distance == Action::MELEE || ranged_move)
			{
				value *= 3/(3+dist);
			}

			if (value > m_ai.m_command_value)
			{
				// aktuelle Aktion ist besser als alle vorher bewerteten
				m_ai.m_command_value = value;
				m_ai.m_command.m_type = act;
				m_ai.m_command.m_goal_coordinate_x =cgoal->getGeometry()->m_shape.m_coordinate_x;
				m_ai.m_command.m_goal_coordinate_y =cgoal->getGeometry()->m_shape.m_coordinate_y;
				m_ai.m_command.m_goal_object_id =cgoal->getId();
				if (aci->m_distance == Action::MELEE)
				{
					m_ai.m_command.m_range = getBaseAttr()->m_attack_range;
				}
				else
				{
					m_ai.m_command.m_range = 20;
				}

				if (ranged_move)
				{
					m_ai.m_command.m_type = Action::WALK;
				}

			}
		}
	}
	else
	{
		// Bewertung pauschal
	}
}


void Monster::die()
{
	DEBUG5("die");	
	if (getWorld()->isServer())
	{
		Geometry* geom = getGeometry();
		//Zeiger auf letzten Angreifer per ID  holen
	
		int id = getDynAttr()->m_last_attacker_id;
		//schauen ob dieser noch lebt, wenn ja gainExperience bei ihm aufrufen mit der _experience dieser Instanz
	
		// Object per ID von der World holen
		WorldObject* object;
		object = getWorld()->getSWObject(id,getGridLocation()->m_region);
	
		if (object!=0)
		{
			if (object->getState() == STATE_ACTIVE && object->getTypeInfo()->m_type==TypeInfo::TYPE_PLAYER)
			{
				Creature* pl = (Creature*) object;
	
				// Für Erfahrungspunkte bitte noch eine vernünftige Formel einfallen lassen...
				//pl->gainExperience((int) ceil(pow(1.5,getLevel()-1)*4));
				// Verteilen der Exp auf Spieler in der Nähe
				// TODO: XP nur auf spieler in der Party verteilen
	
				pl->gainExperience((int) ceil(pow(1.5,min(pl->getBaseAttrMod()->m_level,getBaseAttr()->m_level)-1)*2));
	
				list<WorldObject*> ret;
				Shape s;
				s.m_type = Shape::CIRCLE;
				s.m_radius = 20;
				s.m_coordinate_x = geom->m_shape.m_coordinate_x;
				s.m_coordinate_y = geom->m_shape.m_coordinate_y;
	
				getWorld()->getSWObjectsInShape(&s, getGridLocation()->m_region, &ret, Geometry::LAYER_AIR, CREATURE);
	
				list<WorldObject*>::iterator i;
				
				for (i=ret.begin();i!=ret.end();i++)
				{
					pl = (Creature*) (*i);
					pl->gainExperience((int) ceil(pow(1.5,getBaseAttr()->m_level-1)*2/ret.size()));
				}
	
			}
		}
	
		Item* si;
		for (int i=0;i<4;i++)
		{
			si = ItemFactory::createItem(m_drop_slots[i]);
			if (si!=0)
			{
				getRegion()->dropItem(si,getGeometry()->m_shape.m_coordinate_x,getGeometry()->m_shape.m_coordinate_y);
			}
		}
	}
	Creature::die();
}





