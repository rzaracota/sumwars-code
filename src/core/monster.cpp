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
#include "player.h"

Monster::Monster( int id) : Creature( id)
{
	bool tmp=Monster::init();
	if(!tmp)
	{
		DEBUG("Initialisierung des Monsters fehlgeschlagen!");
	}
}


Monster::Monster( int id,MonsterBasicData& data)
	: Creature(   id)
{
	
	memcpy(getBaseAttr(),&data.m_base_attr, sizeof(data.m_base_attr));

	getTypeInfo()->m_type = data.m_type_info.m_type;
	getTypeInfo()->m_subtype = data.m_type_info.m_subtype;
	m_category = data.m_category;
	m_fraction = data.m_fraction;


	memcpy(m_drop_slots,&data.m_drop_slots, 4*sizeof(DropSlot));

	getDynAttr()->m_experience=0;
	getDynAttr()->m_health = getBaseAttr()->m_max_health;
	getBaseAttrMod()->m_max_health = getBaseAttr()->m_max_health;

	// Standardform setzen
	getShape()->m_type = Shape::CIRCLE;
	getShape()->m_radius = data.m_radius;
	m_layer = data.m_layer;
	getShape()->m_angle =0;

	m_ai.m_goals = new WorldObjectValueList;
	m_ai.m_visible_goals = new WorldObjectValueList;
	m_ai.m_state = Ai::ACTIVE;
	m_ai.m_vars = data.m_ai_vars;
	
	calcBaseAttrMod();
	
	m_base_action = Action::NOACTION;
	for (int i = Action::ATTACK; i<= Action::HOLY_ATTACK; ++i)
	{
		if (checkAbility((Action::ActionType) i))
		{
			m_base_action = (Action::ActionType) i;
		}
	}
	DEBUG5("base action %i",m_base_action);
	
}

Monster::~Monster()
{
	// allokierten Speicher freigeben
	delete m_ai.m_goals;
	delete m_ai.m_visible_goals;
}

bool Monster::destroy()
{
	return Creature::destroy();
}

bool Monster::init()
{
	//eigene Initialisierung
	m_ai.m_goals = new WorldObjectValueList;
	m_ai.m_visible_goals = new WorldObjectValueList;

	// Basistyp setzen
	getTypeInfo()->m_type = TypeInfo::TYPE_MONSTER;

	// Standardform setzen
	getShape()->m_type = Shape::CIRCLE;
	getShape()->m_radius = 0.5;
	m_layer = (LAYER_BASE | LAYER_AIR);
	getShape()->m_angle =0;

	getBaseAttr()->m_step_length=0.5;

	m_ai.m_chase_player_id =0;

	m_ai.m_state = Ai::ACTIVE;
	
	for (int i=0; i< NR_AI_MODS; i++)
	{
		m_ai.m_mod_time[i]=0;
	}
	
	m_base_action = Action::NOACTION;
	for (int i = Action::ATTACK; i<= Action::HOLY_ATTACK; ++i)
	{
		if (checkAbility((Action::ActionType) i))
		{
			m_base_action = (Action::ActionType) i;
		}
	}
	
	return true;
}

bool Monster::update(float time)
{
	DEBUG5("Update des Monsters [%i]", getId());

	// AI abhandeln
	for (int i=0; i< NR_AI_MODS; i++)
	{
		if (m_ai.m_mod_time[i] >0)
		{
			m_ai.m_mod_time[i]-= time;
		}
	}

	// Update Funktion der Oberklasse aufrufen
	bool result;
	result = Creature::update(time);
	return result;
}

