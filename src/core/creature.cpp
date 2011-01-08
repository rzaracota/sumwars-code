/*
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
#include "eventsystem.h"
#include "math.h"
#include "itemfactory.h"
#include "mathhelper.h"

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
	 DEBUGX("Creature::init");

	bool tmp=true;
	// eigene Initialisierung

	// keine Aktion/Kommando
	m_action.m_type = "noaction";

	m_command.m_type = "noaction";
	m_command.m_damage_mult=1;
	m_next_command.m_type = "noaction";

	m_trade_id=0;
	m_speak_id =0;
	m_dialogue_id =0;
	m_equipement =0;

	m_interaction_flags = COLLISION_DETECTION | USABLE;

	// Bewegung auf 0 setzen
	setSpeed(Vector(0,0));

	// Wegfindeinformationen auf 0 setzen
	m_small_path_info=0;
	m_small_flying_path_info=0;
	m_medium_path_info=0;
	m_big_path_info=0;
	m_path_info=0;
	m_pathfind_counter =0;

	// Timer nullen
	for (int i=0; i<NR_TIMERS; i++)
	{
		m_timers[i] = 0;
		m_timers_max[i] = 0;
	}

	m_dyn_attr.m_last_attacker_id=0;
	m_dyn_attr.m_experience=0;

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

	setState(STATE_ACTIVE,false);
	m_script_command_timer =0;
	clearCommand(true,true);

	m_action.m_action_equip = Action::NO_WEAPON;
	m_action.m_elapsed_time = 0.0;
	m_action.m_time = 2000.0;

	m_dyn_attr.m_health = getBaseAttr()->m_max_health;
	getBaseAttrMod()->m_max_health = getBaseAttr()->m_max_health;
	getBaseAttrMod()->m_power =1;
	m_base_attr.m_max_experience =1;

	clearNetEventMask();

	m_refname = "";
	getTradeInfo().m_trade_partner =0;
	getTradeInfo().m_last_sold_item=0;

	m_emotion_set="";

	return tmp;
}

bool Creature::destroy()
{
	DEBUGX("destroy");

	WorldObject::destroy();
    return true;
}


void Creature::die()
{
	clearFlags();
	removeAllBaseAttrMod();

	// eigenen Status auf sterbend STATE_DIEING setzen
	getRegion()->changeObjectGroup(this,DEAD);
	setState(STATE_DIEING);
	m_action.m_type ="die";
	DEBUGX("object died: %p",this);
	m_action.m_time =getActionTime("die");

	m_action.m_elapsed_time =0;

	addToNetEventMask(NetEvent::DATA_ACTION | NetEvent::DATA_STATUS_MODS | NetEvent::DATA_FLAGS);

	Trigger* tr = new Trigger("unit_die");
	tr->addVariable("unit",getId());
	getRegion()->insertTrigger(tr);
}

bool Creature::canBeAttacked()
{
	return (getState() == STATE_ACTIVE && getDialogueId() == 0);
}

int Creature::getTimerNr(Action::ActionType action)
{
	std::map<std::string, AbilityInfo>::iterator it;
	it = getBaseAttr()->m_abilities.find(action);

	if (it != getBaseAttr()->m_abilities.end())
	{
		// wenn gleich -1, so wird der Standardtimer verwendet
		if (it->second.m_timer_nr != -1)
		{
			return it->second.m_timer_nr;
		}

		Action::ActionInfo* aci = Action::getActionInfo(action);
		if (aci ==0)
		{
			return 0;
		}

		return aci->m_timer_nr;
	}
	return 0;
}

float Creature::getTimer(Action::ActionType action)
{
	std::map<std::string, AbilityInfo>::iterator it;
	it = getBaseAttr()->m_abilities.find(action);

	if (it != getBaseAttr()->m_abilities.end())
	{
		// wenn gleich -1, so wird der Standardtimer verwendet
		if (it->second.m_timer_nr != -1 && it->second.m_timer != -1)
			return it->second.m_timer;

		Action::ActionInfo* aci = Action::getActionInfo(action);
		if (aci ==0)
			return 0;

		return aci->m_timer;
	}
	return 0;
}

float Creature::getActionTime(Action::ActionType action)
{
	std::map<std::string, AbilityInfo>::iterator it;
	it = getBaseAttrMod()->m_abilities.find(action);

	if (it != getBaseAttrMod()->m_abilities.end())
	{
		// wenn gleich 0, so wird der Standardtimer verwendet
		if (it->second.m_time != 0)
			return it->second.m_time;

		Action::ActionInfo* aci = Action::getActionInfo(action);
		if (aci ==0)
		{
			DEBUG("Information for action %s missing ",action.c_str());
			return 0;
		}

		return aci->m_standard_time;
	}
	DEBUG("creature %s cant use action %s",getSubtype().c_str(), action.c_str());
	return 0;
}

void Creature::initAction()
{
	//wenn Idle Animation schon laeuft, laufen lassen
	if (m_action.m_type== "noaction" && m_action.m_elapsed_time>0)
	{
		return;
	}

	if (m_action.m_type!= "noaction")
	{
		DEBUGX("init Action %s", m_action.m_type.c_str());
	}

	m_action.m_elapsed_time = 0;
	Action::ActionInfo* aci = Action::getActionInfo(m_action.m_type);
	
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

	//Timer und Timerlaufzeit ermitteln
	int timernr = getTimerNr(m_action.m_type);
	float timer = getTimer(m_action.m_type);

	if (timer != 0)
	{
		DEBUGX("timer nr %i time %f Action %s",timernr,timer,m_action.m_type.c_str());
	}

	// Faehigkeit Ausdauer
	if (checkAbility("endurance") && timer ==1)
	{
		DEBUGX("ausdauer");
		// Timerlaufzeit um 15% verringern
		timer *= 0.85;
	}
	
	// Ionisation skill
	if (checkAbility("ionisation") 
		&& (m_action.m_type == "lightning" || m_action.m_type == "lightning_strike"
		|| m_action.m_type == "thunderstorm" || m_action.m_type == "thunderstorm2"
		|| m_action.m_type == "chain_lightning" || m_action.m_type == "chain_lightning2"))
	{
		timer *= 0.8;
	}


	// Testen ob der benoetigte Timer frei ist
	// Wenn der benoetigte Timer noch laeuft wird die Basisaktion ausgefuehrt
	if (timernr != 0)
	{
		if (m_timers[timernr-1] ==0)
		{
			// Timer ist frei, Timer starten
			m_timers[timernr-1] = timer;
			m_timers_max[timernr-1] = timer;
			addToNetEventMask(NetEvent::DATA_TIMER);
		}
		else
		{
			// Timer laeuft noch, Basisaktion verwenden
			m_action.m_type = aci->m_base_action;
			aci = Action::getActionInfo(m_action.m_type);

		}
	}

	// setzen der Standarddauer der Aktion
	m_action.m_time = getActionTime(m_action.m_type);

	Action::ActionType baseact = "noaction";
	Action::ActionInfo* ainfo = Action::getActionInfo(m_action.m_type);
	if (ainfo != 0)
	{
		baseact = ainfo->m_base_action;
	}

	// Zeit der Aktion modifizieren
	if (baseact == "walk")
	{
			// Bei Aktion laufen die Laufgeschwindigkeit einrechnen
			m_action.m_time = 1000000 / getBaseAttrMod()->m_walk_speed;
			Vector speed = getSpeed();
			speed *= getBaseAttr()->m_step_length/m_action.m_time;
			setSpeed(speed);

			DEBUGX("walk time %f walk speed %i",m_action.m_time,getBaseAttrMod()->m_walk_speed);
			DEBUGX("pos %f %f speed %f %f",getShape()->m_center.m_x, getShape()->m_center.m_y, getSpeed().m_x,getSpeed().m_y);
	}
	else
	{
		setSpeed(Vector(0,0));
	}



	// Fuer Aktionen die auf physischen Angriffen beruhen sowie fuer den normalen Magieangriff Waffengeschwindigkeit einrechnen
	if (baseact == "attack" || baseact == "range_attack" || baseact == "holy_attack" || m_action.m_type == "magic_attack")
	{
		float atksp = MathHelper::Min((short) 5000,getBaseAttrMod()->m_attack_speed);


		m_action.m_time *= 1000000/atksp;

	}

	DEBUGX("resulting time %f",m_action.m_time);
	// Drehwinkel setzen
	if (aci->m_target_type != Action::SELF && m_action.m_type != "take_item" && m_action.m_type != "noaction")
	{

		setAngle((m_action.m_goal - getShape()->m_center).angle());

	}
	if (baseact == "walk")
	{
		setAngle(getShape()->m_angle = getSpeed().angle());
	}

	// Daten fuer die Animation setzen
	m_action.m_action_equip = getActionEquip();

	addToNetEventMask(NetEvent::DATA_ACTION);

	if (m_action.m_time ==0)
	{
		clearCommand(false,true);
		m_action.m_type = "noaction";
	}

	// wenn keine Aktion berechnet wurde, Kommando beenden
	if (m_action.m_type == "noaction" && m_command.m_type !="noaction")
	{
		clearCommand(false,true);
	}
	DEBUGX("action %s time %f angle %f",m_action.m_type.c_str(), m_action.m_time,getShape()->m_angle);
}

void Creature::performAction(float &time)
{
	// Wenn Idle Aktion ausgefuehrt wird, aber ein Kommando vorliegt, IdleAktion sofort beenden
	if (m_action.m_type == "noaction" && m_command.m_type != "noaction")
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
			clearCommand(false,false);
		}
	}

	if (!World::getWorld()->isServer() && m_action.m_type != "noaction")
	{
		DEBUGX("perform Action %s for %f msec, %f / %f done", m_action.m_type.c_str(),dtime,m_action.m_elapsed_time,m_action.m_time);
	}


	//Behandlung des Laufens
	if (m_action.m_type == "walk")
	{

		// Kollisionen behandeln
		collisionDetection(dtime);

		// neue Koordinaten ausrechnen
		Vector newpos = getShape()->m_center + getSpeed()*dtime;

		// Bewegung ausfuehren
		moveTo(newpos,false);

	}



	// Behandlung der Wirkung der Aktion
	Action::ActionInfo* ainfo = Action::getActionInfo(m_action.m_type);
	if (ainfo == 0)
	{
		ERRORMSG("action information missing for action %s",m_action.m_type.c_str());
		m_action.m_type ="noaction";
		clearCommand(false,true);
		return;

	}

	// Prozentsatz bei dessen Erreichen die Wirkung der Aktion berechnet wird
	float pct = ainfo->m_critical_perc;

	if (m_action.m_type != "noaction")
	{
		DEBUGX("pos %f %f  speed %f %f  pct %f",getShape()->m_center.m_x, getShape()->m_center.m_y, getSpeed().m_x,getSpeed().m_y,p1);
	}

	// Triple Shot
	if (m_action.m_type == "triple_shot" || m_action.m_type == "guided_triple_shot" ||  m_action.m_type =="whirl_blow")
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
		DEBUGX("critical point %f %f %f",p1,pct,p2);

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
		if (m_action.m_goal_object_id!=0 && m_action.m_type!= "take_item")
		{
			// Zielobjekt durch ID gegeben, Objekt von der Welt holen
			goalobj = getRegion()->getObject(m_action.m_goal_object_id);
		}
		else
		{
			// Kein Zielobjekt per ID gegeben
			DEBUGX("no Goal ID!");
			// Im Falle von Nahkampf Ziel anhand des Zielpunktes suchen
			if (ainfo->m_target_type == Action::MELEE && m_action.m_type!= "take_item")
			{
				DEBUGX("Searching goal %f %f",goal.m_x,goal.m_y);
				goalobj = getRegion()->getObjectAt(goal,LAYER_AIR);
				
				// second try: search Object on line between attacker and target point
				if (goalobj == 0)
				{
					WorldObjectList hitobj;
					Line line(getShape()->m_center, goal);
					getRegion()->getObjectsOnLine( line, &hitobj, LAYER_AIR);
					
					// get the first object, that is really hostile
					WorldObjectList::iterator it;
					for (it = hitobj.begin(); it != hitobj.end(); ++it)
					{
						if (World::getWorld()->getRelation(this->getFraction(), *it) == Fraction::HOSTILE)
						{
							goalobj = *it;
							break;
						}
					}
				}
				DEBUGX("got object %p",goalobj);
			}
		}

		// Party Zauber suchen sich ihr Ziel
		if (ainfo->m_target_type == Action::PARTY)
		{
			Shape s;
			s.m_type = Shape::CIRCLE;
			s.m_center = goal;
			s.m_radius =8;
			WorldObjectList res;
			WorldObjectList::iterator it;

			getRegion()->getObjectsInShape(&s, &res, LAYER_ALL,CREATURE,0);

			// naechstgelegenen verbuendeten Spieler suchen
			float dist = 10000;
			goalobj = this;

			for (it = res.begin(); it != res.end(); ++it)
			{
				if (World::getWorld()->getRelation(getFraction(), (*it)->getFraction()) == Fraction::ALLIED)
				{
					if (goal.distanceTo((*it)->getShape()->m_center) < dist)
					{
						goalobj = (*it);
						dist = goal.distanceTo((*it)->getShape()->m_center);
						DEBUGX("chose player %i with distance %f",(*it)->getId(),dist);
					}
				}
			}

		}


		// Zielobjekt als Creature* pointer
		Creature* cgoal=0;

		// Zielobjekt im Nahkampf suchen an der Stelle an der die Waffe trifft
		if (goalobj ==0 && ainfo->m_target_type == Action::MELEE && m_action.m_type!= "take_item")
		{
			goalobj = getRegion()->getObjectAt(goal,LAYER_AIR);
		}

		// Wenn ein Zielobjekt existiert
		if (goalobj !=0)
		{
			if (!goalobj->isCreature())
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

				DEBUGX("goal object %p",cgoal);
			}
		}

		// ausfuehren des kritischen Teiles der Aktion
		performActionCritPart(goal, goalobj);

	}

	// Wenn die Aktion beenden wurde evtl Kommando abschließen
	if (finish)
	{
		float disttogoal = getShape()->m_center.distanceTo(goal);
		if (m_action.m_goal_object_id!=0 && m_action.m_type!= "take_item")
		{
			// Zielobjekt durch ID gegeben, Objekt von der Welt holen
			goalobj = getRegion()->getObject(m_action.m_goal_object_id);
			if (goalobj != 0)
			{
				goal = goalobj->getShape()->m_center;
				disttogoal = goalobj->getShape()->getDistance(*(getShape()));
			}

		}

		DEBUGX("finished action");

		if (m_action.m_type == "walk")
		{
			// Trigger erzeugen
			Trigger* tr = new Trigger("unit_moved");
			tr->addVariable("unit",getId());
			getRegion()->insertTrigger(tr);

			if (getType() == "PLAYER")
			{
				Trigger* tr = new Trigger("player_moved");
				tr->addVariable("player",getId());
				getRegion()->insertTrigger(tr);
			}

		}

		// Kommando ist beendet wenn die gleichnamige Aktion beendet wurde
		// Ausnahme: Bewegungskommando ist beendet wenn das Ziel erreicht ist
		Action::ActionType baseact = ainfo->m_base_action;
		if ((((m_action.m_type == m_command.m_type) || m_action.m_type == baseact) && m_action.m_type != "walk") || (m_command.m_type == "walk" && disttogoal < getBaseAttr()->m_step_length
				  && !(m_command.m_type == "charge" || m_command.m_type == "storm_charge")))
		{
			bool recalc = false;

			if (m_command.m_type == "charge" || m_command.m_type == "storm_charge")
			{
				recalc = true;
			}
			DEBUGX("finished command %s (base %s) with action %s",m_command.m_type.c_str(),baseact.c_str(),m_action.m_type.c_str() );
			if (m_command.m_type != "noaction")
			{
				addToNetEventMask(NetEvent::DATA_COMMAND);
				DEBUGX("goal %f %f goalobj %i",goal.m_x, goal.m_y, m_action.m_goal_object_id);
				clearCommand(true,false);
			}
			m_command.m_type = "noaction";
			m_action.m_elapsed_time=0;
			m_command.m_damage_mult = 1;


			// Schaden neu berechnen
			if (recalc)
			{
				recalcDamage();
			}
		}

		// Aktion ist beendet
		if (m_action.m_type != "noaction")
		{
			addToNetEventMask(NetEvent::DATA_ACTION);
		}
		m_action.m_type = "noaction";
		m_action.m_elapsed_time =0;
		addToNetEventMask(NetEvent::DATA_ACTION);
	}




}

void Creature::performActionCritPart(Vector goal, WorldObject* goalobj)
{
	// Zielobjekt als Creature* pointer
	// null, wenn das Objekt kein Lebewesen ist
	Creature* cgoal =0;
	int cgoalid = 0;
	if (goalobj !=0 && !goalobj->isCreature())
	{
		cgoal =0;
	}
	else
	{
		cgoal = (Creature*) goalobj;
		if (cgoal != 0)
		{
			cgoalid= goalobj->getId();
		}
	}

   Action::ActionInfo* ainfo = Action::getActionInfo(m_action.m_type);
   if (ainfo ==0 )
	   return;


	WorldObjectList res;
	res.clear();
	WorldObjectList::iterator it;

	// Koordinaten das ausfuehrenden Objektes
	Vector &pos = getShape()->m_center;

	Creature* cr =0;
	Projectile* pr =0;

	// Struktur fuer Basisattributmods, initialisiert mit Nullen
	CreatureBaseAttrMod cbam;

	// Struktur fuer Modifikationen der dyn. Attribute, initialisiert mit Nullen
	CreatureDynAttrMod cdam;

	calcActionAttrMod(m_action.m_type,cbam,cdam);

	// Projectiltyp
	Projectile::Subtype projtype = ainfo->m_projectile_type;

	//Faehigkeit Windpfeile
	if (projtype == "arrow" && m_base_attr_mod.m_special_flags & WIND_ARROWS)
		projtype = "wind_arrow";

	// Faehigkeit Eispfeile
	if (projtype == "arrow" && m_base_attr_mod.m_special_flags & ICE_ARROWS)
		projtype = "ice_arrow";


	std::list<std::string>::iterator kt;
	for (kt = ainfo->m_effect.m_cpp_impl.begin(); kt != ainfo->m_effect.m_cpp_impl.end(); ++kt)
	{
		// Form, wird initialisiert mit der Form des Ausfuehrenden
		Shape s;
		s.m_center = pos;
		s.m_type = Shape::CIRCLE;
		s.m_radius = ainfo->m_radius;

		if (s.m_radius < 0)
		{
			s.m_radius = getShape()->m_radius + m_command.m_range;
		}

		// Daten fuer Geschosse: Zielrichtung und Startpunkt
		Vector dir = goal - pos;
		Vector dir2,tdir;
		dir.normalize();

		// Startpunkt fuer Geschosse
		// aeusserer Rand des Ausfuehrenden plus 5%
		Vector sproj;
		Fraction::Id fr = m_fraction;
		sproj = pos + dir*1.05*s.m_radius;

		if (*kt == "dmg_at_target")
		{
			if (cgoal != 0)
			{
				cgoal->takeDamage(&m_damage);
			}
		}
		else if (*kt == "dmg_at_enemies_in_radius")
		{
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,this);

			// an alle Schaden austeilen
			for (it=res.begin();it!=res.end();++it)
			{
				if ((*it)->isCreature())
				{
					cr = (Creature*) (*it);
					cr->takeDamage(&m_damage);

				}
			}
		}
		else if (*kt == "dmg_at_enemies_around_target")
		{
			s.m_center = goal;
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,this);

			// an alle einfachen Schaden austeilen
			for (it=res.begin();it!=res.end();++it)
			{
				if ((*it)->isCreature())
				{
					cr = (Creature*) (*it);
					cr->takeDamage(&m_damage);

				}
			}
		}
		else if (*kt == "basemod_at_self")
		{
			applyBaseAttrMod(&cbam);
		}
		else if (*kt == "basemod_at_allies_in_radius")
		{
			s.m_center = pos;
			getRegion()->getObjectsInShape(&s, &res, LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (World::getWorld()->getRelation(fr,(*it)) == Fraction::ALLIED)
				{
					if ((*it)->isCreature())
					{
						cgoal = (Creature*) (*it);
						cgoal->applyBaseAttrMod(&cbam);
					}
				}
			}
		}
		else if (*kt == "basemod_at_target")
		{
			if (cgoal != 0)
			{
				cgoal->applyBaseAttrMod(&cbam);
			}
		}
		else if (*kt == "basemod_at_enemies_in_radius")
		{
			s.m_center = pos;
			getRegion()->getObjectsInShape(&s, &res, LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (World::getWorld()->getRelation(fr,(*it)) == Fraction::HOSTILE)
				{
					if ((*it)->isCreature())
					{
						cgoal = (Creature*) (*it);
						cgoal->applyBaseAttrMod(&cbam);
					}
				}
			}
		}
		else if (*kt == "dynmod_at_self")
		{
			applyDynAttrMod(&cdam);
		}
		else if (*kt == "dynmod_at_allies_in_radius")
		{
			s.m_center = pos;
			getRegion()->getObjectsInShape(&s, &res, LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (World::getWorld()->getRelation(fr,(*it)) == Fraction::ALLIED)
				{
					if ((*it)->isCreature())
					{
						cgoal = (Creature*) (*it);
						cgoal->applyDynAttrMod(&cdam);
					}
				}
			}
		}
		else if (*kt == "dynmod_at_target")
		{
			if (cgoal != 0)
			{
				cgoal->applyDynAttrMod(&cdam);
			}
		}
		else if (*kt == "dynmod_at_enemies_in_radius")
		{
			s.m_center = pos;
			getRegion()->getObjectsInShape(&s, &res, LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (World::getWorld()->getRelation(fr,(*it)) == Fraction::HOSTILE)
				{
					if ((*it)->isCreature())
					{
						cgoal = (Creature*) (*it);
						cgoal->applyDynAttrMod(&cdam);
					}
				}
			}
		}
		else if (*kt == "proj_at_target")
		{
			pr = ObjectFactory::createProjectile(projtype);
			if (pr !=0)
			{
				pr->setDamage(&m_damage);
				pr->addFlags(ainfo->m_projectile_flags);
				pr->setGoalObject(cgoalid);
				if (ainfo->m_projectile_counter !=0)
				{
					pr->setCounter(ainfo->m_projectile_counter);
				}
				if (ainfo->m_projectile_radius != 0)
				{
					pr->setRadius(ainfo->m_projectile_radius);
				}
				getRegion()->insertProjectile(pr,goal);
			}
		}
		else if (*kt == "proj_fly_at_target")
		{
			pr = ObjectFactory::createProjectile(projtype);

			if (pr !=0)
			{
				pr->setGoalObject(cgoalid);
				pr->setDamage(&m_damage);
				pr->addFlags(ainfo->m_projectile_flags);
				if (ainfo->m_projectile_counter !=0)
				{
					pr->setCounter(ainfo->m_projectile_counter);
				}
				if (ainfo->m_projectile_radius != 0)
				{
					pr->setRadius(ainfo->m_projectile_radius);
				}
				pr->setSpeed(dir*(ainfo->m_projectile_speed/1000000));

				getRegion()->insertProjectile(pr,sproj);
			}
		}
		else if (*kt == "proj_at_self")
		{

			pr = ObjectFactory::createProjectile(projtype);
			if (pr !=0)
			{
				pr->setDamage(&m_damage);
				pr->addFlags(ainfo->m_projectile_flags);
				if (ainfo->m_projectile_counter !=0)
				{
					pr->setCounter(ainfo->m_projectile_counter);
				}
				DEBUGX("proj radius %f",ainfo->m_projectile_radius);
				if (ainfo->m_projectile_radius != 0)
				{
					pr->setRadius(ainfo->m_projectile_radius);
				}
				getRegion()->insertProjectile(pr,pos);
			}
		}
		else if (*kt == "proj_at_enemies_around_target")
		{
			s.m_center = goal;
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,this);

			// an alle einfachen Schaden austeilen
			for (it=res.begin();it!=res.end();++it)
			{
				if ((*it)->isCreature() && World::getWorld()->getRelation(fr,(*it)) == Fraction::HOSTILE)
				{
					pr = ObjectFactory::createProjectile(projtype);
					pr->setDamage(&m_damage);
					pr->addFlags(ainfo->m_projectile_flags);
					if (ainfo->m_projectile_counter !=0)
					{
						pr->setCounter(ainfo->m_projectile_counter);
					}
					if (ainfo->m_projectile_radius != 0)
					{
						pr->setRadius(ainfo->m_projectile_radius);
					}
					cr = (Creature*) (*it);
					getRegion()->insertProjectile(pr,cr->getShape()->m_center);
				}
			}
		}
		else if (*kt == "proj_at_enemies_in_radius")
		{
			s.m_center = pos;
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,this);

			// an alle einfachen Schaden austeilen
			for (it=res.begin();it!=res.end();++it)
			{
				if ((*it)->isCreature() && World::getWorld()->getRelation(fr,(*it)) == Fraction::HOSTILE)
				{
					pr = ObjectFactory::createProjectile(projtype);
					if (pr !=0)
					{
						pr->setDamage(&m_damage);
						pr->addFlags(ainfo->m_projectile_flags);
						if (ainfo->m_projectile_counter !=0)
						{
							pr->setCounter(ainfo->m_projectile_counter);
						}
						if (ainfo->m_projectile_radius != 0)
						{
							pr->setRadius(ainfo->m_projectile_radius);
						}
						cr = (Creature*) (*it);
						getRegion()->insertProjectile(pr,cr->getShape()->m_center);
					}
				}
			}
		}
		// spezielle Implementationen
		else if (*kt == "hammer_bash")
		{
			m_damage.m_multiplier[Damage::PHYSICAL]=1;
			s.m_center = goal;
			s.m_radius = 1.5;
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,this);

			// an alle einfachen Schaden austeilen
			for (it=res.begin();it!=res.end();++it)
			{
				if ((*it)->isCreature() && (*it)!=goalobj)
				{
					cr = (Creature*) (*it);
					cr->takeDamage(&m_damage);

				}
			}
		}
		else if (*kt == "multishot" )
		{

			// 5 Pfeile erzeugen
			// mittlerer Pfeil erhaelt die Zielrichtung
			for (int i=-2;i<=2;i++)
			{
				dir2 = dir;
				dir2.m_x += i*0.15*dir.m_y;
				dir2.m_y -= i*0.15*dir.m_x;
				dir2.normalize();

				sproj = pos+dir2* 1.05 * s.m_radius;
				pr = ObjectFactory::createProjectile(projtype);
				if (pr !=0)
				{
					pr->setDamage(&m_damage);
					pr->setSpeed(dir2/80);

					getRegion()->insertProjectile(pr,sproj);
				}
			}

		}
		else if (*kt == "volley_shot" )
		{
			// 7 Pfeile erzeugen
			// mittlerer Pfeil erhaelt die Zielrichtung
			for (int i=-3;i<=3;i++)
			{
				dir2 = dir;
				dir2.m_x += i*0.1*dir.m_y;
				dir2.m_y -= i*0.1*dir.m_x;
				dir2.normalize();

				sproj = pos+dir2* 1.05 * s.m_radius;
				pr = ObjectFactory::createProjectile(projtype);
				if (pr !=0)
				{
					pr->setDamage(&m_damage);
					pr->setSpeed(dir2/80);
					getRegion()->insertProjectile(pr,sproj);
				}
			}

		}
		else if (*kt == "battle_cry" )
		{
		// alle Lebewesen im Umkreis um den Ausfuehrenden auswaehlen
			// Radius gleich Waffenreichweite
			cbam.m_time =10000;

			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,this);

			// an alle Schaden austeilen
			for (it=res.begin();it!=res.end();++it)
			{
				if ((*it)->isCreature() && World::getWorld()->getRelation(getFraction(),(*it)->getFraction()) ==  Fraction::HOSTILE)
				{
					cr = (Creature*) (*it);
					cbam.m_dwalk_speed = -cr->getBaseAttrMod()->m_walk_speed/2;
					cbam.m_dattack_speed = -cr->getBaseAttrMod()->m_attack_speed/2;
					cr->applyBaseAttrMod(&cbam);
				}
			}
		}
		else if (*kt == "scare" )
		{
			getRegion()->getObjectsInShape(&s, &res,LAYER_AIR,CREATURE,this);

			// make enemies run away
			for (it=res.begin();it!=res.end();++it)
			{
				if ((*it)->isCreature() && World::getWorld()->getRelation(getFraction(),(*it)->getFraction()) ==  Fraction::HOSTILE)
				{
					cr = (Creature*) (*it);
					float rel = m_damage.m_ai_mod_power[TAUNT]*1.0 / cr->getBaseAttrMod()->m_willpower;
					float chance = atan(rel)/(3.1415/2);
					if (Random::random() < chance && Random::random() < 0.5)
					{
						// vector pointing away from the caster
						Vector dir = cr->getShape()->m_center - getShape()->m_center;
						// rotate by [-90,90] degreee at random
						float angle = 3.14159*(Random::random()-0.5)*0.5;
						dir.rotate(angle);
						
						float length = MathHelper::Min(8.0f, 3*rel);
						dir.normalize();
						dir *= length;
						
						Vector goal = cr->getShape()->m_center + dir;
						Command cmd;
						cmd.m_goal = goal;
						cmd.m_goal_object_id = 0;
						cmd.m_range = 1;
						cmd.m_type = "walk";
						cr->insertScriptCommand(cmd,5000);
					}
				}
			}
		}		
		else if (*kt == "berserk")
		{
			m_dyn_attr.m_status_mod_time[Damage::BERSERK] = 15000;
			addToNetEventMask(NetEvent::DATA_STATUS_MODS);
		}
		else if (*kt == "speak")
		{
			if (goalobj != 0 && goalobj->isCreature() && getDialogueId() ==0)
			{
				cr = static_cast<Creature*>(goalobj);
				if (cr->getDialogueId() ==0 && !(cr->hasScriptCommand()))
				{
					Vector dir = getShape()->m_center - cr->getShape()->m_center;
					cr->setAngle(dir.angle());
					setAngle(dir.angle() + PI);
					Dialogue* dia = new Dialogue(getRegion(), cr->getRefName());
					EventSystem::setDialogue(dia);
					dia->addSpeaker(getId(),"player");
					dia->addSpeaker(goalobj->getId(),cr->getRefName());
					dia->addSpeaker(goalobj->getId(),"npc");
					dia->setSpeakerPosition(getId(), Dialogue::LEFT);
					dia->setSpeakerPosition(goalobj->getId(), Dialogue::RIGHT);
					dia->changeTopic("start");

					if (dia->isFinished())
					{
						// keine Gespraechsoption vorhanden
						delete dia;
					}
					else
					{
						getRegion()->insertDialogue(dia);
						EventSystem::setDialogue(0);
					}
				}
			}
		}
		else if (*kt == "use")
		{
			if (goalobj)
				goalobj->reactOnUse(getId());
		}

	}

	if (ainfo->m_effect.m_lua_impl != LUA_NOREF)
	{

		EventSystem::setRegion(getRegion());
		EventSystem::setDamage(&m_damage);

		EventSystem::pushVector(EventSystem::getLuaState(), goal);
		lua_setglobal(EventSystem::getLuaState(), "goal");

		int target =0;
		if (goalobj !=0)
			target = goalobj->getId();
		lua_pushnumber(EventSystem::getLuaState(),target);
		lua_setglobal(EventSystem::getLuaState(), "target");

		lua_pushnumber(EventSystem::getLuaState(),getId());
		lua_setglobal(EventSystem::getLuaState(), "self");

		lua_pushstring(EventSystem::getLuaState(),projtype.c_str());
		lua_setglobal(EventSystem::getLuaState(), "projectile_type");

		lua_pushnumber(EventSystem::getLuaState(),ainfo->m_projectile_speed);
		lua_setglobal(EventSystem::getLuaState(), "projectile_speed");

		EventSystem::executeCodeReference(ainfo->m_effect.m_lua_impl);

		EventSystem::setDamage(0);

	}

	// Faehigkeit Monsterjaeger
	// Wenn das Ziel nach der Aktion unter 0 Lebenspunkte hat Bonus austeilen
	if ((checkAbility("monster_hunter") || (checkAbility("monster_slayer"))) && cgoal && cgoal->getDynAttr()->m_health<0)
	{
		DEBUGX("monster killed, apply mod");
		CreatureBaseAttrMod cbam;

		// Modifikation:
		// 5% mehr Staerke, bei aufgewerteter Version erhoehte Angriffsgeschwindigkeit fuer 10 sec
		cbam.m_dstrength = getBaseAttr()->m_strength / 5;
		cbam.m_time = 10000;
		if (checkAbility("monster_slayer"))
		{
			cbam.m_dattack_speed = 100;
		}
		applyBaseAttrMod(&cbam);

	}
}

void Creature::collisionDetection(float time)
{
	if (getSpeed().getLength() < 0.000001)
		return;

	WorldObjectList result;
	// Punkt zu dem das Objekt bewegt werden soll
	Vector newpos = getShape()->m_center + getSpeed()*time;

	// Kreis um den Zielpunkt
	Shape scopy;
	scopy.m_radius = getShape()->m_radius*1.05;
	scopy.m_center = newpos;
	scopy.m_type = Shape::CIRCLE;

	// Ebene in der gesucht werden soll
	short layer = getLayer();

	// Alle kollidierenden Objekte suchen
	getRegion()->getObjectsInShape(&(scopy),&result,layer, CREATURE | FIXED,this);

	if (result.size()!=0)
	{
		// es gibt kollidierende Objekte
		DEBUGX("aktuelle Koordinaten %f %f",getShape()->m_center.m_x, getShape()->m_center.m_y);
		WorldObjectList::iterator i;

		Shape* s2;
        // Liste der kollidierenden Objekte durchgehen
		for (i= result.begin();i!=result.end();++i)
		{
			DEBUGX("Kollision %i",(*i)->getId());
			s2 =(*i)->getShape();
			// wenn mit dem Zielobjekt kollidiert Bewegung beenden
			if ((*i)->getId() == getCommand()->m_goal_object_id)
			{
				setSpeed(Vector(0,0));
				break;
			}

			// Wenn die Faehigkeit Stuermen ist dann Bewegung beenden
			if (m_command.m_type == "charge" || m_command.m_type == "storm_charge")
			{
				DEBUGX("charge goal object %i",(*i)->getId());
				// Behandlung von *Charge*
				m_command.m_goal_object_id = (*i)->getId();
				setSpeed(Vector(0,0));
				return;
			}
			else
			{
				// Kollision behandeln
				handleCollision(s2);
			}

		}

		if (getSpeed().getLength() > 0.000001)
		{
			// neuen erreichten Punkt ausrechnen und testen ob dieser akzeptabel ist
			newpos = getShape()->m_center + getSpeed()*time;


			// Kreis um den neuen Zielpunkt
			scopy.m_center = newpos;
			DEBUGX("neue Koordinaten %f %f",newpos.m_x, newpos.m_y);
			scopy.m_radius = getShape()->m_radius;
			result.clear();

			// Suchen der Objekte um den neuen Zielpunkt
			getRegion()->getObjectsInShape(&(scopy),&result,layer, CREATURE | FIXED,this);

			// Testen ob die Richtung fuer alle diese Objekte akzeptabel ist
			bool change;
			for (i= result.begin();i!=result.end();++i)
			{
				DEBUGX("Kollision %i",(*i)->getId());
				s2 =(*i)->getShape();
				// Kollision behandeln
				change = handleCollision(s2);

				// Richtung wurde geaendert, war also nicht akzeptabel
				// Bewegung abbrechen
				if (change)
				{
					DEBUGX("still colliding");
					setSpeed(Vector(0,0));
					break;
				}

			}
		}

		// Kommando abbrechen, wenn man zu lange sich nicht bewegen kann
		if (getSpeed().getLength() <= 0.000001)
		{
			m_pathfind_counter +=15;
			DEBUGX("counter %f",m_pathfind_counter);
			if (m_pathfind_counter>=50)
			{
				clearCommand(false,true);
			}
		}

	}
}

bool Creature::handleCollision(Shape* s2)
{
	// eigene Koordinaten
	Vector pos = getShape()->m_center;

	// Koordinaten des kollidierenden Objektes
	Vector cpos = s2->m_center;

	DEBUGX("collision %f %f",cpos.m_x, cpos.m_y);
	DEBUGX("player pos %f %f",pos.m_x, pos.m_y);
	DEBUGX("old speed %f %f", getSpeed().m_x, getSpeed().m_y);

	// erlaubte Richtung berechnen
	// Projektion des Punktes auf den Rand der Flaeche
	Vector proj = s2->projectionOnBorder(pos);
	if (proj.distanceTo(pos) < 0.0001)
		proj = cpos;

	DEBUGX("projection %f %f",proj.m_x, proj.m_y);

	// Vektor der erlaubten Richtung
	// immer von der Flaeche weg
	Vector dir = pos - proj;
	if (cpos.distanceTo(proj) > cpos.distanceTo(pos))
		dir = proj - pos;

	DEBUGX("direction %f %f",dir.m_x, dir.m_y);

	// Erlaubt sind alle Richtungen, die maximal einen rechten Winkel mit der erlaubten Richtung bilden
	Vector sp = getSpeed();
	if (dir*sp < 0)
	{
		// Geschwindigkeit aendern
		sp.normalPartTo(dir);
		setSpeed(sp);
	}
	else
	{
		return false;
	}

	/*
	// alte Kollisionsengine
	bool circ = true;

	DEBUGX("old speed %f %f", getSpeed().m_x, getSpeed().m_y);
	if (s2->m_type==Shape::RECT)
	{
		// Kollision mit Rechteckt
		// Drehen des Bezugssystems, sodass das Rechteck ausgerichtet ist

		Vector locpos = pos - cpos;
		locpos.rotate(-s2->m_angle);

		Vector locspeed = getSpeed();
		locspeed.rotate(-s2->m_angle);

		circ = false;
		DEBUGX("Rechteck");

		// Ausdehnung des Rechtseckes
		Vector ext = s2->m_extent;
		float r = getShape()->m_radius;

		if (fabs(locpos.m_x) > ext.m_x+r)
		{
			// Kollision mit einer senkrechten Kante
			// x-Anteil der Bewegung auf 0 setzen
			locspeed.m_x =0;
			DEBUGX("x-anteil genullt");
		}
		else
		{
			if (fabs(locpos.m_y) >ext.m_y+r)
			{
				// Kollision mit einer waagerechten Kante
				// y-Anteil der Bewegung auf 0 setzen
				DEBUGX("y-anteil genullt");
				locspeed.m_y =0;
			}
			else
			{
				// Kollision an der Ecke des Rechteckes
				DEBUGX("kollision an einer Ecke");

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
			DEBUGX("locspeed %f %f",locspeed.m_x, locspeed.m_y);
			locspeed.rotate(s2->m_angle);
			setSpeed(locspeed);

		}

	}

	if (circ)
	{
		// Behandlung Kollision mit Kreis
		DEBUGX("obj pos %f %f", pos.m_x, pos.m_y);
		DEBUGX("collision pos %f %f", cpos.m_x, cpos.m_y);


		// Vektor vom eigenen Mittelpunkt zum Mittelpunkt des kollidierenden Objektes (normiert)
		Vector dir = cpos - pos;
		dir.normalize();
		DEBUGX("normal dir %f %f", dir.m_x, dir.m_y);


		// vom Geschwindigkeitsvektor nur den zu dieser Richtung senkrechten Teil uebrig lassen
		Vector speed = getSpeed();
		speed.normalPartTo(dir);
		setSpeed(speed);
	}
*/
	// neue Geschwindigkeit normieren
	Vector speed = getSpeed();
	DEBUGX("new speed %f %f", getSpeed().m_x, getSpeed().m_y);
	if (speed.getLength() < 0.000001)
	{
		setSpeed(Vector(0,0));
	}
	else
	{
		speed.normalize();
		DEBUGX("new speed %f %f", getSpeed().m_x, getSpeed().m_y);
		speed *= getBaseAttr()->m_step_length/m_action.m_time;
		setSpeed(speed);
	}

	//char dummy;
	//std::cin >> dummy;

	return true;

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
	DEBUGX("insert script command %s at %i flag %i",cmd.m_type.c_str(),cmd.m_goal_object_id,cmd.m_flags);
}

