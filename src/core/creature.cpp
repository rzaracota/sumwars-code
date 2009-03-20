/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Hans Wulf, Chris Drechsler, Daniel Erler

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
#include "creature.h"
#include <sys/time.h>
#include "eventsystem.h"

//Constructors/Destructors
Creature::Creature(int id) : WorldObject(id)
{
	if (!Creature::init())
	{
		DEBUG("Creature::init() hat false zurückgeliefert");
	}
}


Creature::~Creature()
{
	if (m_small_path_info)
	{
		// Wegfindeinformationen fuer kleine Lebewesen loeschen
		delete m_small_path_info;
	}

	if (m_medium_path_info)
	{
		// Wegfindeinformationen fuer mittelgrosse Lebewesen loeschen
		delete m_medium_path_info;
	}

	if (m_big_path_info)
	{
		// Wegfindeinformationen fuer grosse Lebewesen loeschen
		delete m_big_path_info;
	}

	if (m_small_flying_path_info)
	{
		// Wegfindeinformationen fuer kleine fliegende Lebewesen loeschen
		delete m_small_flying_path_info;
	}

	if (m_path_info)
	{
		// eigene Wegfindeinformationen loeschen
		delete m_path_info;
	}
	
	if (m_equipement)
	{
		delete m_equipement;
	}

}

//Methods
bool Creature::init()
 {
	 DEBUG5("Creature::init");

	bool tmp=true;
	// eigene Initialisierung

	// keine Aktion/Kommando
	m_action.m_type = Action::NOACTION;
	m_command.m_type = Action::NOACTION;
	m_command.m_damage_mult=1;
	m_next_command.m_type = Action::NOACTION;

	m_trade_id=0;
	m_speak_id =0;
	m_dialogue_id =0;
	m_equipement =0;

	// Bewegung auf 0 setzen
	m_speed = Vector(0,0);

	// Wegfindeinformationen auf 0 setzen
	m_small_path_info=0;
	m_small_flying_path_info=0;
	m_medium_path_info=0;
	m_big_path_info=0;
	m_path_info=0;

	// Timer nullen
	m_timer1 =0;
	m_timer2 =0;
	m_timer1_max =0;
	m_timer2_max =0;

	m_dyn_attr.m_last_attacker_id=0;
	m_dyn_attr. m_experience=0;

	// Statusmods auf null setzen
	int i;
	for (i=0;i<NR_STATUS_MODS;i++)
	{
		m_dyn_attr.m_status_mod_time[i]=0;
		m_dyn_attr.m_status_mod_immune_time[i]=0;

	}
	m_dyn_attr.m_temp_mods.clear();

	for (i=0;i<NR_EFFECTS;i++)
	{
		m_dyn_attr.m_effect_time[i]=0;
	}

	setState(STATE_ACTIVE);
	m_script_command_timer =0;
	clearCommand(true);
	
	m_action.m_animation_number=0;
	m_action.m_action_equip = Action::NO_WEAPON;
	m_action.m_time =0;
	m_action.m_elapsed_time =0;
	
	m_dyn_attr.m_health = getBaseAttr()->m_max_health;
	getBaseAttrMod()->m_max_health = getBaseAttr()->m_max_health;

	m_event_mask =0;
	
	m_refname = "";
	getTradeInfo().m_trade_partner =0;
	getTradeInfo().m_last_sold_item=0;
	
	return tmp;
}

bool Creature::destroy()
{
	DEBUG5("destroy");

	WorldObject::destroy();
    return true;
}


void Creature::die()
{

	// eigenen Status auf sterbend STATE_DIEING setzen
	getRegion()->changeObjectGroup(this,DEAD);
	setState(STATE_DIEING);
	m_action.m_type =Action::DIE;
	DEBUG5("object died: %p",this);
	m_action.m_time =1000;
	m_action.m_elapsed_time =0;

	m_event_mask |= NetEvent::DATA_ACTION | NetEvent::DATA_STATE;
	
	Trigger* tr = new Trigger("unit_die");
	tr->addVariable("unit",getId());
	getRegion()->insertTrigger(tr);
}

void Creature::initAction()
{
	//wenn Idle Animation schon laeuft, laufen lassen
	if (m_action.m_type== Action::NOACTION && m_action.m_elapsed_time>0)
	{
		return;

	}
	
	
	DEBUG5("init Action %i", m_action.m_type);

	

	m_action.m_elapsed_time = 0;
	Action::ActionInfo* aci = Action::getActionInfo(m_action.m_type);

	//Timer und Timerlaufzeit ermitteln
	int timer =aci->m_timer_nr;
	float time = aci->m_timer;

	DEBUG4("timer nr %i",timer);

	// Faehigkeit Ausdauer
	if (checkAbility(Action::ENDURANCE) && timer ==1)
	{
		DEBUG5("ausdauer");
		// Timerlaufzeit um 15% verringern
		time *= 0.85;
	}


	// Testen ob der benoetigte Timer frei ist
	// Wenn der benoetigte Timer noch laeuft wird die Basisaktion ausgefuehrt
	if (timer==1)
	{
		if (m_timer1==0)
		{
			// Timer ist frei, Timer starten
			m_timer1 = time;
			m_timer1_max = time;
			m_event_mask |= NetEvent::DATA_TIMER;
		}
		else
		{
			// Timer laeuft noch, Basisaktion verwenden
			m_action.m_type = aci->m_base_action;
			aci = Action::getActionInfo(m_action.m_type);

		}
	}
	else if (timer==2)
	{
		if (m_timer2==0)
		{
			// Timer ist frei, Timer starten
			m_timer2 = time;
			m_timer2_max = time;
			m_event_mask |= NetEvent::DATA_TIMER;
		}
		else
		{
			// Timer laeuft noch, Basisaktion verwenden
			m_action.m_type = aci->m_base_action;
			aci = Action::getActionInfo(m_action.m_type);
		}
	}

	// Stumm behandeln
	if (m_dyn_attr.m_status_mod_time[Damage::MUTE]>0 )
	{
				// Wenn die Aktion durch Stumm beeinfluss wird
		if (aci->m_flags & Action::MUTE_AFFECTED)
		{
					// Basisaktion verwenden
			m_action.m_type = aci->m_base_action;
			DEBUG("using Base Action due to mute");
		}
	}

	// setzen der Standarddauer der Aktion
	m_action.m_time = aci->m_standard_time;

	Action::ActionType baseact = Action::getActionInfo(m_action.m_type)->m_base_action;

	// Zeit der Aktion modifizieren
	if (baseact == Action::WALK)
	{
			// Bei Aktion laufen die Laufgeschwindigkeit einrechnen
			m_action.m_time = 1000000 / getBaseAttrMod()->m_walk_speed;
			m_speed *= getBaseAttr()->m_step_length/m_action.m_time;
			m_event_mask |= NetEvent::DATA_MOVE_INFO;
			//collisionDetection(m_action.m_time);

			DEBUG5("walk time %f walk speed %i",m_action.m_time,getBaseAttrMod()->m_walk_speed);
			DEBUG5("pos %f %f speed %f %f",getShape()->m_center.m_x, getShape()->m_center.m_y, m_speed.m_x,m_speed.m_y);
	}
	else
	{
		m_speed = Vector(0,0);
	}



	// Fuer Aktionen die auf physischen Angriffen beruhen sowie fuer den normalen Magieangriff Waffengeschwindigkeit einrechnen
	if (baseact == Action::ATTACK || baseact == Action::RANGE_ATTACK || baseact == Action::HOLY_ATTACK || m_action.m_type == Action::MAGIC_ATTACK)
	{
		float atksp = std::min((short) 5000,getBaseAttrMod()->m_attack_speed);


		m_action.m_time *= 1000000/atksp;

	}
	else if (baseact != Action::WALK)
	{
		m_action.m_time = aci->m_standard_time;

	}

	DEBUG5("resulting time %f",m_action.m_time);
	// Drehwinkel setzen
	if (aci->m_distance != Action::SELF && m_action.m_type != Action::TAKE_ITEM)
	{
		
		getShape()->m_angle = (m_action.m_goal - getShape()->m_center).angle();

	}
	if (baseact == Action::WALK)
	{
		getShape()->m_angle = m_speed.angle();
	}

	// Daten fuer die Animation setzen
	m_action.m_action_equip = getActionEquip();
	m_action.m_animation_number = Random::randi(120);

	// Besondere Initialisierungen

	if ( m_action.m_type== Action::TRADE)
	{
	}

	m_event_mask |= NetEvent::DATA_ACTION;
	
	if (m_action.m_time ==0)
	{
		ERRORMSG("Aktion mit Dauer 0 erzeugt");
	}
	
	// wenn keine Aktion berechnet wurde, Kommando beenden
	if (m_action.m_type == Action::NOACTION && m_command.m_type !=Action::NOACTION)
	{
		clearCommand(false);
	}
}

void Creature::performAction(float &time)
{
	// Wenn Idle Aktion ausgefuehrt wird, aber ein Kommando vorliegt, IdleAktion sofort beenden
	if (m_action.m_type == Action::NOACTION && m_command.m_type != Action::NOACTION)
	{
		return;
	}


	// Reziprokes des Ausfuehrungszeit
	float rezt = 1/m_action.m_time;

	// Prozentsatz der Ausfuehrung vor und nach dem Zeitquantum
	float p1 = rezt * m_action.m_elapsed_time,p2;

	// Zeitdauer die die aktuelle Aktion tatsaechlich ausgefuehrt wird
	float dtime;

	// true, wenn die aktuelle Aktion beendet wird
	bool finish = false;
	

	// Testen ob man die Aktion abschließen kann
	if (time >= m_action.m_time-m_action.m_elapsed_time)
	{
		// Aktion wird abgeschlossen
		p2=1;
		dtime = m_action.m_time-m_action.m_elapsed_time;
		time -= dtime;
		finish = true;
	}
	else
	{
		// Aktion wird nicht abgeschlossen
		m_action.m_elapsed_time += time;
		dtime =time;
		time=0;
		p2 = rezt * m_action.m_elapsed_time;
	}
	
	// Fuer gescriptete Kommandos Zeitschranken pruefen
	if (m_script_command_timer>0)
	{
		m_script_command_timer -= time;
		if (m_script_command_timer<=0)
		{
			m_script_command_timer += time;
			clearCommand(false);
		}
	}

	if (!World::getWorld()->isServer() && m_action.m_type != Action::NOACTION)
	{
		DEBUG5("perform Action %i for %f msec, %f / %f done", m_action.m_type,dtime,m_action.m_elapsed_time,m_action.m_time);
	}


	//Behandlung des Laufens
	if (m_action.m_type == Action::WALK)
	{

		// Kollisionen behandeln
		collisionDetection(dtime);

		// neue Koordinaten ausrechnen
		Vector newpos = getShape()->m_center + m_speed*dtime;

		// Bewegung ausfuehren
		moveTo(newpos);

	}



	// Behandlung der Wirkung der Aktion

	// Prozentsatz bei dessen Erreichen die Wirkung der Aktion berechnet wird
	float pct = Action::getActionInfo(m_action.m_type)->m_critical_perc;
	
	if (m_action.m_type != Action::NOACTION)
	{
		DEBUG5("pos %f %f  speed %f %f  pct %f",getShape()->m_center.m_x, getShape()->m_center.m_y, m_speed.m_x,m_speed.m_y,p1);
	}
	
	// Triple Shot
	if (m_action.m_type == Action::TRIPLE_SHOT || m_action.m_type == Action::GUIDED_TRIPLE_SHOT)
	{
		// Faehigkeit hat drei Prozentsaetze
		if (p1>0.3)
			pct = 0.6;
		if (p1>0.6)
			pct = 0.9;
	}
	
	// Zielobjekt der Aktion
	WorldObject* goalobj =0;
	Vector goal = m_action.m_goal;
	
	// Testen ob der kritische Prozentsatz durch das aktuelle Zeitquantum ueberschritten wurde
	if (p1<pct && pct <=p2 && World::getWorld()->isServer())
	{
		DEBUG5("critical point %f %f %f",p1,pct,p2);

		// Statusmod blind behandeln
		// TODO: aktuell wird die Richtung beim austeilen des Schadens ausgewuerfelt
		// besser waere es, sie beim starten der Aktion auszuwuerfeln
		if (m_dyn_attr.m_status_mod_time[Damage::BLIND]>0)
		{
			// Ziehe Kreis um die eigentlichen Zielkoordinaten der Aktion
			// Durchmesser = halber Abstand zum Zielpunkt
			// Waehle in dem Kreis einen Punkt der der neue Zielpunkt wird
			// konkretes Objekt kann nicht Ziel der Aktion sein
			m_action.m_goal_object_id =0;
			Vector dir;
			dir = goal - getShape()->m_center;
			float d = dir.getLength();
			float r = 0.5*d;
			Vector gadd(r,r);
			while (gadd.getLength() > r)
			{
				gadd.m_x = r-2*r*rand()*1.0/RAND_MAX;
				gadd.m_y = r-2*r*rand()*1.0/RAND_MAX;
			}
			goal += gadd;

		}

		// Zielobjekt ermitteln
		if (m_action.m_goal_object_id!=0 && m_action.m_type!= Action::TAKE_ITEM)
		{
			// Zielobjekt durch ID gegeben, Objekt von der Welt holen
			goalobj = getRegion()->getObject(m_action.m_goal_object_id);
		}
		else
		{
			// Kein Zielobjekt per ID gegeben
			DEBUG5("no Goal ID!");
			// Im Falle von Nahkampf Ziel anhand des Zielpunktes suchen
			if (Action::getActionInfo(m_action.m_type)->m_distance == Action::MELEE && m_action.m_type!= Action::TAKE_ITEM)
			{
				DEBUG5("Searching goal %f %f",goal.m_x,goal.m_y);
				goalobj = getRegion()->getObjectAt(goal,LAYER_AIR);
				DEBUG5("got object %p",goalobj);
			}
		}




		// Zielobjekt als Creature* pointer
		Creature* cgoal=0;

		// Zielobjekt im Nahkampf suchen an der Stelle an der die Waffe trifft
		if (goalobj ==0 && Action::getActionInfo(m_action.m_type)->m_distance == Action::MELEE && m_action.m_type!= Action::TAKE_ITEM)
		{
			goalobj = getRegion()->getObjectAt(goal,LAYER_AIR);
		}

		// Wenn ein Zielobjekt existiert
		if (goalobj !=0)
		{
			if (goalobj->getTypeInfo()->m_type == TypeInfo::TYPE_FIXED_OBJECT)
			{
				cgoal =0;
				// Ziel existiert nicht mehr, evtl abbrechen
			}
			else
			{
				// umwandeln in Creature* Pointer
				cgoal = (Creature*) goalobj;
				// anpassen des Zielpunktes
				goal = cgoal->getShape()->m_center;

				DEBUG5("goal object %p",cgoal);
			}
		}

		// ausfuehren des kritischen Teiles der Aktion
		performActionCritPart(goal, goalobj);

	}

	// Wenn die Aktion beenden wurde evtl Kommando abschließen
	if (finish)
	{
		DEBUG5("finished action");
		if (m_action.m_type != Action::WALK)
		{
			m_action.m_prev_type = m_action.m_type;
		}
		
		if (m_action.m_type == Action::WALK)
		{
			// Trigger erzeugen
			Trigger* tr = new Trigger("unit_moved");
			tr->addVariable("unit",getId());
			getRegion()->insertTrigger(tr);
			
			if (getTypeInfo()->m_type == TypeInfo::TYPE_PLAYER)
			{
				Trigger* tr = new Trigger("player_moved");
				tr->addVariable("player",getId());
				getRegion()->insertTrigger(tr);
			}
	
		}

		// Kommando ist beendet wenn die gleichnamige Aktion beendet wurde
		// Ausnahme: Bewegungskommando ist beendet wenn das Ziel erreicht ist
		Action::ActionType baseact = Action::getActionInfo(m_command.m_type)->m_base_action;
		if (((m_action.m_type == m_command.m_type) || m_action.m_type == baseact) && m_action.m_type != Action::WALK || m_command.m_type == Action::WALK && getShape()->m_center.distanceTo(goal) < getBaseAttr()->m_step_length 
				  && !(m_command.m_type == Action::CHARGE || m_command.m_type == Action::STORM_CHARGE))
		{
			DEBUG5("finished command %i (base %i) with action %i",m_command.m_type,baseact,m_action.m_type );
			if (m_command.m_type != Action::NOACTION)
			{
				m_event_mask |= NetEvent::DATA_COMMAND;
				clearCommand(true);
			}
			m_command.m_type = Action::NOACTION;
			m_action.m_elapsed_time=0;
			m_command.m_damage_mult = 1;


			// Schaden neu berechnen
			recalcDamage();
		}

		// Aktion ist beendet
		if (m_action.m_type != Action::NOACTION)
		{
			m_event_mask |= NetEvent::DATA_ACTION;
		}
		m_action.m_type = Action::NOACTION;
		m_action.m_elapsed_time =0;
		m_event_mask |= NetEvent::DATA_ACTION;
	}




}

