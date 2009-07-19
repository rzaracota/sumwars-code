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
#include "itemfactory.h"

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
	
	*(getBaseAttr()) = data.m_base_attr;

	setType(data.m_type);
	setSubtype(data.m_subtype);
	m_category = data.m_category;
	m_fraction = data.m_fraction;


	memcpy(m_drop_slots,&data.m_drop_slots, 4*sizeof(DropSlot));

	getDynAttr()->m_experience=0;
	getDynAttr()->m_health = getBaseAttr()->m_max_health;
	getBaseAttrMod()->m_max_health = getBaseAttr()->m_max_health;

	// Standardform setzen
	getShape()->m_type = Shape::CIRCLE;
	getShape()->m_radius = data.m_radius;
	setLayer(data.m_layer);
	getShape()->m_angle =0;

	m_ai.m_goals = new WorldObjectValueList;
	m_ai.m_visible_goals = new WorldObjectValueList;
	m_ai.m_allies = new WorldObjectValueList;
	m_ai.m_state = Ai::ACTIVE;
	m_ai.m_vars = data.m_ai_vars;
	m_ai.m_chase_player_id =0;
	
	// HP Faktor bei mehreren Spielern
	WorldObjectMap * pl = World::getWorld()->getPlayers();
	float fak = (0.5 + 0.5*pl->size());
	
	getBaseAttr()->m_max_health *= fak;
	getBaseAttrMod()->m_max_health *= fak;
	getDynAttr()->m_health *= fak;
	calcBaseAttrMod();
	
	m_base_action = "noaction";
	
}