void Creature::clearScriptCommands()
{
	if (hasScriptCommand())
	{
		clearCommand(false,true);
	}

	m_script_commands.clear();
	m_script_command_timer =0;
}


void Creature::updateCommand()
{
	// Wenn aktuelles Kommando keine Aktion vorschreibt
	DEBUGX("next command: %s ",m_next_command.m_type.c_str());
	if ((!hasScriptCommand() && m_next_command.m_type != "noaction" && !getRegion()->getCutsceneMode()) ||
			 (!m_script_commands.empty() && m_command.m_type == "noaction"))
	{
		// Naechstes Kommando uebernehmen
		if (!m_script_commands.empty())
		{

			m_command = m_script_commands.front().first;
			m_script_command_timer = m_script_commands.front().second;
			if (! (m_command.m_flags & Command::REPEAT) )
			{
				m_script_commands.pop_front();

			}
			DEBUGX("script command %s at %i time %f",m_command.m_type.c_str(), m_command.m_goal_object_id, m_script_command_timer);
		}
		else
		{
			m_command.m_type=m_next_command.m_type;
			m_command.m_goal = m_next_command.m_goal;
			m_command.m_goal_object_id=m_next_command.m_goal_object_id;
			m_command.m_range=m_next_command.m_range;
			m_command.m_damage_mult = 1;

			// Naechstes Kommando auf nichts setzen
			m_next_command.m_type = "noaction";

			addToNetEventMask(NetEvent::DATA_NEXT_COMMAND);
		}

		addToNetEventMask(NetEvent::DATA_COMMAND);


	}

	// Kommando ausrechnen das evtl aus einem Statusmod resultiert
	calcStatusModCommand();



}