void Creature::performActionCritPart(Vector goal, WorldObject* goalobj)
{
	// Zielobjekt als Creature* pointer
	// null, wenn das Objekt kein Lebewesen ist
	Creature* cgoal =0;
	if (goalobj!=0 && goalobj->getTypeInfo()->m_type == TypeInfo::TYPE_FIXED_OBJECT)
	{
		cgoal =0;
	}
	else
	{
		cgoal = (Creature*) goalobj;
	}

    //	Action::ActionType baseact = Action::getActionInfo(m_action.m_type)->m_base_action;

	// Wenn die Aktion auf Nahkampf beruht Schaden an das Zielobjekt austeilen
	// Ausname: Rundumschlag
	if (Action::getActionInfo(m_action.m_type)->m_distance == Action::MELEE)
	{
		if (cgoal && m_action.m_type!=Action::AROUND_BLOW 
				  &&  m_action.m_type!=Action::WHIRL_BLOW
				  && m_action.m_type!=Action::SPEAK)
		{
			cgoal->takeDamage(&m_damage);
		}
	}



	WorldObjectList res;
	res.clear();
	WorldObjectList::iterator it;

	// Koordinaten das ausfuehrenden Objektes
	Vector &pos = getShape()->m_center;

	// Form, wird initialisiert mit der Form des Ausfuehrenden
	Shape s;
	s.m_center = pos;
	s.m_type = Shape::CIRCLE;
	s.m_radius = getShape()->m_radius;

	Creature* cr =0;

	// Struktur fuer Basisattributmods, initialisiert mit Nullen
	CreatureBaseAttrMod cbam;

	Projectile* pr =0;

	// Struktur fuer Modifikationen der dyn. Attribute, initialisiert mit Nullen
	CreatureDynAttrMod cdam;


	// Daten fuer Geschosse: Zielrichtung und Startpunkt
	Vector dir = goal - pos;
	Vector dir2,tdir;
	dir.normalize();
	
	// Startpunkt fuer Geschosse
	// aeusserer Rand des Ausfuehrenden plus 5%
	Vector sproj;
	Fraction fr = m_fraction;
	sproj = pos + dir*1.05*s.m_radius;

	// Standardtyp fuer Pfeile festlegen
	Projectile::ProjectileType arrow = Projectile::ARROW;

	//Faehigkeit Windpfeile
	if (m_base_attr_mod.m_special_flags & WIND_ARROWS)
		arrow = Projectile::WIND_ARROW;

	// Faehigkeit Eispfeile
	if (m_base_attr_mod.m_special_flags & ICE_ARROWS)
		arrow = Projectile::ICE_ARROW;


		// Behandlung der Wirkung der Aktion nach Aktionstyp
	switch(m_action.m_type)
	{
		case Action::SPEAK:
			if (goalobj->isCreature() && getDialogue() ==0)
			{
				cr = static_cast<Creature*>(goalobj);
				Dialogue* dia = new Dialogue(getRegion(), cr->getRefName());
				EventSystem::setDialogue(dia);
				dia->addSpeaker(getId(),"player");
				dia->addSpeaker(goalobj->getId(),cr->getRefName());
				dia->changeTopic("start");
				getRegion()->insertDialogue(dia);
			}
			
			break;
		
		case Action::USE:
			if (cgoal)
				goalobj->reactOnUse(getId());
			break;

		// Kriegerfaehigkeiten
		case Action::HAMMER_BASH:
			
			// alle Lebewesen im Umkreis von 1.5 um den Zielpunkt auswaehlen
			m_damage.m_multiplier[Damage::PHYSICAL]=1;
			s.m_center = goal;
			s.m_radius = 1.5;
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,this);

			// an alle einfachen Schaden austeilen
			for (it=res.begin();it!=res.end();++it)
			{
				if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
				{
					cr = (Creature*) (*it);
					cr->takeDamage(&m_damage);

				}
			}
			break;

		case Action::AROUND_BLOW:
		case Action::WHIRL_BLOW:
			// alle Lebewesen im Umkreis um den Ausfuehrenden auswaehlen
			// Radius gleich Waffenreichweite
			s.m_radius += m_command.m_range;
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,this);

			// an alle Schaden austeilen
			for (it=res.begin();it!=res.end();++it)
			{
				if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
				{
					cr = (Creature*) (*it);
					cr->takeDamage(&m_damage);

				}
			}
			break;
			
		case Action::DECOY:
		case Action::SCARE:
			// alle Lebewesen im Umkreis um den Ausfuehrenden auswaehlen
			// Radius gleich Waffenreichweite
			s.m_radius =12;
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,this);

			// an alle Schaden austeilen
			for (it=res.begin();it!=res.end();++it)
			{
				if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
				{
					cr = (Creature*) (*it);
					cr->takeDamage(&m_damage);

				}
			}
			
			if (m_action.m_type == Action::SCARE)
			{
				cbam.m_time =60000;
				cbam.m_darmor = getBaseAttrMod()->m_armor;
 				applyBaseAttrMod(&cbam);
			}
			break;
			
		case Action::BERSERK:
		case Action::WARCRY:
			// alle Lebewesen im Umkreis um den Ausfuehrenden auswaehlen
			// Radius gleich Waffenreichweite
			cbam.m_time =60000;
			
			s.m_radius =12;
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,this);

			// an alle Schaden austeilen
			for (it=res.begin();it!=res.end();++it)
			{
				if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
				{
					cr = (Creature*) (*it);
					cbam.m_dwalk_speed = -cr->getBaseAttrMod()->m_walk_speed/2;
					cbam.m_dattack_speed = -cr->getBaseAttrMod()->m_attack_speed/2;
					cr->applyBaseAttrMod(&cbam);
					if (m_action.m_type == Action::WARCRY)
					{
						cr->takeDamage(&m_damage);
					}
				}
			}
			break;
		case Action::FLAMESWORD:
			// Modifikationen:
			// Flags fuer 120 sec
			cbam.m_time =120000;
			cbam.m_xspecial_flags |= (FLAMESWORD | FIRESWORD);
			applyBaseAttrMod(&cbam);
			break;

		case Action::FIRESWORD:
			// Modifikationen:
			// Flag fuer 60 sec
			cbam.m_time =60000;
			cbam.m_xspecial_flags |=  FIRESWORD;
			applyBaseAttrMod(&cbam);
			break;

		case Action::FLAMEARMOR:
			// Modifikationen:
			// Flag, 50% Feuerres,25% Feuermaxres fuer 60 sec
			cbam.m_time = 100000;
			cbam.m_xspecial_flags |= FLAMEARMOR;
			cbam.m_dresistances_cap[Damage::FIRE] = 25;
			cbam.m_dresistances[Damage::FIRE] = 50;
			applyBaseAttrMod(&cbam);
			break;

		case Action::REGENERATE:
			// 50% der Lebenspunkte wieder auffuellen
			m_dyn_attr.m_health = std::min(m_dyn_attr.m_health+0.5f*m_base_attr_mod.m_max_health,m_base_attr_mod.m_max_health);
			m_event_mask |= NetEvent::DATA_HP;
			break;

		case Action::ANGER:
			// Modifikationen:
			// doppelte Staerke, halbierte Ruestung, Berserker fuer 30 sec
			cbam.m_time = 40000;
			cbam.m_dstrength =m_base_attr.m_strength;
			cbam.m_darmor = -m_base_attr_mod.m_armor /2;
			applyBaseAttrMod(&cbam);
			m_dyn_attr.m_status_mod_time[Damage::BERSERK] = 30000;
			m_event_mask |= NetEvent::DATA_STATUS_MODS;
			break;

		case Action::FURY:
			
			// Modifikationen:
			// doppelte Staerke, -25% Ruestung, Berserker, erhoehte Angriffsgeschwindigkeit fuer 80 sec
			cbam.m_time = 40000;
			cbam.m_dstrength =m_base_attr.m_strength;
			cbam.m_darmor = -m_base_attr_mod.m_armor /4;
			cbam.m_dattack_speed = 1000;
			applyBaseAttrMod(&cbam);
			m_dyn_attr.m_status_mod_time[Damage::BERSERK] = 30000;
			m_event_mask |= NetEvent::DATA_STATUS_MODS;
			break;

		// Magierfaehigkeiten
		case Action::MAGIC_ATTACK:
			// Projektil magischer Pfeil erzeugen
			pr = new Projectile(Projectile::MAGIC_ARROW,&m_damage, World::getWorld()->getValidProjectileId());
			pr->setSpeed(dir/70);
			
			getRegion()->insertProjectile(pr,sproj);
			break;

		case Action::FIRE_BOLT:
		case Action::FIRE_STRIKE:
			// Projektil Feuerblitz erzeugen
			pr = new Projectile(Projectile::FIRE_BOLT,&m_damage,World::getWorld()->getValidProjectileId());
			pr->setSpeed(dir/70);
			
			getRegion()->insertProjectile(pr,sproj);
			break;

		case Action::FIRE_BALL:
		case Action::INFERNO_BALL:
			// Projektil Feuerball erzeugen
			pr = new Projectile(Projectile::FIRE_BALL,&m_damage, World::getWorld()->getValidProjectileId());
			pr->setSpeed(dir/70);
			getRegion()->insertProjectile(pr,sproj);
			break;

		case Action::FIRE_WAVE:
		case Action::FIRE_STORM:
			// Projektil Feuerwelle erzeugen
			pr = new Projectile(Projectile::FIRE_WAVE,&m_damage, World::getWorld()->getValidProjectileId());
			getRegion()->insertProjectile(pr,pos);
			break;

		case Action::FIRE_WALL:
			// Projektil Feuersaeule erzeugen
			pr = new Projectile(Projectile::FIRE_WALL,&m_damage, World::getWorld()->getValidProjectileId());
			getRegion()->insertProjectile(pr,goal);
			break;

		case Action::ICE_BOLT:
		case Action::ICE_SPIKE:
			// Projektil Eisblitz erzeugen
			pr = new Projectile(Projectile::ICE_BOLT,&m_damage, World::getWorld()->getValidProjectileId());
			pr->setSpeed(dir/70);
			getRegion()->insertProjectile(pr,sproj);
			break;

		case Action::SNOW_STORM:
		case Action::BLIZZARD:
			// Projektil Blizzard erzeugen
			pr = new Projectile(Projectile::BLIZZARD,&m_damage, World::getWorld()->getValidProjectileId());
			getRegion()->insertProjectile(pr,goal);
			break;

		case Action::ICE_RING:
		case Action::FROST_RING:
			// Projektil Eisring erzeugen
			pr = new Projectile(Projectile::ICE_RING,&m_damage, World::getWorld()->getValidProjectileId());
			getRegion()->insertProjectile(pr,pos);
			break;

		case Action::FREEZE:
			// Projektil Einfrieren erzeugen
			pr = new Projectile(Projectile::FREEZE,&m_damage, World::getWorld()->getValidProjectileId());
			getRegion()->insertProjectile(pr,goal);
			break;

		case Action::LIGHTNING:
		case Action::LIGHTNING_STRIKE:
			// Projektil Blitz erzeugen
			pr = new Projectile(Projectile::LIGHTNING,&m_damage, World::getWorld()->getValidProjectileId());
			getRegion()->insertProjectile(pr,goal);
			break;

		case Action::THUNDERSTORM:
		case Action::THUNDERSTORM2:
			// Projektil Gewitter erzeugen
			pr = new Projectile(Projectile::THUNDERSTORM,&m_damage, World::getWorld()->getValidProjectileId());
			getRegion()->insertProjectile(pr,goal);
			break;

		case Action::CHAIN_LIGHTNING:
		case Action::CHAIN_LIGHTNING2:
			// Projektil Kettenblitz erzeugen
			pr = new Projectile(Projectile::CHAIN_LIGHTNING,&m_damage, World::getWorld()->getValidProjectileId());
			if (m_action.m_type == Action::CHAIN_LIGHTNING2)
			{
				// Verbesserte Version springt noch viermal extra, daher einfach bei -4 anfangen zu zaehlen
				pr->setCounter(-4);
			}
			pr->setSpeed(dir/50);
			getRegion()->insertProjectile(pr,sproj);
			break;

		case Action::STATIC_SHIELD:
			// Modifikation:
			// Flag fuer 18 Sekunden
			cbam.m_time =18000;
			cbam.m_xspecial_flags |= (STATIC_SHIELD);
			applyBaseAttrMod(&cbam);
			break;

		// Schuetze Faehigkeiten
		case Action::RANGE_ATTACK:
		case Action::TRIPLE_SHOT:
		case Action::WEAK_POINT:
		case Action::BLIND_RAGE:
			// Projektil Pfeil erzeugen
			pr = new Projectile(arrow,&m_damage, World::getWorld()->getValidProjectileId());
			pr->setSpeed(dir/80);
			tdir = dir*(1/80);
			DEBUG5("set speed %f %f",tdir.m_x, tdir.m_y);
			
			getRegion()->insertProjectile(pr,sproj);
			break;

		case Action::GUIDED_TRIPLE_SHOT:
			// Projektil gelenkter Pfeil erzeugen
			pr = new Projectile(Projectile::GUIDED_ARROW,&m_damage, World::getWorld()->getValidProjectileId());
			pr->setSpeed(dir/100);
			
			if (cgoal!=0)
			{
				DEBUG5("has goal");
				pr->setGoalObject(cgoal->getId());
			}
			getRegion()->insertProjectile(pr,sproj);
			break;

		case Action::MULTISHOT:
			// 5 Pfeile erzeugen
			// mittlerer Pfeil erhaelt die Zielrichtung
			for (int i=-2;i<=2;i++)
			{
				dir2 = dir;
				dir2.m_x += i*0.2*dir.m_y;
				dir2.m_y += i*0.2*dir.m_x;
				dir2.normalize();

				sproj = pos+dir2* 1.05 * s.m_radius;
				pr = new Projectile(arrow,&m_damage, World::getWorld()->getValidProjectileId());
				pr->setSpeed(dir2/80);
				
				getRegion()->insertProjectile(pr,sproj);
			}
			break;

		case Action::VOLLEY_SHOT:
			// 7 Pfeile erzeugen
			// mittlerer Pfeil erhaelt die Zielrichtung
			for (int i=-3;i<=3;i++)
			{
				dir2 = dir;
				dir2.m_x += i*0.2*dir.m_y;
				dir2.m_y += i*0.2*dir.m_x;
				dir2.normalize();

				sproj = pos+dir2* 1.05 * s.m_radius;
				pr = new Projectile(arrow,&m_damage, World::getWorld()->getValidProjectileId());
				pr->setSpeed(dir2/80);
				getRegion()->insertProjectile(pr,sproj);
			}
			break;

		case Action::PIERCE:
			// Projektil Pfeil erzeugen
			pr = new Projectile(arrow,&m_damage, World::getWorld()->getValidProjectileId());
			pr->setSpeed(dir/80);
			
			// Flag durchschlagend setzen
			pr->setFlags(Projectile::PIERCING);
			getRegion()->insertProjectile(pr,sproj);
			break;

		case Action::VACUUM:
			// Projektil Windpfeil erzeugen
			pr = new Projectile(Projectile::WIND_ARROW,&m_damage, World::getWorld()->getValidProjectileId());
			pr->setSpeed(dir/80);
			
			getRegion()->insertProjectile(pr,sproj);
			break;

		case Action::EXPLODING_ARROW:
		case Action::EXPLOSION_CASCADE:
			// Projektil Pfeil erzeugen
			pr = new Projectile(arrow,&m_damage, World::getWorld()->getValidProjectileId());
			pr->setSpeed(dir/80);
			
			// Flag explodierend setzen
			pr->setFlags(Projectile::EXPLODES);
			// bei verbesserter Version Flag mehrfach explodierend setzen
			if (m_action.m_type==Action::EXPLOSION_CASCADE)
				pr->setFlags(Projectile::MULTI_EXPLODES);
			getRegion()->insertProjectile(pr,sproj);
			break;

		case Action::DEATH_ROULETTE:
			// Projektil Pfeil erzeugen
			pr = new Projectile(arrow,&m_damage, World::getWorld()->getValidProjectileId());
			pr->setSpeed(dir/80);
			
			// Maximale Anzahl der Spruenge
			pr->setCounter(-20);
			// Flag zufaellig weiterspringen setzen
			pr->setFlags(Projectile::PROB_BOUNCING);
			getRegion()->insertProjectile(pr,sproj);
			break;

		case Action::AIMED_SHOT:
			// Modifikationen:
			// 50% mehr Geschick fuer 50 sec
			cbam.m_time = 50000;
			cbam.m_ddexterity =m_base_attr.m_dexterity/2;
			applyBaseAttrMod(&cbam);
			break;

		case Action::BOW_SPIRIT:
			// Modifikationen
			// 50% mehr Geschick, Flag fuer 50 sec
			cbam.m_time = 50000;
			cbam.m_ddexterity =m_base_attr.m_dexterity/2;
			cbam.m_xspecial_flags |= CRIT_HITS;
			applyBaseAttrMod(&cbam);
			break;

		case Action::ICE_ARROWS:
			// Modifikationen:
			// Flag fuer 80 sec
			cbam.m_time = 80000;
			cbam.m_xspecial_flags |= ICE_ARROWS;
			applyBaseAttrMod(&cbam);
			break;

		case Action::FREEZING_ARROWS:
			// Modifikationen:
			// Flags fuer 80 sec
			cbam.m_time = 80000;
			cbam.m_xspecial_flags |= (ICE_ARROWS | FROST_ARROWS);
			applyBaseAttrMod(&cbam);
			break;

		case Action::WIND_ARROWS:
			// Modifikationen:
			// Flag fuer 80 sec
			cbam.m_time = 80000;
			cbam.m_xspecial_flags |= WIND_ARROWS;
			applyBaseAttrMod(&cbam);
			break;

		case Action::STORM_ARROWS:
			// Modifikationen:
			// Flags fuer 80 sec
			cbam.m_time = 80000;
			cbam.m_xspecial_flags |= (WIND_ARROWS | STORM_ARROWS);
			applyBaseAttrMod(&cbam);
			break;

		case Action::WIND_WALK:
			// Modifikationen:
			// Flag, erhoehte Laufgeschwindigkeit fuer 25 sec
			cbam.m_time = 25000;
			cbam.m_xspecial_flags |= WIND_WALK;
			cbam.m_dwalk_speed = 1500;
			applyBaseAttrMod(&cbam);
			break;

		// Priester Faehigkeiten
		case Action::HOLY_LIGHT:
		case Action::HOLY_FIRE:
			// Projektil Lichtstrahl erzeugen
			pr = new Projectile(Projectile::LIGHT_BEAM,&m_damage, World::getWorld()->getValidProjectileId());
			getRegion()->insertProjectile(pr,goal);
			break;

		case Action::BURNING_RAGE:
			// Modifikationen:
			// Flag fuer 80 sec
			cbam.m_time =80000;
			cbam.m_xspecial_flags |= BURNING_RAGE;

			// alle Verbuendeten im Umkreis von 12 suchen und Modifikation anwenden
			s.m_center = pos;
			s.m_radius = 12;
			getRegion()->getObjectsInShape(&s, &res, LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (World::getWorld()->getRelation(fr,(*it)) == WorldObject::ALLIED)
				{
					if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
					{
						cgoal = (Creature*) (*it);
						cgoal->applyBaseAttrMod(&cbam);
					}
				}
			}
			break;

		case Action::CURE_BLIND_MUTE:
			// Modifikation:
			// heilt blind/stumm (wirkt nur auf Verbuendete)
			if (cgoal !=0)
			{
				cdam.m_dstatus_mod_immune_time[Damage::BLIND] = 1;
				cdam.m_dstatus_mod_immune_time[Damage::MUTE] = 1;
				if (World::getWorld()->getRelation(fr,cgoal) == WorldObject::ALLIED)
				{
					cgoal->applyDynAttrMod(&cdam);
				}
			}
			break;

		case Action::BLAZING_SHIELD:
			// Modifikationen
			// 50% der Willenskraft auf Blockwert fuer 60 sec
			cbam.m_time =60000;
			cbam.m_dblock = m_base_attr_mod.m_willpower /2;

			// alle Verbuendeten im Umkreis von 12 suchen und Modifikation anwenden
			s.m_center = pos;
			s.m_radius = 12;
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (World::getWorld()->getRelation(fr,(*it)) == WorldObject::ALLIED)
				{
					if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
					{
						cgoal = (Creature*) (*it);
						cgoal->applyBaseAttrMod(&cbam);
					}
				}
			}
			break;

		case Action::CURE_BLIND_MUTE_PARTY:
			// Modifikation:
			// heilt blind/stumm, immunisiert fuer 30 sec (wirkt nur auf Verbuendete)
			cdam.m_dstatus_mod_immune_time[Damage::BLIND] = 30000;
			cdam.m_dstatus_mod_immune_time[Damage::MUTE] = 30000;

			// alle Verbuendeten im Umkreis von 6 um den Zielpunkt suchen und Modifikation anwenden
			s.m_center = goal;
			s.m_radius = 6;
			getRegion()->getObjectsInShape(&s, &res,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (World::getWorld()->getRelation(fr,(*it)) == WorldObject::ALLIED)
				{
					if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
					{
						cgoal = (Creature*) (*it);
						cgoal->applyDynAttrMod(&cdam);
					}
				}
			}
			break;

		case Action::LIGHT_BEAM:
			// Projektil Lichtstrahl erzeugen
			pr = new Projectile(Projectile::LIGHT_BEAM,&m_damage, World::getWorld()->getValidProjectileId());
			getRegion()->insertProjectile(pr,goal);
			break;

		case Action::BURNING_SUN:
			// alle Lebewesen im Umkreis von 3 um den Zielpunkt suchen
			s.m_center = goal;
			s.m_radius = 3;
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,this);
			for (it=res.begin();it!=res.end();++it)
			{
				if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
				{
					// Projektil Lichtstrahl fuer jedes Objekt erzeugen
					goal = (*it)->getShape()->m_center;
					pr = new Projectile(Projectile::LIGHT_BEAM,&m_damage, World::getWorld()->getValidProjectileId());
					getRegion()->insertProjectile(pr,goal);
				}
			}
			break;

		case Action::BREAK_BINDING:
		case Action::DISRUPT_BINDING:
			// Projektil Elementarzerstoerung erzeugen
			pr = new Projectile(Projectile::ELEM_EXPLOSION,&m_damage, World::getWorld()->getValidProjectileId());
			getRegion()->insertProjectile(pr,goal);
			break;

		case Action::MAGIC_SHIELD:
			// Modifikation:
			// 50% mehr Willenskraft fuer 90 sec
			cbam.m_time =90000;
			cbam.m_dwillpower = m_base_attr_mod.m_willpower /2;

			// alle Verbuendeten im Umkreis von 12 suchen und Modifikation anwenden
			s.m_center = pos;
			s.m_radius = 12;
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (World::getWorld()->getRelation(fr,(*it)) == WorldObject::ALLIED)
				{
					if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
					{
						cgoal = (Creature*) (*it);
						cgoal->applyBaseAttrMod(&cbam);
					}
				}
			}
			break;

		case Action::CURE_POIS_BURN:
			// Modifikation:
			// heilt vergiftet/brennend (wirkt nur auf Verbuendete)
			if (cgoal !=0)
			{
				cdam.m_dstatus_mod_immune_time[Damage::POISONED] = 1;
				cdam.m_dstatus_mod_immune_time[Damage::BURNING] = 1;
				if (World::getWorld()->getRelation(fr,cgoal) == WorldObject::ALLIED)
				{
					cgoal->applyDynAttrMod(&cdam);
				}
			}
			break;

		case Action::CURE_POIS_BURN_PARTY:
			// Modifikation:
			// heilt vergiftet/brennend und immunisiert fuer 30 sec (wirkt nur auf Verbuendete)
			cdam.m_dstatus_mod_immune_time[Damage::POISONED] = 30000;
			cdam.m_dstatus_mod_immune_time[Damage::BURNING] = 30000;

			// alle Verbuendeten im Umkreis von 6 um den Zielpunkt suchen und Modifikation anwenden
			s.m_center = goal;
			s.m_radius = 6;
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (World::getWorld()->getRelation(fr,(*it)) == WorldObject::ALLIED)
				{
					if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
					{
						cgoal = (Creature*) (*it);
						cgoal->applyDynAttrMod(&cdam);
					}
				}
			}
			break;

		case Action::ACID:
			// Projektil Saeure erzeugen
			pr = new Projectile(Projectile::ACID,&m_damage, World::getWorld()->getValidProjectileId());
			getRegion()->insertProjectile(pr,goal);
			break;

		case Action::HEAL:
			// Modifikation:
			// 300% der Willenskraft auf HP (nur Verbuendete)
			if (cgoal !=0)
			{
				cdam.m_dhealth = 3* m_base_attr_mod.m_willpower;
				if (World::getWorld()->getRelation(fr,cgoal) == WorldObject::ALLIED)
				{
					cgoal->applyDynAttrMod(&cdam);
				}
			}
			break;

		case Action::HEAL_PARTY:
			// Modifikation:
			// 200% der Willenskraft auf HP
			cdam.m_dhealth = 2* m_base_attr_mod.m_willpower;

			// alle Verbuendeten im Umkreis von 6 um den Zielpunkt suchen und Modifikation anwenden
			s.m_center = goal;
			s.m_radius = 6;
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (World::getWorld()->getRelation(fr,(*it)) == WorldObject::ALLIED)
				{
					if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
					{
						cgoal = (Creature*) (*it);
						cgoal->applyDynAttrMod(&cdam);
					}
				}
			}
			break;

		case Action::DIVINE_WIND:
			// Projektil heiliger Strahl erzeugen
			pr = new Projectile(Projectile::DIVINE_BEAM,&m_damage, World::getWorld()->getValidProjectileId());
			getRegion()->insertProjectile(pr,goal);
			break;


		case Action::DIVINE_STORM:
			// Alle Objekte im Umkreis von 3 um den Zielpunkt suchen
			s.m_center = goal;
			s.m_radius = 3;
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,this);
			for (it=res.begin();it!=res.end();++it)
			{
				// fuer alle Lebewesen ein Projektil heiliger Strahl erzeugen
				if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
				{
					goal = (*it)->getShape()->m_center;
					pr = new Projectile(Projectile::DIVINE_BEAM,&m_damage, World::getWorld()->getValidProjectileId());
					getRegion()->insertProjectile(pr,goal);
				}
			}
			break;

		case Action::BLADE_STORM:
			// Modifikation:
			// Angriffsgeschwindigkeit erhoeht fuer 70 sec
			cbam.m_time =70000;
			cbam.m_dattack_speed = 1500;

			// alle Verbuendeten im Umkreis von 12 suchen und Modifikation anwenden
			s.m_center = pos;
			s.m_radius = 12;
			getRegion()->getObjectsInShape(&s,&res,LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (World::getWorld()->getRelation(fr,(*it)) == WorldObject::ALLIED)
				{
					if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
					{
						cgoal = (Creature*) (*it);
						cgoal->applyBaseAttrMod(&cbam);
					}
				}
			}
			break;

		case Action::CURE_CONF_BSRK:
			// Modifikation:
			// heilt verwirrt/Berserker (wirkt nur auf Verbuendete)
			if (cgoal !=0)
			{
				cdam.m_dstatus_mod_immune_time[Damage::CONFUSED] = 1;
				cdam.m_dstatus_mod_immune_time[Damage::BERSERK] = 1;
				if (World::getWorld()->getRelation(fr,cgoal) == WorldObject::ALLIED)
				{
					cgoal->applyDynAttrMod(&cdam);
				}
			}
			break;

		case Action::CURE_CONF_BSRK_PARTY:
			// Modifikation:
			// heilt verwirrt/Berserker und immunisiert fuer 30 sec (wirkt nur auf Verbuendete)
			cdam.m_dstatus_mod_immune_time[Damage::CONFUSED] = 30000;
			cdam.m_dstatus_mod_immune_time[Damage::BERSERK] = 30000;

			// alle Verbuendeten im Umkreis von 6 um den Zielpunkt suchen und Modifikation anwenden
			s.m_center = goal;
			s.m_radius = 6;
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (World::getWorld()->getRelation(fr,(*it)) == WorldObject::ALLIED)
				{
					if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
					{
						cgoal = (Creature*) (*it);
						cgoal->applyDynAttrMod(&cdam);
					}
				}
			}
			break;

		case Action::HYPNOSIS:
			// Projektil Hypnose erzeugen
			pr = new Projectile(Projectile::HYPNOSIS,&m_damage, World::getWorld()->getValidProjectileId());
			getRegion()->insertProjectile(pr,goal);
			break;

		case Action::HYPNOSIS2:
			// Alle Objekte im Umkreis von 2 um den Zielpunkt suchen
			s.m_center = goal;
			s.m_radius = 2;
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,this);
			for (it=res.begin();it!=res.end();++it)
			{
				// Fuer alle Lebewesen ein Projektil Hypnose erzeugen
				if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
				{
					goal = (*it)->getShape()->m_center;
					pr = new Projectile(Projectile::HYPNOSIS,&m_damage, World::getWorld()->getValidProjectileId());
					getRegion()->insertProjectile(pr,goal);
				}
			}
			break;

		case Action::KEEN_MIND:
			// Modifikation:
			// 50% der Willenskraft auf Magie
			cbam.m_time =70000;
			cbam.m_dmagic_power = m_base_attr_mod.m_willpower/2;

			// alle Verbuendeten im Umkreis von 12 suchen und Modifikation anwenden
			s.m_center = pos;
			s.m_radius = 12;
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (World::getWorld()->getRelation(fr,(*it)) == WorldObject::ALLIED)
				{
					if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
					{
						cgoal = (Creature*) (*it);
						cgoal->applyBaseAttrMod(&cbam);
					}
				}
			}
			break;

        default:
            break;
	}

	// Faehigkeit Monsterjaeger
	// Wenn das Ziel nach der Aktion unter 0 Lebenspunkte hat Bonus austeilen
	if ((checkAbility(Action::MONSTER_HUNTER) || (checkAbility(Action::MONSTER_SLAYER))) && cgoal && cgoal->getDynAttr()->m_health<0)
	{
		DEBUG5("monster killed, apply mod");
		CreatureBaseAttrMod cbam;

		// Modifikation:
		// 10% mehr Staerke, bei aufgewerteter Version erhoehte Angriffsgeschwindigkeit fuer 10 sec
		cbam.m_dstrength = getBaseAttr()->m_strength / 10;
		cbam.m_time = 10000;
		if (checkAbility(Action::MONSTER_SLAYER))
		{
			cbam.m_dattack_speed = 200;
		}
		applyBaseAttrMod(&cbam);

	}
}