Monster::~Monster()
{
	// allokierten Speicher freigeben
	delete m_ai.m_goals;
	delete m_ai.m_visible_goals;
	delete m_ai.m_allies;
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
	m_ai.m_allies = new WorldObjectValueList;

	// Basistyp setzen
	setType("MONSTER");

	// Standardform setzen
	getShape()->m_type = Shape::CIRCLE;
	getShape()->m_radius = 0.5;
	setLayer(LAYER_BASE | LAYER_AIR);
	getShape()->m_angle =0;

	getBaseAttr()->m_step_length=0.5;

	m_ai.m_chase_player_id =0;

	m_ai.m_state = Ai::ACTIVE;
	
	for (int i=0; i< NR_AI_MODS; i++)
	{
		m_ai.m_mod_time[i]=0;
	}
	
	m_base_action = "noaction";
	
	clearNetEventMask();

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
	// keine AI wenn Kommando per Script vorgegeben
	if (hasScriptCommand())
	{
		Creature::updateCommand();
		return;
	}
	
	// bei Cutscenes keine AI verwenden
	if (getRegion()->getCutsceneMode())
		return;
	
	m_ai.m_goals->clear();
	m_ai.m_visible_goals->clear();
	m_ai.m_allies->clear();

	// eigene Koordinaten
	Vector &pos = getShape()->m_center;
	
	DEBUG5("update monster command %i %s",getId(), getSubtype().c_str());
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

	// Liste der moeglichen Ziele
	WorldObjectList potgoals;
	Shape s;
	s.m_center = pos;
	s.m_type = Shape::CIRCLE;
	s.m_radius = m_ai.m_vars.m_sight_range;
	
	getRegion()->getObjectsInShape(&s,&potgoals,LAYER_BASE | LAYER_AIR,WorldObject::CREATURE,0);

	Creature* pl;
	WorldObjectList ret;
	
	// Linie vom Monster zum Ziel
	Line gline(pos,Vector(0,0));

	// Entfernungen und Sichtbarkeit der Ziele ermitteln
	// nur, wenn das Monster nicht wegen Taunt einen bestimmten Spieler angreifen muss
	float dist;
	if (m_ai.m_mod_time[TAUNT]<=0 || m_ai.m_chase_player_id==0)
	{
		for (WorldObjectList::iterator it = potgoals.begin(); it!=potgoals.end(); ++it)
		{
			if (!(*it)->isCreature())
				continue;
			
			pl = static_cast<Creature*>(*it);
			
			// Spieler nur als Ziel, wenn aktiv und nicht in Dialog
			if (! pl->canBeAttacked())
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

	// Verbuendete ermitteln
	
	s.m_center = pos;
	s.m_radius = 12;
	ret.clear();
	getRegion()->getObjectsInShape(&s, &ret, LAYER_AIR,CREATURE,0);
	WorldObjectList::iterator it;
	
	
	for (it=ret.begin();it!=ret.end();++it)
	{
		if (World::getWorld()->getRelation(m_fraction,(*it)) == WorldObject::ALLIED)
		{
			dist = getShape()->getDistance(*((*it)->getShape()));
			m_ai.m_allies->push_back(std::make_pair(*it,dist) );
		}
	}
	
	
	// Kommando ermitteln
	m_ai.m_command.m_type = "noaction";
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
		
		DEBUG5("calculated command %s",m_ai.m_command.m_type.c_str());
		

		addToNetEventMask(NetEvent::DATA_COMMAND);

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
	Action::ActionType act;

	if (!m_ai.m_goals->empty())
	{
		std::set<std::string>::iterator it;
		for (it = getBaseAttrMod()->m_abilities.begin(); it != getBaseAttrMod()->m_abilities.end(); ++it)
		{
			act = *it;
			if (checkAbility( act))
			{
				// Kommando evaluieren
				evalCommand(act);
			}
		}
	}
}

void Monster::evalCommand(Action::ActionType act)
{
	DEBUG5("evaluation command %s",act.c_str());
	WorldObjectValueList::iterator it;
	WorldObjectValueList* goal_list=0;
	Creature* cgoal=0;
	
	// Liste, die nur das Objekt selber enthaelt
	WorldObjectValueList self;
	self.push_back(std::make_pair(this,0) );

	float dist;
	float value;

	// true, wenn sich Monster bewegen muss um Fernangriff auszufuehren
	bool ranged_move = false;;

	Action::ActionInfo* aci = Action::getActionInfo(act);
	if (aci == 0)
	{
		DEBUG("unknown action %s",act.c_str());
		return;
	}
	
	// Liste der Ziele festlegen
	if (aci->m_target_type == Action::MELEE)
	{
		// Nahkampfaktionen, Ziele sind alle Objekte im Sichtradius
		goal_list =m_ai.m_goals;
	}
	else if (aci->m_target_type == Action::RANGED)
	{
		// Fernkamfangriff, Ziele sind alle Objekte die direkt Sichtbar sind
		goal_list = m_ai.m_visible_goals;

		if (goal_list->empty() && !m_ai.m_goals->empty())
		{
			ranged_move = true;
			goal_list =m_ai.m_goals;
		}
	}
	else if (aci->m_target_type == Action::PASSIVE)
	{
		// Passive Faehigkeiten werden nicht betrachtet
		return;
	}
	else if (aci->m_target_type == Action::SELF)
	{
		// Faehigkeit auf selbst, es gibt keine ziele
		goal_list =&self;
	}
	else if (aci->m_target_type == Action::PARTY || aci->m_target_type == Action::PARTY_MULTI)
	{
		// Faehigkeiten auf Verbuendete
		goal_list = m_ai.m_allies;
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
	
	// weitere Wirkungen der Aktion ausrechnen
	CreatureBaseAttrMod cbam;
	CreatureDynAttrMod cdam;
	calcActionAttrMod(act,cbam,cdam);

	if (goal_list)
	{
		// alle potentiellen Ziele durchgehen
		for (it = goal_list->begin(); it !=goal_list->end(); ++it)
		{
			// moegliches Ziel
			cgoal = (Creature*) it->first;
			dist = it->second;

			// Bewertung:
			
			
			value =0;
			if (aci->m_target_type == Action::SELF || aci->m_target_type == Action::PARTY || aci->m_target_type == Action::PARTY_MULTI)
			{
				float extra_value =0;
				extra_value += 2*(cbam.m_dstrength + cbam.m_dwillpower +  cbam.m_dmagic_power + cbam.m_ddexterity);
				extra_value += cbam.m_dpower + cbam.m_dattack + cbam.m_dblock;
				
				// TODO: bessere Bewertungen
				extra_value += cbam.m_dwalk_speed / 50;
				extra_value += cbam.m_dattack_speed / 20;
				
				if (aci->m_target_type == Action::PARTY_MULTI)
				{
					extra_value *= m_ai.m_allies->size();
				}
				
				DEBUG5("extra value %f",extra_value);
				
				value += extra_value;
			}
			else
			{
				value = (dmg.getSumMinDamage()+dmg.getSumMaxDamage())/(dist+0.1);
			}

			if (aci->m_target_type == Action::MELEE || ranged_move)
			{
				value *= 3/(3+dist);
			}
			
			bool takerand = m_ai.m_rand_command && (Random::randi(m_ai.m_command_count) ==1);
			DEBUG5("value %f",value);

			if (value > m_ai.m_command_value || takerand)
			{
				DEBUG5("set new command %s value %f",act.c_str(),value);
				
				// aktuelle Aktion ist besser als alle vorher bewerteten
				m_ai.m_command_value = value;
				m_ai.m_command.m_type = act;
				m_ai.m_command.m_goal =cgoal->getShape()->m_center;
				m_ai.m_command.m_goal_object_id =cgoal->getId();
				if (aci->m_target_type == Action::MELEE)
				{
					m_ai.m_command.m_range = getBaseAttr()->m_attack_range;
				}
				else
				{
					m_ai.m_command.m_range = 20;
				}
								
				if (ranged_move)
				{
					m_ai.m_command.m_type = "walk";
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
			// umliegende Monster alarmieren
			if (m_ai.m_chase_player_id ==0)
			{
				Shape shape = *(getShape());
				shape.m_radius = m_ai.m_vars.m_warn_radius;
				WorldObjectList res;
				WorldObjectList::iterator it;
				
				getRegion()->getObjectsInShape(&shape, &res, LAYER_BASE | LAYER_AIR, CREATURE_ONLY,this);
				Monster * mon;
				Ai* ai;
				for (it = res.begin(); it!= res.end(); ++it)
				{
					mon = dynamic_cast<Monster*>(*it);
					
					if (mon != 0)
					{
						if (World::getWorld()->getRelation(getFraction(),mon) == ALLIED)
						{
							ai = &(mon->getAi());
							if (ai->m_chase_player_id==0)
							{
								ai->m_chase_player_id = damage->m_attacker_id;
							}
						}
					}
				}
			}
			
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
					
					// Spieler ist nicht in der Region
					if (pl2 ==0)
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