void Creature::calcAction()
{


	updateCommand();

	// wenn kein Kommando existiert keine Aktion ausfuehren, beenden
	if (m_command.m_type == "noaction")
	{
		if (m_action.m_type!= "noaction")
		{
			addToNetEventMask(NetEvent::DATA_ACTION);
		}
		m_action.m_type = "noaction";
		return;

	}



	DEBUGX("calc action for command %s",m_command.m_type.c_str());
	addToNetEventMask(NetEvent::DATA_ACTION);


	// Reichweite der Aktion berechnen
	float range = m_command.m_range;

	/*
	if (m_command.m_type == "walk")
	{
		range =getShape()->m_radius;
	}
	*/

	if (m_command.m_type == "take_item")
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
	if ( m_command.m_goal_object_id !=0 && m_command.m_type != "take_item")
	{
		DEBUGX("goal ID: %i",m_command.m_goal_object_id);
		// Zielobjekt holen
		goalobj = getRegion()->getObject(m_command.m_goal_object_id);

		if (goalobj ==0)
		{
			// Zielobjekt existiert nicht mehr, abbrechen
			m_action.m_type = "noaction";
			m_action.m_elapsed_time =0;
			clearCommand(false,true);
			return;
		}

		// Ziel muss aktiv sein
		if (goalobj->getState() != STATE_ACTIVE)
		{
			DEBUGX("refused to interact with inactive object %i",m_command.m_goal_object_id);
			m_action.m_type = "noaction";
			m_action.m_elapsed_time =0;
			clearCommand(false,true);
			return;
		}

		// Zielkoordinaten neu setzen (Koordinaten des Zielobjektes)
		goal = goalobj->getShape()->m_center;

		// Abstandsberechnung
		dist = getShape()->getDistance(*(goalobj->getShape()));
	}

	// Aktion Item aufheben
	if (m_command.m_type == "take_item")
	{
		DropItem* di = getRegion()->getDropItem(m_command.m_goal_object_id);
		if (di ==0)
		{
			// Zielobjekt existiert nicht mehr, abbrechen
			m_action.m_type = "noaction";
			m_action.m_elapsed_time =0;
			clearCommand(false,true);
			return;
		}
		else
		{
			Vector pos = di->getPosition();
			goal = pos;
			dist = getShape()->m_center.distanceTo(pos);
		}
	}


	// Stuermen ohne Zielobjekt hat keinen Zielradius
	if ((m_command.m_type == "charge" || m_command.m_type == "storm_charge") &&  m_command.m_goal_object_id==0)
	{
		range=0;
		dist =1;
	}


	if (Action::getActionInfo(m_command.m_type)->m_target_type == Action::MELEE 
		|| Action::getActionInfo(m_command.m_type)->m_base_action == "walk" 
		|| m_dyn_attr.m_status_mod_time[Damage::BERSERK]>0)
	{
		// Aktion fuer die man an das Ziel hinreichend nahe herankommen muss
		DEBUGX("range %f dist %f",range,dist);

		// Testen ob das Ziel in Reichweite ist
		if (range > dist)
		{
			// Ziel ist in Reichweite, geplante Aktion ausfuehren
			if (m_command.m_type != "walk")
			{
				m_action.m_type = m_command.m_type;
				m_action.m_goal_object_id = m_command.m_goal_object_id;
				m_action.m_goal = goal;
			}
			else
			{
				m_action.m_type = "noaction";
				clearCommand(true,false);
			}

		}
		else
		{
			// Ziel ist nicht in Reichweite, hinlaufen

			m_action.m_type = "walk";
			if ((m_command.m_type == "charge" || m_command.m_type == "storm_charge")  )
			{
				// Sturmangriff
				DEBUGX("Charge");

				// beim ersten Mal richtung neu ausrechnen, das ist der Fall wenn der Schadensmultiplikator gleich 1 ist
				// sonst nur beschleunigen
				if (m_command.m_damage_mult>1)
				{
					// Richtung nicht neu ausrechnen, beschleunigen
					m_command.m_damage_mult += 2;

					// Geschwindigkeit normieren, mit Schadensmultiplikator multiplizieren
					Vector speed = getSpeed();
					speed.normalize();

					if (m_command.m_type == "charge")
						m_command.m_damage_mult *= 0.85;
					else
						m_command.m_damage_mult *= 0.93;

					// Schaden neu berechnen
					recalcDamage();

					speed *= sqrt(m_command.m_damage_mult);
					setSpeed(speed);
				}
				else
				{
					// Richtung: direct zum Ziel;
					DEBUGX("calc charge dir");
					// Vektor von der eigenen Position zum Ziel, normieren
					Vector dir = goal-pos;
					dir.normalize();
					setSpeed(dir);

					if (dir.getLength()!=0)
					{
						m_command.m_damage_mult = (m_command.m_damage_mult+2);
					}
					else
					{
						m_action.m_type = "noaction";
						m_action.m_elapsed_time =0;
					}

				}


			}
			else
			{
				// Berechnen der Bewegungsrichtung
				calcWalkDir(goal, goalobj);

			}
		}
		m_action.m_goal_object_id = m_command.m_goal_object_id;

	}
	else
	{
		// keine Nahkampfaktion bzw laufen -> Action direkt ausfuehren
		m_action.m_type = m_command.m_type;
		m_action.m_goal_object_id = m_command.m_goal_object_id;
		m_action.m_goal = goal;
	}

	// wenn keine Aktion berechnet wurde, Kommando beenden
	if (m_action.m_type == "noaction")
	{
		clearCommand(true,false);
	}
}