void Creature::collisionDetection(float time)
{
	WorldObjectList result;
	// Punkt zu dem das Objekt bewegt werden soll
	Vector newpos = getShape()->m_center + m_speed*time;

	// Kreis um den Zielpunkt
	Shape scopy;
	scopy.m_radius = getShape()->m_radius*1.05;
	scopy.m_center = newpos;
	scopy.m_type = Shape::CIRCLE;

	// Ebene in der gesucht werden soll
	short layer = m_layer;

	// Alle kollidierenden Objekte suchen
	getRegion()->getObjectsInShape(&(scopy),&result,layer, CREATURE | FIXED,this);

	if (result.size()!=0)
	{
		// es gibt kollidierende Objekte
		DEBUG5("aktuelle Koordinaten %f %f",getShape()->m_center.m_x, getShape()->m_center.m_y);
		WorldObjectList::iterator i;

		Shape* s2;
        // Liste der kollidierenden Objekte durchgehen
		for (i= result.begin();i!=result.end();++i)
		{
			DEBUG5("Kollision %i",(*i)->getId());
			s2 =(*i)->getShape();
			// wenn mit dem Zielobjekt kollidiert Bewegung beenden
			if ((*i)->getId() == getCommand()->m_goal_object_id)
			{
				m_speed = Vector(0,0);
				m_event_mask |= NetEvent::DATA_MOVE_INFO;
				break;
			}

			// Wenn die Faehigkeit Stuermen ist dann Bewegung beenden
			if (m_command.m_type == Action::CHARGE || m_command.m_type == Action::STORM_CHARGE)
			{
				DEBUG5("charge goal object %i",(*i)->getId());
				// Behandlung von *Charge*
				m_command.m_goal_object_id = (*i)->getId();
				m_speed = Vector(0,0);
				m_event_mask |= NetEvent::DATA_MOVE_INFO;
				return;
			}
			else
			{
				// Kollision behandeln
				handleCollision(s2);
			}

		}

		// neuen erreichten Punkt ausrechnen und testen ob dieser akzeptabel ist
		newpos = getShape()->m_center + m_speed*time;
		

		// Kreis um den neuen Zielpunkt
		scopy.m_center = newpos;
		DEBUG5("neue Koordinaten %f %f",newpos.m_x, newpos.m_y);
		scopy.m_radius = getShape()->m_radius;
		result.clear();

		// Suchen der Objekte um den neuen Zielpunkt
		getRegion()->getObjectsInShape(&(scopy),&result,layer, CREATURE | FIXED,this);

		// Wenn immer noch Kollision vorliegt Bewegung beenden
		if (result.size()!=0)
		{
			DEBUG5("still colliding");

			m_speed = Vector(0,0);
			m_event_mask |= NetEvent::DATA_MOVE_INFO;
		}

	}
}

void Creature::handleCollision(Shape* s2)
{
	// eigene Koordinaten
	Vector pos = getShape()->m_center;
	
	// Koordinaten des kollidierenden Objektes
	Vector cpos = s2->m_center;
	
	bool circ = true;

	DEBUG5("old speed %f %f", m_speed.m_x, m_speed.m_y);
	if (s2->m_type==Shape::RECT)
	{
		// Kollision mit Rechteckt
		// Drehen des Bezugssystems, sodass das Rechteck ausgerichtet ist
		
		Vector locpos = pos - cpos;
		locpos.rotate(-s2->m_angle);
		
		Vector locspeed = m_speed;
		locspeed.rotate(-s2->m_angle);
		
		circ = false;
		DEBUG5("Rechteck");

		// Ausdehnung des Rechtseckes
		Vector ext = s2->m_extent;
		float r = getShape()->m_radius;

		if (fabs(locpos.m_x) > ext.m_x+r)
		{
			// Kollision mit einer senkrechten Kante
			// x-Anteil der Bewegung auf 0 setzen
			locspeed.m_x =0;
			DEBUG5("x-anteil genullt");
		}
		else
		{
			if (fabs(locpos.m_y) >ext.m_y+r)
			{
				// Kollision mit einer waagerechten Kante
				// y-Anteil der Bewegung auf 0 setzen
				DEBUG5("y-anteil genullt");
				locspeed.m_y =0;
			}
			else
			{
				// Kollision an der Ecke des Rechteckes
				DEBUG5("kollision an einer Ecke");

				// ermitteln der Ecke an der die Kollision stattfand
				if (locpos.m_x>0)
				{
					cpos.m_x += ext.m_x*cos(s2->m_angle);
					cpos.m_y += ext.m_x*sin(s2->m_angle);

				}
				else
				{
					cpos.m_x -= ext.m_x*cos(s2->m_angle);
					cpos.m_y -= ext.m_x*sin(s2->m_angle);
				}
				if (locpos.m_y>0)
				{
					cpos.m_x -= ext.m_y*sin(s2->m_angle);
					cpos.m_y += ext.m_y*cos(s2->m_angle);
				}
				else
				{
					cpos.m_x += ext.m_y*sin(s2->m_angle);
					cpos.m_y -= ext.m_y*cos(s2->m_angle);
				}

				// Problem behandeln wie einen Kreis um diese Ecke
				circ = true;
			}
		}
		
		
		if (!circ)
		{
			DEBUG5("locspeed %f %f",locspeed.m_x, locspeed.m_y);
			locspeed.rotate(s2->m_angle);
			m_speed = locspeed;
			
		}
	
	}

	if (circ)
	{
		// Behandlung Kollision mit Kreis
		DEBUG5("obj pos %f %f", pos.m_x, pos.m_y);
		DEBUG5("collision pos %f %f", cpos.m_x, cpos.m_y);
		
		
		// Vektor vom eigenen Mittelpunkt zum Mittelpunkt des kollidierenden Objektes (normiert)
		Vector dir = cpos - pos;
		dir.normalize();
		DEBUG5("normal dir %f %f", dir.m_x, dir.m_y);
		
		
		// vom Geschwindigkeitsvektor nur den zu dieser Richtung senkrechten Teil uebrig lassen
		m_speed.normalPartTo(dir);

	}

	// neue Geschwindigkeit normieren
	m_speed.normalize();
	DEBUG5("new speed %f %f", m_speed.m_x, m_speed.m_y);
	m_speed *= getBaseAttr()->m_step_length/m_action.m_time;
	
	
	
	m_event_mask |= NetEvent::DATA_MOVE_INFO;

}

bool Creature::hasScriptCommand()
{
	return (m_script_command_timer>0 || !m_script_commands.empty());
}

void Creature::insertScriptCommand(Command &cmd, float time)
{
	if (!hasScriptCommand())
	{
		clearCommand(false);
	}
	m_script_commands.push_back(std::make_pair(cmd,time));	
	DEBUG("insert script command %i",cmd.m_type);
}


void Creature::updateCommand()
{
	// Wenn aktuelles Kommando keine Aktion vorschreibt
	DEBUG5("next command: %i ",m_next_command.m_type);
	if ((!hasScriptCommand() && m_next_command.m_type != Action::NOACTION && !getRegion()->getCutsceneMode()) || 
			 (!m_script_commands.empty() && m_command.m_type == Action::NOACTION))
	{
		// Naechstes Kommando uebernehmen
		if (!m_script_commands.empty())
		{
			
			m_command = m_script_commands.front().first;
			m_script_command_timer = m_script_commands.front().second;
			m_script_commands.pop_front();
			DEBUG5("script command %i time %f",m_command.m_type, m_script_command_timer);
		}
		else
		{
			m_command.m_type=m_next_command.m_type;
			m_command.m_goal = m_next_command.m_goal;
			m_command.m_goal_object_id=m_next_command.m_goal_object_id;
			m_command.m_range=m_next_command.m_range;
			m_command.m_damage_mult = 1;
	
			// Naechstes Kommando auf nichts setzen
			m_next_command.m_type = Action::NOACTION;
			
			m_event_mask |= NetEvent::DATA_NEXT_COMMAND;
		}

		m_event_mask |= NetEvent::DATA_COMMAND;
		

	}

	// Kommando ausrechnen das evtl aus einem Statusmod resultiert
	calcStatusModCommand();



}