void Monster::updateCommand()
{
	// bei Cutscenes keine AI verwenden
	if (getRegion()->getCutsceneMode())
		return;
	
	m_ai.m_goals->clear();
	m_ai.m_visible_goals->clear();

	// eigene Koordinaten
	Vector &pos = getShape()->m_center;
	
	DEBUG5("update monster command %i %s",getId(), getTypeInfo()->m_subtype.c_str());
	DEBUG5("randaction prob %f",m_ai.m_vars.m_randaction_prob);
	if (Random::random() < m_ai.m_vars.m_randaction_prob)
	{
		m_ai.m_rand_command = true;
	}
	else
	{
		m_ai.m_rand_command = false;
	}
	m_ai.m_command_count=0;
	
	// moegliche Ziele ermitteln

	// Liste der Spieler
	WorldObjectMap* players = getRegion()->getPlayers();
	WorldObjectList ret;

	Creature* pl;
	
	// Linie vom Monster zum Ziel
	Line gline(pos,Vector(0,0));

	// Entfernungen und Sichtbarkeit der Ziele ermitteln
	// nur, wenn das Monster nicht wegen Taunt einen bestimmten Spieler angreifen muss
	float dist;
	if (m_ai.m_mod_time[TAUNT]<=0 || m_ai.m_chase_player_id==0)
	{
		for (WorldObjectMap::iterator it = players->begin(); it!=players->end(); ++it)
		{
			pl = static_cast<Creature*>(it->second);
			
			// Spieler nur als Ziel, wenn aktiv und nicht in Dialog
			if (pl->getState() != STATE_ACTIVE || pl->getDialogueId() != 0)
				continue;
			
			if (World::getWorld()->getRelation(getFraction(), pl ) != HOSTILE)
				continue;
				
			dist = getShape()->getDistance(*(pl->getShape()));
			if ( dist< m_ai.m_vars.m_sight_range)
			{
				
			
				// Spieler ist in Sichtweite
				m_ai.m_goals->push_back(std::make_pair(pl,dist));
	
				gline.m_end = pl->getShape()->m_center;
	
				// Testen, ob der Weg zum Spieler frei ist
				ret.clear();
				getRegion()->getObjectsOnLine(gline,&ret,LAYER_AIR, CREATURE | FIXED,pl);
				
				// alle verbuendeten Objekte loeschen, weil durch diese *durchgeschossen* werden kann
				WorldObjectList::iterator it;
				for (it = ret.begin(); it != ret.end();)
				{
					if (World::getWorld()->getRelation(m_fraction,*it) == ALLIED )
					{
						it = ret.erase(it);
					}
					else
					{
						++it;
					}
				}
	
				if (ret.empty() && dist< m_ai.m_vars.m_shoot_range)
				{
					// Keine Objekte auf der Linie vom Monster zum Ziel
					m_ai.m_visible_goals->push_back(std::make_pair(pl,dist));
	
				}
				else
				{
					/*
					WorldObjectList::iterator it;
					for (it = ret.begin(); it != ret.end();++it)
					{
						DEBUG("blocking obj %i",(*it)->getId());
					}
					*/
				}
			}
		}
	}
	else
	{
		DEBUG5("taunt %f",m_ai.m_mod_time[TAUNT]);
	}
	
	// Angriff auf Spieler durch anlocken
	if (m_ai.m_goals->empty() && m_ai.m_chase_player_id !=0)
	{
		pl = dynamic_cast<Creature*>(getRegion()->getObject(m_ai.m_chase_player_id));
		
		// Spieler nur als Ziel, wenn aktiv und nicht in Dialog
		if (pl!=0 && pl->getState() == STATE_ACTIVE && pl->getDialogueId() == 0)
		{
			
		
			if (World::getWorld()->getRelation(getFraction(), pl ) == HOSTILE)
			{
				dist = getShape()->getDistance(*(pl->getShape()));
				if (dist < m_ai.m_vars.m_chase_distance)
				{
					m_ai.m_goals->push_back(std::make_pair(pl,dist));
					DEBUG5("chase %i",m_ai.m_chase_player_id);
				}
				else
				{
					m_ai.m_mod_time[TAUNT] =0;
					m_ai.m_chase_player_id =0;
				}
			}
			else
			{
				m_ai.m_mod_time[TAUNT] =0;
				m_ai.m_chase_player_id =0;
			}
		}
		else
		{
			m_ai.m_mod_time[TAUNT] =0;
			m_ai.m_chase_player_id =0;
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
		Command* cmd = getCommand();
		cmd->m_type = m_ai.m_command.m_type;
		cmd->m_goal = m_ai.m_command.m_goal;
		cmd->m_goal_object_id = m_ai.m_command.m_goal_object_id;
		cmd->m_range = m_ai.m_command.m_range;
		
		DEBUG5("calculated command %i",m_ai.m_command.m_type);
		

		m_event_mask |= NetEvent::DATA_COMMAND;

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
	DEBUG5("evaluation command %i",act);
	WorldObjectValueList::iterator it;
	WorldObjectValueList* goal_list=0;
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
	m_ai.m_command_count ++;

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
			
			bool takerand = m_ai.m_rand_command && (Random::randi(m_ai.m_command_count) ==1);

			if (value > m_ai.m_command_value || takerand)
			{
				DEBUG5("set new command %i value %f",act,value);
				
				// aktuelle Aktion ist besser als alle vorher bewerteten
				m_ai.m_command_value = value;
				m_ai.m_command.m_type = act;
				m_ai.m_command.m_goal =cgoal->getShape()->m_center;
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

bool Monster::takeDamage(Damage* damage)
{
	bool atk = Creature::takeDamage(damage);
	
	if (atk)
	{
		if (m_ai.m_mod_time[TAUNT]<=0 || m_ai.m_chase_player_id==0)
		{
			m_ai.m_chase_player_id =damage->m_attacker_id;
		}
		
		// Anwenden der AI Veraenderungen
		for (int i=0;i<NR_AI_MODS;i++)
		{
			if (damage->m_ai_mod_power[i]>0)
			{
				DEBUG5("mod %i modpow %i wp %i",i,damage->m_ai_mod_power[i],getBaseAttrMod()->m_willpower);
				if (damage->m_ai_mod_power[i]>getBaseAttrMod()->m_willpower)
				{
				// Modifikation anwenden
					float t = (damage->m_ai_mod_power[i]-getBaseAttrMod()->m_willpower)*1.0 / getBaseAttrMod()->m_willpower;
					t *= 3000;
					if (t>m_ai.m_mod_time[i])
					{
						m_ai.m_mod_time[i] =t;
						// TODO
						//m_event_mask |= NetEvent::DATA_STATUS_MODS;
					}

					DEBUG5("applying ai mod %i for %f ms",i,t);
				}
			}
		}
		
	}
	return atk;
}


void Monster::die()
{
	DEBUG5("die");
	if (World::getWorld()->isServer())
	{
		//Zeiger auf letzten Angreifer per ID  holen

		int id = getDynAttr()->m_last_attacker_id;
		//schauen ob dieser noch lebt, wenn ja gainExperience bei ihm aufrufen mit der _experience dieser Instanz

		// Object per ID von der World holen
		WorldObject* object;
		object = getRegion()->getObject(id);
		Player* pl = dynamic_cast<Player*>(object);
		Player* pl2;
		short diff;
		
		Shape* sh = getShape();
		if (pl!=0)
		{
			if (pl->getState() == STATE_ACTIVE)
			{
				

				float exp = getBaseAttr()->m_max_experience;
				pl->gainExperience(exp);

				// Verteilen der Exp auf Spieler in der Nähe
				std::set<int>& members = pl->getParty()->getMembers();
				std::set<int>::iterator i;

				for (i=members.begin();i!=members.end();i++)
				{
					pl2 = dynamic_cast<Player*>(getRegion()->getObject(*i));
					
					// An den Spieler der das Monster getoetet hat nicht doppelt verteilen...
					if (pl2 == pl)
						continue;
					
					if (pl2->getShape()->getDistance(*sh) <20)
					{
						// volle xp nur wenn das Monster nicht zu stark war
						diff = getBaseAttr()->m_level - pl2->getBaseAttr()->m_level;
						if (diff >5)
						{
							pl2->gainExperience(exp*pow(0.86,diff));
						}
						else
						{
							pl2->gainExperience(exp);
						}
					}
					
				}
				

			}
		}

		Item* si;
		for (int i=0;i<4;i++)
		{
			si = ItemFactory::createItem(m_drop_slots[i]);
			if (si!=0)
			{
				getRegion()->dropItem(si,getShape()->m_center);
			}
		}
	}
	Creature::die();
}