Action::ActionType Creature::getBerserkAction()
{
	Action::ActionType act = "attack";
	
	if (getBaseAttrMod()->m_abilities.count(act) == 0)
	{
		// this happens, when action is not present due to berserk
		std::map<std::string, AbilityInfo>::iterator it;
		for (it = getBaseAttrMod()->m_abilities.begin(); it != getBaseAttrMod()->m_abilities.end(); ++it)
		{
			if (it->first == "noaction" || it->first == "die" || it->first == "walk" || it->first == "dead" 
				|| it->first == "take_item" || it->first == "use" || it->first == "speak" || it->first == "trade")
				continue;
			
			Action::ActionInfo* aci2 = Action::getActionInfo(it->first);
			if (aci2 != 0 && aci2->m_timer_nr == 0 && (aci2->m_target_type == Action::MELEE || aci2->m_target_type == Action::RANGED))
			{
				act = it->first;
				DEBUGX("set berserk action to %s",act.c_str());
				
				break;
			}
		}
	}
	return act;
}

void Creature::calcStatusModCommand()
{
	// eigene Position
	Vector& pos = getShape()->m_center;

	if (m_dyn_attr.m_status_mod_time[Damage::BERSERK]>0)
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

			DEBUGX("checking obj %i",(*i)->getId());
			// Fuer feindliche Lebewesen
			if (World::getWorld()->getRelation(m_fraction,*i) == Fraction::HOSTILE)
			{
				// Abstand zum eigenen Mittelpunkt berechnen
				DEBUGX("hostile");
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
			DEBUGX("attack id %i",id);
			// Angriff
			m_command.m_type = getBerserkAction();
			m_command.m_goal_object_id = id;
			m_command.m_goal = goal;
			m_command.m_range = getBaseAttrMod()->m_attack_range;

			// nur Nahkampf, daher Reichweite die von Fernwaffen kommt reduzieren
			if (m_command.m_range >4)
			{
				DEBUGX("capped range for berserk");
				m_command.m_range= 2;
			}

		}
		else
		{
			// nichts machen
			m_command.m_type = "noaction";
			if (getDynAttr()->m_timer.getTime() >1000)
			{
				m_dyn_attr.m_status_mod_time[Damage::BERSERK] =0;
				addToNetEventMask(NetEvent::DATA_STATUS_MODS);
			}

		}
		addToNetEventMask(NetEvent::DATA_COMMAND);
	}
	
	// Statusmod verwirrt
	// diese Aktion nur vom Server ausloesen lassen
	if (m_command.m_type != "noaction" 
		&& Random::randi(3) >0
		&& m_dyn_attr.m_status_mod_time[Damage::CONFUSED]>0 
		&& World::getWorld()->isServer())
	{
		// aktuelle Bewegungsrichtung
		Vector goal = m_command.m_goal;
		if (m_command.m_goal_object_id != 0)
		{
			WorldObject* obj = getRegion()->getObject(m_command.m_goal_object_id);
			if (obj != 0)
			{
				goal = obj->getShape()->m_center;
			}
		}
		
		// change v by a random angle
		Vector v = goal- getShape()->m_center;
		float angle = 3.14159*(Random::random()-0.5)*0.5;
		v.rotate(angle);

		// 30% chance to reduce distance to melee
		bool melee= false;
		if (Random::random() < 0.0)
		{
			v.normalize();
			v *= (m_base_attr_mod.m_attack_range + getShape()->m_radius);
			melee = true;
		}
		
		Vector newgoal = getShape()->m_center + v;

		// Kreis um den Zielpunkt, Radius gleich Radius des Lebewesens
		Shape s;
		s.m_center =newgoal;
		s.m_type = Shape::CIRCLE;
		s.m_radius = getShape()->m_radius;
		WorldObject* wo =0;
		WorldObjectList res;
		WorldObjectList::iterator it;

		// ermitteln der Objekte mit denen bei der Bewegung kollidiert wird
		getRegion()->getObjectsInShape(&s,&res,LAYER_AIR,CREATURE | FIXED,this);

		if (!res.empty())
		{
			// Es gibt ein kollidierendes Objekt, als Zielobjekt setzen
			int num = Random::randi(res.size());
			it = res.begin();
			for (int i=0; i<num; i++)
			{
				++it;
			}
			wo = *it;
			newgoal = wo->getShape()->m_center;
		}
		
		// 30% chance to change the action
		float r = Random::random();
		if (r < 0.3 && !melee)
		{
			DEBUGX("confused: swap command");
			if (m_command.m_type == "walk")
			{
				m_command.m_type = getBerserkAction();
				m_command.m_range = getBaseAttrMod()->m_attack_range;
			}
			else
			{
				m_command.m_type = "walk";
				m_command.m_range = 2;
			}
		}
		m_command.m_goal = newgoal;
		m_command.m_goal_object_id =0;
		addToNetEventMask(NetEvent::DATA_COMMAND);

		DEBUGX("confused command %s to %f %f",m_command.m_type.c_str(), newgoal.m_x, newgoal.m_y);
	}
	
}

void Creature::calcWalkDir(Vector goal,WorldObject* goalobj)
{
	// maximale Entfernung bei der normale Wegfindung genommen wird
	int pathmaxdist = 16;

	// Qualitaet der Suche, geht quadratisch in die Laufzeit ein
	int qual=4;

	float recalctime = 500;

	bool highquality = true;

	if (getType() == "MONSTER")
	{
		highquality = false;
		pathmaxdist = 16;
		qual = 2;
		recalctime = 1000;
	}

	// eigene Position
	Vector& pos = getShape()->m_center;
	Vector dir;

	// wenn als Ziel ein Lebenwesen angegeben ist
	if (goalobj !=0 && goalobj->isCreature() )
	{
		DEBUGX("using pot field of object %i",goalobj->getId());
		Creature* cr = (Creature*) goalobj;
		// Potentialfeld des Ziellebewesens verwenden
		cr->getPathDirection(pos,getRegionId(), 2*getShape()->m_radius, getLayer(),dir);
	}
	else
	{
		DEBUGX("using own pot field");
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
			DEBUGX("allocating new pot field");
			m_path_info = new PathfindInfo;


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
			m_path_info->m_region=getRegionId();
			m_path_info->m_base_size = getShape()->m_radius*2;
			m_path_info->m_quality=qual;
			m_path_info->m_id = getId();
			// neu berechnen noetig
			recalc = true;

		}
		else
		{

			if ( m_path_info->m_region != getRegionId())
			{
				// Ziel befindet sich in einer anderen Region als die des Potentialfeldes
				// Region anpassen, neu berechnen
				m_path_info->m_region=getRegionId();
				recalc = true;
			}

			// Ziel hat sich deutlich bewegt, neuen Weg suchen
			float d = m_path_info->m_start.distanceTo(goal);
			if (d>1)
				recalc = true;

			// Potentialfeld ist aelter als 500 ms, neu berechnen
			if (m_path_info->m_timer > recalctime)
				recalc = true;

		}

		if (!direct)
		{
			if (recalc)
			{
				// neu berechnen notwendig
				DEBUGX("recalc walk field");

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
			DEBUGX("using direct way");
			dir = goal - pos;
		}

	}

	// Richtung normieren
	dir.normalize();

	if (dir.getLength() ==0)
	{
		clearCommand(true,false);
		m_action.m_type = "noaction";
		m_action.m_elapsed_time =0;
		m_command.m_damage_mult=1;
		addToNetEventMask(NetEvent::DATA_COMMAND | NetEvent::DATA_ACTION);
		return;
	}
	else
	{
		// TODO: Wende über 90 Grad behandeln
		Vector oldspeed = getSpeed();
		if (oldspeed * dir <0)
		{
			m_pathfind_counter +=10;
			DEBUGX("counter %f",m_pathfind_counter);
			if (m_pathfind_counter >= 50)
			{
				clearCommand(false,true);
				m_action.m_type = "noaction";
				m_action.m_elapsed_time =0;
				m_command.m_damage_mult=1;
				addToNetEventMask(NetEvent::DATA_COMMAND | NetEvent::DATA_ACTION);
			}
		}
		else
		{
			m_pathfind_counter -=8;
			if (m_pathfind_counter<0)
				m_pathfind_counter =0;
		}

		setSpeed(dir);

	}
}

void Creature::clearCommand(bool success, bool norepeat)
{
	DEBUGX("%s clear command %s %f",getRefName().c_str(),m_command.m_type.c_str() , m_script_command_timer);
	if (hasScriptCommand() && m_command.m_type!= "noaction")
	{
		m_script_command_timer =0;
		DEBUGX("command %s by %s ended with success %i",m_command.m_type.c_str(), getRefName().c_str(), success);
		Trigger* tr = new Trigger("command_complete");
		tr->addVariable("unit",getId());
		tr->addVariable("command",m_command.m_type);
		tr->addVariable("success",success);
		tr->addVariable("last_command",!(hasScriptCommand()));
		getRegion()->insertTrigger(tr);

		// ein wiederholtes Kommando bleibt in der Script Kommand Queue
		// und muss deshalb auch von dort entfernt werden
		if (norepeat && (m_command.m_flags & Command::REPEAT) && !m_script_commands.empty())
		{
			m_script_commands.pop_front();
		}

		if (m_script_commands.empty())
		{
			Trigger* tr = new Trigger("all_commands_complete");
			tr->addVariable("unit",getId());
			tr->addVariable("command",m_command.m_type);
			tr->addVariable("success",success);
			getRegion()->insertTrigger(tr);
		}
	}
	m_command.m_type = "noaction";
	m_command.m_damage_mult = 1;
	m_command.m_goal = Vector(0,0);
	m_command.m_goal_object_id =0;
	m_command.m_range =1;
	m_command.m_flags =0;
	m_script_command_timer =0;
	addToNetEventMask(NetEvent::DATA_COMMAND);
}

bool Creature::update (float time)
{

	WorldObject::update(time);

	Timer timer;
	timer.start();

	if (m_action.m_elapsed_time> m_action.m_time)
	{
		DEBUG("elapsed time %f all time %f",m_action.m_elapsed_time,	m_action.m_time);
	}

	DEBUGX("Update des Creatureobjekts [%i] wird gestartet", getId());
	// interne Variable um Fehler zu speichern
	bool result=true;


	// Timer fuer Sprache anpassen
	if (!m_speak_text.empty())
	{
		if (m_speak_text.m_time < time)
		{
			m_speak_text.clear();
		}
		else
		{
			m_speak_text.m_time -= time;
			m_speak_text.m_displayed_time += time;
		}
	}



	// Timer herunterzaehlen lassen
	for (int i=0; i<NR_TIMERS; i++)
	{
		m_timers[i] -= time;
		if (m_timers[i] < 0)
			m_timers[i] = 0;
	}


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
		if (m_dyn_attr.m_status_mod_time[i] >0)
		{
			m_dyn_attr.m_status_mod_time[i] -= time;
			if (m_dyn_attr.m_status_mod_time[i]<0)
			{
				// Statusmod beenden
				m_dyn_attr.m_status_mod_time[i]=0;
				// aktuelle Aktion abbrechen nach auslaufen von Berserker / verwirrt
				// (da die Aktion idr ungewollt bzw ungeplant ist)
				if (i==Damage::BERSERK || i==Damage::CONFUSED)
				{
					m_command.m_type = "noaction";
					addToNetEventMask(NetEvent::DATA_COMMAND);
				}
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
			DEBUGX("removing base attr mod");
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
		DEBUGX("modifizierte Attribute neu berechnen");
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
			d.m_special_flags |= Damage::NOVISUALIZE;

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
				if ((*it)->isCreature())
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
				// Schaden pro Sekunde 1/90 der HP
				DEBUGX("poisoned");
				m_dyn_attr.m_health -= 500*m_base_attr_mod.m_max_health / 90000;
				addToNetEventMask(NetEvent::DATA_HP);
			}

			// brennend
			if (m_dyn_attr.m_status_mod_time[Damage::BURNING]>0)
			{
				// Schaden pro Sekunde 1/60 der HP (bei 0 Feuerresistenz)
				DEBUGX("burning");
				m_dyn_attr.m_health -= (100-m_base_attr_mod.m_resistances[Damage::FIRE])*500*m_base_attr_mod.m_max_health / 6000000;
				addToNetEventMask(NetEvent::DATA_HP);
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
				DEBUGX("health %f",m_dyn_attr.m_health);
				if (m_dyn_attr.m_health <= 0)
				{
					die();
				}
				else
				{
					// Solange noch Zeit zur Verfügung steht
					DEBUGX("Objekt aktiv");

					while (time>0)
					{
						// Wenn keine Aktion gesetzt Aktion ausrechnen und initialisieren
						if (m_action.m_type == "noaction")
						{
							calcAction();
							initAction();
							calcDamage(m_action.m_type,m_damage);
						}

						// Aktion ausfuehren
						performAction(time);

					}
				}


				break;


			// sonst, eventuell hier später weitere Stati gesondert bearbeiten STATES_DEAD, STATES_INACTIVE
 			case STATE_DIEING:
				performAction(time);
				if (m_action.m_type == "noaction")
				{
					setState(STATE_DEAD);
					m_action.m_type = "dead";
					m_action.m_time = 1000;
					// Fliegende Objekte bleiben nicht lange liegen
					if ((getLayer() & LAYER_BASE) ==0)
					{
						m_action.m_time = 50;
					}

					addToNetEventMask(NetEvent::DATA_ACTION);

					Trigger* tr = new Trigger("unit_dead");
					tr->addVariable("unit",getId());
					getRegion()->insertTrigger(tr);
				}
				break;

			case STATE_DEAD:
				performAction(time);
				if (m_action.m_type == "noaction")
				{
					setDestroyed(true);
				}

			default:
				DEBUGX("unknown state: %i",getState());
				time=0 ;
		}

	}

	if (timer.getTime()>10)
	{
		DEBUGX("object %i update time %f",getId(), timer.getTime());
	}
	return result;
}



void Creature::gainExperience (float exp)
{
	if (getType() != "PLAYER")
		return;


	// Erfahrung dazu addieren
	m_dyn_attr.m_experience += exp;
	addToNetEventMask(NetEvent::DATA_EXPERIENCE);

	// Solange Level aufsteigen, bis exp < max_exp
	while (m_dyn_attr.m_experience>= m_base_attr.m_max_experience)
	{
		gainLevel();
	}
}

void Creature::gainLevel()
{
	addToNetEventMask(NetEvent::DATA_ATTRIBUTES_LEVEL);

}