void Creature::calcAction()
{

	
	updateCommand();

	// wenn kein Kommando existiert keine Aktion ausfuehren, beenden
	if (m_command.m_type == Action::NOACTION)
	{
		if (m_action.m_type!= Action::NOACTION)
		{
			m_event_mask |= NetEvent::DATA_ACTION;
		}
		m_action.m_type = Action::NOACTION;
		return;

	}

	

	DEBUG5("calc action for command %i",m_command.m_type);
	m_event_mask |= NetEvent::DATA_ACTION;


	// Reichweite der Aktion berechnen
	float range = m_command.m_range;

	if (m_command.m_type == Action::WALK)
		range =getShape()->m_radius;

	if (m_command.m_type == Action::TAKE_ITEM)
		range = 2;

	// Koordinaten des Zielpunktes
	Vector goal = m_command.m_goal;
	m_action.m_goal = goal;

	// Zeiger auf das Zielobjekt
	WorldObject* goalobj=0;

	// eigene Position
	Vector& pos = getShape()->m_center;

	// Abstand zum Ziel
	float dist = pos.distanceTo(goal)-getShape()->m_radius;

	// Wenn Zielobjekt per ID gegeben
	if ( m_command.m_goal_object_id !=0 && m_command.m_type != Action::TAKE_ITEM)
	{
		DEBUG5("goal ID: %i",m_command.m_goal_object_id);
		// Zielobjekt holen
		goalobj = getRegion()->getObject(m_command.m_goal_object_id);



		if (goalobj ==0)
		{
			// Zielobjekt existiert nicht mehr, abbrechen
			m_action.m_type = Action::NOACTION;
			m_action.m_elapsed_time =0;
			clearCommand(false);
			return;
		}

		// Ziel muss aktiv sein
		if (goalobj->getState() != STATE_ACTIVE)
		{
			DEBUG5("refused to interact with inactive objekt %i",m_command.m_goal_object_id);
			m_action.m_type = Action::NOACTION;
			m_action.m_elapsed_time =0;
			clearCommand(false);
			return;
		}

		// Zielkoordinaten neu setzen (Koordinaten des Zielobjektes)
		goal = goalobj->getShape()->m_center;
		
		// Abstandsberechnung
		dist = getShape()->getDistance(*(goalobj->getShape()));
	}
	
	if (m_command.m_type == Action::TAKE_ITEM)
	{
		
		DropItem* di = getRegion()->getDropItem(m_command.m_goal_object_id);
		if (di ==0)
		{
			// Zielobjekt existiert nicht mehr, abbrechen
			m_action.m_type = Action::NOACTION;
			m_action.m_elapsed_time =0;
			clearCommand(false);
			return;
		}
		else
		{
			Vector pos(di->m_x/2.0, di->m_y / 2.0);
			goal = pos;
			dist = getShape()->m_center.distanceTo(pos);
		}
	}

	
	// Stuermen ohne Zielobjekt hat keinen Zielradius
	// TODO: nachfragen
	
	if ((m_command.m_type == Action::CHARGE || m_command.m_type == Action::STORM_CHARGE) &&  m_command.m_goal_object_id==0)
	{
		range=0;
		dist =1;
	}
	

	if (Action::getActionInfo(m_command.m_type)->m_distance == Action::MELEE || Action::getActionInfo(m_command.m_type)->m_base_action == Action::WALK)
	{
		// Aktion fuer die man an das Ziel hinreichend nahe herankommen muss
		DEBUG5("range %f dist %f",range,dist);

		// Testen ob das Ziel in Reichweite ist
		if (range > dist)
		{
			// Ziel ist in Reichweite, geplante Aktion ausfuehren
			if (m_command.m_type != Action::WALK)
			{
				m_action.m_type = m_command.m_type;
				m_action.m_goal_object_id = m_command.m_goal_object_id;
				m_action.m_goal = goal;
			}
			else
			{
				m_action.m_type = Action::NOACTION;
				clearCommand(true);
			}

		}
		else
		{
			// Ziel ist nicht in Reichweite, hinlaufen

			m_action.m_type = Action::WALK;
			if ((m_command.m_type == Action::CHARGE || m_command.m_type == Action::STORM_CHARGE)  )
			{
				// Sturmangriff
				DEBUG5("Charge");
				
				// beim ersten Mal richtung neu ausrechnen, das ist der Fall wenn der Schadensmultiplikator gleich 1 ist
				// sonst nur beschleunigen
				if (m_command.m_damage_mult>1)
				{
					// Richtung nicht neu ausrechnen, beschleunigen
					m_command.m_damage_mult += 2;
					
					// Geschwindigkeit normieren, mit Schadensmultiplikator multiplizieren
					m_speed.normalize();

					if (m_command.m_type == Action::CHARGE)
						m_command.m_damage_mult *= 0.85;
					else
						m_command.m_damage_mult *= 0.93;

					// Schaden neu berechnen
					recalcDamage();

					m_speed *= sqrt(m_command.m_damage_mult);
					m_event_mask |= NetEvent::DATA_MOVE_INFO;
				}
				else
				{
					// Richtung: direct zum Ziel;
					DEBUG5("calc charge dir");
					// Vektor von der eigenen Position zum Ziel, normieren
					Vector dir = goal-pos;
					dir.normalize();
					m_speed = dir;
					
					if (dir.getLength()!=0)
					{
						m_command.m_damage_mult = (m_command.m_damage_mult+2);
					}
					else
					{
						m_action.m_type = Action::NOACTION;
						m_action.m_elapsed_time =0;
					}
					m_event_mask |= NetEvent::DATA_MOVE_INFO;

				}


			}
			else
			{
				// Berechnen der Bewegungsrichtung
				calcWalkDir(goal, goalobj);

			}
		}

	}
	else
	{
		// keine Nahkampfaktion bzw laufen -> Action direkt ausfuehren
		m_action.m_type = m_command.m_type;
		m_action.m_goal_object_id = m_command.m_goal_object_id;
		m_action.m_goal = goal;
	}

	// wenn keine Aktion berechnet wurde, Kommando beenden
	if (m_action.m_type == Action::NOACTION)
	{
		clearCommand(true);
	}
}

void Creature::calcStatusModCommand()
{
	// eigene Position
	Vector& pos = getShape()->m_center;
	
	// Statusmod verwirrt
	// diese Aktion nur vom Server ausloesen lassen
	if (m_dyn_attr.m_status_mod_time[Damage::CONFUSED]>0 && World::getWorld()->isServer())
	{
		// aktuelle Bewegungsrichtung
		Vector v = m_speed;
		float range = m_base_attr.m_step_length;

		// Normieren der Bewegungsgeschwindigkeit
		v.normalize();
		
		// zufaellige Richtung auswuerfeln und normieren
		Vector dir(1-rand()*2.0/RAND_MAX, 1-rand()*2.0/RAND_MAX) ;
		dir.normalize();

		// neue Bewegungsrichtung = 70% der alten + 30% der zufaellig ausgewaehlten
		v = v*0.7+dir*0.3;

		// neue Bewegungsrichtung normieren
		v.normalize();

		// Berechnen den Punktes der bei dieser Bewegung erreicht wird
		Vector npos = pos + v*range;

		// Kreis um den Zielpunkt, Radius gleich Radius des Lebewesens
		Shape s;
		s.m_center =npos;
		s.m_type = Shape::CIRCLE;
		s.m_radius = getShape()->m_radius;
		WorldObject* wo =0;
		WorldObjectList res;
		WorldObjectList::iterator it;

		// ermitteln der Objekte mit denen bei der Bewegung kollidiert wird
		getRegion()->getObjectsInShape(&s,&res,LAYER_AIR,CREATURE | FIXED,this);

		// Zufallszahl fuer zufaellige Angriffe
		float r = rand()*1.0/RAND_MAX;
		if (!res.empty())
		{
			// Es gibt ein kollidierendes Objekt, als Zielobjekt setzen
			wo = *(res.begin());
			npos = wo->getShape()->m_center;

		}
		else
		{
			// es gibt kein kollidierendes Objekt, mit 30% Wahrscheinlichkeit Angriff ins nichts
			if (r>0.7)
			{
				range += m_base_attr_mod.m_attack_range;
				npos = pos + v*range;
			}
		}
		DEBUG5("entscheidung durch %p, %f",wo,r);
		// Angriff ausfuehren, wenn Zielobjekt vorhanden, sonst mit 30% Wahrscheinlichkeit
		if (wo!=0 || r > 0.7)
		{
			// Angriff
			m_command.m_type = m_base_action;
			m_command.m_goal = npos;
			m_command.m_goal_object_id =0;
			m_command.m_range = getBaseAttrMod()->m_attack_range;
			m_event_mask |= NetEvent::DATA_COMMAND;

			// Im Falle von Beserker nur Nahkampf
			if (m_dyn_attr.m_status_mod_time[Damage::BERSERK]>0)
			{
				m_command.m_type = Action::ATTACK;
				if (m_command.m_range >4)
				{
					m_command.m_range= 1;
				}
			}
		}
		else
		{
			// Laufen
			m_command.m_type = Action::WALK;
			m_speed = v;
			m_event_mask |= NetEvent::DATA_COMMAND | NetEvent::DATA_MOVE_INFO;
			return;
		}
		DEBUG5("confused command %i",m_command.m_type);

	}
	else if (m_dyn_attr.m_status_mod_time[Damage::BERSERK]>0)
	{
		// Behandlung von Berserker
		int id =0;
		float rmin =1000;

		Vector wpos , goal(0,0);

		// Kreis mit Radius 8 um eigenen Mittelpunkt
		Shape s;
		s.m_type = Shape::CIRCLE;
		s.m_radius =8;
		s.m_center = pos;
		WorldObjectList res;
		WorldObjectList::iterator i;
		res.clear();

		// Suchen aller Objekte im Kreis
		getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,this);
		if (res.empty())
		{
			WorldObject* wo = getRegion()->getObject(getDynAttr()->m_last_attacker_id);
			if (wo != 0)
			{
				res.push_back(wo);
			}
		}
		
		for (i=res.begin();i!= res.end();++i)
		{
			// nur aktive Lebewesen beruecksichtigen
			if ((*i)->getState() != STATE_ACTIVE)
			{
				continue;
			}

			DEBUG5("checking obj %i",(*i)->getId());
			// Fuer feindliche Lebewesen
			if (World::getWorld()->getRelation(m_fraction,*i) == WorldObject::HOSTILE)
			{
				// Abstand zum eigenen Mittelpunkt berechnen
				DEBUG5("hostile");
				wpos = (*i)->getShape()->m_center;
				
				// Objekt mit dem kleinsten Abstand als Zielobjekt setzen
				if (pos.distanceTo(wpos)<rmin)
				{
					rmin =pos.distanceTo(wpos);
					id = (*i)->getId();
					goal = wpos;
				}

			}
		}

		// Wenn Zielobjekt gefunden wurde
		if (id !=0)
		{
			getDynAttr()->m_timer.start();
			DEBUG5("attack id %i",id);
			// Angriff
			m_command.m_type = Action::ATTACK;
			m_command.m_goal_object_id = id;
			m_command.m_goal = goal;
			m_command.m_range = getBaseAttrMod()->m_attack_range;

			// nur Nahkampf, daher Reichweite die von Fernwaffen kommt reduzieren
			if (m_command.m_range >4)
			{
				DEBUG5("capped range for berserk");
				m_command.m_range= 1;
			}

		}
		else
		{
			// nichts machen
			m_command.m_type = Action::NOACTION;
			if (getDynAttr()->m_timer.getTime() >1000)
			{
				m_dyn_attr.m_status_mod_time[Damage::BERSERK] =0;
				m_event_mask |= NetEvent::DATA_STATUS_MODS;
			}
			
		}
		m_event_mask |= NetEvent::DATA_COMMAND;
	}
}

void Creature::calcWalkDir(Vector goal,WorldObject* goalobj)
{
	// maximale Entfernung bei der normale Wegfindung genommen wird
	int pathmaxdist = 16;
	
	// eigene Position
	Vector& pos = getShape()->m_center;
	Vector dir;

	// wenn als Ziel ein Lebenwesen angegeben ist
	if (goalobj !=0 && goalobj->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT )
	{
		DEBUG5("using pot field of object %i",goalobj->getId());
		Creature* cr = (Creature*) goalobj;
		// Potentialfeld des Ziellebewesens verwenden
		cr->getPathDirection(pos,getGridLocation()->m_region, 2*getShape()->m_radius, m_layer,dir);
	}
	else
	{
		DEBUG5("using own pot field");
		// Bewegung zu einem Punkt oder festen Objekt
		// eigenes Potentialfeld verwenden

		// true, wenn das eigene Potentailfeld neu berechnet werden muss
		bool recalc = false;

		// true, wenn als Richtung die direkte Verbindung genutzt wird
		bool direct = false;

		// Wenn in einer Koordinatenrichtung Abstand groesser 10 ist direkte Verbindung benutzen
		if (fabs(pos.m_x-goal.m_x)>pathmaxdist || fabs(pos.m_y-goal.m_y)>pathmaxdist)
		{
			direct = true;
		}

		if (m_path_info ==0)
		{
			// Potentialfeld existiertnoch nicht
			DEBUG5("allocating new pot field");
			m_path_info = new PathfindInfo;

			// Qualitaet der Suche, geht quadratisch in die Laufzeit ein
			int qual=4;
			int dim = 2*pathmaxdist*qual+1;

			// Potentialfeld
			m_path_info->m_pot = new Matrix2d<float>(dim,dim);
			// Blockmatrix
			m_path_info->m_block = new Matrix2d<char>(dim,dim);

			// Kantenlaenge eines Quadrates im Suchfeld
			//float sqs =  getGeometry()->m_shape.m_radius*2/ qual;

			// Senke des Feldes ist der Zielpunkt
			m_path_info->m_start= goal;

			m_path_info->m_dim = dim;
			m_path_info->m_layer= LAYER_BASE | LAYER_AIR;
			m_path_info->m_region=getGridLocation()->m_region;
			m_path_info->m_base_size = getShape()->m_radius*2;
			m_path_info->m_quality=qual;
			m_path_info->m_id = getId();
			// neu berechnen noetig
			recalc = true;

		}
		else
		{

			if ( m_path_info->m_region != getGridLocation()->m_region)
			{
				// Ziel befindet sich in einer anderen Region als die des Potentialfeldes
				// Region anpassen, neu berechnen
				m_path_info->m_region=getGridLocation()->m_region;
				recalc = true;
			}

			// Ziel hat sich deutlich bewegt, neuen Weg suchen
			float d = m_path_info->m_start.distanceTo(goal);
			if (d>1)
				recalc = true;

			// Potentialfeld ist aelter als 500 ms, neu berechnen
			if (m_path_info->m_timer>500)
				recalc = true;

		}

		if (!direct)
		{
			if (recalc)
			{
				// neu berechnen notwendig
				DEBUG5("recalc walk field");
				
				// Zentrum ist die eigene Position
				m_path_info->m_center.m_x= goal.m_x +roundf(pos.m_x-goal.m_x);
				m_path_info->m_center.m_y=goal.m_y +roundf(pos.m_y-goal.m_y);
				// Senke ist der Zielpunkt
				m_path_info->m_start= goal;


				// Timer auf 0 setzen
				m_path_info->m_timer=0;


				// Blockmatrix berechnen
				World::getWorld()->calcBlockmat(m_path_info);


				// Potentialfeld berechnen
				World::getWorld()->calcPotential(m_path_info);

			}

			// Richtung auf Basis des eigenen Potentialfeldes berechnen
			World::getWorld()->calcPathDirection(m_path_info, pos, dir);
		}
		else
		{
			// direkte Verbindung als Richtung verwenden
			DEBUG5("using direct way");
			dir = goal - pos;
		}

	}

	// Richtung normieren
	dir.normalize();
	
	if (dir.getLength() ==0)
	{
		m_command.m_type = Action::NOACTION;
		m_action.m_type = Action::NOACTION;
		m_action.m_elapsed_time =0;
		m_command.m_damage_mult=1;
		m_event_mask |= NetEvent::DATA_COMMAND | NetEvent::DATA_ACTION;
		return;
	}
	else
	{
		m_event_mask |= NetEvent::DATA_MOVE_INFO;
		// TODO: Wende über 90 Grad behandeln
		m_speed = dir;

	}
}

void Creature::clearCommand(bool success)
{
	
	if (hasScriptCommand() && m_command.m_type!= Action::NOACTION)
	{
		DEBUG5("command %i ended with success %i",m_command.m_type, success);
		Trigger* tr = new Trigger("command_complete");
		tr->addVariable("unit",getId());
		tr->addVariable("command",Action::getActionInfo(m_command.m_type)->m_enum_name);
		tr->addVariable("success",success);
		getRegion()->insertTrigger(tr);
	}
	m_command.m_type = Action::NOACTION;
	m_command.m_damage_mult = 1;
	m_command.m_goal = Vector(0,0);
	m_command.m_goal_object_id =0;
	m_command.m_range =1;
	m_script_command_timer =0;
	m_event_mask |= NetEvent::DATA_COMMAND;
}

bool Creature::update (float time)
{
	Timer timer;
	timer.start();

	if (m_action.m_elapsed_time> m_action.m_time)
	{
		DEBUG("elapsed time %f all time %f",m_action.m_elapsed_time,	m_action.m_time);
	}

	float ptime = time;

	DEBUG5("Update des Creatureobjekts [%i] wird gestartet", getId());
	// interne Variable um Fehler zu speichern
	bool result=true;

	
	// Timer fuer Sprache anpassen
	if (!m_speak_text.empty() && m_speak_text.m_answers.empty())
	{
		if (m_speak_text.m_time < time)
		{
			m_speak_text.clear();
		}
		else
		{
			m_speak_text.m_time -= time;
		}
	}
	


	// Timer herunterzaehlen lassen
	m_timer1 -=time;
	if (m_timer1<0)
	{
		m_timer1=0;
	}

	m_timer2 -=time;
	if (m_timer2<0)
	{
		m_timer2=0;
	}

	DEBUG5("timer1 %f timer2 %f",m_timer1,m_timer2);


	// Timer fuer Wegsucheinfo inkrementieren
	if (m_path_info)
		m_path_info->m_timer += time;

	if (m_small_path_info)
		m_small_path_info->m_timer += time;

	if (m_medium_path_info)
		m_medium_path_info->m_timer += time;

	if (m_big_path_info)
		m_big_path_info->m_timer += time;

	if (m_small_flying_path_info)
		m_small_flying_path_info->m_timer += time;



	// Zeit fuer Statusveraenderungen / Immunisierungen reduzieren
	for (int i=0;i<NR_STATUS_MODS;i++)
	{
		m_dyn_attr.m_status_mod_time[i] -= time;
		if (m_dyn_attr.m_status_mod_time<0)
		{
			m_dyn_attr.m_status_mod_time[i]=0;
			// aktuelle Aktion abbrechen nach auslaufen von Berserker / verwirrt
			// (da die Aktion idr ungewollt bzw ungeplant ist)
			if (i==Damage::BERSERK || i==Damage::CONFUSED)
			{
				m_command.m_type = Action::NOACTION;
				m_event_mask |= NetEvent::DATA_COMMAND;
			}
		}

		m_dyn_attr.m_status_mod_immune_time[i] -= time;
		if (m_dyn_attr.m_status_mod_immune_time[i]<0)
		{
			m_dyn_attr.m_status_mod_immune_time[i]=0;
		}

	}

	// Zeit fuer Effekte reduzieren
	for (int i=0;i<NR_EFFECTS;i++)
	{
		m_dyn_attr.m_effect_time[i] -= time;
		if (m_dyn_attr.m_effect_time[i]<0)
		{
			m_dyn_attr.m_effect_time[i]=0;
		}
	}

	// Zeit fuer temporaere Effekte reduzieren und ggf deaktivieren
	// true, wenn die modifizierten Attribute neu berechnet werden muessen
	bool recalc = false;
	std::list<CreatureBaseAttrMod>::iterator i;
	for (i=m_dyn_attr.m_temp_mods.begin();i!=m_dyn_attr.m_temp_mods.end();)
	{

		i->m_time -= time;
		if (i->m_time <=0)
		{
			// Zeit abgelaufen, Modifikation deaktivieren
			DEBUG5("removing base attr mod");
			recalc |= removeBaseAttrMod((CreatureBaseAttrMod*) &(*i));
			i=m_dyn_attr.m_temp_mods.erase(i);
		}
		else
		{
			++i;
		}

	}

	if (recalc)
	{
		// modifizierte Attribute neu berechnen
		DEBUG5("modifizierte Attribute neu berechnen");
		calcBaseAttrMod();

	}



	// besondere zeitabhaengige Effekte berechnen
	if (World::getWorld()->isServer())
	{
		if (m_base_attr_mod.m_special_flags & FLAMEARMOR && World::getWorld()->timerLimit(1))
		{
			// Flammenruestung


			// Schaden fuer Flammenruestung setzen
			Damage d;
			d.m_min_damage[Damage::FIRE] = 500*m_base_attr_mod.m_magic_power*0.0003;
			d.m_max_damage[Damage::FIRE] = 500*m_base_attr_mod.m_magic_power*0.0005;
			d.m_multiplier[Damage::FIRE]=1;
			d.m_attacker_fraction = m_fraction;

			WorldObjectList res;
			res.clear();
			WorldObjectList::iterator it;

			// Kreis um eigenen Mittelpunkt mit Radius eigener Radius plus 1
			Shape s;
			s.m_center  = getShape()->m_center;
			s.m_type = Shape::CIRCLE;
			s.m_radius = getShape()->m_radius+1;
			Creature* cr;
			
			// Alle Objekte im Kreis suchen
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,this);
			for (it=res.begin();it!=res.end();++it)
			{
				// Schaden austeilen
				if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
				{
					cr = (Creature*) (*it);
					cr->takeDamage(&d);

				}
			}

		}
	}

	// Statusmods behandeln
	if (getState()==STATE_ACTIVE)
	{
		if (World::getWorld()->isServer() && World::getWorld()->timerLimit(1))
		{
			// Vergiftet
			if (m_dyn_attr.m_status_mod_time[Damage::POISONED]>0)
			{
				// Schaden pro Sekunde 1/60 der HP
				DEBUG5("poisoned");
				m_dyn_attr.m_health -= 500*m_base_attr_mod.m_max_health / 60000;
				m_event_mask |= NetEvent::DATA_HP;
			}

			// brennend
			if (m_dyn_attr.m_status_mod_time[Damage::BURNING]>0)
			{
				// Schaden pro Sekunde 1/90 der HP (bei 0 Feuerresistenz)
				DEBUG5("burning");
				m_dyn_attr.m_health -= (100-m_base_attr_mod.m_resistances[Damage::FIRE])*500*m_base_attr_mod.m_max_health / 9000000;
				m_event_mask |= NetEvent::DATA_HP;
			}
		}

		// eingefroren
		if (m_dyn_attr.m_status_mod_time[Damage::FROZEN]>0)
		{
			// keine Zeit fuer Aktionen
			time=0;
		}

		// gelaehmt
		if (m_dyn_attr.m_status_mod_time[Damage::PARALYZED]>0)
		{
			// Zeit auf 40% reduziert
			time *= 0.4;
		}

		// Wenn Lebenspunkte unter 0
		if (m_dyn_attr.m_health <= 0)
		{
			// sterben
			die();
		}
	}

	// Solange noch Zeit zur Verfügung steht Aktionen ausfuehren lassen
	while (time>0)
	{
		switch (getState())
		{
			// wenn das Objekt aktiv ist
			case STATE_ACTIVE:



				// wenn die Lebenspunkte des Lebewesens kleiner gleich 0 sind stirbt das Lebewesen
				DEBUG5("health %f",m_dyn_attr.m_health);
				if (m_dyn_attr.m_health <= 0)
				{
					die();
				}
				else
				{
					// Solange noch Zeit zur Verfügung steht
					DEBUG5("Objekt aktiv");

					while (time>0)
					{
						// Wenn keine Aktion gesetzt Aktion ausrechnen und initialisieren
						if (m_action.m_type == Action::NOACTION)
						{
							calcAction();
							initAction();
							calcDamage(m_action.m_type,m_damage);
						}

						// Aktion ausfuehren
						performAction(time);

						if (ptime < 1000 && time > 1000)
						{
							DEBUG("prev time %f time set to %f",ptime, time);
							ptime = time;
						}


					}
				}


				break;


			// sonst, eventuell hier später weitere Stati gesondert bearbeiten STATES_DEAD, STATES_INACTIVE
 			case STATE_DIEING:
				performAction(time);
				if (m_action.m_type == Action::NOACTION)
				{
					setState(STATE_DEAD);
					m_action.m_type = Action::DEAD;
					m_action.m_time = 1000;

					m_event_mask |= NetEvent::DATA_STATE | NetEvent::DATA_ACTION;
				}
				break;

			case STATE_DEAD:
				performAction(time);
				if (m_action.m_type == Action::NOACTION)
				{
					setDestroyed(true);
				}

			default:
				DEBUG5("unknown state: %i",getState());
				time=0 ;
		}

	}

	if (timer.getTime()>10)
	{
		DEBUG5("object %i update time %f",getId(), timer.getTime());
	}
	return result;
}



void Creature::gainExperience (float exp)
{
	if (getTypeInfo()->m_type != TypeInfo::TYPE_PLAYER)
		return;


	// Erfahrung dazu addieren
	m_dyn_attr.m_experience += exp;
	m_event_mask |= NetEvent::DATA_EXPERIENCE;

	// Solange Level aufsteigen, bis exp < max_exp
	while (m_dyn_attr.m_experience>= m_base_attr.m_max_experience)
	{
		gainLevel();
	}
}

void Creature::gainLevel()
{
	m_event_mask |= NetEvent::DATA_LEVEL;

}


void Creature::calcDamage(Action::ActionType act,Damage& dmg)
{
	// alles nullen
	dmg.init();

	dmg.m_attacker_id = getId();
	dmg.m_attacker_fraction = m_fraction;

	DEBUG5("Calc Damage for action %i",m_action.m_type);
	for (int i=0;i<4;i++)
		dmg.m_multiplier[i]=1;

	if (act == Action::NOACTION)
		return;


	// Schaden durch Basisattribute und ggf Ausruestung
	calcBaseDamage(act,dmg);

	// Modifikation des Schadens durch Faehigkeiten
	calcAbilityDamage(act,dmg);

	// Statusmods
	// Blind
	if (m_dyn_attr.m_status_mod_time[Damage::BLIND]>0)
	{
		// keine kritischen Treffer, Attackewert halbieren
		dmg.m_crit_perc=0;
		dmg.m_attack *= 0.5;
	}

	// verwirrt
	if (m_dyn_attr.m_status_mod_time[Damage::CONFUSED]>0)
	{
		// Fraktion auf feindlich gegen alle setzen
		dmg.m_attacker_fraction = FRAC_HOSTILE_TO_ALL;
	}
}

void Creature::calcBaseDamage(Action::ActionType act,Damage& dmg)
{
	// Basisaktion
	Action::ActionType basact = Action::getActionInfo(act)->m_base_action;
	CreatureBaseAttr* basm = getBaseAttrMod();


	if (basact == Action::ATTACK)
	{
		// Basisaktion ist normaler Angriff
		DEBUG5("base str %i mod str %i",m_base_attr.m_strength,basm->m_strength);

		dmg.m_min_damage[Damage::PHYSICAL] += basm->m_strength/4;
		dmg.m_min_damage[Damage::PHYSICAL] += basm->m_dexterity/10;
		dmg.m_max_damage[Damage::PHYSICAL] += basm->m_strength/3;
		dmg.m_max_damage[Damage::PHYSICAL] += basm->m_dexterity/8;


		dmg.m_attack += basm->m_attack;
		dmg.m_attack += basm->m_dexterity/2;

		dmg.m_power += basm->m_strength/2;

		dmg.m_special_flags = Damage::NOFLAGS;

	}

	if (basact == Action::MAGIC_ATTACK)
	{
		// Basisaktion ist magischer Angriff
		dmg.m_min_damage[Damage::FIRE] += basm->m_magic_power/10;
		dmg.m_min_damage[Damage::FIRE] += basm->m_willpower/20;
		dmg.m_max_damage[Damage::FIRE] += basm->m_magic_power/6;
		dmg.m_max_damage[Damage::FIRE] += basm->m_willpower/15;

		dmg.m_special_flags = Damage::UNBLOCKABLE;

		dmg.m_attack=0;

	}

	if (basact == Action::RANGE_ATTACK)
	{
		// Basisaktion ist Fernangriff
		dmg.m_min_damage[Damage::PHYSICAL] += basm->m_strength/10;
		dmg.m_min_damage[Damage::PHYSICAL] += basm->m_dexterity/6;
		dmg.m_max_damage[Damage::PHYSICAL] += basm->m_strength/8;
		dmg.m_max_damage[Damage::PHYSICAL] += basm->m_dexterity/4;

		dmg.m_attack += basm->m_attack;
		dmg.m_attack += basm->m_dexterity/2;

		dmg.m_power += basm->m_dexterity/2;

		dmg.m_special_flags = Damage::NOFLAGS;

	}

	if (basact == Action::HOLY_ATTACK)
	{
		// Basisaktion ist heiliger Angriff
		dmg.m_min_damage[Damage::PHYSICAL] += basm->m_strength/9;
		dmg.m_min_damage[Damage::PHYSICAL] += basm->m_willpower/6;
		dmg.m_max_damage[Damage::PHYSICAL] += basm->m_strength/6;
		dmg.m_max_damage[Damage::PHYSICAL] += basm->m_willpower/3;

		dmg.m_attack += basm->m_attack;
		dmg.m_attack += basm->m_dexterity/2;

		dmg.m_power += basm->m_willpower/2;

		dmg.m_special_flags = Damage::NOFLAGS;

	}
}