void  Creature::calcActionAttrMod(Action::ActionType act,CreatureBaseAttrMod & bmod, CreatureDynAttrMod& dmod)
{
	bmod.init();
	dmod.init();

	Action::ActionInfo* ainfo = Action::getActionInfo(m_action.m_type);
	if (ainfo ==0 )
		return;

	// Cpp Implementation fuer Bmod
	std::list<std::string>::iterator kt;
	for (kt = ainfo->m_base_mod.m_cpp_impl.begin(); kt != ainfo->m_base_mod.m_cpp_impl.end(); ++kt)
	{
		if (*kt == "flamesword")
		{
			bmod.m_xspecial_flags |= (FLAMESWORD | FIRESWORD);
		}
		else if (*kt == "firesword")
		{
			bmod.m_xspecial_flags |= FIRESWORD;
		}
		else if (*kt == "flamearmor")
		{
			bmod.m_xspecial_flags |= FLAMEARMOR;
		}
		else if (*kt == "crit_hits")
		{
			bmod.m_xspecial_flags |= CRIT_HITS;
		}
		else if (*kt == "ice_arrows")
		{
			bmod.m_xspecial_flags |= ICE_ARROWS;
		}
		else if (*kt == "frost_arrows")
		{
			bmod.m_xspecial_flags |= (ICE_ARROWS | FROST_ARROWS);
		}
		else if (*kt == "wind_arrows")
		{
			bmod.m_xspecial_flags |= WIND_ARROWS;
		}
		else if (*kt == "storm_arrows")
		{
			bmod.m_xspecial_flags |= (WIND_ARROWS | STORM_ARROWS);
		}
		else if (*kt == "wind_walk")
		{
			bmod.m_xspecial_flags |= WIND_WALK;
		}
		else if (*kt == "burning_rage")
		{
			bmod.m_xspecial_flags |=  BURNING_RAGE;
		}
		else if (*kt == "static_shield")
		{
			bmod.m_xspecial_flags |= STATIC_SHIELD;
		}
	}

	// Lua Implementation fuer bmod
	if (ainfo->m_base_mod.m_lua_impl != LUA_NOREF)
	{

		EventSystem::setRegion(getRegion());
		EventSystem::setCreatureBaseAttrMod(&bmod);

		lua_pushnumber(EventSystem::getLuaState(),getId());
		lua_setglobal(EventSystem::getLuaState(), "self");

		EventSystem::executeCodeReference(ainfo->m_base_mod.m_lua_impl);

		EventSystem::setCreatureBaseAttrMod(0);
	}

	// Cpp Implementation fuer dmod
	for (kt = ainfo->m_dyn_mod.m_cpp_impl.begin(); kt != ainfo->m_dyn_mod.m_cpp_impl.end(); ++kt)
	{


	}

	// Lua Implementation fuer bmod
	if (ainfo->m_dyn_mod.m_lua_impl != LUA_NOREF)
	{

		EventSystem::setRegion(getRegion());
		EventSystem::setCreatureDynAttrMod(&dmod);

		lua_pushnumber(EventSystem::getLuaState(),getId());
		lua_setglobal(EventSystem::getLuaState(), "self");

		EventSystem::executeCodeReference(ainfo->m_dyn_mod.m_lua_impl);

		EventSystem::setCreatureDynAttrMod(0);
	}

	/*
	if (act == "scare")
	{

			bmod.m_time =60000;
			bmod.m_darmor = getBaseAttrMod()->m_armor;
	}
	else if (act == "berserk")
	{
			// Nur eine Schaetzung !
			bmod.m_dwalk_speed = -1000;
			bmod.m_dattack_speed = -500;
	}
	else if (act == "flamesword")
	{

			// Modifikationen:
			// Flags fuer 120 sec
			bmod.m_time =120000;
			bmod.m_xspecial_flags |= (FLAMESWORD | FIRESWORD);
	}
	else if (act == "firesword")
	{
			// Modifikationen:
			// Flag fuer 60 sec
			bmod.m_time =60000;
			bmod.m_xspecial_flags |=  FIRESWORD;
	}
	else if (act == "flamearmor")
	{
			// Modifikationen:
			// Flag, 50% Feuerres,25% Feuermaxres fuer 60 sec
			bmod.m_time = 100000;
			bmod.m_xspecial_flags |= FLAMEARMOR;
			bmod.m_dresistances_cap[Damage::FIRE] = 25;
			bmod.m_dresistances[Damage::FIRE] = 50;
	}
	else if (act == "anger")
	{

			// Modifikationen:
			// doppelte Staerke, halbierte Ruestung, Berserker fuer 30 sec
			bmod.m_time = 40000;
			bmod.m_dstrength =m_base_attr.m_strength;
			bmod.m_darmor = -m_base_attr_mod.m_armor /2;
	}
	else if (act == "regenerate")
	{
		dmod.m_dhealth = 0.5f*m_base_attr_mod.m_max_health;
	}
	else if (act == "fury")
	{


			// Modifikationen:
			// doppelte Staerke, -25% Ruestung, Berserker, erhoehte Angriffsgeschwindigkeit fuer 80 sec
			bmod.m_time = 40000;
			bmod.m_dstrength =m_base_attr.m_strength;
			bmod.m_darmor = -m_base_attr_mod.m_armor /4;
			bmod.m_dattack_speed = 1000;
	}
	else if (act == "static_shield")
	{
			// Modifikation:
			// Flag fuer 18 Sekunden
			bmod.m_time =18000;
			bmod.m_xspecial_flags |= (STATIC_SHIELD);
	}
	else if (act == "aimed_shot")
	{			// Modifikationen:
			// 50% mehr Geschick fuer 50 sec
			bmod.m_time = 50000;
			bmod.m_ddexterity =m_base_attr.m_dexterity/2;
	}
	else if (act == "bow_spirit")
	{
			// Modifikationen
			// 50% mehr Geschick, Flag fuer 50 sec
			bmod.m_time = 50000;
			bmod.m_ddexterity =m_base_attr.m_dexterity/2;
			bmod.m_xspecial_flags |= CRIT_HITS;
	}
	else if (act == "ice_arrows")
	{
			// Modifikationen:
			// Flag fuer 80 sec
			bmod.m_time = 80000;
			bmod.m_xspecial_flags |= ICE_ARROWS;
	}
	else if (act == "freezing_arrows")
	{
			// Modifikationen:
			// Flags fuer 80 sec
			bmod.m_time = 80000;
			bmod.m_xspecial_flags |= (ICE_ARROWS | FROST_ARROWS);
	}
	else if (act == "wind_arrows")
	{
			// Modifikationen:
			// Flag fuer 80 sec
			bmod.m_time = 80000;
			bmod.m_xspecial_flags |= WIND_ARROWS;
	}
	else if (act == "storm_arrows")
	{
			// Modifikationen:
			// Flags fuer 80 sec
			bmod.m_time = 80000;
			bmod.m_xspecial_flags |= (WIND_ARROWS | STORM_ARROWS);
	}
	else if (act == "wind_walk")
	{
			// Modifikationen:
			// Flag, erhoehte Laufgeschwindigkeit fuer 25 sec
			bmod.m_time = 25000;
			bmod.m_xspecial_flags |= WIND_WALK;
			bmod.m_dwalk_speed = 1500;
	}
	else if (act == "burning_rage")
	{
			// Modifikationen:
			// Flag fuer 80 sec
			bmod.m_time =80000;
			bmod.m_xspecial_flags |= BURNING_RAGE;
	}
	else if (act == "cure_blind_mute")
	{
			// Modifikation:
			// heilt blind/stumm (wirkt nur auf Verbuendete)
			dmod.m_dstatus_mod_immune_time[Damage::BLIND] = 1;
			dmod.m_dstatus_mod_immune_time[Damage::MUTE] = 1;
	}
	else if (act == "blazing_shield")
	{
			// Modifikationen
			// 50% der Willenskraft auf Blockwert fuer 60 sec
			bmod.m_time =60000;
			bmod.m_dblock = m_base_attr_mod.m_willpower /2;
	}
	else if (act == "cure_blind_mute_party")
	{
			// Modifikation:
			// heilt blind/stumm, immunisiert fuer 30 sec (wirkt nur auf Verbuendete)
			dmod.m_dstatus_mod_immune_time[Damage::BLIND] = 30000;
			dmod.m_dstatus_mod_immune_time[Damage::MUTE] = 30000;
	}
	else if (act == "magic_shield")
	{
			// Modifikation:
			// 50% mehr Willenskraft fuer 90 sec
			bmod.m_time =90000;
			bmod.m_dwillpower = m_base_attr_mod.m_willpower /2;
	}
	else if (act == "cure_pois_burn")
	{
			// Modifikation:
			// heilt vergiftet/brennend (wirkt nur auf Verbuendete)
			dmod.m_dstatus_mod_immune_time[Damage::POISONED] = 1;
			dmod.m_dstatus_mod_immune_time[Damage::BURNING] = 1;
	}
	else if (act == "cure_pois_burn_party")
	{
			// Modifikation:
			// heilt vergiftet/brennend und immunisiert fuer 30 sec (wirkt nur auf Verbuendete)
			dmod.m_dstatus_mod_immune_time[Damage::POISONED] = 30000;
			dmod.m_dstatus_mod_immune_time[Damage::BURNING] = 30000;
	}
	else if (act == "blade_storm")
	{
			// Modifikation:
			// Angriffsgeschwindigkeit erhoeht fuer 70 sec
			bmod.m_time =70000;
			bmod.m_dattack_speed = 600;
	}
	else if (act == "cure_conf_bsrk")
	{
			// Modifikation:
			// heilt verwirrt/Berserker (wirkt nur auf Verbuendete)
			dmod.m_dstatus_mod_immune_time[Damage::CONFUSED] = 1;
			dmod.m_dstatus_mod_immune_time[Damage::BERSERK] = 1;
	}
	else if (act == "cure_conf_bsrk_party")
	{
			// Modifikation:
			// heilt verwirrt/Berserker und immunisiert fuer 30 sec (wirkt nur auf Verbuendete)
			dmod.m_dstatus_mod_immune_time[Damage::CONFUSED] = 30000;
			dmod.m_dstatus_mod_immune_time[Damage::BERSERK] = 30000;
	}
	else if (act == "keen_mind")
	{
			// Modifikation:
			// 50% der Willenskraft auf Magie
			bmod.m_time =70000;
			bmod.m_dmagic_power = m_base_attr_mod.m_willpower/2;
	}
	else if (act == "heal")
	{
			dmod.m_dhealth = 3* m_base_attr_mod.m_willpower;
	}
	else if (act == "heal_party")
	{
			dmod.m_dhealth = 3* m_base_attr_mod.m_willpower;
	}
	*/
}

void Creature::calcDamage(Action::ActionType act,Damage& dmg)
{
	// alles nullen
	dmg.init();

	dmg.m_attacker_id = getId();
	dmg.m_attacker_fraction = m_fraction;

	for (int i=0;i<4;i++)
		dmg.m_multiplier[i]=1;

	if (act == "noaction")
		return;

	DEBUGX("Calc Damage for action %s",act.c_str());

	Action::ActionInfo* ainfo = Action::getActionInfo(act);
	if (ainfo ==0 )
		return;

	std::list<std::string>::iterator kt;
	for (kt = ainfo->m_damage.m_cpp_impl.begin(); kt != ainfo->m_damage.m_cpp_impl.end(); ++kt)
	{

		// Schaden durch Basisattribute und ggf Ausruestung
		calcBaseDamage(*kt,dmg);

		// Modifikation des Schadens durch Faehigkeiten
		calcAbilityDamage(*kt,dmg);
	}

	if (ainfo->m_damage.m_lua_impl != LUA_NOREF)
	{
		EventSystem::setRegion(getRegion());
		EventSystem::setDamage(&dmg);

		lua_pushnumber(EventSystem::getLuaState(),getId());
		lua_setglobal(EventSystem::getLuaState(), "self");

		//Timer t;
		//t.start();
		EventSystem::executeCodeReference(ainfo->m_damage.m_lua_impl);
		//DEBUG("time %f",t.getTime());
		EventSystem::setDamage(0);

	}

	// Eigenschaften durch passive  Faehigkeiten
	if (checkAbility("critical_strike"))
	{
		// 10% extra Chance auf kritische Treffer
		dmg.m_crit_perc += 0.1;
	}

	// klirrende Kaelte
	if (checkAbility("chill"))
	{
		// 20% mehr Schaden
		dmg.m_multiplier[Damage::ICE] *= 1.2;
	}


	// Entzuenden
	if (checkAbility("inflame") && dmg.m_min_damage[Damage::FIRE]>0 )
	{
		dmg.m_status_mod_power[Damage::BURNING] += MathHelper::Min(int(dmg.m_min_damage[Damage::FIRE]) , m_base_attr_mod.m_magic_power);
	}


	// Eigenschaften durch temporaere Faehigkeiten

	// Faehigkeit Feuer und Schwert
	if (m_base_attr_mod.m_special_flags & FIRESWORD)
	{
		dmg.m_min_damage[Damage::PHYSICAL]*=0.75;
		dmg.m_max_damage[Damage::PHYSICAL]*=0.75;
		// Feuerschaden hinzufuegen
		dmg.m_min_damage[Damage::FIRE] += MathHelper::Min(m_base_attr_mod.m_magic_power*4.0f,dmg.m_min_damage[Damage::PHYSICAL]);
		dmg.m_max_damage[Damage::FIRE] += MathHelper::Min(m_base_attr_mod.m_magic_power*6.0f,dmg.m_max_damage[Damage::PHYSICAL]);

		// kein Eisschaden
		dmg.m_min_damage[Damage::ICE]=0;
		dmg.m_max_damage[Damage::ICE]=0;

		if (m_base_attr_mod.m_special_flags & FLAMESWORD)
		{
			// Statusmod brennend austeilen
			dmg.m_status_mod_power[Damage::BURNING] += m_base_attr_mod.m_magic_power*3;
		}

		dmg.m_multiplier[Damage::FIRE] *= dmg.m_multiplier[Damage::PHYSICAL];
		dmg.m_multiplier[Damage::PHYSICAL]=1;
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
		dmg.m_min_damage[Damage::ICE] += MathHelper::Min(m_base_attr_mod.m_magic_power*4.0f,dmg.m_min_damage[Damage::PHYSICAL]);
		dmg.m_max_damage[Damage::ICE] += MathHelper::Min(m_base_attr_mod.m_magic_power*6.0f,dmg.m_max_damage[Damage::PHYSICAL]);
		dmg.m_min_damage[Damage::PHYSICAL]*=0.5;
		dmg.m_max_damage[Damage::PHYSICAL]*=0.5;
		dmg.m_min_damage[Damage::FIRE]=0;
		dmg.m_max_damage[Damage::FIRE]=0;

		if (m_base_attr_mod.m_special_flags & FROST_ARROWS)
		{
			// Statusmod einfrieren
			dmg.m_status_mod_power[Damage::FROZEN] += m_base_attr_mod.m_magic_power*3;
		}
		dmg.m_multiplier[Damage::ICE] *= dmg.m_multiplier[Damage::PHYSICAL];
	}

	// Windpfeile
	if (m_base_attr_mod.m_special_flags & WIND_ARROWS)
	{
		dmg.m_min_damage[Damage::AIR] += MathHelper::Min(m_base_attr_mod.m_magic_power*4.0f,dmg.m_min_damage[Damage::PHYSICAL]);
		dmg.m_max_damage[Damage::AIR] += MathHelper::Min(m_base_attr_mod.m_magic_power*6.0f,dmg.m_max_damage[Damage::PHYSICAL]);
		dmg.m_min_damage[Damage::PHYSICAL]*=0.5;
		dmg.m_max_damage[Damage::PHYSICAL]*=0.5;

		if (m_base_attr_mod.m_special_flags & STORM_ARROWS)
		{
			// Statusmod paralyze
			dmg.m_status_mod_power[Damage::PARALYZED] += m_base_attr_mod.m_magic_power*2;
			dmg.m_multiplier[Damage::AIR] *= 1.4;
		}
		dmg.m_multiplier[Damage::AIR] *= dmg.m_multiplier[Damage::PHYSICAL];
	}

	// Statusmods
	// Blind
	if (m_dyn_attr.m_status_mod_time[Damage::BLIND]>0)
	{
		// keine kritischen Treffer, Attackewert reduzieren
		dmg.m_crit_perc=0;
		dmg.m_attack *= 0.5;
	}

	// verwirrt
	if (m_dyn_attr.m_status_mod_time[Damage::CONFUSED]>0)
	{
		// Fraktion auf feindlich gegen alle setzen
		dmg.m_attacker_fraction = Fraction::HOSTILE_TO_ALL;
	}

	// Faehigkeit brennende Wut
	if (getBaseAttrMod()->m_special_flags & BURNING_RAGE)
	{
		// 50% mehr physischen Schaden
		dmg.m_multiplier[Damage::PHYSICAL] *= 1.5;
	}
}

void Creature::calcBaseDamage(std::string impl ,Damage& dmg)
{
	CreatureBaseAttr* basm = getBaseAttrMod();
	if (impl == "attack")
	{
		// Basisaktion ist normaler Angriff
		DEBUGX("base str %i mod str %i",m_base_attr.m_strength,basm->m_strength);

		dmg.m_min_damage[Damage::PHYSICAL] += basm->m_strength/4;
		dmg.m_min_damage[Damage::PHYSICAL] += basm->m_dexterity/10;
		dmg.m_max_damage[Damage::PHYSICAL] += basm->m_strength/3;
		dmg.m_max_damage[Damage::PHYSICAL] += basm->m_dexterity/8;


		dmg.m_attack += basm->m_attack;
		dmg.m_attack += basm->m_dexterity/2;

		dmg.m_power += basm->m_power;
		dmg.m_power += basm->m_strength/2;

		dmg.m_special_flags = Damage::NOFLAGS;

	}

	if (impl == "magic_attack")
	{
		// Basisaktion ist magischer Angriff
		dmg.m_min_damage[Damage::FIRE] += basm->m_magic_power/10;
		dmg.m_min_damage[Damage::FIRE] += basm->m_willpower/20;
		dmg.m_max_damage[Damage::FIRE] += basm->m_magic_power/6;
		dmg.m_max_damage[Damage::FIRE] += basm->m_willpower/15;
		dmg.m_special_flags |= Damage::UNBLOCKABLE | Damage::IGNORE_ARMOR;
		dmg.m_attack=0;

	}

	if (impl == "magic_attack_fire")
	{
		// Basisaktion ist magischer Angriff
		dmg.m_min_damage[Damage::FIRE] += basm->m_magic_power/6;
		dmg.m_max_damage[Damage::FIRE] += basm->m_magic_power/3;
		dmg.m_special_flags |= Damage::UNBLOCKABLE | Damage::IGNORE_ARMOR;
		dmg.m_attack=0;
	}

	if (impl == "magic_attack_ice")
	{
		// Basisaktion ist magischer Angriff
		dmg.m_min_damage[Damage::ICE] += basm->m_magic_power/5;
		dmg.m_max_damage[Damage::ICE] += basm->m_magic_power/4;
		dmg.m_special_flags |= Damage::UNBLOCKABLE | Damage::IGNORE_ARMOR;
		dmg.m_attack=0;
	}

	if (impl == "magic_attack_air")
	{
		// Basisaktion ist magischer Angriff
		dmg.m_min_damage[Damage::AIR] += basm->m_magic_power/8;
		dmg.m_max_damage[Damage::AIR] += basm->m_magic_power/2;
		dmg.m_special_flags |= Damage::UNBLOCKABLE | Damage::IGNORE_ARMOR;
		dmg.m_attack=0;
	}

	if (impl == "range_attack")
	{
		// Basisaktion ist Fernangriff
		dmg.m_min_damage[Damage::PHYSICAL] += basm->m_strength/10;
		dmg.m_min_damage[Damage::PHYSICAL] += basm->m_dexterity/6;
		dmg.m_max_damage[Damage::PHYSICAL] += basm->m_strength/8;
		dmg.m_max_damage[Damage::PHYSICAL] += basm->m_dexterity/4;

		dmg.m_attack += basm->m_attack;
		dmg.m_attack += basm->m_dexterity/2;

		dmg.m_power += basm->m_power;
		dmg.m_power += basm->m_strength/2;

		dmg.m_special_flags = Damage::NOFLAGS;

	}

	if (impl == "holy_attack")
	{
		// Basisaktion ist heiliger Angriff
		dmg.m_min_damage[Damage::PHYSICAL] += basm->m_strength/9;
		dmg.m_min_damage[Damage::PHYSICAL] += basm->m_willpower/6;
		dmg.m_max_damage[Damage::PHYSICAL] += basm->m_strength/6;
		dmg.m_max_damage[Damage::PHYSICAL] += basm->m_willpower/3;

		dmg.m_attack += basm->m_attack;
		dmg.m_attack += basm->m_dexterity/2;

		dmg.m_power += basm->m_power;
		dmg.m_power += basm->m_strength/2;

		dmg.m_special_flags = Damage::NOFLAGS;

	}
}

void Creature::calcAbilityDamage(std::string impl, Damage& dmg)
{
	// CreatureBaseAttr* basm = getBaseAttrMod();

	if (impl == "charge")
	{
		dmg.m_multiplier[Damage::PHYSICAL] *= m_command.m_damage_mult;
		dmg.m_attack *=m_command.m_damage_mult*0.5;
	}
	else if (impl == "storm_charge")
	{

		dmg.m_multiplier[Damage::PHYSICAL] *= m_command.m_damage_mult;
		dmg.m_attack *=m_command.m_damage_mult*0.5;
		dmg.m_status_mod_power[Damage::PARALYZED] += (short)  (m_base_attr_mod.m_strength*m_command.m_damage_mult*0.2);
	}
	else if (impl == "small_numbers")
	{
		dmg.m_special_flags |= Damage::VISUALIZE_SMALL;
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
	m_base_attr_mod.m_power =m_base_attr.m_power;
	m_base_attr_mod.m_strength =m_base_attr.m_strength;
	m_base_attr_mod.m_dexterity =m_base_attr.m_dexterity;
	m_base_attr_mod.m_willpower =m_base_attr.m_willpower;
	m_base_attr_mod.m_magic_power =m_base_attr.m_magic_power;
	m_base_attr_mod.m_walk_speed =m_base_attr.m_walk_speed;
	m_base_attr_mod.m_attack_speed =m_base_attr.m_attack_speed;
	m_base_attr_mod.m_attack_range =m_base_attr.m_attack_range;
	m_base_attr_mod.m_level =m_base_attr.m_level;
	m_base_attr_mod.m_max_health =m_base_attr.m_max_health;
	m_base_attr_mod.m_immunity =m_base_attr.m_immunity;

	m_dyn_attr.m_health = hppercent * m_base_attr.m_max_health;

	for (i=0;i<4;i++)
	{
		m_base_attr_mod.m_resistances[i] = m_base_attr.m_resistances[i];
		m_base_attr_mod.m_resistances_cap[i] = m_base_attr.m_resistances_cap[i];

	}

	m_base_attr_mod.m_abilities = m_base_attr.m_abilities;


	m_base_attr_mod.m_special_flags = m_base_attr.m_special_flags;

	// Alle Modifikationen neu anwenden, aber nicht neu in die Liste aufnehmen
	std::list<CreatureBaseAttrMod>::iterator j;
	for (j=m_dyn_attr.m_temp_mods.begin(); j!= m_dyn_attr.m_temp_mods.end();++j)
	{
		DEBUGX("%f / %f",getDynAttr()->m_health, getBaseAttr()->m_max_health);

		applyBaseAttrMod(&(*j),false);
	}

	// Wirkungen durch passive Faehigkeiten
	if (checkAbility("resist_ice"))
	{
		m_base_attr_mod.m_resistances[Damage::ICE] += 20;
		m_base_attr_mod.m_resistances[Damage::AIR] += 20;

	}

	if (checkAbility("resist_air"))
	{
		m_base_attr_mod.m_resistances[Damage::ICE] += 10;
		m_base_attr_mod.m_resistances[Damage::AIR] += 10;
		m_base_attr_mod.m_resistances_cap[Damage::ICE] += 10;
		m_base_attr_mod.m_resistances_cap[Damage::AIR] += 10;

	}


}


bool Creature::takeDamage(Damage* d)
{
	// Lebewesen kann nur im Zustand aktiv Schaden nehmen
	// und wenn es nicht gerade in einen Dialog verwickelt ist
	if (!canBeAttacked())
		return false;

	DEBUGX("take Damage %i",getId());
	// Testen ob der Verursacher des Schadens feindlich gesinnt ist
	if (World::getWorld()->getRelation(d->m_attacker_fraction,this) != Fraction::HOSTILE)
	{
		// Verursacher ist nicht feindlich, kein Schaden
		DEBUGX("not hostile, no dmg");
		DEBUGX("fractions %i %i",d->m_attacker_fraction, this->getFraction());
		return false;
	}


	// Testen ob man selbst der Verursacher ist
	// (man kann sich selbst generell nicht schaden)
	if (d->m_attacker_id == getId())
	{
		return false;
	}

	bool blocked = false;

	float dmg=0,dmgt;
	short res;
	float rez = 1.0/RAND_MAX;
	// testen ob der Schaden geblockt wird
	float blockchance =0;
	if (!(d->m_special_flags & Damage::UNBLOCKABLE))
	{
		float block = m_base_attr_mod.m_block  ;
		if (m_dyn_attr.m_status_mod_time[Damage::BLIND]>0)
		{
			block *= 0.5;
		}

		// Chance zu blocken
		if (d->m_attack>0 && block>0)
		{
			blockchance = 1-atan(d->m_attack/block)/(3.1415/2);


			DEBUGX("attack %f block %f -> blockchance %f",d->m_attack,block, blockchance);
			if (Random::random()<blockchance)
			{
				// Schaden abgewehrt
				blocked = true;
			}
		}



	}

	m_dyn_attr.m_last_attacker_id = d->m_attacker_id;

	// Wirkungen durch passive Faehigkeiten
	if (checkAbility("concentration"))
	{
		d->m_status_mod_power[Damage::CONFUSED]=0;
	}

	if (checkAbility("mental_wall"))
	{
		d->m_status_mod_power[Damage::BERSERK]=0;
	}

	// Testen auf kritische Treffer
	bool critical = false;
	if (rand()*rez <d->m_crit_perc)
	{
		DEBUGX("critical");
		critical = true;
		d->m_multiplier[Damage::PHYSICAL] *= 3;
	}


	// Berechnen des Schadens

	// physischer Schaden
	dmgt = d->m_min_damage[Damage::PHYSICAL] + rand()*rez *(d->m_max_damage[Damage::PHYSICAL] -d->m_min_damage[Damage::PHYSICAL]);
	dmgt *= d->m_multiplier[Damage::PHYSICAL];

	// Resistenz anwenden
	res = MathHelper::Min (m_base_attr_mod.m_resistances_cap[Damage::PHYSICAL],m_base_attr_mod.m_resistances[Damage::PHYSICAL]);
	dmgt *= 0.01*(100-res);

	// Ruestung anwenden
	float armor = m_base_attr_mod.m_armor;

	// Faehigkeit Turm in der Schlacht
	float physfaktor = 1.0;
	if (checkAbility("steadfast"))
	{
		float maxhealth = getBaseAttrMod()->m_max_health;
		physfaktor = 1.0 - 0.5*(maxhealth - m_dyn_attr.m_health)/maxhealth;
	}
	dmgt *= physfaktor;

	float armorfak = 1.0;
	if (armor>0 && !(d->m_special_flags & Damage::IGNORE_ARMOR) && dmgt>0)
	{
		armorfak = atan(d->m_power/armor)/(3.1415/2);
		DEBUGX("power %f armor %f -> damage perc. %f",d->m_power, armor, armorfak);
		dmgt = dmgt*armorfak;
	}

	dmg += dmgt;

	DEBUGX("phys dmg %f",dmgt);

	// Daten um Trefferwahrscheinlichkeiten / Schadensreduktion anzuzeigen
	WorldObject* wo = getRegion()->getObject(d->m_attacker_id);
	FightStatistic* fstat= &(getFightStatistic());
	FightStatistic* attfstat=0;
	Creature* attacker = dynamic_cast<Creature*>(getRegion()->getObject(d->m_attacker_id));
	if (wo !=0  && wo->getType() == "PLAYER")
	{
		attfstat = &(attacker->getFightStatistic());
	}

	// eigene Daten setzen
	if (wo !=0 && getType()=="PLAYER")
	{
		if (fabs(fstat->m_block_chance - blockchance) >0.01
				  || fabs(fstat->m_damage_got_perc - armorfak) >0.01
				  || fstat->m_last_attacker != wo->getName())
		{
			fstat->m_block_chance = blockchance;
			fstat->m_last_attacker = wo->getName();
			fstat->m_damage_got_perc = armorfak;
			addToNetEventMask(NetEvent::DATA_FIGHT_STAT);

			DEBUGX("blockchance %f damage perc %f attacker %s",blockchance, armorfak, fstat->m_last_attacker.getTranslation().c_str());
		}
	}

	// Daten des Angreifers setzen
	if (attacker !=0 && attacker->getType()=="PLAYER")
	{
		attacker->updateFightStat(1-blockchance, armorfak,getName());
	}

	// wenn Schaden geblockt wurde, hier beenden
	if (blocked)
	{
		FloatingText::Size size = FloatingText::NORMAL;
		if (d->m_special_flags & Damage::VISUALIZE_SMALL)
		{
			size =FloatingText::SMALL;
		}
		getRegion()->createFloatingText(TranslatableString("Miss"),getShape()->m_center,size);
		return true;
	}

	// restliche 3 Schadensarten
	int i;
	for (i=Damage::AIR;i<=Damage::FIRE;i++)
	{
		dmgt = d->m_min_damage[i] + rand()*rez *(d->m_max_damage[i] -d->m_min_damage[i]);
		dmgt *= d->m_multiplier[i];

		// Resistenz anwenden
		res = MathHelper::Min(m_base_attr_mod.m_resistances_cap[i],m_base_attr_mod.m_resistances[i]);
		dmgt *=0.01*(100-res);

		DEBUGX("dmg %i min %f max %f real %f",i,d->m_min_damage[i],d->m_max_damage[i],dmgt);
		dmg += dmgt;

	}

	float t;
	// Anwenden der Statusveraenderungen
	for (i=0;i<NR_STATUS_MODS;i++)
	{
		// Anwenden wenn nicht immun, nicht temporaer immun und Staerke der Modifikation groesser als die eigene Willenskraft
		if (d->m_status_mod_power[i]>0 && !(m_base_attr_mod.m_immunity & (1 << i)) && m_dyn_attr.m_status_mod_immune_time[i]==0)
		{
			float rel;
			if (m_base_attr_mod.m_willpower == 0)
			{
				rel = 1000000;
			}
			else
			{
				rel = d->m_status_mod_power[i]*1.0 / m_base_attr_mod.m_willpower;
			}
			float chance = atan(rel)/(3.1415/2);
			DEBUGX("mod %i modpow %i wp %i chance %f",i,d->m_status_mod_power[i],m_base_attr_mod.m_willpower,chance);
			if (Random::random()<chance)
			{
				// Modifikation anwenden
				float times[NR_STATUS_MODS] = {15000 /* BLIND*/,10000 /*POISONED*/ ,10000 /*BERSERK*/ ,8000 /*CONFUSED*/,15000 /*MUTE*/,4000 /*PARALYZED*/,2500 /*FROZEN*/,10000 /*BURNING*/};
				t = rel * times[i];
				if (t>m_dyn_attr.m_status_mod_time[i] && t>500)
				{
					m_dyn_attr.m_status_mod_time[i] =t;
					addToNetEventMask(NetEvent::DATA_STATUS_MODS);
				}

				DEBUGX("applying status mod %i for %f ms",i,t);
			}
		}
	}

	DEBUGX("sum dmg %f / %f",dmg,getDynAttr()->m_health );

	// Extraschaden berechnen
	if (d->m_extra_dmg_race == getRace() && getRace() != "")
	{
		DEBUGX("triple damage");
		dmg *= 3;
		critical = true;
	}

	// Lebenspunkte abziehen
	getDynAttr()->m_health -= dmg;

	if (dmg>0)
	{
		m_dyn_attr.m_effect_time[CreatureDynAttr::BLEEDING] = MathHelper::Max(m_dyn_attr.m_effect_time[CreatureDynAttr::BLEEDING],250.0f);
		addToNetEventMask(NetEvent::DATA_HP | NetEvent::DATA_EFFECTS);
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
		Projectile* pr = ObjectFactory::createProjectile("static_shield");
		if (pr !=0)
		{
			pr->setDamage(&dmg);
			pr->getShape()->m_radius =getShape()->m_radius+1;
			getRegion()->insertProjectile(pr,getShape()->m_center);
		}
	}



	// Trigger erzeugen
	Trigger* tr = new Trigger("unit_hit");
	tr->addVariable("defender",getId());
	tr->addVariable("attacker",d->m_attacker_id);
	tr->addVariable("damage",dmg);
	getRegion()->insertTrigger(tr);

	WorldObject::takeDamage(d);

	// Visualisierung erzeugen
	if (getRegion()!= 0 && World::getWorld()->isServer() && dmg>0.5 && !(d->m_special_flags & Damage::NOVISUALIZE))
	{
		FloatingText::Size size = FloatingText::NORMAL;
		if ((d->m_special_flags & Damage::VISUALIZE_SMALL) && !critical)
		{
			size =FloatingText::SMALL;
		}
		getRegion()->visualizeDamage(int(dmg + 0.5),getShape()->m_center,size);
	}

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
		addToNetEventMask( NetEvent::DATA_HP);
	}


	for (int i = 0;i< NR_STATUS_MODS;i++)
	{
		if (mod->m_dstatus_mod_immune_time[i]>0)
		{
			m_dyn_attr.m_status_mod_immune_time[i] = MathHelper::Max(m_dyn_attr.m_status_mod_immune_time[i],mod->m_dstatus_mod_immune_time[i]);
			m_dyn_attr.m_status_mod_time[i]=0;

			if (i==Damage::CONFUSED || i==Damage::BERSERK)
			{
				clearCommand(true,false);
				addToNetEventMask(NetEvent::DATA_COMMAND | NetEvent::DATA_ACTION);

			}
			addToNetEventMask(NetEvent::DATA_STATUS_MODS);
		}
	}
}