void Creature::calcAbilityDamage(Action::ActionType act,Damage& dmg)
{
	// CreatureBaseAttr* basm = getBaseAttrMod();

	// Eigenschaften durch passive  Faehigkeiten
	if (checkAbility(Action::CRITICAL_STRIKE))
	{
		// 10% extra Chance auf kritische Treffer
		dmg.m_crit_perc += 0.1;
	}

	// klirrende Kaelte
	if (checkAbility(Action::CHILL))
	{
		// 20% mehr Schaden
		dmg.m_multiplier[Damage::ICE] *= 1.2;
	}

	// Ionisation
	if (checkAbility(Action::IONISATION))
	{
		// 20% mehr Schaden
		dmg.m_multiplier[Damage::AIR] *= 1.2;
	}

	// Entzuenden
	if (checkAbility(Action::INFLAME) && act >=Action::FIRE_BOLT && act<=Action::FIRE_WALL )
	{
		dmg.m_status_mod_power[Damage::BURNING] += m_base_attr_mod.m_magic_power;
	}


	// Eigenschaften durch temporaere Faehigkeiten

	// Faehigkeit Feuer und Schwert
	if (m_base_attr_mod.m_special_flags & FIRESWORD)
	{
		dmg.m_min_damage[Damage::PHYSICAL]*=0.75;
		dmg.m_max_damage[Damage::PHYSICAL]*=0.75;
		// Feuerschaden hinzufuegen
		dmg.m_min_damage[Damage::FIRE] += std::min(m_base_attr_mod.m_magic_power*2.0f,dmg.m_min_damage[Damage::PHYSICAL]);
		dmg.m_max_damage[Damage::FIRE] += std::min(m_base_attr_mod.m_magic_power*3.0f,dmg.m_max_damage[Damage::PHYSICAL]);

		// kein Eisschaden
		dmg.m_min_damage[Damage::ICE]=0;
		dmg.m_max_damage[Damage::ICE]=0;

		if (m_base_attr_mod.m_special_flags & FLAMESWORD)
		{
			// Statusmod brennend austeilen
			dmg.m_status_mod_power[Damage::BURNING] += m_base_attr_mod.m_magic_power*3;
		}
	}

	// Elfenaugen
	if (m_base_attr_mod.m_special_flags & CRIT_HITS)
	{
		// +20% Chance auf kritische Treffer
		dmg.m_crit_perc += 0.2;
	}

	// Eispfeile
	if (m_base_attr_mod.m_special_flags & ICE_ARROWS)
	{
		dmg.m_min_damage[Damage::ICE] += std::min(m_base_attr_mod.m_magic_power*2.0f,dmg.m_min_damage[Damage::PHYSICAL]);
		dmg.m_max_damage[Damage::ICE] += std::min(m_base_attr_mod.m_magic_power*3.0f,dmg.m_max_damage[Damage::PHYSICAL]);
		dmg.m_min_damage[Damage::PHYSICAL]*=0.5;
		dmg.m_max_damage[Damage::PHYSICAL]*=0.5;
		dmg.m_min_damage[Damage::FIRE]=0;
		dmg.m_max_damage[Damage::FIRE]=0;

		if (m_base_attr_mod.m_special_flags & FROST_ARROWS)
		{
			// Statusmod einfrieren
			dmg.m_status_mod_power[Damage::FROZEN] += m_base_attr_mod.m_magic_power*3;
		}
	}

	// Windpfeile
	if (m_base_attr_mod.m_special_flags & WIND_ARROWS)
	{
		dmg.m_min_damage[Damage::AIR] += std::min(m_base_attr_mod.m_magic_power*2.0f,dmg.m_min_damage[Damage::PHYSICAL]);
		dmg.m_max_damage[Damage::AIR] += std::min(m_base_attr_mod.m_magic_power*3.0f,dmg.m_max_damage[Damage::PHYSICAL]);
		dmg.m_min_damage[Damage::PHYSICAL]*=0.5;
		dmg.m_max_damage[Damage::PHYSICAL]*=0.5;

		if (m_base_attr_mod.m_special_flags & STORM_ARROWS)
		{
			dmg.m_multiplier[Damage::AIR] *= 1.4;
		}
	}

	// Schaden der Faehigkeit an sich

	switch (act)
	{
		case Action::BASH:
			dmg.m_multiplier[Damage::PHYSICAL] *= 3;
			break;

		case Action::HAMMER_BASH:
			dmg.m_multiplier[Damage::PHYSICAL] *= 2;
			break;

		case Action::AROUND_BLOW:
			dmg.m_multiplier[Damage::PHYSICAL] *= 0.6;
			break;

		case Action::WHIRL_BLOW:
			dmg.m_multiplier[Damage::PHYSICAL] *= 1;
			break;

		case Action::SMASH:
			dmg.m_multiplier[Damage::PHYSICAL] *= 3;
			// kann nicht geblockt werden,  Ruestung ignorieren
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			dmg.m_special_flags |= Damage::IGNORE_ARMOR;
			break;

		case Action::HATE_MAGE:
			dmg.m_multiplier[Damage::PHYSICAL] *= 1;
			dmg.m_status_mod_power[Damage::MUTE] += m_base_attr_mod.m_strength;
			break;

		case Action::CHARGE:
			dmg.m_multiplier[Damage::PHYSICAL] *= m_command.m_damage_mult;
			dmg.m_attack *=m_command.m_damage_mult*0.5;
			break;

		case Action::STORM_CHARGE:

			dmg.m_multiplier[Damage::PHYSICAL] *= m_command.m_damage_mult;
			dmg.m_attack *=m_command.m_damage_mult*0.5;
			dmg.m_status_mod_power[Damage::PARALYZED] += (short)  (m_base_attr_mod.m_strength*m_command.m_damage_mult*0.2);
			break;
			
		case Action::DECOY:
			dmg.m_ai_mod_power[TAUNT] += m_base_attr_mod.m_strength/2 + std::min(m_base_attr_mod.m_strength/2,m_base_attr_mod.m_willpower*2);
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;
			
		case Action::SCARE:
			dmg.m_ai_mod_power[TAUNT] += m_base_attr_mod.m_strength/2 + std::min(m_base_attr_mod.m_strength/2,m_base_attr_mod.m_willpower*2);
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;
			
		case Action::WARCRY:
			dmg.m_status_mod_power[Damage::CONFUSED] += m_base_attr_mod.m_strength/2 + std::min(m_base_attr_mod.m_strength/2,m_base_attr_mod.m_willpower*2) ;
			break;

		case Action::FIRE_BOLT:
			dmg.m_min_damage[Damage::FIRE] += m_base_attr_mod.m_magic_power*2;
			dmg.m_max_damage[Damage::FIRE] += m_base_attr_mod.m_magic_power*3;
			dmg.m_min_damage[Damage::ICE]=0;
			dmg.m_max_damage[Damage::ICE]=0;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case Action::FIRE_STRIKE:
			dmg.m_min_damage[Damage::FIRE] += m_base_attr_mod.m_magic_power*3.3;
			dmg.m_max_damage[Damage::FIRE] += m_base_attr_mod.m_magic_power*5;
			dmg.m_min_damage[Damage::ICE]=0;
			dmg.m_max_damage[Damage::ICE]=0;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case Action::FIRE_WAVE:
			dmg.m_min_damage[Damage::FIRE] += m_base_attr_mod.m_magic_power*1.5;
			dmg.m_max_damage[Damage::FIRE] += m_base_attr_mod.m_magic_power*2;
			dmg.m_min_damage[Damage::ICE]=0;
			dmg.m_max_damage[Damage::ICE]=0;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case Action::FIRE_STORM:
			dmg.m_min_damage[Damage::FIRE] += m_base_attr_mod.m_magic_power*2.5;
			dmg.m_max_damage[Damage::FIRE] += m_base_attr_mod.m_magic_power*3;
			dmg.m_min_damage[Damage::ICE]=0;
			dmg.m_max_damage[Damage::ICE]=0;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case Action::FIRE_BALL:
			dmg.m_min_damage[Damage::FIRE] += m_base_attr_mod.m_magic_power*2;
			dmg.m_max_damage[Damage::FIRE] += m_base_attr_mod.m_magic_power*3;
			dmg.m_min_damage[Damage::ICE]=0;
			dmg.m_max_damage[Damage::ICE]=0;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case Action::INFERNO_BALL:
			dmg.m_min_damage[Damage::FIRE] += m_base_attr_mod.m_magic_power*3.3;
			dmg.m_max_damage[Damage::FIRE] += m_base_attr_mod.m_magic_power*5;
			dmg.m_min_damage[Damage::ICE]=0;
			dmg.m_max_damage[Damage::ICE]=0;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case	Action::FIRE_WALL:
			dmg.m_min_damage[Damage::FIRE] = m_base_attr_mod.m_magic_power/1500.0;
			dmg.m_max_damage[Damage::FIRE] = m_base_attr_mod.m_magic_power/1000.0;
			dmg.m_min_damage[Damage::ICE]=0;
			dmg.m_max_damage[Damage::ICE]=0;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case	Action::ICE_BOLT:
			dmg.m_min_damage[Damage::ICE] += m_base_attr_mod.m_magic_power*2;
			dmg.m_max_damage[Damage::ICE] += m_base_attr_mod.m_magic_power*3;
			dmg.m_min_damage[Damage::FIRE]=0;
			dmg.m_max_damage[Damage::FIRE]=0;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case	Action::ICE_SPIKE:
			dmg.m_min_damage[Damage::ICE] += m_base_attr_mod.m_magic_power*3;
			dmg.m_max_damage[Damage::ICE] += m_base_attr_mod.m_magic_power*4.5;
			dmg.m_min_damage[Damage::FIRE]=0;
			dmg.m_max_damage[Damage::FIRE]=0;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case	Action::SNOW_STORM:
			dmg.m_min_damage[Damage::ICE] = m_base_attr_mod.m_magic_power/3;
			dmg.m_max_damage[Damage::ICE] = m_base_attr_mod.m_magic_power/2;
			dmg.m_min_damage[Damage::FIRE]=0;
			dmg.m_max_damage[Damage::FIRE]=0;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case	Action::BLIZZARD:
			dmg.m_min_damage[Damage::ICE] = m_base_attr_mod.m_magic_power/2;
			dmg.m_max_damage[Damage::ICE] = m_base_attr_mod.m_magic_power/1.3;
			dmg.m_min_damage[Damage::FIRE]=0;
			dmg.m_max_damage[Damage::FIRE]=0;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case	Action::ICE_RING:
			dmg.m_min_damage[Damage::ICE] += m_base_attr_mod.m_magic_power;
			dmg.m_max_damage[Damage::ICE] += m_base_attr_mod.m_magic_power*1.5;
			dmg.m_min_damage[Damage::FIRE]=0;
			dmg.m_max_damage[Damage::FIRE]=0;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case	Action::FROST_RING:
			dmg.m_min_damage[Damage::ICE] += m_base_attr_mod.m_magic_power*1.6;
			dmg.m_max_damage[Damage::ICE] += m_base_attr_mod.m_magic_power*2.4;
			dmg.m_min_damage[Damage::FIRE]=0;
			dmg.m_max_damage[Damage::FIRE]=0;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case	Action::FREEZE:
			dmg.m_min_damage[Damage::FIRE]=0;
			dmg.m_max_damage[Damage::FIRE]=0;
			dmg.m_status_mod_power[Damage::FROZEN] = m_base_attr_mod.m_magic_power;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case Action::LIGHTNING:
			dmg.m_min_damage[Damage::AIR] += m_base_attr_mod.m_magic_power*1.5;
			dmg.m_max_damage[Damage::AIR] += m_base_attr_mod.m_magic_power*3.5;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case Action::LIGHTNING_STRIKE:
			dmg.m_min_damage[Damage::AIR] += m_base_attr_mod.m_magic_power*2.3;
			dmg.m_max_damage[Damage::AIR] += m_base_attr_mod.m_magic_power*5;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case Action::THUNDERSTORM:
			dmg.m_min_damage[Damage::AIR] = m_base_attr_mod.m_magic_power/5.0;
			dmg.m_max_damage[Damage::AIR] = m_base_attr_mod.m_magic_power/1.5;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case Action::THUNDERSTORM2:
			dmg.m_min_damage[Damage::AIR] = m_base_attr_mod.m_magic_power/3.0;
			dmg.m_max_damage[Damage::AIR] = m_base_attr_mod.m_magic_power/1.0;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case Action::CHAIN_LIGHTNING:
			dmg.m_min_damage[Damage::AIR] += m_base_attr_mod.m_magic_power*1;
			dmg.m_max_damage[Damage::AIR] += m_base_attr_mod.m_magic_power*2.5;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case Action::CHAIN_LIGHTNING2:
			dmg.m_min_damage[Damage::AIR] += m_base_attr_mod.m_magic_power*1.7;
			dmg.m_max_damage[Damage::AIR] += m_base_attr_mod.m_magic_power*3.5;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		// Faehigkeiten Schuetze
		case Action::GUIDED_TRIPLE_SHOT:
			dmg.m_multiplier[Damage::PHYSICAL] *= 1.5;
			break;

		case Action::MULTISHOT:
			dmg.m_multiplier[Damage::PHYSICAL] *=0.75;
			break;

		case Action::WEAK_POINT:
			dmg.m_status_mod_power[Damage::BERSERK] = m_base_attr_mod.m_dexterity;
			break;

		case Action::BLIND_RAGE:
			dmg.m_status_mod_power[Damage::BLIND] = (short) (m_base_attr_mod.m_dexterity*1.5f);
			dmg.m_status_mod_power[Damage::BERSERK] = m_base_attr_mod.m_dexterity;
			break;

		case Action::PIERCE:
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case Action::VOLLEY_SHOT:
			dmg.m_multiplier[Damage::PHYSICAL] *=1;
			break;

		case Action::VACUUM:
			dmg.m_min_damage[Damage::AIR] += m_base_attr_mod.m_dexterity*1;
			dmg.m_max_damage[Damage::AIR] += m_base_attr_mod.m_dexterity*2;
			dmg.m_status_mod_power[Damage::PARALYZED] = m_base_attr_mod.m_dexterity;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

		case Action::DEATH_ROULETTE:
			dmg.m_multiplier[Damage::PHYSICAL] *=2;
			break;

		case Action::EXPLODING_ARROW:
		case Action::EXPLOSION_CASCADE:
			dmg.m_multiplier[Damage::PHYSICAL] *=2;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;



		// Priester Faehigkeiten
		case Action::HOLY_LIGHT:
			dmg.m_min_damage[Damage::PHYSICAL]=0;
			dmg.m_max_damage[Damage::PHYSICAL]=0;
			dmg.m_min_damage[Damage::FIRE] +=m_base_attr_mod.m_willpower/3;
			dmg.m_max_damage[Damage::FIRE] +=m_base_attr_mod.m_willpower/2;
			dmg.m_min_damage[Damage::FIRE] += std::min(m_base_attr_mod.m_willpower/6,m_base_attr_mod.m_magic_power*2);
			dmg.m_max_damage[Damage::FIRE] += std::min(m_base_attr_mod.m_willpower/4,m_base_attr_mod.m_magic_power*3);
			dmg.m_min_damage[Damage::ICE]=0;
			dmg.m_max_damage[Damage::ICE]=0;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			dmg.m_special_flags |= Damage::EXTRA_UNDEAD_DMG;
			break;

		case Action::HOLY_FIRE:
			dmg.m_min_damage[Damage::PHYSICAL]=0;
			dmg.m_max_damage[Damage::PHYSICAL]=0;
			dmg.m_min_damage[Damage::FIRE] +=m_base_attr_mod.m_willpower/2;
			dmg.m_max_damage[Damage::FIRE] +=m_base_attr_mod.m_willpower/1.33;
			dmg.m_min_damage[Damage::FIRE] += std::min(m_base_attr_mod.m_willpower/6,m_base_attr_mod.m_magic_power*2);
			dmg.m_max_damage[Damage::FIRE] += std::min(m_base_attr_mod.m_willpower/4,m_base_attr_mod.m_magic_power*3);
			dmg.m_min_damage[Damage::ICE]=0;
			dmg.m_max_damage[Damage::ICE]=0;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			dmg.m_special_flags |= Damage::EXTRA_UNDEAD_DMG;
			break;

		case Action::LIGHT_BEAM:
		case Action::BURNING_SUN:
			dmg.init();
			dmg.m_attacker_fraction = m_fraction;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			dmg.m_status_mod_power[Damage::BLIND] = (short) (1.5*m_base_attr_mod.m_willpower);
			break;

		case Action::HYPNOSIS:
		case Action::HYPNOSIS2:
			dmg.init();
			dmg.m_attacker_fraction = m_fraction;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			dmg.m_status_mod_power[Damage::CONFUSED] = m_base_attr_mod.m_willpower;
			break;

		case Action::BREAK_BINDING:
			dmg.m_min_damage[Damage::ICE] =m_base_attr_mod.m_willpower/6;
			dmg.m_max_damage[Damage::ICE] =m_base_attr_mod.m_willpower/4;
			dmg.m_min_damage[Damage::PHYSICAL] += std::min(m_base_attr_mod.m_willpower/3,m_base_attr_mod.m_magic_power*2);
			dmg.m_max_damage[Damage::PHYSICAL] += std::min(m_base_attr_mod.m_willpower/2,m_base_attr_mod.m_magic_power*3);
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			dmg.m_special_flags |= Damage::EXTRA_SUMMONED_DMG;
			break;

		case Action::DISRUPT_BINDING:
			dmg.m_min_damage[Damage::ICE]=0;
			dmg.m_max_damage[Damage::ICE]=0;
			dmg.m_min_damage[Damage::ICE] +=m_base_attr_mod.m_willpower/3;
			dmg.m_max_damage[Damage::ICE] +=m_base_attr_mod.m_willpower/2;
			dmg.m_min_damage[Damage::PHYSICAL] += std::min(m_base_attr_mod.m_willpower/2,m_base_attr_mod.m_magic_power*2);
			dmg.m_max_damage[Damage::PHYSICAL] += std::min(m_base_attr_mod.m_willpower/1.33,m_base_attr_mod.m_magic_power*3.0);
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			dmg.m_special_flags |= Damage::EXTRA_SUMMONED_DMG;
			break;

		case Action::ACID:
			dmg.init();
			dmg.m_attacker_fraction = m_fraction;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			dmg.m_status_mod_power[Damage::POISONED] = 2*m_base_attr_mod.m_willpower;
			break;

		case Action::DIVINE_WIND:
		case Action::DIVINE_STORM:
			dmg.m_min_damage[Damage::AIR] =m_base_attr_mod.m_willpower/2;
			dmg.m_max_damage[Damage::AIR] =m_base_attr_mod.m_willpower/1;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			break;

        default:
            break;
	}

	// Faehigkeit brennende Wut
	if (getBaseAttrMod()->m_special_flags & BURNING_RAGE)
	{
		// 50% mehr physischen Schaden
		dmg.m_multiplier[Damage::PHYSICAL] *= 1.5;
	}

	// temporaere Effekte, Multiplikatoren umsetzen

	// Feuerschwert
	if (m_base_attr_mod.m_special_flags & FIRESWORD)
	{
		dmg.m_multiplier[Damage::FIRE] *= dmg.m_multiplier[Damage::PHYSICAL];
		dmg.m_multiplier[Damage::PHYSICAL]=1;
	}

	// Eispfeile
	if (m_base_attr_mod.m_special_flags & ICE_ARROWS)
	{
		dmg.m_multiplier[Damage::ICE] *= dmg.m_multiplier[Damage::PHYSICAL];
	}

	// Windpfeile
	if (m_base_attr_mod.m_special_flags & WIND_ARROWS)
	{
		dmg.m_multiplier[Damage::AIR] *= dmg.m_multiplier[Damage::PHYSICAL];
	}



}


void Creature::recalcDamage()
{

}

void Creature::calcBaseAttrMod()
{

	int i;
	float hppercent = 1;
	if (m_base_attr_mod.m_max_health != 0)
	{
		hppercent = m_dyn_attr.m_health / m_base_attr_mod.m_max_health;
	}
	// Alle Werte auf die Basiswerte setzen
	m_base_attr_mod.m_armor = m_base_attr.m_armor;
	m_base_attr_mod.m_block =m_base_attr.m_block;
	m_base_attr_mod.m_attack =m_base_attr.m_attack;
	m_base_attr_mod.m_strength =m_base_attr.m_strength;
	m_base_attr_mod.m_dexterity =m_base_attr.m_dexterity;
	m_base_attr_mod.m_willpower =m_base_attr.m_willpower;
	m_base_attr_mod.m_magic_power =m_base_attr.m_magic_power;
	m_base_attr_mod.m_walk_speed =m_base_attr.m_walk_speed;
	m_base_attr_mod.m_attack_speed =m_base_attr.m_attack_speed;
	m_base_attr_mod.m_attack_range =m_base_attr.m_attack_range;
	m_base_attr_mod.m_level =m_base_attr.m_level;
	m_base_attr_mod.m_max_health =m_base_attr.m_max_health;

	m_dyn_attr.m_health = hppercent * m_base_attr.m_max_health;
	
	for (i=0;i<4;i++)
	{
		m_base_attr_mod.m_resistances[i] = m_base_attr.m_resistances[i];
		m_base_attr_mod.m_resistances_cap[i] = m_base_attr.m_resistances_cap[i];

	}

	for (i=0;i<6;i++)
	{
		m_base_attr_mod.m_abilities[i] = m_base_attr.m_abilities[i];
	}

	m_base_attr_mod.m_special_flags = m_base_attr.m_special_flags;

	// Alle Modifikationen neu anwenden, aber nicht neu in die Liste aufnehmen
	std::list<CreatureBaseAttrMod>::iterator j;
	for (j=m_dyn_attr.m_temp_mods.begin(); j!= m_dyn_attr.m_temp_mods.end();++j)
	{
		DEBUG5("%f / %f",getDynAttr()->m_health, getBaseAttr()->m_max_health);
	
		applyBaseAttrMod(&(*j),false);
	}

	// Wirkungen durch passive Faehigkeiten
	if (checkAbility(Action::RESIST_ICE))
		m_base_attr_mod.m_resistances[Damage::ICE] += 30;

	if (checkAbility(Action::RESIST_AIR))
		m_base_attr_mod.m_resistances[Damage::AIR] += 30;


}

bool Creature::reactOnUse( int id)
{
    return true;
}