void Creature::applyBaseAttrMod(CreatureBaseAttrMod* mod, bool add)
{
	int i;
	// Deltawerte dazu addieren
	float oldmaxhp = m_base_attr_mod.m_max_health;

	if (m_base_attr_mod.m_dexterity + mod->m_ddexterity < 80)
	{
		m_base_attr_mod.m_attack_speed += mod->m_ddexterity*10;
	}
	else if (m_base_attr_mod.m_dexterity < 80)
	{
		m_base_attr_mod.m_attack_speed += (80-m_base_attr_mod.m_dexterity)*10;
	}

	m_base_attr_mod.m_armor +=mod->m_darmor;
	m_base_attr_mod.m_block +=mod->m_dblock;
	m_base_attr_mod.m_strength +=mod->m_dstrength;
	m_base_attr_mod.m_dexterity +=mod->m_ddexterity;
	m_base_attr_mod.m_willpower +=mod->m_dwillpower;
	m_base_attr_mod.m_magic_power +=mod->m_dmagic_power;
	m_base_attr_mod.m_walk_speed +=mod->m_dwalk_speed;
	m_base_attr_mod.m_max_health += mod->m_dmax_health;
	m_base_attr_mod.m_max_health += mod->m_dstrength*5;
	m_dyn_attr.m_health *= m_base_attr_mod.m_max_health /oldmaxhp;

	m_base_attr_mod.m_attack +=mod->m_dattack;
	m_base_attr_mod.m_power +=mod->m_dpower;

	m_base_attr_mod.m_attack_speed +=mod->m_dattack_speed;

	// Modifikationen feststellen
	if (mod->m_dwalk_speed!=0 )
	{
		addToNetEventMask(NetEvent::DATA_ATTACK_WALK_SPEED);
	}
	if (mod->m_dattack_speed !=0 || mod->m_ddexterity!=0)
	{
		addToNetEventMask(NetEvent::DATA_ATTACK_WALK_SPEED);
	}
	if (mod->m_dmax_health !=0 || mod->m_dstrength!=0)
	{
		addToNetEventMask(NetEvent::DATA_ATTRIBUTES_LEVEL | NetEvent::DATA_HP);
	}

	// einige Untergrenzen pruefen
	m_base_attr_mod.m_strength = MathHelper::Max(m_base_attr_mod.m_strength,(short) 1);
	m_base_attr_mod.m_dexterity = MathHelper::Max(m_base_attr_mod.m_dexterity,(short) 1);
	m_base_attr_mod.m_willpower = MathHelper::Max(m_base_attr_mod.m_willpower,(short) 1);
	m_base_attr_mod.m_magic_power = MathHelper::Max(m_base_attr_mod.m_magic_power,(short) 1);
	m_base_attr_mod.m_walk_speed = MathHelper::Max(m_base_attr_mod.m_walk_speed,(short) 200);
	m_base_attr_mod.m_attack_speed = MathHelper::Max(m_base_attr_mod.m_attack_speed,(short) 200);

	// Resistenzen dazu addieren
	for (i=0;i<4;i++)
	{
		m_base_attr_mod.m_resistances[i] += mod->m_dresistances[i];
		m_base_attr_mod.m_resistances_cap[i] += mod->m_dresistances_cap[i];

	}

	// Faehigkeiten mit OR hinzufuegen
	std::set<std::string>::iterator it;
	for (it = mod->m_xabilities.begin(); it != mod->m_xabilities.end(); ++it )
	{
		m_base_attr_mod.m_abilities[*it].m_time =0;
		addToNetEventMask(NetEvent::DATA_ABILITIES);
	}

	// Flags setzen
	if (mod->m_flag != "")
	{
		setFlag(mod->m_flag, true);
		addToNetEventMask(NetEvent::DATA_FLAGS);
	}

	// Flags mit OR hinzufuegen
	m_base_attr_mod.m_special_flags |= mod->m_xspecial_flags;
	m_base_attr_mod.m_immunity |= mod->m_ximmunity;

	if (mod->m_xspecial_flags!=0)
	{
		addToNetEventMask(NetEvent::DATA_FLAGS);
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

	if (m_base_attr_mod.m_dexterity + mod->m_ddexterity < 80)
	{
		m_base_attr_mod.m_attack_speed -= mod->m_ddexterity*10;
	}
	else if (m_base_attr_mod.m_dexterity < 80)
	{
		m_base_attr_mod.m_attack_speed -= (80-m_base_attr_mod.m_dexterity)*10;
	}


	m_dyn_attr.m_health *= m_base_attr_mod.m_max_health /oldmaxhp;

	m_base_attr_mod.m_attack -=mod->m_dattack;
	m_base_attr_mod.m_power -=mod->m_dpower;

	// Modifikationen feststellen
	if (mod->m_dwalk_speed!=0)
	{
		addToNetEventMask(NetEvent::DATA_ATTACK_WALK_SPEED);
	}
	if (mod->m_dattack_speed !=0 || mod->m_ddexterity!=0)
	{
		addToNetEventMask(NetEvent::DATA_ATTACK_WALK_SPEED);
	}
	if (mod->m_dmax_health !=0 || mod->m_dstrength!=0)
	{
		addToNetEventMask(NetEvent::DATA_ATTRIBUTES_LEVEL | NetEvent::DATA_HP);
	}

	for (i=0;i<4;i++)
	{
		m_base_attr_mod.m_resistances[i] -= mod->m_dresistances[i];
		m_base_attr_mod.m_resistances_cap[i] -= mod->m_dresistances_cap[i];

	}

	// Flags entfernen
	if (mod->m_flag != "")
	{
		setFlag(mod->m_flag, false);
		ret = true;
	}

	// Wenn Faehigkeit veraendert wurde neu berechnen
	if ( mod->m_xabilities.size() !=0)
	{
		ret = true;
		addToNetEventMask(NetEvent::DATA_ABILITIES);
	}

	// Wenn Flags veraendert wurden neu berechnen
	if (mod->m_xspecial_flags!=0)
	{
		addToNetEventMask(NetEvent::DATA_FLAGS);
		ret = true;
	}

	if( mod->m_ximmunity!=0)
		ret = true;

	// Schaden neu berechnen
	recalcDamage();

	return ret;
}

void Creature::removeAllBaseAttrMod()
{
	m_dyn_attr.m_temp_mods.clear();
	addToNetEventMask(NetEvent::DATA_FLAGS);
	addToNetEventMask(NetEvent::DATA_ABILITIES);
	addToNetEventMask(NetEvent::DATA_ATTRIBUTES_LEVEL | NetEvent::DATA_HP);
	addToNetEventMask(NetEvent::DATA_ATTACK_WALK_SPEED);

	calcBaseAttrMod();
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
		DEBUGX("switching to medium size info");
		pi = &m_medium_path_info;
		bsize=2;
	}
	if (base_size>2)
	{
		DEBUGX("switching to big size info");
		pi = &m_big_path_info;
		bsize=4;
	}

	if ((layer & LAYER_BASE) ==0)
	{
		DEBUGX("switching to flying info");
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
		(*pi)->m_region=getRegionId();
		(*pi)->m_base_size = bsize;
		(*pi)->m_quality=qual;
		(*pi)->m_id = getId();
		recalc = true;
		DEBUGX("recalc: no pathinfo so far");
	}
	else
	{
		if ( (*pi)->m_region != getRegionId())
		{
			// Wegfindeinformation gilt fuer eine andere Region als die, in der sich das Lebewesen befindet
			// Region neu setzen und neu berechnen
			(*pi)->m_region=getRegionId();
			recalc = true;
			DEBUGX("recalc: new in Region");
		}

		// Ziel befindet sich in einer anderen Region, keinen Weg angeben
		if ( region != getRegionId())
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
			DEBUGX("recalc: goal has moved");
			recalc = true;
		}

		// Wenn Informationen ueber 500ms alt neu berechnen
		if ((*pi)->m_timer >500)
		{
			DEBUGX("recalc: info too old");
			recalc = true;
		}

	}

	if (recalc && !direct)
	{
		// neu berechnen
		DEBUGX("recalc");
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
		DEBUGX("direct way");
		// Direkte Richtung
		// Vektor vom Start zum Ziel, normiert
		dir = goal - pos;
		dir.normalize();

	}
	else
	{
		DEBUGX("use pot field");
		// Potentialfeld verwenden um die Richtung zu berechnen
		World::getWorld()->calcPathDirection(*pi, pos, dir);
	}
}

void Creature::toString(CharConv* cv)
{
	DEBUGX("Creature::tostring");
	WorldObject::toString(cv);
	cv->toBuffer(m_refname);

	m_action.toString(cv);
	m_command.toString(cv);
	m_next_command.toString(cv);

	cv->toBuffer(m_dyn_attr.m_health);
	cv->toBuffer(m_base_attr_mod.m_max_health);
	DEBUGX("write offset: %i",cv->getBitStream()->GetNumberOfBitsUsed());



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
	int nr_timers = NR_TIMERS;
	if (cv->getVersion() < 17)
	{
		nr_timers = 2;
	}
	for (int i=0; i< nr_timers; i++)
	{
		cv->toBuffer(m_timers[i]);
		cv->toBuffer(m_timers_max[i]);
	}

	cv->toBuffer(static_cast<short>(getBaseAttrMod()->m_abilities.size()));
	std::map<std::string, AbilityInfo>::iterator it;
	for (it= getBaseAttrMod()->m_abilities.begin(); it != getBaseAttrMod()->m_abilities.end(); ++it)
	{
		cv->toBuffer(it->first);
		cv->toBuffer(it->second.m_timer_nr);
		cv->toBuffer(it->second.m_time);
		cv->toBuffer(it->second.m_timer);
	}

	cv->toBuffer(getSpeed().m_x);
	cv->toBuffer(getSpeed().m_y);

	cv->toBuffer(m_emotion_set);
}

void Creature::fromString(CharConv* cv)
{
	WorldObject::fromString(cv);
	cv->fromBuffer(m_refname);

	m_action.fromString(cv);
	m_command.fromString(cv);
	m_next_command.fromString(cv);

	cv->fromBuffer(m_dyn_attr.m_health);
	cv->fromBuffer(m_base_attr_mod.m_max_health);
	m_base_attr.m_max_health = m_base_attr_mod.m_max_health;
	DEBUGX("read offset: %i",cv->getBitStream()->GetReadOffset());
	// Statusveraenderungen
	char c=0;
	cv->fromBuffer(c);

	for (int i=0;i<NR_STATUS_MODS;i++)
	{
		if (c & (1 <<i ))
		{
			cv->fromBuffer(m_dyn_attr.m_status_mod_time[i]);
		}
	}


	for (int i=0;i<NR_EFFECTS;i++)
	{
		cv->fromBuffer(m_dyn_attr.m_effect_time[i]);
	}

	cv->fromBuffer(getBaseAttrMod()->m_special_flags);

	cv->fromBuffer(getBaseAttr()->m_step_length);
	cv->fromBuffer(getBaseAttrMod()->m_attack_speed);
	cv->fromBuffer(getBaseAttrMod()->m_walk_speed);
	int nr_timers = NR_TIMERS;
	if (cv->getVersion() < 17)
	{
		nr_timers = 2;
	}
	for (int i=0; i<nr_timers; i++)
	{
		cv->fromBuffer(m_timers[i]);
		cv->fromBuffer(m_timers_max[i]);
	}

	short nr;
	cv->fromBuffer(nr);
	Action::ActionType type;
	getBaseAttrMod()->m_abilities.clear();
	int timer_nr;
	float time, timer;
	for (int i=0; i<nr; i++)
	{
		cv->fromBuffer(type);
		cv->fromBuffer(timer_nr);
		cv->fromBuffer(time);
		cv->fromBuffer(timer);
		getBaseAttrMod()->m_abilities[type].m_timer_nr = timer_nr;
		getBaseAttrMod()->m_abilities[type].m_timer =timer;
		getBaseAttrMod()->m_abilities[type].m_time = time;
	}

	Vector speed;
	cv->fromBuffer(speed.m_x);
	cv->fromBuffer(speed.m_y);
	setSpeed(speed);

	cv->fromBuffer(m_emotion_set);
}

bool Creature::checkAbility(Action::ActionType act)
{
	if (act == "noaction")
		return true;

	return ( m_base_attr_mod.m_abilities.count(act) > 0 );
}


float Creature::getTimerPercent(int timer)
{
	if (timer != 0)
	{
		if (m_timers_max[timer-1] ==0)
			return 0;

		return m_timers[timer-1] / m_timers_max[timer-1];
	}

	return 0;
}