bool Creature::takeDamage(Damage* d)
{
	// Lebewesen kann nur im Zustand aktiv Schaden nehmen
	// und wenn es nicht gerade in einen Dialog verwickelt ist
	if (getState() != STATE_ACTIVE || getDialogueId() != 0)
		return false;

	DEBUG5("take Damage %i",getId());
	// Testen ob der Verursacher des Schadens feindlich gesinnt ist
	if (World::getWorld()->getRelation(d->m_attacker_fraction,this) != HOSTILE)
	{
		// Verursacher ist nicht feindlich, kein Schaden
		DEBUG("not hostile, no dmg");
		DEBUG("fractions %i %i",d->m_attacker_fraction, this->getFraction());
		return false;
	}
	

	// Testen ob man selbst der Verursacher ist
	// (man kann sich selbst generell nicht schaden)
	if (d->m_attacker_id == getId())
	{
		return false;
	}


	float dmg=0,dmgt;
	short res;
	float rez = 1.0/RAND_MAX;
	// testen ob der Schaden geblockt wird
	if (!(d->m_special_flags & Damage::UNBLOCKABLE))
	{
		float block = m_base_attr_mod.m_block  ;


		// Chance zu blocken ist 1 - Attackewert / Blockwert
		if (d->m_attack>0 && block>0 && d->m_attack<block)
		{
			if (rand()*rez>d->m_attack/block)
			{
				DEBUG5("blocked");
				// Schaden abgewehrt
				return true;
			}
		}



	}

	m_dyn_attr.m_last_attacker_id = d->m_attacker_id;

	// Wirkungen durch passive Faehigkeiten
	if (checkAbility(Action::CONCENTRATION))
	{
		d->m_status_mod_power[Damage::CONFUSED]=0;
	}

	if (checkAbility(Action::MENTAL_WALL))
	{
		d->m_status_mod_power[Damage::BERSERK]=0;
	}

	// Testen auf kritische Treffer
	if (rand()*rez <d->m_crit_perc)
	{
		DEBUG5("critical");
		d->m_multiplier[Damage::PHYSICAL] *= 3;
	}


	// Berechnen des Schadens

	// physischer Schaden
	dmgt = d->m_min_damage[Damage::PHYSICAL] + rand()*rez *(d->m_max_damage[Damage::PHYSICAL] -d->m_min_damage[Damage::PHYSICAL]);
	dmgt *= d->m_multiplier[Damage::PHYSICAL];

	// Resistenz anwenden
	res = std::min (m_base_attr_mod.m_resistances_cap[Damage::PHYSICAL],m_base_attr_mod.m_resistances[Damage::PHYSICAL]);
	dmgt *= 0.01*(100-res);

	// Ruestung anwenden
	float armor = m_base_attr_mod.m_armor;

	// Faehigkeit Turm in der Schlacht
	if (checkAbility(Action::STEADFAST) && m_base_attr_mod.m_max_health *0.3 > m_dyn_attr.m_health)
		armor *=1.5;


	if (d->m_power>0 && armor>0 && armor>d->m_power && !(d->m_special_flags & Damage::IGNORE_ARMOR))
	{
		dmgt = dmgt*d->m_power/armor;
	}

	dmg += dmgt;

	DEBUG5("phys dmg %f",dmgt);


	// restliche 3 Schadensarten
	int i;
	for (i=Damage::AIR;i<=Damage::FIRE;i++)
	{
		dmgt = d->m_min_damage[i] + rand()*rez *(d->m_max_damage[i] -d->m_min_damage[i]);
		dmgt *= d->m_multiplier[i];

		// Resistenz anwenden
		res = std::min(m_base_attr_mod.m_resistances_cap[i],m_base_attr_mod.m_resistances[i]);
		dmgt *=0.01*(100-res);

		DEBUG5("dmg %i min %f max %f real %f",i,d->m_min_damage[i],d->m_max_damage[i],dmgt);
		dmg += dmgt;

	}

	float t;
	// Anwenden der Statusveraenderungen
	for (i=0;i<NR_STATUS_MODS;i++)
	{
		// Anwenden wenn nicht immun, nicht temporaer immun und Staerke der Modifikation groesser als die eigene Willenskraft
		if (d->m_status_mod_power[i]>0 && !(m_base_attr_mod.m_immunity & (1 << i)) && m_dyn_attr.m_status_mod_immune_time[i]==0)
		{
			DEBUG5("mod %i modpow %i wp %i",i,d->m_status_mod_power[i],m_base_attr_mod.m_willpower);
			if (d->m_status_mod_power[i]>m_base_attr_mod.m_willpower)
			{
				// Modifikation anwenden
				t = (d->m_status_mod_power[i]-m_base_attr_mod.m_willpower)*1.0 / m_base_attr_mod.m_willpower;
				t *= 3000;
				if (t>m_dyn_attr.m_status_mod_time[i])
				{
					m_dyn_attr.m_status_mod_time[i] =t;
					m_event_mask |= NetEvent::DATA_STATUS_MODS;
				}

				DEBUG5("applying status mod %i for %f ms",i,t);
			}
		}
	}

	DEBUG5("sum dmg %f / %f",dmg,getDynAttr()->m_health );

	// Lebenspunkte abziehen
	getDynAttr()->m_health -= dmg;

	if (dmg>0)
	{
		m_dyn_attr.m_effect_time[CreatureDynAttr::BLEEDING] = std::max(m_dyn_attr.m_effect_time[CreatureDynAttr::BLEEDING],150.0f);
		m_event_mask |= NetEvent::DATA_HP | NetEvent::DATA_EFFECTS;
	}

	// Statikschild wenn mehr als 2% der Lebenspunkte verloren
	if ((m_base_attr_mod.m_special_flags & STATIC_SHIELD) && dmg > m_base_attr_mod.m_max_health * 0.02)
	{
		// Schaden festlegen
		Damage dmg;
		dmg.m_status_mod_power[Damage::PARALYZED] = m_base_attr_mod.m_magic_power;
		dmg.m_min_damage[Damage::AIR] = m_base_attr_mod.m_magic_power*0.2;
		dmg.m_max_damage[Damage::AIR] = m_base_attr_mod.m_magic_power*0.3;
		dmg.m_multiplier[Damage::AIR]=1;
		dmg.m_attacker_fraction = m_fraction;
		
		// Projektil Statikschild erzeugen
		Projectile* pr = new Projectile(Projectile::STATIC_SHIELD,&dmg, World::getWorld()->getValidProjectileId());

		pr->getShape()->m_radius =getShape()->m_radius+1;
		getRegion()->insertProjectile(pr,getShape()->m_center);
	}
	
	// Trigger erzeugen
	Trigger* tr = new Trigger("unit_hit");
	tr->addVariable("defender",getId());
	tr->addVariable("attacker",d->m_attacker_id);
	tr->addVariable("damage",dmg);
	getRegion()->insertTrigger(tr);
	
	return true;
}

void Creature::applyDynAttrMod(CreatureDynAttrMod* mod)
{
	m_dyn_attr.m_health += mod->m_dhealth;

	if (m_dyn_attr.m_health>m_base_attr_mod.m_max_health)
	{
		m_dyn_attr.m_health=m_base_attr_mod.m_max_health;
	}

	if (mod->m_dhealth !=0)
	{
		m_event_mask |= NetEvent::DATA_HP;
	}


	for (int i = 0;i< NR_STATUS_MODS;i++)
	{
		if (mod->m_dstatus_mod_immune_time[i]>0)
		{
			m_dyn_attr.m_status_mod_immune_time[i] = std::max(m_dyn_attr.m_status_mod_immune_time[i],mod->m_dstatus_mod_immune_time[i]);
			m_dyn_attr.m_status_mod_time[i]=0;
			m_event_mask |= NetEvent::DATA_STATUS_MODS;
		}
	}
}

void Creature::applyBaseAttrMod(CreatureBaseAttrMod* mod, bool add)
{
	int i;
	// Deltawerte dazu addieren
	float oldmaxhp = m_base_attr_mod.m_max_health;

	m_base_attr_mod.m_armor +=mod->m_darmor;
	m_base_attr_mod.m_block +=mod->m_dblock;
	m_base_attr_mod.m_strength +=mod->m_dstrength;
	m_base_attr_mod.m_dexterity +=mod->m_ddexterity;
	m_base_attr_mod.m_willpower +=mod->m_dwillpower;
	m_base_attr_mod.m_magic_power +=mod->m_dmagic_power;
	m_base_attr_mod.m_walk_speed +=mod->m_dwalk_speed;
	m_base_attr_mod.m_max_health += mod->m_dmax_health;
	m_base_attr_mod.m_max_health += mod->m_dstrength*5;
	m_base_attr_mod.m_attack_speed += mod->m_ddexterity*20;
	m_dyn_attr.m_health *= m_base_attr_mod.m_max_health /oldmaxhp;


	m_base_attr_mod.m_attack_speed +=mod->m_dattack_speed;

	// Modifikationen feststellen
	if (mod->m_dwalk_speed!=0 )
	{
		m_event_mask |= NetEvent::DATA_WALK_SPEED;
	}
	if (mod->m_dattack_speed !=0 || mod->m_ddexterity!=0)
	{
		m_event_mask |= NetEvent::DATA_ATTACK_SPEED;
	}
	if (mod->m_dmax_health !=0 || mod->m_dstrength!=0)
	{
		m_event_mask |= NetEvent::DATA_MAX_HP | NetEvent::DATA_HP;
	}

	// einige Untergrenzen pruefen
	m_base_attr_mod.m_strength = std::max(m_base_attr_mod.m_strength,(short) 1);
	m_base_attr_mod.m_dexterity = std::max(m_base_attr_mod.m_dexterity,(short) 1);
	m_base_attr_mod.m_willpower = std::max(m_base_attr_mod.m_willpower,(short) 1);
	m_base_attr_mod.m_magic_power = std::max(m_base_attr_mod.m_magic_power,(short) 1);
	m_base_attr_mod.m_walk_speed = std::max(m_base_attr_mod.m_walk_speed,(short) 200);
	m_base_attr_mod.m_attack_speed = std::max(m_base_attr_mod.m_attack_speed,(short) 200);

	// Resiszenzen dazu addieren
	for (i=0;i<4;i++)
	{
		m_base_attr_mod.m_resistances[i] += mod->m_dresistances[i];
		m_base_attr_mod.m_resistances_cap[i] += mod->m_dresistances_cap[i];

	}

	// Faehigkeiten mit OR hinzufuegen
	for (i=0;i<6;i++)
	{
		m_base_attr_mod.m_abilities[i] |= mod->m_xabilities[i];
		if (mod->m_xabilities[i]!=0)
		{
			m_event_mask |= NetEvent::DATA_ABILITIES;
		}
	}

	// Flags mit OR hinzufuegen
	m_base_attr_mod.m_special_flags |= mod->m_xspecial_flags;
	m_base_attr_mod.m_immunity |= mod->m_ximmunity;

	if (mod->m_xspecial_flags!=0)
	{
		m_event_mask |= NetEvent::DATA_FLAGS;
	}

	// Wenn add == true in die Liste der wirksamen Modifikationen aufnehmen
	if (mod->m_time!=0 && add)
	{
		m_dyn_attr.m_temp_mods.push_back(*mod);

		// Schaden neu berechnen
		recalcDamage();
	}

}

bool Creature::removeBaseAttrMod(CreatureBaseAttrMod* mod)
{
	int i;
	bool ret = false;
	float oldmaxhp = m_base_attr_mod.m_max_health;

	// Deltas abziehen
	m_base_attr_mod.m_armor -=mod->m_darmor;
	m_base_attr_mod.m_block -=mod->m_dblock;
	m_base_attr_mod.m_strength -=mod->m_dstrength;
	m_base_attr_mod.m_dexterity -=mod->m_ddexterity;
	m_base_attr_mod.m_willpower -=mod->m_dwillpower;
	m_base_attr_mod.m_magic_power -=mod->m_dmagic_power;
	m_base_attr_mod.m_walk_speed -=mod->m_dwalk_speed;
	m_base_attr_mod.m_attack_speed -=mod->m_dattack_speed;
	m_base_attr_mod.m_max_health -= mod->m_dstrength*5;
	m_base_attr_mod.m_attack_speed -= mod->m_ddexterity*20;

	m_dyn_attr.m_health *= m_base_attr_mod.m_max_health /oldmaxhp;


	// Modifikationen feststellen
	if (mod->m_dwalk_speed!=0)
	{
		m_event_mask |= NetEvent::DATA_WALK_SPEED;
	}
	if (mod->m_dattack_speed !=0 || mod->m_ddexterity!=0)
	{
		m_event_mask |= NetEvent::DATA_ATTACK_SPEED;
	}
	if (mod->m_dmax_health !=0 || mod->m_dstrength!=0)
	{
		m_event_mask |= NetEvent::DATA_MAX_HP | NetEvent::DATA_HP;
	}

	for (i=0;i<4;i++)
	{
		m_base_attr_mod.m_resistances[i] -= mod->m_dresistances[i];
		m_base_attr_mod.m_resistances_cap[i] -= mod->m_dresistances_cap[i];

	}

	// Wenn Faehigkeit veraendert wurde neu berechnen
	for (i=0;i<6;i++)
	{
		if ( mod->m_xabilities[i]!=0)
		{
			ret = true;
			m_event_mask |= NetEvent::DATA_ABILITIES;
		}
	}

	// Wenn Flags veraendert wurden neu berechnen
	if (mod->m_xspecial_flags!=0)
	{
		m_event_mask |= NetEvent::DATA_FLAGS;
		ret = true;
	}

	if( mod->m_ximmunity!=0)
		ret = true;

	// Schaden neu berechnen
	recalcDamage();

	return ret;
}

void Creature::getPathDirection(Vector pos,short region, float base_size, short layer, Vector& dir)
{
	// maximale Entfernung bei der normale Wegfindung genommen wird
	int pathmaxdist = 16;
	
	PathfindInfo** pi= &m_small_path_info;
	int bsize =1;
	// true wenn wegsuchendes Objekt fliegt
	bool fly=false;

	// ermitteln welche Wegfindeinformation genutzt werden soll
	if (base_size>1)
	{
		DEBUG5("switching to medium size info");
		pi = &m_medium_path_info;
		bsize=2;
	}
	if (base_size>2)
	{
		DEBUG5("switching to big size info");
		pi = &m_big_path_info;
		bsize=4;
	}

	if ((layer & LAYER_AIR) ==0)
	{
		DEBUG5("switching to flying info");
		pi = &m_small_flying_path_info;
		bsize=1;
		fly = true;
	}

	// true, wenn einfach direkter Weg zum Ziel gewaehlt werden soll
	bool direct = false;

	// true, wenn Wegfindeinformation neu berechnet werden muss
	bool recalc = false;

	// Position des Zielobjekts
	Vector goal = getShape()->m_center;


	if (*pi == 0)
	{
		// Wegfindeinfo existiert noch nicht
		*pi = new PathfindInfo;
		// Qualitaet der Suche
		int qual=4;
		int dim = 2*pathmaxdist * qual / bsize +1;

		// Potenzialmatrix
		(*pi)->m_pot = new Matrix2d<float>(dim,dim);
		// Blockmatrix
		(*pi)->m_block = new Matrix2d<char>(dim,dim);

		(*pi)->m_dim = dim;

		// Ebene ist Base und Air fuer normale, nur AIR fuer fliegende Lebewesen
		(*pi)->m_layer= LAYER_BASE | LAYER_AIR;
		if (fly)
		{
			(*pi)->m_layer= LAYER_AIR;
		}
		(*pi)->m_region=getGridLocation()->m_region;
		(*pi)->m_base_size = bsize;
		(*pi)->m_quality=qual;
		(*pi)->m_id = getId();
		recalc = true;
		DEBUG5("recalc: no pathinfo so far");
	}
	else
	{
		if ( (*pi)->m_region != getGridLocation()->m_region)
		{
			// Wegfindeinformation gilt fuer eine andere Region als die, in der sich das Lebewesen befindet
			// Region neu setzen und neu berechnen
			(*pi)->m_region=getGridLocation()->m_region;
			recalc = true;
			DEBUG5("recalc: new in Region");
		}

		// Ziel befindet sich in einer anderen Region, keinen Weg angeben
		if ( region != getGridLocation()->m_region)
		{
			dir = Vector(0,0);
			return;
		}

		// Abstand aktuelle Position zu Position fuer die die Wegfindeinformation erstellt wurde
		float d = (*pi)->m_start.distanceTo(goal);
		
		// Abstand des wegsuchenden Objektes zur einen Position
		float d2 = (*pi)->m_start.distanceTo(pos);
		
		if (fabs(goal.m_x - pos.m_x)>pathmaxdist || fabs(goal.m_y - pos.m_y)>pathmaxdist)
		{
			// Direkte Wegsuche wenn das Ziel in einer Richtung mehr als 10 entfernt ist
			direct = true;
		}
		else if (d>(*pi)->m_base_size && d > 0.25*d2)
		{
			// Ziel hat sich deutlich bewegt, neuen Weg suchen
			DEBUG5("recalc: goal has moved");
			recalc = true;
		}

		// Wenn Informationen ueber 500ms alt neu berechnen
		if ((*pi)->m_timer >500)
		{
			DEBUG5("recalc: info too old");
			recalc = true;
		}

	}

	if (recalc && !direct)
	{
		// neu berechnen
		DEBUG5("recalc");
		// Zentrum und Senke sind die aktuelle Position
		(*pi)->m_start= goal;
		(*pi)->m_center= goal;
		(*pi)->m_timer =0;

		// Blockmatrix berechnen
		bool suc;
		suc = World::getWorld()->calcBlockmat(*pi);
		if (suc!=true)
			direct = true;
		else
		{
			// Potentialfeld berechnen
			suc = World::getWorld()->calcPotential(*pi);
			if (suc!=true)
				direct = true;
		}
	}

	if (direct)
	{
		DEBUG5("direct way");
		// Direkte Richtung
		// Vektor vom Start zum Ziel, normiert
		dir = goal - pos;
		dir.normalize();

	}
	else
	{
		DEBUG5("use pot field");
		// Potentialfeld verwenden um die Richtung zu berechnen
		World::getWorld()->calcPathDirection(*pi, pos, dir);
	}
}

void Creature::toString(CharConv* cv)
{
	DEBUG5("Creature::tostring");
	WorldObject::toString(cv);
	cv->toBuffer((short) m_category);
	cv->toBuffer((char) m_fraction);

	m_action.toString(cv);
	m_command.toString(cv);
	m_next_command.toString(cv);

	cv->toBuffer(m_dyn_attr.m_health);
	cv->toBuffer(m_base_attr_mod.m_max_health);

	DEBUG5("write offset: %i",cv->getBitStream()->GetNumberOfBitsUsed());

	
	
	// Statusveraenderungen
	char c=0;
	for (int i=0;i<NR_STATUS_MODS;i++)
	{
		if (m_dyn_attr.m_status_mod_time[i]>0)
		{
			c |= (1 <<i );
		}
	}
	cv->toBuffer(c);
	for (int i=0;i<NR_STATUS_MODS;i++)
	{
		if (m_dyn_attr.m_status_mod_time[i]>0)
		{
			cv->toBuffer(m_dyn_attr.m_status_mod_time[i]);
		}
	}
	
	// Effekte
	for (int i=0;i<NR_EFFECTS;i++)
	{
		cv->toBuffer(m_dyn_attr.m_effect_time[i]);
	}

	cv->toBuffer(getBaseAttrMod()->m_special_flags);

	cv->toBuffer(getBaseAttr()->m_step_length);
	cv->toBuffer(getBaseAttrMod()->m_attack_speed);
	cv->toBuffer(getBaseAttrMod()->m_walk_speed);
	cv->toBuffer(m_timer1);
	cv->toBuffer(m_timer1_max);
	cv->toBuffer(m_timer2);
	cv->toBuffer(m_timer2_max);
	for (int i=0; i<6; i++)
	{
		cv->toBuffer(getBaseAttrMod()->m_abilities[i]);
	}

	cv->toBuffer(m_speed.m_x);
	cv->toBuffer(m_speed.m_y);


}