void Creature::writeNetEvent(NetEvent* event, CharConv* cv)
{
	WorldObject::writeNetEvent(event,cv);

	if (event->m_data & NetEvent::DATA_NAME)
	{
		cv->toBuffer(m_refname);
	}

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

		DEBUGX("sending action %s angle %f",m_action.m_type.c_str(), getShape()->m_angle);
		if (m_action.m_type!="noaction")
		{
			float acttime = m_action.m_time - m_action.m_elapsed_time;
			Vector goal = getShape()->m_center + getSpeed() * acttime;
		}
	}

	if (event->m_data & NetEvent::DATA_HP)
	{
		cv->toBuffer(getDynAttr()->m_health);
	}

	if (event->m_data & NetEvent::DATA_ATTRIBUTES_LEVEL)
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
		int nr_timers = NR_TIMERS;
		if (cv->getVersion() < 17)
			nr_timers = 2;
		for (int i=0; i<nr_timers; i++)
		{
			cv->toBuffer(m_timers[i]);
		}
		for (int i=0; i<nr_timers; i++)
		{
			cv->toBuffer(m_timers_max[i]);
		}
	}

	if (event->m_data & NetEvent::DATA_STATE)
	{
		cv->toBuffer((char) getState());
		DEBUGX("sending state %i %i",getId(),getState());
	}

	if (event->m_data & NetEvent::DATA_ATTACK_WALK_SPEED)
	{
		cv->toBuffer(getBaseAttrMod()->m_walk_speed);
		cv->toBuffer(getBaseAttrMod()->m_attack_speed);
	}


	if (event->m_data & NetEvent::DATA_NEXT_COMMAND)
	{
		m_next_command.toString(cv);
	}

	if (event->m_data & NetEvent::DATA_ABILITIES)
	{
		cv->toBuffer(static_cast<short>(getBaseAttrMod()->m_abilities.size()));
		std::map<std::string, AbilityInfo>::iterator it;
		for (it= getBaseAttrMod()->m_abilities.begin(); it != getBaseAttrMod()->m_abilities.end(); ++it)
		{
			cv->toBuffer(it->first);
			cv->toBuffer(it->second.m_timer_nr);
			cv->toBuffer(it->second.m_time);
			cv->toBuffer(it->second.m_timer);
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

	if (event->m_data & NetEvent::DATA_SPEED)
	{
		cv->toBuffer(getSpeed().m_x);
		cv->toBuffer(getSpeed().m_y);
	}

	if (event->m_data & NetEvent::DATA_ATTRIBUTES_LEVEL)
	{
		cv->toBuffer(getBaseAttr()->m_level);
		cv->toBuffer(getBaseAttr()->m_max_experience);
		cv->toBuffer(getBaseAttr()->m_max_health);
		cv->toBuffer(getBaseAttr()->m_strength);
		cv->toBuffer(getBaseAttr()->m_dexterity);
		cv->toBuffer(getBaseAttr()->m_willpower);
		cv->toBuffer(getBaseAttr()->m_magic_power);
	}

	if (event->m_data & NetEvent::DATA_SPEAK_TEXT)
	{
		getSpeakText().m_text.toString(cv);
		cv->toBuffer(getSpeakText().m_emotion);
		cv->toBuffer(getSpeakText().m_time);
		cv->toBuffer(getSpeakText().m_in_dialogue);
		cv->toBuffer(getSpeakText().m_displayed_time);
		cv->toBuffer(m_emotion_set);
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
	WorldObject::processNetEvent(event,cv);

	if (event->m_data & NetEvent::DATA_NAME)
	{
		std::string name;
		cv->fromBuffer(name);
		m_refname = name;
	}

	DEBUGX("object %i processing NetEvent %i data %i",getId(),event->m_type, event->m_data);
	if (event->m_data & NetEvent::DATA_COMMAND)
	{
		m_command.fromString(cv);
		DEBUGX("got Command %s",m_command.m_type.c_str());
	}

	float  atime = m_action.m_time - m_action.m_elapsed_time;

	Vector newpos= getShape()->m_center;
	Vector newspeed;
	Vector goal = newpos;

	bool newact= false;
	bool newmove = false;
	float delay = cv->getDelay();
	float newangle=getShape()->m_angle;

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

	if (event->m_data & NetEvent::DATA_ATTRIBUTES_LEVEL)
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
		cv->fromBuffer(ctmp);
		for (int i=0;i<NR_STATUS_MODS;i++)
		{
			if (ctmp & (1 <<i ))
			{
				cv->fromBuffer(m_dyn_attr.m_status_mod_time[i]);
			}
		}
	}

	if (event->m_data & NetEvent::DATA_TIMER)
	{
		int nr_timers = NR_TIMERS;
		if (cv->getVersion() < 17)
			nr_timers = 2;
		for (int i=0; i<nr_timers; i++)
		{
			cv->fromBuffer(m_timers[i]);
		}
		for (int i=0; i<nr_timers; i++)
		{
			cv->fromBuffer(m_timers_max[i]);
		}
	}

	if (event->m_data & NetEvent::DATA_STATE)
	{
		State oldstate = getState();
		char ctmp;
		cv->fromBuffer(ctmp);

		// einige fuer die Steuerung wichtige zustaende werden ausgeklammert
		if (ctmp <=STATE_DEAD || ctmp >=STATE_STATIC)
		{
			if (oldstate != STATE_DIEING && oldstate != STATE_DEAD && (ctmp==STATE_DIEING || ctmp == STATE_DEAD))
			{
				die();
			}
			setState((State) ctmp);
			DEBUGX("object %i changed state from %i to %i",getId(),oldstate,ctmp);
		}

	}

	if (event->m_data & NetEvent::DATA_ATTACK_WALK_SPEED)
	{
		cv->fromBuffer(getBaseAttrMod()->m_walk_speed);
		cv->fromBuffer(getBaseAttrMod()->m_attack_speed);
	}

	if (event->m_data & NetEvent::DATA_NEXT_COMMAND)
	{
		m_next_command.fromString(cv);
	}

	if (event->m_data & NetEvent::DATA_ABILITIES)
	{
		short nr;
		cv->fromBuffer(nr);
		int timer_nr;
		float time, timer;
		Action::ActionType type;
		for (int i=0; i<nr; i++)
		{
			cv->fromBuffer(type);
			cv->fromBuffer(timer_nr);
			cv->fromBuffer(time);
			cv->fromBuffer(timer);
			getBaseAttr()->m_abilities[type].m_timer_nr = timer_nr;
			getBaseAttr()->m_abilities[type].m_timer = timer;
			getBaseAttr()->m_abilities[type].m_time = time;
			calcBaseAttrMod();
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

	if (event->m_data & NetEvent::DATA_SPEED)
	{
		cv->fromBuffer(newspeed.m_x);
		cv->fromBuffer(newspeed.m_y);
		if (newspeed.getLength() > 0.00001f)
		{
			newmove = true;
		}

	}

	if (event->m_data & NetEvent::DATA_ATTRIBUTES_LEVEL)
	{
		cv->fromBuffer(getBaseAttr()->m_level);
		cv->fromBuffer(getBaseAttr()->m_max_experience);
		cv->fromBuffer(getBaseAttr()->m_max_health);
		cv->fromBuffer(getBaseAttr()->m_strength);
		cv->fromBuffer(getBaseAttr()->m_dexterity);
		cv->fromBuffer(getBaseAttr()->m_willpower);
		cv->fromBuffer(getBaseAttr()->m_magic_power);
	}

	if (event->m_data & NetEvent::DATA_SPEAK_TEXT)
	{
		getSpeakText().clear();
		getSpeakText().m_text.fromString(cv);
		cv->fromBuffer(getSpeakText().m_emotion);
		cv->fromBuffer(getSpeakText().m_time);
		cv->fromBuffer(getSpeakText().m_in_dialogue);
		cv->fromBuffer(getSpeakText().m_displayed_time);
		cv->fromBuffer(m_emotion_set);
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

			DEBUGX("time to reach goal %f (prev time %f) delay %f",goaltime,atime,cv->getDelay());

		}
		DEBUGX("previously planned time %f",atime);

		if (goaltime <0)
		{
			// wenn man schon lange da sein muesste, Objekt an den Zielort versetzen
			moveTo(goal);
		}
		else
		{
			DEBUGX("pos %f %f speed %f %f", getShape()->m_center.m_x, getShape()->m_center.m_y, getSpeed().m_x, getSpeed().m_y);
			// Bewegungsgeschwindigkeit so setzen, dass Ziel in der richtigen Zeit erreicht wird
			Vector calcspeed = (goal - getShape()->m_center) * (1/goaltime);
			setSpeed(calcspeed);

			if (calcspeed.getLength() > 4* newspeed.getLength())
			{
				moveTo(goal - newspeed * goaltime);
				setSpeed(newspeed);
			}
			DEBUGX("goal %f %f newspeed %f %f", goal.m_x, goal.m_y, getSpeed().m_x, getSpeed().m_y);
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

			m_action.m_type = "noaction";
			m_action.m_elapsed_time =0;

		}
		else
		{
			// Drehwinkel korrekt setzen
			if (!newmove && m_action.m_type != "noaction")
			{
				// Wenn die Aktion nicht laufen ist, Spieler an die richtige Position versetzen
				moveTo(newpos);
				getShape()->m_angle = newangle;

			}

			Action::ActionInfo* ainfo = Action::getActionInfo(m_action.m_type);
			if (ainfo !=0 && ainfo->m_base_action == "walk" && newmove)
			{
				setAngle(getSpeed().angle());

			}
		}
	}
}

int Creature::getValue(std::string valname)
{
	if (valname == "emotionset")
	{
		lua_pushstring(EventSystem::getLuaState() , m_emotion_set.c_str() );
		return 1;
	}
	else if (valname.substr(0,5) == "timer")
	{
		std::string number = valname.substr(5);
		std::stringstream stream(number);
		int timer = 0;
		stream >> timer;
		if (timer > 0 && timer <= NR_TIMERS)
		{
			lua_pushnumber(EventSystem::getLuaState(),	m_timers[timer-1]);
			return 1;
		}
		return 0;
	}
	else if (valname == "ignored_by_ai")
	{
		lua_pushboolean(EventSystem::getLuaState(), bool(getBaseAttrMod()->m_special_flags & IGNORED_BY_AI));
		return 1;
	}

	if (valname.find("base:") == 0)
	{
		std::string val2 = valname.substr(5);
		int ret = getBaseAttr()->getValue(val2);
		if (ret >0)
		{
			return ret;
		}
	}

	int ret = getBaseAttrMod()->getValue(valname);
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
	if (valname == "emotionset")
	{
		m_emotion_set = lua_tostring(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		addToNetEventMask(NetEvent::DATA_SPEAK_TEXT);
		return true;
	}
	else if (valname.substr(0,5) == "timer")
	{
		std::string number = valname.substr(5);
		std::stringstream stream(number);
		int timer = 0;
		stream >> timer;
		if (timer > 0 && timer < NR_TIMERS)
		{
			m_timers[timer-1] =  lua_tonumber(EventSystem::getLuaState(),-1);
			lua_pop(EventSystem::getLuaState(), 1);
			addToNetEventMask(NetEvent::DATA_TIMER);
			return true;
		}
		return false;
	}
	else if (valname == "ignored_by_ai")
	{
		bool ignore = lua_toboolean(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		if (ignore)
		{
			getBaseAttr()->m_special_flags |= IGNORED_BY_AI;
		}
		else
		{
			getBaseAttr()->m_special_flags &= (~IGNORED_BY_AI);
		}
		calcBaseAttrMod();
		return true;
	}

	bool ret;
	ret = getBaseAttr()->setValue(valname, getEventMaskRef());
	if (ret)
	{
		calcBaseAttrMod();
		return ret;
	}

	ret = m_dyn_attr.setValue(valname, getEventMaskRef());
	if (ret)
	{
		if (m_dyn_attr.m_health >0 && (getState() == STATE_DIEING || getState() == STATE_DEAD))
		{
			if (getType() == "PLAYER")
			{
				getRegion()->changeObjectGroup(this,PLAYER);
			}
			else
			{
				getRegion()->changeObjectGroup(this,CREATURE);
			}
			setState(STATE_ACTIVE);

			m_action.m_type ="noaction";
			m_action.m_elapsed_time =0;
			addToNetEventMask(NetEvent::DATA_ACTION | NetEvent::DATA_STATE);
		}

		// Solange Level aufsteigen, bis exp < max_exp
		// Fuer den Fall dass Exp geaendert wurde
		while (m_dyn_attr.m_experience>= m_base_attr.m_max_experience && m_dyn_attr.m_experience>0)
		{
			gainLevel();
		}

		return ret;
	}

	ret = WorldObject::setValue(valname);
	return ret;
}

void Creature::clearSpeakText()
{
	addToNetEventMask(NetEvent::DATA_SPEAK_TEXT);
	m_speak_text.clear();
}

void Creature::speakText(CreatureSpeakText& text)
{
	// Text aus Dialogen hat Vorrang
	if (text.m_in_dialogue == false && !m_speak_text.empty() && m_speak_text.m_in_dialogue)
	{
		return;
	}

	addToNetEventMask(NetEvent::DATA_SPEAK_TEXT);

	m_speak_text = text;
	m_speak_text.m_displayed_time = 0;

	DEBUGX("speak %s for %f ms",text.m_text.getTranslation().c_str(), text.m_time);
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
	DEBUGX("buy %p",item);
	if (item != 0 && getEquipement() !=0)
	{
		NPCTrade& tradeinfo = Dialogue::getNPCTrade(getRefName());
		float factor = tradeinfo.m_pay_multiplier;

		// Geld auszahlen
		// zuletzt gekauftes Item kann zum vollen Preis zurueck gegebebn werden
		if (item == m_trade_info.m_last_sold_item)
		{
			gold +=(int) (item->m_price * m_trade_info.m_price_factor);
		}
		else
		{
			gold += MathHelper::Max(1, int(item->m_price*factor));
		}
		item->m_price = (int) (item->m_price * m_trade_info.m_price_factor);

		// beim Haendler einfuegen
		short pos = getEquipement()->insertItem(item,false);
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
	if (id != m_dialogue_id)
	{
		DEBUGX("Creature %s has now Dialogue %i",getRefName().c_str(),id);
		m_dialogue_id = id;
		addToNetEventMask(NetEvent::DATA_DIALOGUE);

		if (id !=0)
		{
			getAction()->m_type = "noaction";
			m_action.m_elapsed_time =0;
			clearCommand(true,true);
		}
	}
}

void Creature::updateFightStat(float hitchance, float armorperc, TranslatableString attacked)
{
	FightStatistic* fstat = &(getFightStatistic());
	if (fabs(hitchance - fstat->m_hit_chance) > 0.01
		   || fabs(armorperc - fstat->m_damage_dealt_perc) > 0.01
		   || attacked != fstat->m_last_attacked)
	{
		fstat->m_hit_chance = hitchance;
		fstat->m_damage_dealt_perc = armorperc;
		fstat->m_last_attacked = attacked;
		addToNetEventMask(NetEvent::DATA_FIGHT_STAT);

		DEBUGX("hitchance %f damage perc %f attacked %s",hitchance, armorperc, attacked.c_str());
	}
}

std::string Creature::getActionString()
{
	if (getState() == STATE_DEAD)
		return "die";

	if (m_action.m_type == "noaction")
	{
		// bei Noaction wird das Animationssystem von WorldObject angefragt
		// im Fall von Sprechen statt noaction talk ausgeben
		std::string act = WorldObject::getActionString();
		if (act != "")
		{
			return act;
		}

		if (!getSpeakText().empty())
		{
			return "talk";
		}
	}

	return m_action.m_type;
}

float Creature::getActionPercent()
{
	if (getState() == STATE_DEAD)
		return 0.999999;

	if (m_action.m_type == "noaction")
	{
		// bei Noaction wird das Animationssystem von WorldObject angefragt
		// im Fall von Sprechen statt noaction talk ausgeben
		std::string act = WorldObject::getActionString();
		if (act != "")
		{
			return WorldObject::getActionPercent();
		}

		if (!getSpeakText().empty())
		{
			return fmod(getSpeakText().m_displayed_time,1000.0f)/1000.0f;
		}
	}

	return m_action.m_elapsed_time / m_action.m_time;
}

void Creature::getFlags(std::set<std::string>& flags)
{
	WorldObject::getFlags(flags);

	// Statusmods
	float* mods = m_dyn_attr.m_status_mod_time;
	static const std::string modnames[NR_STATUS_MODS]= {"blind", "poisoned", "berserk","confused", "mute", "paralyzed", "frozen", "burning" };
	for (int i=0; i<NR_STATUS_MODS; i++)
	{
		if (mods[i] >0)
		{
			flags.insert(modnames[i]);
		}
	}

	// Effekte
	float* effects = m_dyn_attr.m_effect_time;
	static const std::string effectnames[NR_EFFECTS]= {"bleed"};
	for (int i=0; i<NR_EFFECTS; i++)
	{
		if (effects[i] >0)
		{
			flags.insert(effectnames[i]);
		}
	}

	// weitere Flags
	int flgs = m_base_attr_mod.m_special_flags;
	if (flgs & FIRESWORD)
		flags.insert("firesword");
	if (flgs & FLAMESWORD)
		flags.insert("flamesword");
	if (flgs & FLAMEARMOR)
		flags.insert("flamearmor");
	if (flgs & CRIT_HITS)
		flags.insert("critical_hits");
	if (flgs & ICE_ARROWS)
		flags.insert("ice_arrows");
	if (flgs & FROST_ARROWS)
		flags.insert("frost_arrows");
	if (flgs & WIND_ARROWS)
		flags.insert("wind_arrows");
	if (flgs & STORM_ARROWS)
		flags.insert("storm_arrows");
	if (flgs & WIND_WALK)
		flags.insert("wind_walk");
	if (flgs & BURNING_RAGE)
		flags.insert("burning_rage");
	if (flgs & STATIC_SHIELD)
		flags.insert("static_shield");
}

void Creature::clearFlags()
{
	WorldObject::clearFlags();

	// Status flags loeschen
	for ( int i=0; i< NR_STATUS_MODS; i++)
	{
		m_dyn_attr.m_status_mod_time[i] =0;
	}

	m_base_attr_mod.m_special_flags=0;
}

std::string Creature::getEmotionImage(std::string emotion)
{
	EmotionSet* es = ObjectFactory::getEmotionSet(m_emotion_set);
	if (es ==0)
		return "";

	return es->getEmotionImage(emotion);
}