void Creature::fromString(CharConv* cv)
{
	WorldObject::fromString(cv);
	char ctmp;
	short stmp;
	cv->fromBuffer<short>(stmp);
	m_category = (Category) stmp;

	cv->fromBuffer<char>(ctmp);
	m_fraction = (Fraction) ctmp;

	m_action.fromString(cv);
	m_command.fromString(cv);
	m_next_command.fromString(cv);

	cv->fromBuffer(m_dyn_attr.m_health);
	cv->fromBuffer(m_base_attr_mod.m_max_health);
	DEBUG5("read offset: %i",cv->getBitStream()->GetReadOffset());

	// Statusveraenderungen
	char c=0;
	cv->fromBuffer<char>(c);
	
	for (int i=0;i<NR_STATUS_MODS;i++)
	{
		if (c & (1 <<i ))
		{
			cv->fromBuffer<float>(m_dyn_attr.m_status_mod_time[i]);
		}
	}


	for (int i=0;i<NR_EFFECTS;i++)
	{
		cv->fromBuffer<float>(m_dyn_attr.m_effect_time[i]);
	}

	cv->fromBuffer(getBaseAttrMod()->m_special_flags);

	cv->fromBuffer(getBaseAttr()->m_step_length);
	cv->fromBuffer(getBaseAttrMod()->m_attack_speed);
	cv->fromBuffer(getBaseAttrMod()->m_walk_speed);
	cv->fromBuffer(m_timer1);
	cv->fromBuffer(m_timer1_max);
	cv->fromBuffer(m_timer2);
	cv->fromBuffer(m_timer2_max);
	for (int i=0; i<6; i++)
	{
		cv->fromBuffer(getBaseAttrMod()->m_abilities[i]);
	}

	cv->fromBuffer(m_speed.m_x);
	cv->fromBuffer(m_speed.m_y);
}

bool Creature::checkAbility(Action::ActionType at)
{
	if (at<0 || at >=192)
		return false;

	int nr = at / 32;
	int mask = 1 << (at % 32);
	DEBUG5("test: %i %i %i",at,nr,mask);

	if ((m_base_attr_mod.m_abilities[nr] & mask) !=0)
	{
		DEBUG5("funktioniert %x %x %x",m_base_attr_mod.m_abilities[nr],mask,m_base_attr_mod.m_abilities[nr] & mask);
		return true;
	}
	return false;
}


bool Creature::checkAbilityLearnable(Action::ActionType at)
{
	if (checkAbility(at))
	{
		// Faehigkeit ist schon erlernt
		return false;
	}

	Action::ActionInfo* aci = Action::getActionInfo(at);

	if (aci->m_req_level > m_base_attr.m_level)
	{
		// Levelvorraussetzung nicht erfuellt
		return false;
	}


	for (int i=0;i<3;i++)
	{
		if (!checkAbility(aci->m_req_ability[i]))
		{
			// Faehigkeiten Vorraussetzung nicht erfuellt
			return false;
		}
	}
	return true;
}

float Creature::getTimerPercent(int timer)
{
	if (timer ==1)
	{
		if (m_timer1_max ==0)
			return 0;

		return m_timer1 / m_timer1_max;
	}

	if (timer ==2)
	{
		if (m_timer2_max ==0)
			return 0;

		return m_timer2 / m_timer2_max;
	}

	return 0;
}


void Creature::writeNetEvent(NetEvent* event, CharConv* cv)
{
	if (event->m_data & NetEvent::DATA_COMMAND)
	{
		m_command.toString(cv);
	}

	if (event->m_data & NetEvent::DATA_ACTION)
	{
		m_action.toString(cv);
		cv->toBuffer(getShape()->m_center.m_x);
		cv->toBuffer(getShape()->m_center.m_y);
		cv->toBuffer(getShape()->m_angle);

		if (m_action.m_type!=0)
		{
			float acttime = m_action.m_time - m_action.m_elapsed_time;
			Vector goal = getShape()->m_center + m_speed * acttime;
		}
	}

	if (event->m_data & NetEvent::DATA_HP)
	{
		cv->toBuffer(getDynAttr()->m_health);
	}

	if (event->m_data & NetEvent::DATA_MAX_HP)
	{
		cv->toBuffer(getBaseAttrMod()->m_max_health);
	}

	if (event->m_data & NetEvent::DATA_EFFECTS)
	{
		for (int i=0;i<NR_EFFECTS;i++)
		{
			cv->toBuffer(m_dyn_attr.m_effect_time[i]);
		}
	}

	if (event->m_data & NetEvent::DATA_STATUS_MODS)
	{
		char c=0;
		for (int i=0;i<NR_STATUS_MODS;i++)
		{
			if (m_dyn_attr.m_status_mod_time[i]>0)
			{
				c |= (1 <<i );
			}
		}
		cv->toBuffer(c);

		for (int i=0;i<NR_STATUS_MODS;i++)
		{
			if (m_dyn_attr.m_status_mod_time[i]>0)
			{
				cv->toBuffer(m_dyn_attr.m_status_mod_time[i]);
			}
		}
	}

	if (event->m_data & NetEvent::DATA_TIMER)
	{
		cv->toBuffer(m_timer1_max);
		cv->toBuffer(m_timer2_max);
	}

	if (event->m_data & NetEvent::DATA_STATE)
	{
		cv->toBuffer((char) getState());
	}

	if (event->m_data & NetEvent::DATA_WALK_SPEED)
	{
		cv->toBuffer(getBaseAttrMod()->m_walk_speed);
	}

	if (event->m_data & NetEvent::DATA_ATTACK_SPEED)
	{
		cv->toBuffer(getBaseAttrMod()->m_attack_speed);
	}

	if (event->m_data & NetEvent::DATA_NEXT_COMMAND)
	{
		m_next_command.toString(cv);
	}

	if (event->m_data & NetEvent::DATA_ABILITIES)
	{
		for (int i=0; i<6; i++)
		{
			cv->toBuffer(getBaseAttrMod()->m_abilities[i]);
		}
	}

	if (event->m_data & NetEvent::DATA_FLAGS )
	{
		cv->toBuffer(getBaseAttrMod()->m_special_flags);
	}

	if (event->m_data & NetEvent::DATA_EXPERIENCE)
	{
		cv->toBuffer(getDynAttr()->m_experience);
	}

	if (event->m_data & NetEvent::DATA_MOVE_INFO)
	{
		cv->toBuffer(m_speed.m_x);
		cv->toBuffer(m_speed.m_y);
	}

	if (event->m_data & NetEvent::DATA_LEVEL)
	{
		cv->toBuffer(getBaseAttr()->m_level);
	}
	
	if (event->m_data & NetEvent::DATA_SPEAK_TEXT)
	{
		cv->toBuffer(getSpeakText().m_text);
		cv->toBuffer(getSpeakText().m_time);
		
		cv->toBuffer<short>(getSpeakText().m_answers.size());
		
		std::list< std::pair<std::string, std::string> >::iterator it;
		for (it = getSpeakText().m_answers.begin(); it != getSpeakText().m_answers.end(); ++it)
		{
			cv->toBuffer(it->first);
			cv->toBuffer(it->second);	
		}
	}
	
	if (event->m_data & NetEvent::DATA_TRADE_INFO)
	{
		cv->toBuffer(m_trade_info.m_trade_partner);
		cv->toBuffer(m_trade_info.m_price_factor);
	}
	
	if (event->m_data & NetEvent::DATA_DIALOGUE)
	{
		cv->toBuffer(m_dialogue_id);
	}
}


void Creature::processNetEvent(NetEvent* event, CharConv* cv)
{
	DEBUG5("object %i processing NetEvent %i data %i",getId(),event->m_type, event->m_data);
	if (event->m_data & NetEvent::DATA_COMMAND)
	{
		m_command.fromString(cv);
		DEBUG5("got Command %i",m_command.m_type);
	}

	float  atime = m_action.m_time - m_action.m_elapsed_time;

	Vector newpos= getShape()->m_center;
	Vector newspeed;
	Vector goal = newpos;

	bool newact= false;
	bool newmove = false;
	float delay = cv->getDelay();
	float newangle=0;

	if (delay>1000)
	{
		DEBUG("got packet with delay %f %f",cv->getDelay(),delay);
	}


	if (event->m_data & NetEvent::DATA_ACTION)
	{


		m_action.fromString(cv);

		cv->fromBuffer(newpos.m_x);
		cv->fromBuffer(newpos.m_y);
		cv->fromBuffer(newangle);
		newact = true;
	}



	if (event->m_data & NetEvent::DATA_HP)
	{
		
		cv->fromBuffer(getDynAttr()->m_health);
	}

	if (event->m_data & NetEvent::DATA_MAX_HP)
	{
		cv->fromBuffer(getBaseAttrMod()->m_max_health);
	}

	if (event->m_data & NetEvent::DATA_EFFECTS)
	{
		for (int i=0;i<NR_EFFECTS;i++)
		{
			cv->fromBuffer(m_dyn_attr.m_effect_time[i]);
		}
	}

	if (event->m_data & NetEvent::DATA_STATUS_MODS)
	{
		char ctmp;
		cv->fromBuffer<char>(ctmp);
		for (int i=0;i<NR_STATUS_MODS;i++)
		{
			if (ctmp & (1 <<i ))
			{
				cv->fromBuffer<float>(m_dyn_attr.m_status_mod_time[i]);
			}
		}
	}

	if (event->m_data & NetEvent::DATA_TIMER)
	{
		cv->fromBuffer(m_timer1_max);
		cv->fromBuffer(m_timer2_max);
		m_timer1 = m_timer1_max;
		m_timer2 = m_timer2_max;

	}

	if (event->m_data & NetEvent::DATA_STATE)
	{
		State oldstate = getState();
		char ctmp;
		cv->fromBuffer(ctmp);

		if (oldstate != STATE_DIEING && oldstate != STATE_DEAD && (ctmp==STATE_DIEING || ctmp == STATE_DEAD))
		{
			die();
		}
		setState((State) ctmp);
		DEBUG5("object %i changed state from %i to %i",getId(),oldstate,ctmp);


	}

	if (event->m_data & NetEvent::DATA_WALK_SPEED)
	{
		cv->fromBuffer(getBaseAttrMod()->m_walk_speed);
	}

	if (event->m_data & NetEvent::DATA_ATTACK_SPEED)
	{
		cv->fromBuffer(getBaseAttrMod()->m_attack_speed);
	}

	if (event->m_data & NetEvent::DATA_NEXT_COMMAND)
	{
		m_next_command.fromString(cv);
	}

	if (event->m_data & NetEvent::DATA_ABILITIES)
	{
		for (int i=0; i<6; i++)
		{
			cv->fromBuffer(getBaseAttrMod()->m_abilities[i]);
		}
	}

	if (event->m_data & NetEvent::DATA_FLAGS )
	{
		cv->fromBuffer(getBaseAttrMod()->m_special_flags);
	}

	if (event->m_data & NetEvent::DATA_EXPERIENCE)
	{
		cv->fromBuffer(getDynAttr()->m_experience);
	}

	if (event->m_data & NetEvent::DATA_MOVE_INFO)
	{
		cv->fromBuffer(newspeed.m_x);
		cv->fromBuffer(newspeed.m_y);
		newmove = true;

	}

	if (event->m_data & NetEvent::DATA_LEVEL)
	{
		cv->fromBuffer(getBaseAttr()->m_level);
	}
	
	if (event->m_data & NetEvent::DATA_SPEAK_TEXT)
	{
		getSpeakText().clear();
		cv->fromBuffer(getSpeakText().m_text);
		cv->fromBuffer(getSpeakText().m_time);
		
		short n;
		cv->fromBuffer<short>(n);
		
		std::string text;
		std::string topic;
		for (int i=0; i<n; i++)
		{
			cv->fromBuffer(text);
			cv->fromBuffer(topic);
			getSpeakText().m_answers.push_back(std::make_pair(text,topic));
		}
		
		DEBUG5("speak %s for %f ms",getSpeakText().m_text.c_str(), getSpeakText().m_time);
		std::list< std::pair<std::string, std::string> >::iterator it;
		for (it = getSpeakText().m_answers.begin(); it != getSpeakText().m_answers.end(); ++it)
		{
			DEBUG5("answer %s %s",it->first.c_str(), it->second.c_str());
		}
	}
	
	if (event->m_data & NetEvent::DATA_TRADE_INFO)
	{
		cv->fromBuffer(m_trade_info.m_trade_partner);
		cv->fromBuffer(m_trade_info.m_price_factor);
	}
	
	if (event->m_data & NetEvent::DATA_DIALOGUE)
	{
		int id;
		cv->fromBuffer(id);
		setDialogue(id);
	}

	if (newmove)
	{
		// Zielort der Aktion berechnen
		float acttime = m_action.m_time - m_action.m_elapsed_time;
		goal = newpos + newspeed * acttime;

		/*
		timeval tv;
		gettimeofday(&tv, NULL);
		DEBUG("time elapsed ingame %f delay %i system time %i",etime,cv->getDelay(),tv.tv_usec/1000);
		*/
		// Zeit die zum erreichen des Zieles uebrig ist
		float goaltime = acttime;
		if (event->m_data & NetEvent::DATA_ACTION)
		{
			goaltime -= delay;
		}
		if (goaltime < 100)
		{

			DEBUG5("time to reach goal %f (prev time %f) delay %f",goaltime,atime,cv->getDelay());

		}
		DEBUG5("previously planned time %f",atime);

		if (goaltime <0)
		{
			// wenn man schon lange da sein muesste, Objekt an den Zielort versetzen
			moveTo(goal);
		}
		else
		{
			DEBUG5("pos %f %f speed %f %f", getShape()->m_center.m_x, getShape()->m_center.m_y, m_speed.m_x, m_speed.m_y); 
			// Bewegungsgeschwindigkeit so setzen, dass Ziel in der richtigen Zeit erreicht wird
			m_speed = (goal - getShape()->m_center) * (1/goaltime);
			DEBUG5("goal %f %f newspeed %f %f", goal.m_x, goal.m_y, m_speed.m_x, m_speed.m_y); 
		}

	}

	if (newact)
	{

		m_action.m_elapsed_time += delay;


		if (m_action.m_elapsed_time> m_action.m_time)
		{
			// Aktion sollte schon beenden sein
			if (!newmove)
			{
				moveTo(newpos);
				getShape()->m_angle = newangle;
			}

			m_action.m_type = Action::NOACTION;
			m_action.m_elapsed_time =0;

		}
		else
		{
			// Drehwinkel korrekt setzen
			if (!newmove && m_action.m_type != Action::NOACTION)
			{
				// Wenn die Aktion nicht laufen ist, Spieler an die richtige Position versetzen
				moveTo(newpos);
				getShape()->m_angle = newangle;
			}

			if (Action::getActionInfo(m_action.m_type)->m_distance != Action::SELF)
			{

				getShape()->m_angle = (m_action.m_goal - newpos).angle();

			}
			if (Action::getActionInfo(m_action.m_type)->m_base_action == Action::WALK)
			{
				getShape()->m_angle = m_speed.angle();

			}
		}
	}
}

int Creature::getValue(std::string valname)
{
	int ret = m_base_attr.getValue(valname);
	if (ret >0)
	{
		return ret;
	}
	
	ret = m_dyn_attr.getValue(valname);
	if (ret >0)
	{
		return ret;
	}
	
	ret = WorldObject::getValue(valname);
	return ret;
	
}

bool Creature::setValue(std::string valname)
{
	bool ret;
	ret = m_base_attr.setValue(valname, m_event_mask);
	if (ret >0)
	{
		calcBaseAttrMod();
		return ret;
	}
	
	ret = m_dyn_attr.setValue(valname, m_event_mask);
	if (ret >0)
	{
		return ret;
	}
	
	ret = WorldObject::getValue(valname);
	return ret;
}

void Creature::clearSpeakText()
{
	m_event_mask |= NetEvent::DATA_SPEAK_TEXT;
	m_speak_text.clear();
}

void Creature::speakText(CreatureSpeakText& text)
{
	m_event_mask |= NetEvent::DATA_SPEAK_TEXT;
	
	m_speak_text = text;
	DEBUG5("speak %s for %f ms",text.m_text.c_str(), text.m_time);
	std::list< std::pair<std::string, std::string> >::iterator it;
	for (it = text.m_answers.begin(); it != text.m_answers.end(); ++it)
	{
		DEBUG5("answer %s %s",it->first.c_str(), it->second.c_str());
	}
}

Creature* Creature::getTradePartner()
{
	if (m_trade_info.m_trade_partner == 0)
		return 0;
	
	if (getRegion() ==0 )
		return 0;
	
	return dynamic_cast<Creature*>(getRegion()->getObject(m_trade_info.m_trade_partner));
}

void Creature::sellItem(short position, Item* &item, int& gold)
{
	if (getEquipement() !=0)
	{
		// Item das verkauft wird
		Item* it = 0;
		getEquipement()->swapItem(it,position);
		
		if (it != 0 && gold >= it->m_price)
		{
			item = it;
			
			// Geld abziehen
			gold -= item->m_price;
			item->m_price = (int) ((item->m_price+0.999f) / m_trade_info.m_price_factor );
			
			NetEvent event;
			event.m_type =  NetEvent::TRADER_ITEM_SELL;
			event.m_data = position;
			event.m_id = getId();

			getRegion()->insertNetEvent(event);
			
			m_trade_info.m_last_sold_item = item;
		}
		else
		{
			getEquipement()->swapItem(it,position);
		}	
	}
}

void Creature::buyItem(Item* &item, int& gold)
{
	DEBUG5("buy %p",item);
	if (item != 0 && getEquipement() !=0)
	{
		// Geld auszahlen
		// zuletzt gekauftes Item kann zum vollen Preis zurueck gegebebn werden
		if (item == m_trade_info.m_last_sold_item)
		{
			gold +=(int) (item->m_price * m_trade_info.m_price_factor);
		}
		else
		{
			gold += item->m_price;
		}
		item->m_price = (int) (item->m_price * m_trade_info.m_price_factor);
		
		// beim Haendler einfuegen
		short pos = getEquipement()->insertItem(item);
		if (pos == Equipement::NONE)
		{
			delete item;
		}
		else
		{
			NetEvent event;
			event.m_type =  NetEvent::TRADER_ITEM_BUY;
			event.m_data = pos;
			event.m_id = getId();

			getRegion()->insertNetEvent(event);
		}
		item =0;
	}
}


void Creature::setDialogue(int id)
{
	m_dialogue_id = id;
	m_event_mask |= NetEvent::DATA_DIALOGUE;
	
	if (id !=0)
	{
		getAction()->m_type = Action::NOACTION;
		m_action.m_elapsed_time =0;
		clearCommand(true);
	}
}


