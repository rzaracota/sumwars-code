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

//Constructors/Destructors
Creature::Creature(World* world, unsigned int id) : ServerWObject(world, id)
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

	// Bewegung auf 0 setzen
	getMoveInfo()->m_speed_x=0;
	getMoveInfo()->m_speed_y=0;

	// Wegfindeinformationen auf 0 setzen
	m_small_path_info=0;
	m_small_flying_path_info=0;
	m_medium_path_info=0;
	m_big_path_info=0;
	m_path_info=0;

	// Timer nullen
	m_timer1 =0;
	m_timer2 =0;

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

	//Lebenspunkts auf 1 (debugging)
	m_dyn_attr.m_health = 1;
	
	m_event_mask =0;

	return tmp;
}

bool Creature::destroy()
{
	// Wenn Spieler geloescht wird, automatisch Handel abbrechen
	if (getTradeId() !=0)
	{
		Trade* trade = getWorld()->getTrade(getTradeId());

		// eigene Position bei dem Handel bestimmen
		int idx = trade->getPlayerIndex(getId());

		// Wenn der Handel nicht existiert Fehler ausgeben
		if (trade == 0 || idx ==-1)
		{
			setTradeId(0);
		}
		else
		{
			trade->abortTrade();
		}
	}

	DEBUG5("destroy");

	ServerWObject::destroy();
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
	
	m_event_mask |= Event::DATA_ACTION | Event::DATA_STATE;
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
			m_event_mask |= Event::DATA_TIMER;
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
			m_event_mask |= Event::DATA_TIMER;
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
			getMoveInfo()->m_speed_x *= getBaseAttr()->m_step_length/m_action.m_time;
			getMoveInfo()->m_speed_y *= getBaseAttr()->m_step_length/m_action.m_time;
			m_event_mask |= Event::DATA_MOVE_INFO;
			//collisionDetection(m_action.m_time);

			DEBUG5("walk time %f walk speed %i",m_action.m_time,getBaseAttrMod()->m_walk_speed);
			DEBUG5("speed %f %f",getMoveInfo()->m_speed_x,getMoveInfo()->m_speed_y);
	}
	else
	{
		getMoveInfo()->m_speed_x=0;
		getMoveInfo()->m_speed_y=0;
	}



	// Fuer Aktionen die auf physischen Angriffen beruhen sowie fuer den normalen Magieangriff Waffengeschwindigkeit einrechnen
	if (baseact == Action::ATTACK || baseact == Action::RANGE_ATTACK || baseact == Action::HOLY_ATTACK || m_action.m_type == Action::MAGIC_ATTACK)
	{
		float atksp = getBaseAttrMod()->m_attack_speed;


		m_action.m_time *= 1000000/atksp;

	}
	else if (baseact != Action::WALK)
	{
		m_action.m_time = aci->m_standard_time;

	}

	DEBUG5("resulting time %f",m_action.m_time);
	// Drehwinkel setzen
	if (aci->m_distance != Action::SELF)
	{
		float x = getGeometry()->m_shape.m_coordinate_x;
		float y = getGeometry()->m_shape.m_coordinate_y;
		float goalx =  m_action.m_goal_coordinate_x;
		float goaly =  m_action.m_goal_coordinate_y;

		getGeometry()->m_angle = atan2(goaly-y,goalx-x);

	}
	if (baseact == Action::WALK)
	{
		getGeometry()->m_angle = atan2(getMoveInfo()->m_speed_y,getMoveInfo()->m_speed_x);

	}

	// Daten fuer die Animation setzen
	m_action.m_action_equip = getActionEquip();
	m_action.m_animation_number = Random::randi(120);

	// Besondere Initialisierungen

	if ( m_action.m_type== Action::TRADE)
	{
	}

	m_event_mask |= Event::DATA_ACTION;
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

	// Extrabehandlung von Trade (Zeitunabhängig)
	if (m_action.m_type == Action::TRADE)
	{
		time=0;
	}

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

	if (!getWorld()->isServer() && m_action.m_type != Action::NOACTION)
	{
		DEBUG5("perform Action %i for %f msec, %f / %f done", m_action.m_type,dtime,m_action.m_elapsed_time,m_action.m_time);
	}


	//Behandlung des Laufens
	if (m_action.m_type == Action::WALK)
	{

		// Kollisionen behandeln
		collisionDetection(dtime);

		// neue Koordinaten ausrechnen
		float xnew = getGeometry()->m_shape.m_coordinate_x+getMoveInfo()->m_speed_x * dtime;
		float ynew = getGeometry()->m_shape.m_coordinate_y+getMoveInfo()->m_speed_y * dtime;

		DEBUG5("new coord %f %f",xnew,ynew);
		// Bewegung ausfuehren
		moveTo(xnew,ynew);

	}



	// Behandlung der Wirkung der Aktion

	// Prozentsatz bei dessen Erreichen die Wirkung der Aktion berechnet wird
	float pct = Action::getActionInfo(m_action.m_type)->m_critical_perc;

	// Triple Shot
	if (m_action.m_type == Action::TRIPLE_SHOT || m_action.m_type == Action::GUIDED_TRIPLE_SHOT)
	{
		// Faehigkeit hat drei Prozentsaetze
		if (p1>0.3)
			pct = 0.6;
		if (p1>0.6)
			pct = 0.9;
	}

	// Testen ob der kritische Prozentsatz durch das aktuelle Zeitquantum ueberschritten wurde
	if (p1<pct && pct <=p2 && getWorld()->isServer())
	{
		DEBUG5("critical point %f %f %f",p1,pct,p2);

		// Zielobjekt der Aktion
		ServerWObject* goal =0;

		float goalx =  m_action.m_goal_coordinate_x;
		float goaly =  m_action.m_goal_coordinate_y;

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
			float dir[2];
			dir[0] = goalx - getGeometry()->m_shape.m_coordinate_x;
			dir[1] = goaly - getGeometry()->m_shape.m_coordinate_y;
			float d = sqrt(sqr(dir[0])+sqr(dir[1]));
			float r = 0.5*d;
			float dx = r, dy =r;
			while (sqr(r)<sqr(dx)+sqr(dy))
			{
				dx = r-2*r*rand()*1.0/RAND_MAX;
				dy = r-2*r*rand()*1.0/RAND_MAX;
			}
			goalx += dx;
			goaly += dy;


		}

		// Zielobjekt ermitteln
		if (m_action.m_goal_object_id!=0 && m_action.m_type!= Action::TAKE_ITEM)
		{
			// Zielobjekt durch ID gegeben, Objekt von der Welt holen
			goal = getWorld()->getSWObject(m_action.m_goal_object_id,getGridLocation()->m_region);
		}
		else
		{
			// Kein Zielobjekt per ID gegeben
			DEBUG5("no Goal ID!");
			// Im Falle von Nahkampf Ziel anhand des Zielpunktes suchen
			if (Action::getActionInfo(m_action.m_type)->m_distance == Action::MELEE && m_action.m_type!= Action::TAKE_ITEM)
			{
				DEBUG5("Searching goal %f %f",goalx,goaly);
				goal = getWorld()->getSWObjectAt(goalx,goaly,getGridLocation()->m_region,Geometry::LAYER_AIR);
				DEBUG5("got object %p",goal);
			}
		}




		// Zielobjekt als Creature* pointer
		Creature* cgoal=0;

		// Zielobjekt im Nahkampf suchen an der Stelle an der die Waffe trifft
		if (goal ==0 && Action::getActionInfo(m_action.m_type)->m_distance == Action::MELEE && m_action.m_type!= Action::TAKE_ITEM)
		{
			goal = getWorld()->getSWObjectAt(goalx,goaly,getGridLocation()->m_region,Geometry::LAYER_AIR);
		}

		// Wenn ein Zielobjekt existiert
		if (goal !=0)
		{
			if (goal->getTypeInfo()->m_type == TypeInfo::TYPE_FIXED_OBJECT)
			{
				cgoal =0;
				// Ziel existiert nicht mehr, evtl abbrechen
			}
			else
			{
				// umwandeln in Creature* Pointer
				cgoal = (Creature*) goal;
				// anpassen des Zielpunktes
				goalx = cgoal->getGeometry()->m_shape.m_coordinate_x;
				goaly = cgoal->getGeometry()->m_shape.m_coordinate_y;

				DEBUG5("goal object %p",cgoal);
			}
		}

		// ausfuehren des kritischen Teiles der Aktion
		performActionCritPart(goalx, goaly, goal);

	}

	// Wenn die Aktion beenden wurde evtl Kommando abschließen
	if (finish)
	{
		DEBUG5("finished action");
		if (m_action.m_type != Action::WALK)
		{
			m_action.m_prev_type = m_action.m_type;
		}

		DEBUG5("distance to goal %f",sqr(m_command.m_goal_coordinate_x -getGeometry()->m_shape.m_coordinate_x) +  sqr(m_command.m_goal_coordinate_y -getGeometry()->m_shape.m_coordinate_y));

		// Kommando ist beendet wenn die gleichnamige Aktion beendet wurde
		// Ausnahme: Bewegungskommando ist beendet wenn das Ziel erreicht ist
		if (m_action.m_type == m_command.m_type && m_action.m_type != Action::WALK ||
				  m_action.m_type == Action::WALK && sqr(getGeometry()->m_shape.m_radius) > sqr(m_command.m_goal_coordinate_x -getGeometry()->m_shape.m_coordinate_x) +  sqr(m_command.m_goal_coordinate_y -getGeometry()->m_shape.m_coordinate_y) && !(m_command.m_type == Action::CHARGE || m_command.m_type == Action::STORM_CHARGE))
		{
			DEBUG5("finished command");
			if (m_command.m_type != Action::NOACTION)
			{
				m_event_mask |= Event::DATA_COMMAND;
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
			m_event_mask |= Event::DATA_ACTION;
		}
		m_action.m_type = Action::NOACTION;
		m_action.m_elapsed_time =0;
		m_event_mask |= Event::DATA_ACTION;
	}




}

void Creature::performActionCritPart(float goalx, float goaly, ServerWObject* goal)
{
	// Zielobjekt als Creature* pointer
	// null, wenn das Objekt kein Lebewesen ist
	Creature* cgoal =0;
	if (goal!=0 && goal->getTypeInfo()->m_type == TypeInfo::TYPE_FIXED_OBJECT)
	{
		cgoal =0;
	}
	else
	{
		cgoal = (Creature*) goal;
	}

    //	Action::ActionType baseact = Action::getActionInfo(m_action.m_type)->m_base_action;

	// Wenn die Aktion auf Nahkampf beruht Schaden an das Zielobjekt austeilen
	// Ausname: Rundumschlag
	if (Action::getActionInfo(m_action.m_type)->m_distance == Action::MELEE)
	{
		if (cgoal && m_action.m_type!=Action::AROUND_BLOW &&  m_action.m_type!=Action::WHIRL_BLOW)
		{
			cgoal->takeDamage(&m_damage);
		}
	}



	list<ServerWObject*> res;
	res.clear();
	list<ServerWObject*>::iterator it;

	// Koordinaten das ausfuehrenden Objektes
	float x = getGeometry()->m_shape.m_coordinate_x;
	float y = getGeometry()->m_shape.m_coordinate_y;

	// Form, wird initialisiert mit der Form des Ausfuehrenden
	Shape s;
	s.m_coordinate_x = x;
	s.m_coordinate_y = y;
	s.m_type = Shape::CIRCLE;
	s.m_radius = getGeometry()->m_shape.m_radius;

	short reg = getGridLocation()->m_region;
	Creature* cr =0;

	// Struktur fuer Basisattributmods, initialisiert mit Nullen
	CreatureBaseAttrMod cbam;

	DmgProjectile* pr =0;

	// Struktur fuer Modifikationen der dyn. Attribute, initialisiert mit Nullen
	CreatureDynAttrMod cdam;


	// Daten fuer Geschosse: Zielrichtung und Startpunkt
	float dir[2],dir2[2];
	dir[0] = goalx - getGeometry()->m_shape.m_coordinate_x;
	dir[1] = goaly - getGeometry()->m_shape.m_coordinate_y;
	float d = sqr(dir[0])+sqr(dir[1]);

	// Startpunkt fuer Geschosse
	// aeusserer Rand des Ausfuehrenden plus 5%
	float sy,sx;
	TypeInfo::Fraction fr = getTypeInfo()->m_fraction;
	int i;
	if (d>0)
	{
		d= 1/sqrt(d);
		dir[0] *=d;
		dir[1] *=d;
		sx = x+dir[0] * 1.05 * s.m_radius;
		sy = y+dir[1] * 1.05 * s.m_radius;
	}

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
		case Action::USE:
			if (cgoal)
				goal->reactOnUse(getId());
			break;

		// Kriegerfaehigkeiten
		case Action::HAMMER_BASH:
			// alle Lebewesen im Umkreis von 1.5 um den Zielpunkt auswaehlen
			m_damage.m_multiplier[Damage::PHYSICAL]=1;
			s.m_coordinate_x = goalx;
			s.m_coordinate_y = goaly;
			s.m_radius = 1.5;
			getWorld()->getSWObjectsInShape(&s,reg, &res,Geometry::LAYER_AIR,CREATURE,this);

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
			getWorld()->getSWObjectsInShape(&s, reg, &res,Geometry::LAYER_AIR,CREATURE,this);

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
			m_event_mask |= Event::DATA_HP;
			break;

		case Action::ANGER:
			// Modifikationen:
			// doppelte Staerke, halbierte Ruestung, Berserker fuer 30 sec
			cbam.m_time = 40000;
			cbam.m_dstrength =m_base_attr.m_strength;
			cbam.m_darmor = -m_base_attr_mod.m_armor /2;
			applyBaseAttrMod(&cbam);
			m_dyn_attr.m_status_mod_time[Damage::BERSERK] = 30000;
			m_event_mask |= Event::DATA_STATUS_MODS;
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
			m_event_mask |= Event::DATA_STATUS_MODS;
			break;

		// Magierfaehigkeiten
		case Action::MAGIC_ATTACK:
			// Projektil magischer Pfeil erzeugen
			pr = new DmgProjectile(getWorld(),Projectile::MAGIC_ARROW,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			pr->setSpeedX(dir[0]/70);
			pr->setSpeedY(dir[1]/70);
			getWorld()->insertProjectile(pr,sx,sy,reg);
			break;

		case Action::FIRE_BOLT:
		case Action::FIRE_STRIKE:
			// Projektil Feuerblitz erzeugen
			pr = new DmgProjectile(getWorld(),Projectile::FIRE_BOLT,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			pr->setSpeedX(dir[0]/70);
			pr->setSpeedY(dir[1]/70);
			getWorld()->insertProjectile(pr,sx,sy,reg);
			break;

		case Action::FIRE_BALL:
		case Action::INFERNO_BALL:
			// Projektil Feuerball erzeugen
			pr = new DmgProjectile(getWorld(),Projectile::FIRE_BALL,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			pr->setSpeedX(dir[0]/70);
			pr->setSpeedY(dir[1]/70);
			getWorld()->insertProjectile(pr,sx,sy,reg);
			break;

		case Action::FIRE_WAVE:
		case Action::FIRE_STORM:
			// Projektil Feuerwelle erzeugen
			pr = new DmgProjectile(getWorld(),Projectile::FIRE_WAVE,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			getWorld()->insertProjectile(pr,x,y,reg);
			break;

		case Action::FIRE_WALL:
			// Projektil Feuersaeule erzeugen
			pr = new DmgProjectile(getWorld(),Projectile::FIRE_WALL,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			getWorld()->insertProjectile(pr,goalx,goaly,reg);
			break;

		case Action::ICE_BOLT:
		case Action::ICE_SPIKE:
			// Projektil Eisblitz erzeugen
			pr = new DmgProjectile(getWorld(),Projectile::ICE_BOLT,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			pr->setSpeedX(dir[0]/70);
			pr->setSpeedY(dir[1]/70);
			getWorld()->insertProjectile(pr,sx,sy,reg);
			break;

		case Action::SNOW_STORM:
		case Action::BLIZZARD:
			// Projektil Blizzard erzeugen
			pr = new DmgProjectile(getWorld(),Projectile::BLIZZARD,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			getWorld()->insertProjectile(pr,goalx,goaly,reg);
			break;

		case Action::ICE_RING:
		case Action::FROST_RING:
			// Projektil Eisring erzeugen
			pr = new DmgProjectile(getWorld(),Projectile::ICE_RING,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			getWorld()->insertProjectile(pr,x,y,reg);
			break;

		case Action::FREEZE:
			// Projektil Einfrieren erzeugen
			pr = new DmgProjectile(getWorld(),Projectile::FREEZE,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			getWorld()->insertProjectile(pr,goalx,goaly,reg);
			break;

		case Action::LIGHTNING:
		case Action::LIGHTNING_STRIKE:
			// Projektil Blitz erzeugen
			pr = new DmgProjectile(getWorld(),Projectile::LIGHTNING,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			getWorld()->insertProjectile(pr,goalx,goaly,reg);
			break;

		case Action::THUNDERSTORM:
		case Action::THUNDERSTORM2:
			// Projektil Gewitter erzeugen
			pr = new DmgProjectile(getWorld(),Projectile::THUNDERSTORM,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			getWorld()->insertProjectile(pr,goalx,goaly,reg);
			break;

		case Action::CHAIN_LIGHTNING:
		case Action::CHAIN_LIGHTNING2:
			// Projektil Kettenblitz erzeugen
			pr = new DmgProjectile(getWorld(),Projectile::CHAIN_LIGHTNING,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			if (m_action.m_type == Action::CHAIN_LIGHTNING2)
			{
				// Verbesserte Version springt noch viermal extra, daher einfach bei -4 anfangen zu zaehlen
				pr->setCounter(-4);
			}
			pr->setSpeedX(dir[0]/50);
			pr->setSpeedY(dir[1]/50);
			getWorld()->insertProjectile(pr,sx,sy,reg);
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
			pr = new DmgProjectile(getWorld(),arrow,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			pr->setSpeedX(dir[0]/80);
			pr->setSpeedY(dir[1]/80);
			getWorld()->insertProjectile(pr,sx,sy,reg);
			break;

		case Action::GUIDED_TRIPLE_SHOT:
			// Projektil gelenkter Pfeil erzeugen
			pr = new DmgProjectile(getWorld(),Projectile::GUIDED_ARROW,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			pr->setSpeedX(dir[0]/100);
			pr->setSpeedY(dir[1]/100);
			if (cgoal!=0)
			{
				DEBUG5("has goal");
				pr->setGoalObject(cgoal->getId());
			}
			getWorld()->insertProjectile(pr,sx,sy,reg);
			break;

		case Action::MULTISHOT:
			// 5 Pfeile erzeugen
			// mittlerer Pfeil erhaelt die Zielrichtung
			for (i=-2;i<=2;i++)
			{
				dir2[0] = dir[0] + i*0.2*dir[1];
				dir2[1] = dir[1] - i*0.2*dir[0];
				d= 1/sqrt(sqr(dir2[0])+sqr(dir2[1]));
				dir2[0] *=d;
				dir2[1] *=d;

				sx = x+dir2[0] * 1.05 * s.m_radius;
				sy = y+dir2[1] * 1.05 * s.m_radius;
				pr = new DmgProjectile(getWorld(),arrow,fr, getWorld()->getValidProjectileId());
				memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
				pr->setSpeedX(dir2[0]/80);
				pr->setSpeedY(dir2[1]/80);
				getWorld()->insertProjectile(pr,sx,sy,reg);
			}
			break;

		case Action::VOLLEY_SHOT:
			// 7 Pfeile erzeugen
			// mittlerer Pfeil erhaelt die Zielrichtung
			for (i=-3;i<=3;i++)
			{
				dir2[0] = dir[0] + i*0.15*dir[1];
				dir2[1] = dir[1] - i*0.15*dir[0];
				d= 1/sqrt(sqr(dir2[0])+sqr(dir2[1]));
				dir2[0] *=d;
				dir2[1] *=d;

				sx = x+dir2[0] * 1.05 * s.m_radius;
				sy = y+dir2[1] * 1.05 * s.m_radius;
				pr = new DmgProjectile(getWorld(),arrow,fr, getWorld()->getValidProjectileId());
				memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
				pr->setSpeedX(dir2[0]/80);
				pr->setSpeedY(dir2[1]/80);
				getWorld()->insertProjectile(pr,sx,sy,reg);
			}
			break;

		case Action::PIERCE:
			// Projektil Pfeil erzeugen
			pr = new DmgProjectile(getWorld(),arrow,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			pr->setSpeedX(dir[0]/80);
			pr->setSpeedY(dir[1]/80);
			// Flag durchschlagend setzen
			pr->setFlags(DmgProjectile::PIERCING);
			getWorld()->insertProjectile(pr,sx,sy,reg);
			break;

		case Action::VACUUM:
			// Projektil Windpfeil erzeugen
			pr = new DmgProjectile(getWorld(),Projectile::WIND_ARROW,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			pr->setSpeedX(dir[0]/80);
			pr->setSpeedY(dir[1]/80);
			getWorld()->insertProjectile(pr,sx,sy,reg);
			break;

		case Action::EXPLODING_ARROW:
		case Action::EXPLOSION_CASCADE:
			// Projektil Pfeil erzeugen
			pr = new DmgProjectile(getWorld(),arrow,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			pr->setSpeedX(dir[0]/80);
			pr->setSpeedY(dir[1]/80);
			// Flag explodierend setzen
			pr->setFlags(DmgProjectile::EXPLODES);
			// bei verbesserter Version Flag mehrfach explodierend setzen
			if (m_action.m_type==Action::EXPLOSION_CASCADE)
				pr->setFlags(DmgProjectile::MULTI_EXPLODES);
			getWorld()->insertProjectile(pr,sx,sy,reg);
			break;

		case Action::DEATH_ROULETTE:
			// Projektil Pfeil erzeugen
			pr = new DmgProjectile(getWorld(),arrow,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			pr->setSpeedX(dir[0]/80);
			pr->setSpeedY(dir[1]/80);
			// Maximale Anzahl der Spruenge
			pr->setCounter(-20);
			// Flag zufaellig weiterspringen setzen
			pr->setFlags(DmgProjectile::PROB_BOUNCING);
			getWorld()->insertProjectile(pr,sx,sy,reg);
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
			pr = new DmgProjectile(getWorld(),Projectile::LIGHT_BEAM,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			getWorld()->insertProjectile(pr,goalx,goaly,reg);
			break;

		case Action::BURNING_RAGE:
			// Modifikationen:
			// Flag fuer 80 sec
			cbam.m_time =80000;
			cbam.m_xspecial_flags |= BURNING_RAGE;

			// alle Verbuendeten im Umkreis von 12 suchen und Modifikation anwenden
			s.m_coordinate_x = x;
			s.m_coordinate_y = y;
			s.m_radius = 12;
			getWorld()->getSWObjectsInShape(&s,reg, &res, Geometry::LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (getWorld()->getRelation(fr,(*it)) == WorldObject::ALLIED)
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
				if (getWorld()->getRelation(fr,cgoal) == WorldObject::ALLIED)
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
			s.m_coordinate_x = x;
			s.m_coordinate_y = y;
			s.m_radius = 12;
			getWorld()->getSWObjectsInShape(&s, reg, &res,Geometry::LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (getWorld()->getRelation(fr,(*it)) == WorldObject::ALLIED)
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
			s.m_coordinate_x = goalx;
			s.m_coordinate_y = goaly;
			s.m_radius = 6;
			getWorld()->getSWObjectsInShape(&s, reg, &res,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (getWorld()->getRelation(fr,(*it)) == WorldObject::ALLIED)
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
			pr = new DmgProjectile(getWorld(),Projectile::LIGHT_BEAM,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			getWorld()->insertProjectile(pr,goalx,goaly,reg);
			break;

		case Action::BURNING_SUN:
			// alle Lebewesen im Umkreis von 3 um den Zielpunkt suchen
			s.m_coordinate_x = goalx;
			s.m_coordinate_y = goaly;
			s.m_radius = 3;
			getWorld()->getSWObjectsInShape(&s, reg, &res,Geometry::LAYER_AIR,CREATURE,this);
			for (it=res.begin();it!=res.end();++it)
			{
				if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
				{
					// Projektil Lichtstrahl fuer jedes Objekt erzeugen
					goalx = (*it)->getGeometry()->m_shape.m_coordinate_x;
					goaly = (*it)->getGeometry()->m_shape.m_coordinate_y;
					pr = new DmgProjectile(getWorld(),Projectile::LIGHT_BEAM,fr, getWorld()->getValidProjectileId());
					memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
					getWorld()->insertProjectile(pr,goalx,goaly,reg);
				}
			}
			break;

		case Action::BREAK_BINDING:
		case Action::DISRUPT_BINDING:
			// Projektil Elementarzerstoerung erzeugen
			pr = new DmgProjectile(getWorld(),Projectile::ELEM_EXPLOSION,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			getWorld()->insertProjectile(pr,goalx,goaly,reg);
			break;

		case Action::MAGIC_SHIELD:
			// Modifikation:
			// 50% mehr Willenskraft fuer 90 sec
			cbam.m_time =90000;
			cbam.m_dwillpower = m_base_attr_mod.m_willpower /2;

			// alle Verbuendeten im Umkreis von 12 suchen und Modifikation anwenden
			s.m_coordinate_x = x;
			s.m_coordinate_y = y;
			s.m_radius = 12;
			getWorld()->getSWObjectsInShape(&s, reg, &res,Geometry::LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (getWorld()->getRelation(fr,(*it)) == WorldObject::ALLIED)
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
				if (getWorld()->getRelation(fr,cgoal) == WorldObject::ALLIED)
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
			s.m_coordinate_x = goalx;
			s.m_coordinate_y = goaly;
			s.m_radius = 6;
			getWorld()->getSWObjectsInShape(&s, reg, &res,Geometry::LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (getWorld()->getRelation(fr,(*it)) == WorldObject::ALLIED)
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
			pr = new DmgProjectile(getWorld(),Projectile::ACID,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			getWorld()->insertProjectile(pr,goalx,goaly,reg);
			break;

		case Action::HEAL:
			// Modifikation:
			// 300% der Willenskraft auf HP (nur Verbuendete)
			if (cgoal !=0)
			{
				cdam.m_dhealth = 3* m_base_attr_mod.m_willpower;
				if (getWorld()->getRelation(fr,cgoal) == WorldObject::ALLIED)
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
			s.m_coordinate_x = goalx;
			s.m_coordinate_y = goaly;
			s.m_radius = 6;
			getWorld()->getSWObjectsInShape(&s, reg, &res,Geometry::LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (getWorld()->getRelation(fr,(*it)) == WorldObject::ALLIED)
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
			pr = new DmgProjectile(getWorld(),Projectile::DIVINE_BEAM,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			getWorld()->insertProjectile(pr,goalx,goaly,reg);
			break;


		case Action::DIVINE_STORM:
			// Alle Objekte im Umkreis von 3 um den Zielpunkt suchen
			s.m_coordinate_x = goalx;
			s.m_coordinate_y = goaly;
			s.m_radius = 3;
			getWorld()->getSWObjectsInShape(&s, reg, &res,Geometry::LAYER_AIR,CREATURE,this);
			for (it=res.begin();it!=res.end();++it)
			{
				// fuer alle Lebewesen ein Projektil heiliger Strahl erzeugen
				if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
				{
					goalx = (*it)->getGeometry()->m_shape.m_coordinate_x;
					goaly = (*it)->getGeometry()->m_shape.m_coordinate_y;
					pr = new DmgProjectile(getWorld(),Projectile::DIVINE_BEAM,fr, getWorld()->getValidProjectileId());
					memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
					getWorld()->insertProjectile(pr,goalx,goaly,reg);
				}
			}
			break;

		case Action::BLADE_STORM:
			// Modifikation:
			// Angriffsgeschwindigkeit erhoeht fuer 70 sec
			cbam.m_time =70000;
			cbam.m_dattack_speed = 1500;

			// alle Verbuendeten im Umkreis von 12 suchen und Modifikation anwenden
			s.m_coordinate_x = x;
			s.m_coordinate_y = y;
			s.m_radius = 12;
			getWorld()->getSWObjectsInShape(&s, reg, &res,Geometry::LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (getWorld()->getRelation(fr,(*it)) == WorldObject::ALLIED)
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
				if (getWorld()->getRelation(fr,cgoal) == WorldObject::ALLIED)
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
			s.m_coordinate_x = goalx;
			s.m_coordinate_y = goaly;
			s.m_radius = 6;
			getWorld()->getSWObjectsInShape(&s, reg, &res,Geometry::LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (getWorld()->getRelation(fr,(*it)) == WorldObject::ALLIED)
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
			pr = new DmgProjectile(getWorld(),Projectile::HYPNOSIS,fr, getWorld()->getValidProjectileId());
			memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
			getWorld()->insertProjectile(pr,goalx,goaly,reg);
			break;

		case Action::HYPNOSIS2:
			// Alle Objekte im Umkreis von 2 um den Zielpunkt suchen
			s.m_coordinate_x = goalx;
			s.m_coordinate_y = goaly;
			s.m_radius = 2;
			getWorld()->getSWObjectsInShape(&s, reg, &res,Geometry::LAYER_AIR,CREATURE,this);
			for (it=res.begin();it!=res.end();++it)
			{
				// Fuer alle Lebewesen ein Projektil Hypnose erzeugen
				if ((*it)->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT)
				{
					goalx = (*it)->getGeometry()->m_shape.m_coordinate_x;
					goaly = (*it)->getGeometry()->m_shape.m_coordinate_y;
					pr = new DmgProjectile(getWorld(),Projectile::HYPNOSIS,fr, getWorld()->getValidProjectileId());
					memcpy(pr->getDamage(),&m_damage,sizeof(Damage));
					getWorld()->insertProjectile(pr,goalx,goaly,reg);
				}
			}
			break;

		case Action::KEEN_MIND:
			// Modifikation:
			// 50% der Willenskraft auf Magie
			cbam.m_time =70000;
			cbam.m_dmagic_power = m_base_attr_mod.m_willpower/2;

			// alle Verbuendeten im Umkreis von 12 suchen und Modifikation anwenden
			s.m_coordinate_x = x;
			s.m_coordinate_y = y;
			s.m_radius = 12;
			getWorld()->getSWObjectsInShape(&s, reg, &res,Geometry::LAYER_AIR,CREATURE,0);
			for (it=res.begin();it!=res.end();++it)
			{
				if (getWorld()->getRelation(fr,(*it)) == WorldObject::ALLIED)
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
		DEBUG("monster killed, apply mod");
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
	// Punkt zu dem das Objekt bewegt werden soll
	float xnew = getGeometry()->m_shape.m_coordinate_x+getMoveInfo()->m_speed_x * time;
	float ynew = getGeometry()->m_shape.m_coordinate_y+getMoveInfo()->m_speed_y * time;

	// Kreis um den Zielpunkt
	list<ServerWObject*> result;
	Shape scopy;
	scopy.m_radius = getGeometry()->m_shape.m_radius*1.05;
	scopy.m_coordinate_x = xnew;
	scopy.m_coordinate_y = ynew;
	scopy.m_type = Shape::CIRCLE;

	// Ebene in der gesucht werden soll
	short layer = getGeometry()->m_layer;

	// Alle kollidierenden Objekte suchen
	getWorld()->getSWObjectsInShape(&(scopy),getGridLocation()->m_region,&result,layer, CREATURE | FIXED,this);

	if (result.size()!=0)
	{
		// es gibt kollidierende Objekte
		DEBUG5("aktuelle Koordinaten %f %f",getGeometry()->m_shape.m_coordinate_x,getGeometry()->m_shape.m_coordinate_y);
		list<ServerWObject*>::iterator i;

		Shape* s2;
        // Liste der kollidierenden Objekte durchgehen
		for (i= result.begin();i!=result.end();++i)
		{
			DEBUG5("Kollision %i",(*i)->getId());
			s2 =&((*i)->getGeometry()->m_shape);
			// wenn mit dem Zielobjekt kollidiert Bewegung beenden
			if ((*i)->getId() == getCommand()->m_goal_object_id)
			{
				getMoveInfo()->m_speed_x =0;
				getMoveInfo()->m_speed_y =0;
				m_event_mask |= Event::DATA_MOVE_INFO;
				break;
			}

			// Wenn die Faehigkeit Stuermen ist dann Bewegung beenden
			if (m_command.m_type == Action::CHARGE || m_command.m_type == Action::STORM_CHARGE)
			{
				// Behandlung von *Charge*
				m_command.m_goal_object_id = (*i)->getId();
				getMoveInfo()->m_speed_x =0;
				getMoveInfo()->m_speed_y =0;
				m_event_mask |= Event::DATA_MOVE_INFO;
				return;
			}
			else
			{
				// Kollision behandeln
				handleCollision(s2);
			}

		}

		// neuen erreichten Punkt ausrechnen und testen ob dieser akzeptabel ist
		xnew = getGeometry()->m_shape.m_coordinate_x+getMoveInfo()->m_speed_x * time;
		ynew = getGeometry()->m_shape.m_coordinate_y+getMoveInfo()->m_speed_y * time;

		// Kreis um den neuen Zielpunkt
		scopy.m_coordinate_x = xnew;
		scopy.m_coordinate_y = ynew;
		DEBUG5("neue Koordinaten %f %f",xnew,ynew);
		scopy.m_radius = getGeometry()->m_shape.m_radius;
		result.clear();

		// Suchen der Objekte um den neuen Zielpunkt
		getWorld()->getSWObjectsInShape(&(scopy),getGridLocation()->m_region,&result,layer, CREATURE | FIXED,this);

		// Wenn immer noch Kollision vorliegt Bewegung beenden
		if (result.size()!=0)
		{
			DEBUG5("still colliding");

			getMoveInfo()->m_speed_x =0;
			getMoveInfo()->m_speed_y =0;
			m_event_mask |= Event::DATA_MOVE_INFO;
		}

	}
}

void Creature::handleCollision(Shape* s2)
{
	float dx,dy,x1,x2,y1,y2,d;

	// Koordinaten des kollidierenden Objektes
	x1 = getGeometry()->m_shape.m_coordinate_x;
	y1 = getGeometry()->m_shape.m_coordinate_y;

	// eigene Koordinaten
	x2 = s2->m_coordinate_x;
	y2 = s2->m_coordinate_y;
	bool circ = true;


	if (s2->m_type==Shape::RECT)
	{
		// Kollision mit Rechteckt
		circ = false;
		DEBUG5("Rechteck");

		// Ausdehnung des Rechtseckes
		float ex = s2->m_extent_x;
		float ey = s2->m_extent_y;
		float r = getGeometry()->m_shape.m_radius;
		DEBUG5("koll %f %f %f %f %f %f",x1,y1,x2,y2,ex,ey);

		if ((x1<x2-ex-r) || (x1>x2+ex+r))
		{
			// Kollision mit einer senkrechten Kante
			// x-Anteil der Bewegung auf 0 setzen
			getMoveInfo()->m_speed_x=0;
			DEBUG5("x-anteil genullt");
		}
		else
		{
			if (y1<y2-ey-r || y1>y2+ey+r)
			{
				// Kollision mit einer waagerechten Kante
				// y-Anteil der Bewegung auf 0 setzen
				DEBUG5("y-anteil genullt");
				getMoveInfo()->m_speed_y=0;
			}
			else
			{
				// Kollision an der Ecke des Rechteckes
				DEBUG5("kollision an einer Ecke");

				// ermitteln der Ecke an der die Kollision stattfand
				if (x1>x2)
					x2 = x2+ex;
				else
					x2 = x2-ex;

				if (y1>y2)
					y2 = y2+ey;
				else
					y2 = y2-ey;

				// Problem behandeln wie einen Kreis um diese Ecke
				circ = true;
			}
		}
	}

	if (circ)
	{
		// Behandlung Kollision mit Kreis

		// Vektor vom eigenen Mittelpunkt zum Mittelpunkt des kollidierenden Objektes (normiert)
		dx = x2-x1;
		dy = y2-y1;
		d = 1/sqrt(dx*dx+dy*dy);
		DEBUG5("speed %f %f",getMoveInfo()->m_speed_x,getMoveInfo()->m_speed_y);
		dx *= d;
		dy *= d;

		// Projektion des Geschwindigkeitsvektors auf diesen Vektor
		DEBUG5("Richtung k1 k2 %f %f",dx,dy);
		d = dx*getMoveInfo()->m_speed_x +dy*getMoveInfo()->m_speed_y;
		DEBUG5("Skalarprodukt %f",d);
		dx *=d;
		dy *=d;
		DEBUG5("Projektion k1 k2 %f %f",dx,dy);

		// Abziehen vom Geschwindigkeitsvektor (es bleibt nur der Anteil senkrecht zur Verbindungslinie der Mittelpunkt erhalten)
		getMoveInfo()->m_speed_x -= dx;
		getMoveInfo()->m_speed_y -= dy;
		DEBUG5("speed neu %f %f",getMoveInfo()->m_speed_x,getMoveInfo()->m_speed_y);
		/*
		dx = x2-x1;
		dy = y2-y1;
		d = sqrt(dx*dx+dy*dy);
		if (d!=0)
		{
			d= 1/d;
			d = dx*getMoveInfo()->m_speed_x +dy*getMoveInfo()->m_speed_y;
			DEBUG5("Skalarprodukt neu %f",d);
		}
		*/

	}

	// neue Geschwindigkeit normieren
	d =(sqrt(getMoveInfo()->m_speed_x*getMoveInfo()->m_speed_x+getMoveInfo()->m_speed_y*getMoveInfo()->m_speed_y));
	if (d!=0)
	{
		d= 1/d;
		getMoveInfo()->m_speed_x *= d*getBaseAttr()->m_step_length/m_action.m_time;
		getMoveInfo()->m_speed_y *= d*getBaseAttr()->m_step_length/m_action.m_time;
		DEBUG5("speed %f %f",getMoveInfo()->m_speed_x,getMoveInfo()->m_speed_y);
		

	}
	m_event_mask |= Event::DATA_MOVE_INFO;

}

void Creature::updateCommand()
{
	// Wenn aktuelles Kommando keine Aktion vorschreibt
	DEBUG5("next command: %i ",m_next_command.m_type);
	if (m_next_command.m_type != Action::NOACTION)
	{
		// Naechstes Kommando uebernehmen
		m_command.m_type=m_next_command.m_type;
		m_command.m_goal_coordinate_x=m_next_command.m_goal_coordinate_x;
		m_command.m_goal_coordinate_y=m_next_command.m_goal_coordinate_y;
		m_command.m_goal_object_id=m_next_command.m_goal_object_id;
		m_command.m_range=m_next_command.m_range;
		m_command.m_damage_mult = 1;

		// Naechstes Kommando auf nichts setzen
		m_next_command.m_type = Action::NOACTION;
		
		m_event_mask |= Event::DATA_COMMAND;
		m_event_mask |= Event::DATA_NEXT_COMMAND;
			
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
			m_event_mask |= Event::DATA_ACTION;
		}
		m_action.m_type = Action::NOACTION;
		return;

	}


	DEBUG5("calc action for command %i",m_command.m_type);
	m_event_mask |= Event::DATA_ACTION;


	// Reichweite der Aktion berechnen
	float range = m_command.m_range;

	if (m_command.m_type == Action::WALK)
		range =getGeometry()->m_shape.m_radius;

	if (m_command.m_type == Action::TAKE_ITEM)
		range = 1;

	// Koordinaten des Zielpunktes
	float goalx = m_command.m_goal_coordinate_x;
	float goaly = m_command.m_goal_coordinate_y;
	m_action.m_goal_coordinate_x = goalx;
	m_action.m_goal_coordinate_y = goaly;

	// Zeiger auf das Zielobjekt
	ServerWObject* goal=0;

	// eigene Position
	float x = getGeometry()->m_shape.m_coordinate_x;
	float y = getGeometry()->m_shape.m_coordinate_y;

	// Koordinatenn/ Radius des Zieles bei Behandlung als Kreis
	// noetig fuer Abstandsberechnung
	float cx = goalx;
	float cy = goaly;
	float r = getGeometry()->m_shape.m_radius+range;

	// Wenn Zielobjekt per ID gegeben

	if ( m_command.m_goal_object_id !=0 && m_command.m_type != Action::TAKE_ITEM)
	{
		DEBUG5("goal ID: %i",m_command.m_goal_object_id);
		// Zielobjekt holen
		goal = getWorld()->getSWObject(m_command.m_goal_object_id,getGridLocation()->m_region);



		if (goal ==0)
		{
			// Zielobjekt existiert nicht mehr, abbrechen
			m_command.m_type = Action::NOACTION;
			m_action.m_type = Action::NOACTION;
			m_action.m_elapsed_time =0;
			m_command.m_damage_mult=1;
			m_event_mask |= Event::DATA_COMMAND;
			return;
		}

		// Ziel muss aktiv sein
		if (goal->getState() != STATE_ACTIVE)
		{
			DEBUG5("refused to interakt with inactive objekt %i",m_command.m_goal_object_id);
			m_command.m_type = Action::NOACTION;
			m_action.m_type = Action::NOACTION;
			m_command.m_damage_mult=1;
			m_action.m_elapsed_time =0;
			m_event_mask |= Event::DATA_COMMAND;
			return;
		}

		// Zielkoordinaten neu setzen (Koordinaten des Zielobjektes)
		goalx = goal->getGeometry()->m_shape.m_coordinate_x;
		goaly = goal->getGeometry()->m_shape.m_coordinate_y;

		// Berechnen der Koordinaten fuer Abstandsberechnung
		if (goal->getGeometry()->m_shape.m_type == Shape::CIRCLE)
		{
			// Ziel hat Kreisform, Koordinaten uebernehmen
			cx = goalx;
			cy = goaly;
			r += goal->getGeometry()->m_shape.m_radius;
		}
		else
		{
			// Ziel ist rechteckig
			// die am naechsten gelegene Ecke suchen und als Mittelpunkt behandeln
			if (x>goalx)
				cx +=  goal->getGeometry()->m_shape.m_extent_x;
			else
				cx -=  goal->getGeometry()->m_shape.m_extent_x;
			if (y>goaly)
				cy +=  goal->getGeometry()->m_shape.m_extent_y;
			else
				cy -=  goal->getGeometry()->m_shape.m_extent_y;
		}
	}

	DEBUG5("goal of command: %f %f", goalx,goaly);
	DEBUG5("start %f %f ziel %f %f range %f",x,y,cx,cy,r);

	// Stuermen ohne Zielobjekt hat keinen Zielradius
	if ((m_command.m_type == Action::CHARGE || m_command.m_type == Action::STORM_CHARGE) &&  m_command.m_goal_object_id==0)
	{
		r=0;
	}



	if (Action::getActionInfo(m_command.m_type)->m_distance == Action::MELEE || Action::getActionInfo(m_command.m_type)->m_base_action == Action::WALK)
	{
		// Aktion fuer die man an das Ziel hinreichend nahe herankommen muss

		// Testen ob das Ziel in Reichweite ist
		if ((sqr(r) > sqr(cx-x)+sqr(cy-y)))
		{
			// Ziel ist in Reichweite, geplante Aktion ausfuehren
			if (m_command.m_type != Action::WALK)
			{
				m_action.m_type = m_command.m_type;
				m_action.m_goal_object_id = m_command.m_goal_object_id;
				m_action.m_goal_coordinate_x = goalx;
				m_action.m_goal_coordinate_y = goaly;
			}
			else
			{
				m_action.m_type = Action::NOACTION;	
			}
			
			// Kommando damit abgeschlossen
			m_command.m_type = Action::NOACTION;
			m_command.m_damage_mult = 1;
			m_event_mask |= Event::DATA_COMMAND;
		}
		else
		{
			// Ziel ist nicht in Reichweite, hinlaufen

			m_action.m_type = Action::WALK;
			if ((m_command.m_type == Action::CHARGE || m_command.m_type == Action::STORM_CHARGE)  )
			{
				// Sturmangriff
				DEBUG("Charge");
				float d;

				// beim ersten Mal richtung neu ausrechnen, das ist der Fall wenn der Schadensmultiplikator gleich 1 ist
				// sonst nur beschleunigen
				if (m_command.m_damage_mult>1)
				{
					// Richtung nicht neu ausrechnen, beschleunigen
					// Geschwindigkeit normieren, mit Schadensmultiplikator multiplizieren
					d = 1/sqrt( getMoveInfo()->m_speed_x*getMoveInfo()->m_speed_x+getMoveInfo()->m_speed_y*getMoveInfo()->m_speed_y);
					m_command.m_damage_mult = (m_command.m_damage_mult+2);

					if (m_command.m_type == Action::CHARGE)
						m_command.m_damage_mult *= 0.85;
					else
						m_command.m_damage_mult *= 0.93;

					// Schaden neu berechnen
					recalcDamage();

					d *= sqrt(m_command.m_damage_mult);
					getMoveInfo()->m_speed_x *= d;
					getMoveInfo()->m_speed_y *= d;
					m_event_mask |= Event::DATA_MOVE_INFO;
					DEBUG(" dir %f %f dmg mult %f",getMoveInfo()->m_speed_x,getMoveInfo()->m_speed_y,m_command.m_damage_mult);
				}
				else
				{
					// Richtung: direct zum Ziel;
					DEBUG("calc charge dir");
					// Vektor von der eigenen Position zum Ziel, normieren
					getMoveInfo()->m_speed_x = goalx-x;
					getMoveInfo()->m_speed_y = goaly-y;
					d = sqrt( getMoveInfo()->m_speed_x*getMoveInfo()->m_speed_x+getMoveInfo()->m_speed_y*getMoveInfo()->m_speed_y);
					if (d!=0)
					{
						d= 1/d;
						getMoveInfo()->m_speed_x *= d;
						getMoveInfo()->m_speed_y *= d;
						m_command.m_damage_mult = (m_command.m_damage_mult+0.1);
					}
					else
					{
						m_command.m_type = Action::NOACTION;
						m_action.m_type = Action::NOACTION;
						m_action.m_elapsed_time =0;
						m_command.m_damage_mult = 1;
						m_event_mask |= Event::DATA_COMMAND;

					}
					m_event_mask |= Event::DATA_MOVE_INFO;

				}


			}
			else
			{
				// Berechnen der Bewegungsrichtung
				calcWalkDir(goalx,goaly, goal);

			}
		}

	}
	else
	{
		// keine Nahkampfaktion bzw laufen -> Action direkt ausfuehren
		m_action.m_type = m_command.m_type;
		m_action.m_goal_object_id = m_command.m_goal_object_id;
		m_action.m_goal_coordinate_x = goalx;
		m_action.m_goal_coordinate_y = goaly;

		m_command.m_type = Action::NOACTION;
		m_command.m_damage_mult = 1;
		m_event_mask |= Event::DATA_COMMAND;
	}
	
	if (m_action.m_type !=0)
	{
		DEBUG5("calc action: %i at %f %f, dir %f %f",m_action.m_type, getGeometry()->m_shape.m_coordinate_x,	getGeometry()->m_shape.m_coordinate_y, getMoveInfo()->m_speed_x,getMoveInfo()->m_speed_y);
	}

}

void Creature::calcStatusModCommand()
{
	// Statusmod verwirrt
	// diese Aktion nur vom Server ausloesen lassen
	if (m_dyn_attr.m_status_mod_time[Damage::CONFUSED]>0 && getWorld()->isServer())
	{
		// eigene Position
		float x=getGeometry()->m_shape.m_coordinate_x;
		float y=getGeometry()->m_shape.m_coordinate_y;
		// aktuelle Bewegungsrichtung
		float sx = getMoveInfo()->m_speed_x;
		float sy = getMoveInfo()->m_speed_y;
		float range = m_base_attr.m_step_length;
		float nx,ny;

		// Normieren der Bewegungsgeschwindigkeit
		float d;
		d = sqr(sx)+sqr(sy);
		if (d>0)
		{
			d = 1/sqrt(d);
			sx *= d;
			sy *= d;
		}
		DEBUG5("old speed %f %f",sx,sy);
		// zufaellige Richtung auswuerfeln und normieren
		float newsx =1-rand()*2.0/RAND_MAX;
		float newsy =1-rand()*2.0/RAND_MAX;
		d = 1/sqrt(sqr(newsx)+sqr(newsy));
		newsx *= d;
		newsy *= d;

		// neue Bewegungsrichtung = 70% der alten + 30% der zufaellig ausgewaehlten
		sx = 0.7*sx+0.3*newsx;
		sy = 0.7*sy+0.3*newsy;

		// neue Bewegungsrichtung normieren
		d = sqrt(sqr(sx)+sqr(sy));
		if (d>0)
		{
			d= 1/d;
			sx *= d;
			sy *= d;
		}

		DEBUG5("new speed %f %f",sx,sy);
		DEBUG5("pos of player %f %f",x,y);
		// Berechnen den Punktes der bei dieser Bewegung erreicht wird
		nx = x;
		ny = y;
		x += sx *range;
		y += sy *range;
		DEBUG5("pos of goal %f %f",x,y);

		// Kreis um den Zielpunkt, Radius gleich Radius des Lebewesens
		Shape s;
		s.m_coordinate_x =x;
		s.m_coordinate_y =y;
		s.m_type = Shape::CIRCLE;
		s.m_radius = getGeometry()->m_shape.m_radius;
		ServerWObject* wo =0;
		list<ServerWObject*> res;
		list<ServerWObject*>::iterator it;

		// ermitteln der Objekte mit denen bei der Bewegung kollidiert wird
		getWorld()->getSWObjectsInShape(&s,getGridLocation()->m_region,&res,Geometry::LAYER_AIR,CREATURE | FIXED,this);

		// Zufallszahl fuer zufaellige Angriffe
		float r = rand()*1.0/RAND_MAX;
		if (!res.empty())
		{
			// Es gibt ein kollidierendes Objekt, als Zielobjekt setzen
			wo = *(res.begin());
			x = wo->getGeometry()->m_shape.m_coordinate_x;
			y = wo->getGeometry()->m_shape.m_coordinate_y;

		}
		else
		{
			// es gibt kein kollidierendes Objekt, mit 30% Wahrscheinlichkeit Angriff ins nichts
			if (r>0.7)
			{
				range += m_base_attr_mod.m_attack_range;
				x= nx + sx *range;
				y= ny + sy *range;

			}
		}
		DEBUG5("entscheidung durch %p, %f",wo,r);
		// Angriff ausfuehren, wenn Zielobjekt vorhanden, sonst mit 30% Wahrscheinlichkeit
		if (wo!=0 || r > 0.7)
		{
			// Angriff
			m_command.m_type = m_base_action;
			m_command.m_goal_coordinate_x =x;
			m_command.m_goal_coordinate_y =y;
			m_command.m_goal_object_id =0;
			m_command.m_range = getBaseAttr()->m_attack_range;
			m_event_mask |= Event::DATA_COMMAND;

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
			getMoveInfo()->m_speed_x =sx;
			getMoveInfo()->m_speed_y = sy;
			m_event_mask |= Event::DATA_COMMAND | Event::DATA_MOVE_INFO;
			return;
		}

	}
	else if (m_dyn_attr.m_status_mod_time[Damage::BERSERK]>0)
	{
		// Behandlung von Berserker
		int id =0;
		float rmin =1000;
		// eigene Position
		float x=getGeometry()->m_shape.m_coordinate_x;
		float y=getGeometry()->m_shape.m_coordinate_y;

		float wx,wy,gx=0,gy=0;

		// Kreis mit Radius 8 um eigenen Mittelpunkt
		Shape s;
		s.m_type = Shape::CIRCLE;
		s.m_radius =8;
		s.m_coordinate_x = x;
		s.m_coordinate_y = y;
		list<ServerWObject*> res;
		list<ServerWObject*>::iterator i;
		res.clear();

		// Suchen aller Objekte im Kreis
		getWorld()->getSWObjectsInShape(&s,getGridLocation()->m_region, &res,Geometry::LAYER_AIR,CREATURE,this);
		for (i=res.begin();i!= res.end();++i)
		{
			// nur aktive Lebewesen beruecksichtigen
			if ((*i)->getState() != STATE_ACTIVE)
			{
				continue;
			}

			DEBUG5("checking obj %i",(*i)->getId());
			// Fuer feindliche Lebewesen
			if (getWorld()->getRelation(getTypeInfo()->m_fraction,*i) == WorldObject::HOSTILE)
			{
				// Abstand zum eigenen Mittelpunkt berechnen
				DEBUG5("hostile");
				wx = (*i)->getGeometry()->m_shape.m_coordinate_x;
				wy = (*i)->getGeometry()->m_shape.m_coordinate_y;
				// Objekt mit dem kleinsten Abstand als Zielobjekt setzen
				if (sqr(wx-x)+sqr(wy-y)<rmin)
				{
					rmin = sqr(wx-x)+sqr(wy-y);
					id = (*i)->getId();
					gx = wx;
					gy = wy;
				}

			}
		}

		// Wenn Zielobjekt gefunden wurde
		if (id !=0)
		{
			DEBUG5("attack id %i",id);
			// Angriff
			m_command.m_type = Action::ATTACK;
			m_command.m_goal_object_id = id;
			m_command.m_goal_coordinate_x = gx;
			m_command.m_goal_coordinate_y = gy;
			m_command.m_range = getBaseAttr()->m_attack_range;

			// nur Nahkampf, daher Reichweite die von Fernwaffen kommt reduzieren
			if (m_command.m_range >4)
			{
				DEBUG("capped range for berserk");
				m_command.m_range= 1;
			}

		}
		else
		{
			// nichts machen
			m_command.m_type = Action::NOACTION;
		}
		m_event_mask |= Event::DATA_COMMAND;
	}
}

void Creature::calcWalkDir(float goalx,float goaly,ServerWObject* goal)
{
	// eigene Position
	float x = getGeometry()->m_shape.m_coordinate_x;
	float y = getGeometry()->m_shape.m_coordinate_y;
	float dir[2];

	// wenn als Ziel ein Lebenwesen angegeben ist
	if (goal !=0 && goal->getTypeInfo()->m_type != TypeInfo::TYPE_FIXED_OBJECT )
	{
		DEBUG5("using pot field of object %i",goal->getId());
		Creature* cr = (Creature*) goal;
		// Potentialfeld des Ziellebewesens verwenden
		cr->getPathDirection(x,y,getGridLocation()->m_region,2*getGeometry()->m_shape.m_radius, getGeometry()->m_layer,dir);
	}
	else
	{
		DEBUG5("using own pot field");
		float x = getGeometry()->m_shape.m_coordinate_x;
		float y = getGeometry()->m_shape.m_coordinate_y;

		// Bewegung zu einem Punkt oder festen Objekt
		// eigenes Potentialfeld verwenden

		// true, wenn das eigene Potentailfeld neu berechnet werden muss
		bool recalc = false;

		// true, wenn als Richtung die direkte Verbindung genutzt wird
		bool direct = false;

		// Wenn in einer Koordinatenrichtung Abstand groesser 10 ist direkte Verbindung benutzen
		if (fabs(x-goalx)>10 || fabs(y-goaly)>10)
		{
			direct = true;
		}

		if (m_path_info ==0)
		{
			// Potentialfeld existiertnoch nicht
			DEBUG5("allocating new pot field");
			m_path_info = new PathfindInfo;

			// Qualitaet der Suche, gehe quadratisch in die Laufzeit ein
			int qual=4;
			int dim = 20*qual+1;

			// Potentialfeld
			m_path_info->m_pot = new Matrix2d<float>(dim,dim);
			// Blockmatrix
			m_path_info->m_block = new Matrix2d<char>(dim,dim);

			// Kantenlaenge eines Quadrates im Suchfeld
			//float sqs =  getGeometry()->m_shape.m_radius*2/ qual;

			// Senke des Feldes ist der Zielpunkt
			m_path_info->m_start_x= goalx;
			m_path_info->m_start_y= goaly;

			m_path_info->m_dim = dim;
			m_path_info->m_layer= Geometry::LAYER_BASE | Geometry::LAYER_AIR;
			m_path_info->m_region=getGridLocation()->m_region;
			m_path_info->m_base_size = getGeometry()->m_shape.m_radius*2;
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
			float d = sqr(m_path_info->m_start_x -goalx) +sqr(m_path_info->m_start_y -goaly);
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

				// Zentrum ist die eigene Position
				m_path_info->m_center_x= goalx +roundf(x-goalx);
				m_path_info->m_center_y=goaly +roundf(y-goaly);
				// Senke ist der Zielpunkt
				m_path_info->m_start_x= goalx;
				m_path_info->m_start_y= goaly;

				// Timer auf 0 setzen
				m_path_info->m_timer=0;


				// Blockmatrix berechnen
				getWorld()->calcBlockmat(m_path_info);


				// Potentialfeld berechnen
				getWorld()->calcPotential(m_path_info);

			}

			DEBUG5("calcing direction from %f %f",x,y);
			// Richtung auf Basis des eigenen Potentialfeldes berechnen
			getWorld()->calcPathDirection(m_path_info, x, y, dir);
		}
		else
		{
			// direkte Verbindung als Richtung verwenden
			DEBUG5("using direct way");
			dir[0] = goalx-x;
			dir[1] = goaly-y;
		}

	}

	// Richtung normieren
	float l = sqrt(dir[0]*dir[0]+dir[1]*dir[1]);
	if (l==0)
	{
		m_command.m_type = Action::NOACTION;
		m_action.m_type = Action::NOACTION;
		m_action.m_elapsed_time =0;
		m_command.m_damage_mult=1;
		m_event_mask |= Event::DATA_COMMAND | Event::DATA_ACTION;
		return;
	}
	else
	{
		m_event_mask |= Event::DATA_MOVE_INFO;
		// TODO: Wende über 90 Grad behandeln
		if (getMoveInfo()->m_speed_x*dir[0]+getMoveInfo()->m_speed_y*dir[1]>0)
		{
			getMoveInfo()->m_speed_x = dir[0] /l;
			getMoveInfo()->m_speed_y = dir[1]/l;
		}
		else
		{
			DEBUG5("wenden");
			getMoveInfo()->m_speed_x = dir[0] /l;
			getMoveInfo()->m_speed_y = dir[1]/l;
		}
		
	}
}

bool Creature::update (float time)
{
	// CreatureDynAttr* dynattr = getDynAttr();
	DEBUG5("Update des Creatureobjekts [%i] wird gestartet", getId());
	// interne Variable um Fehler zu speichern
	bool result=true;


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
				m_event_mask |= Event::DATA_COMMAND;
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
	list<CreatureBaseAttrMod>::iterator i;
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
	if (getWorld()->isServer())
	{
		if (m_base_attr_mod.m_special_flags & FLAMEARMOR && getWorld()->timerLimit(2))
		{
			// Flammenruestung
			
			
			// Schaden fuer Flammenruestung setzen
			Damage d;
			d.m_min_damage[Damage::FIRE] = 500*m_base_attr_mod.m_magic_power*0.0003;
			d.m_max_damage[Damage::FIRE] = 500*m_base_attr_mod.m_magic_power*0.0005;
			d.m_multiplier[Damage::FIRE]=1;
			d.m_attacker_fraction = getTypeInfo()->m_fraction;
	
			list<ServerWObject*> res;
			res.clear();
			list<ServerWObject*>::iterator it;
	
			// Kreis um eigenen Mittelpunkt mit Radius eigener Radius plus 1
			Shape s;
			s.m_coordinate_x = getGeometry()->m_shape.m_coordinate_x;
			s.m_coordinate_y = getGeometry()->m_shape.m_coordinate_y;
			s.m_type = Shape::CIRCLE;
			s.m_radius = getGeometry()->m_shape.m_radius+1;
			Creature* cr;
			short reg = getGridLocation()->m_region;
	
			// Alle Objekte im Kreis suchen
			getWorld()->getSWObjectsInShape(&s, reg, &res,Geometry::LAYER_AIR,CREATURE,this);
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
		if (getWorld()->isServer() && getWorld()->timerLimit(2))
		{
			// Vergiftet
			if (m_dyn_attr.m_status_mod_time[Damage::POISONED]>0)
			{
				// Schaden pro Sekunde 1/60 der HP
				DEBUG5("poisoned");
				m_dyn_attr.m_health -= 500*m_base_attr_mod.m_max_health / 60000;
				m_event_mask |= Event::DATA_HP;
			}
	
			// brennend
			if (m_dyn_attr.m_status_mod_time[Damage::BURNING]>0)
			{
				// Schaden pro Sekunde 1/90 der HP (bei 0 Feuerresistenz)
				DEBUG5("burning");
				m_dyn_attr.m_health -= (100-m_base_attr_mod.m_resistances[Damage::FIRE])*500*m_base_attr_mod.m_max_health / 9000000;
				m_event_mask |= Event::DATA_HP;
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
					
					m_event_mask |= Event::DATA_STATE | Event::DATA_ACTION;
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
	return result;
}

/*
void  Creature::onTrade (int id) {

	// Testen ob bereits ein Handel existiert
	if (m_trade_id!=0)
	{
		// Wenn ja, weiteren Handel ablehnen
		m_trade_id=0;
		getObjectType().setActionType(ACTION_NONE);
		m_action->setType(ACTION_NONE);
		m_command->setType(COMMAND_NONE);
		return;
	}
	else
	{
		// Es existiert bislang kein Handel
		DEBUG5("Handelsinitialisierung starten");
		ServerWObject* wo = getWorld()->getWorldObject(id);
		// Wenn das Ziel nicht existiert Aktion beenden
		if (wo ==0 || wo->getObjectType().getObjectType()==OBJECTTYPE_FIXED_OBJECT)
		{
			m_trade_id=0;
			getObjectType().setActionType(ACTION_NONE);
			m_action->setType(ACTION_NONE);
			m_command->setType(COMMAND_NONE);
			return;
		}



		DEBUG5("Handel initialisieren\n");
		Creature* pl = (Creature*) wo;

		// wenn der Handelspartner ein feindlich gesinnter Spieler ist, Aktion beenden
		if (pl->getObjectType().getObjectType()==OBJECTTYPE_USER && pl->getMode()==MODE_AGRESSIVE )
		{

			m_trade_id=0;
			getObjectType().setActionType(ACTION_NONE);
			m_action->setType(ACTION_NONE);
			m_command->setType(COMMAND_NONE);
			return;
		}

		// Handelspartner hat schon einen Handel, Aktion abbrechen
		if (pl->getTradeId()!=0)
		{
			m_trade_id=0;
			getObjectType().setActionType(ACTION_NONE);
			m_action->setType(ACTION_NONE);
			m_command->setType(COMMAND_NONE);
			return;
		}

		getObjectType().setActionType(ACTION_TRADE);	//Handel treiben
		getObjectType().setDirection(getWorld()->getDirection(getCoordinateX(),getCoordinateY(),pl->getCoordinateX(),pl->getCoordinateY()));
		m_action->setType(ACTION_TRADE);
		m_action->setTime(200);				//200ms Zeit
		m_action->setProgress(0);			//noch kein Fortschritt
		m_action->setGoalObjectId(id);

		if (pl->getState() == STATES_ACTIVE)
			pl->getObjectType().setDirection(getWorld()->getDirection(pl->getCoordinateX(),pl->getCoordinateY(),getCoordinateX(),getCoordinateY()));

		int trade_id = getWorld()->newTrade(wobase->m_id,id);
		m_trade_id=trade_id;
		Trade* trade = getWorld()->getTrade(trade_id);
		if (trade_id==0 || trade ==0)
		{
			// Fehler aufgetreten, kein Trade Objekt initialisiert
			m_trade_id=0;
			getObjectType().setActionType(ACTION_NONE);
			m_action->setType(ACTION_NONE);
			m_command->setType(COMMAND_NONE);
		}
		else
		{
			// Trade Objekt mit den Daten der Handelspartner füllen
			//trade->setMaxBaggage(getMaxBaggage(),0);
		//	trade->setMaxBaggage(
		//			//maximum weniger linkesitemgewicht, weniger rechtesitemgewicht, wenigerbodyitemgewicht
		//			getMaxBaggage()-getLeftHandItem()->getWeight()-getRightHandItem()->getWeight()-getBodyItem()->getWeight() ,
		//			0
		//							 );
			trade->setMaxBaggage(pl->getMaxBaggage(),0);
		//	trade->setMaxBaggage(
		//			//maximum weniger linkesitemgewicht, weniger rechtesitemgewicht, wenigerbodyitemgewicht
		//			pl->getMaxBaggage()-pl->getLeftHandItem()->getWeight()-pl->getRightHandItem()->getWeight()-pl->getBodyItem()->getWeight(),
		//			1
		//							  );
			trade->setCreatureType(getObjectType().getObjectType(),0);
			trade->setCreatureType(pl->getObjectType().getObjectType(),1);
			*(trade->getInventory(0)->getItems())= *(getItems()->getItems());
			*(trade->getInventory(1)->getItems())= *(pl->getItems()->getItems());

			pl->setTradeId(m_trade_id);
		}
	}
}
*/


void Creature::gainExperience (int exp)
{
	if (getTypeInfo()->m_type != TypeInfo::TYPE_PLAYER)
		return;


	// Erfahrung dazu addieren
	m_dyn_attr.m_experience += exp;
	m_event_mask |= Event::DATA_EXPERIENCE;

	// Solange Level aufsteigen, bis exp < max_exp
	while (m_dyn_attr.m_experience>= m_base_attr.m_max_experience)
	{
		gainLevel();
	}
}

void Creature::gainLevel()
{
	m_event_mask |= Event::DATA_LEVEL;

}

/*
void Creature::useItem(int type_id)
{
	DEBUG5("Item wird benutzt!");
	// Testen ob valide id übergeben
	if (type_id!=0)
	{
		Item* item;

		// Item aus dem Inventar holen
		item = getItems()->getItem(type_id);


		// Testen ob Item erfolgreich geholt, sonst existiert es garnicht im Inventar
		if (item!=0)
		{
			item->setNumber(1);
			// gibt an ob das Item beim benutzen verbraucht wird
			bool use_up=false;

			// Gibt an, ob das Item vertauscht wurde
			bool swap_item=false;
			// altes Item (vor dem vertauschen)
			Item* old_item=0;

			// hier der switch nach itemtyp
			Item* swap;
			switch (item->getGeneralTypeId())
			{
				case ITEM_MONEY:
					//Beispiel für Geld

					break;
				case ITEM_ARMOR:
					// Beispiel für die Ruestung:
					// Item am Körper mit dem rausgenommenen Item austauschen
					swap = getBodyItem();
					setBodyItem(item);
					item=swap;

					// Ruestungsklasse setzen
					setArmor(getBodyItem()->getUsage());

					// Item ist vertauscht worden
					swap_item=true;
					old_item=getBodyItem();
					break;
				case ITEM_WEAPON:
					//Beispiel für Waffen
					// Item in rechter Hand mit dem rausgenommenen Item austauschen
					swap = getRightHandItem();
					setRightHandItem(item);
					item=swap;

					// Angriffsklasse setzen
					setAttack(getRightHandItem()->getUsage());

					// Item ist vertauscht worden
					swap_item=true;
					old_item=getRightHandItem();
					break;
				case ITEM_POTION:
					// Beispiel für Trank
					// Lebenspunkte dazu addieren, wenn es weniger als m_max_health werden
					if ((getMaxHealth())>(getHealth()+item->getUsage()))
					{
						DEBUG5("Lebenspunkte vorher: %i / %i", getHealth(), getMaxHealth());
						setHealth(getHealth()+item->getUsage());
						DEBUG5("Lebenspunkte nachher: %i / %i", getHealth(), getMaxHealth());
					}
					else
					//sonst Lebenspunkte voll befüllen
					{
						DEBUG5("Lebenspunkte vorher: %i / %i", getHealth(), getMaxHealth());
						setHealth(getMaxHealth());
						DEBUG5("Lebenspunkte nachher: %i / %i", getHealth(), getMaxHealth());
					}
					// Traenke werden verbraucht
					use_up=true;
					break;
				case ITEM_SHIELD:
					//Beispiel für Schilde
					// Item in linker Hand mit dem rausgenommenen Item austauschen
					swap = getLeftHandItem();
					setLeftHandItem(item);
					item=swap;

					// Verteidigungsklasse setzen
					setDefense(getLeftHandItem()->getUsage());

					// Item ist vertauscht worden
					swap_item=true;
					old_item=getLeftHandItem();
					break;
				default:
					break;
			};
			// Wenn Gegenstand verbraucht wird, aus dem Inventar entfernen
			if (use_up==true)
			{
				//Bsp 4 traenke, 1 entfernen bleiben 3
				int tmpanz=item->getNumber();
				getItems()->removeItem(item);
				if (tmpanz!=1)
				{
					item->setNumber(--tmpanz);
					getItems()->addItem(item);
				}
			}

			if (swap_item==true)
			{
				getItems()->removeItem(old_item);
				getItems()->addItem(item);
			}

			//das beim entnehmen aus dem Inventar angelegte temporaere Item löschen
			if (item!=0)
			{
				delete item;
			}
			//printf("Aktion fuer beendet erklaeren!\n");
			m_action->setType(ACTION_NONE);
		}

	}
}

*/

void Creature::calcDamage(Action::ActionType act,Damage& dmg)
{
	// alles nullen
	dmg.init();

	dmg.m_attacker_id = getId();
	dmg.m_attacker_fraction = getTypeInfo()->m_fraction;

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
		dmg.m_attacker_fraction = TypeInfo::FRAC_HOSTILE_TO_ALL;
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
		dmg.m_attack += basm->m_dexterity/5;

		dmg.m_power += basm->m_strength;

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
		dmg.m_attack += basm->m_dexterity/3;

		dmg.m_power += basm->m_dexterity;

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
		dmg.m_attack += basm->m_dexterity/5;

		dmg.m_power += basm->m_willpower;

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
		dmg.m_min_damage[Damage::FIRE] += min(m_base_attr_mod.m_magic_power*2.0f,dmg.m_min_damage[Damage::PHYSICAL]);
		dmg.m_max_damage[Damage::FIRE] += min(m_base_attr_mod.m_magic_power*3.0f,dmg.m_max_damage[Damage::PHYSICAL]);

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
		dmg.m_min_damage[Damage::ICE] += min(m_base_attr_mod.m_magic_power*2.0f,dmg.m_min_damage[Damage::PHYSICAL]);
		dmg.m_max_damage[Damage::ICE] += min(m_base_attr_mod.m_magic_power*3.0f,dmg.m_max_damage[Damage::PHYSICAL]);
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
		dmg.m_min_damage[Damage::AIR] += min(m_base_attr_mod.m_magic_power*2.0f,dmg.m_min_damage[Damage::PHYSICAL]);
		dmg.m_max_damage[Damage::AIR] += min(m_base_attr_mod.m_magic_power*3.0f,dmg.m_max_damage[Damage::PHYSICAL]);
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
			dmg.m_min_damage[Damage::FIRE] += min(m_base_attr_mod.m_willpower/6,m_base_attr_mod.m_magic_power*2);
			dmg.m_max_damage[Damage::FIRE] += min(m_base_attr_mod.m_willpower/4,m_base_attr_mod.m_magic_power*3);
			dmg.m_min_damage[Damage::ICE]=0;
			dmg.m_max_damage[Damage::ICE]=0;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			dmg.m_special_flags |= Damage::EXTRA_UNDEAD_DMG;
			DEBUG("min %f max %f",dmg.m_min_damage[Damage::FIRE],dmg.m_max_damage[Damage::FIRE]);
			break;

		case Action::HOLY_FIRE:
			dmg.m_min_damage[Damage::PHYSICAL]=0;
			dmg.m_max_damage[Damage::PHYSICAL]=0;
			dmg.m_min_damage[Damage::FIRE] +=m_base_attr_mod.m_willpower/2;
			dmg.m_max_damage[Damage::FIRE] +=m_base_attr_mod.m_willpower/1.33;
			dmg.m_min_damage[Damage::FIRE] += min(m_base_attr_mod.m_willpower/6,m_base_attr_mod.m_magic_power*2);
			dmg.m_max_damage[Damage::FIRE] += min(m_base_attr_mod.m_willpower/4,m_base_attr_mod.m_magic_power*3);
			dmg.m_min_damage[Damage::ICE]=0;
			dmg.m_max_damage[Damage::ICE]=0;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			dmg.m_special_flags |= Damage::EXTRA_UNDEAD_DMG;
			break;

		case Action::LIGHT_BEAM:
		case Action::BURNING_SUN:
			dmg.init();
			dmg.m_attacker_fraction = getTypeInfo()->m_fraction;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			dmg.m_status_mod_power[Damage::BLIND] = (short) (1.5*m_base_attr_mod.m_willpower);
			break;

		case Action::HYPNOSIS:
		case Action::HYPNOSIS2:
			dmg.init();
			dmg.m_attacker_fraction = getTypeInfo()->m_fraction;
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			dmg.m_status_mod_power[Damage::CONFUSED] = m_base_attr_mod.m_willpower;
			break;

		case Action::BREAK_BINDING:
			dmg.m_min_damage[Damage::ICE] =m_base_attr_mod.m_willpower/6;
			dmg.m_max_damage[Damage::ICE] =m_base_attr_mod.m_willpower/4;
			dmg.m_min_damage[Damage::PHYSICAL] += min(m_base_attr_mod.m_willpower/3,m_base_attr_mod.m_magic_power*2);
			dmg.m_max_damage[Damage::PHYSICAL] += min(m_base_attr_mod.m_willpower/2,m_base_attr_mod.m_magic_power*3);
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			dmg.m_special_flags |= Damage::EXTRA_SUMMONED_DMG;
			break;

		case Action::DISRUPT_BINDING:
			dmg.m_min_damage[Damage::ICE]=0;
			dmg.m_max_damage[Damage::ICE]=0;
			dmg.m_min_damage[Damage::ICE] +=m_base_attr_mod.m_willpower/3;
			dmg.m_max_damage[Damage::ICE] +=m_base_attr_mod.m_willpower/2;
			dmg.m_min_damage[Damage::PHYSICAL] += min(m_base_attr_mod.m_willpower/2,m_base_attr_mod.m_magic_power*2);
			dmg.m_max_damage[Damage::PHYSICAL] += min(m_base_attr_mod.m_willpower/1.33,m_base_attr_mod.m_magic_power*3.0);
			dmg.m_special_flags |= Damage::UNBLOCKABLE;
			dmg.m_special_flags |= Damage::EXTRA_SUMMONED_DMG;
			break;

		case Action::ACID:
			dmg.init();
			dmg.m_attacker_fraction = getTypeInfo()->m_fraction;
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
	m_base_attr_mod.m_level =m_base_attr.m_level;
	m_base_attr_mod.m_max_health =m_base_attr.m_max_health;

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
	list<CreatureBaseAttrMod>::iterator j;
	for (j=m_dyn_attr.m_temp_mods.begin(); j!= m_dyn_attr.m_temp_mods.end();++j)
	{
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

void Creature::takeDamage(Damage* d)
{
	// Lebewesen kann nur im Zustand aktiv Schaden nehmen
	if (getState() != STATE_ACTIVE)
		return;

	DEBUG5("take Damage %i",getId());
	// Testen ob der Verursacher des Schadens feindlich gesinnt ist
	if (getWorld()->getRelation(d->m_attacker_fraction,this) != HOSTILE)
	{
		// Verursacher ist nicht feindlich, kein Schaden
		DEBUG5("not hostile, no dmg");
		return;
	}

	// Testen ob man selbst der Verursacher ist
	// (man kann sich selbst generell nicht schaden)
	if (d->m_attacker_id == getId())
	{
		return;
	}


	float dmg=0,dmgt;
	short res;
	float rez = 1.0/RAND_MAX;
	// testen ob der Schaden geblockt wird
	if (!(d->m_special_flags & Damage::UNBLOCKABLE))
	{
		float block = m_base_attr_mod.m_block  ;


		// Chance zu blocken ist 1 - Attackewert / Blockwert
		if (d->m_attack<block)
		{
			if (rand()*rez>d->m_attack/block)
			{
				DEBUG5("blocked");
				// Schaden abgewehrt
				return;
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
	res = min (m_base_attr_mod.m_resistances_cap[Damage::PHYSICAL],m_base_attr_mod.m_resistances[Damage::PHYSICAL]);
	dmgt *= 0.01*(100-res);

	// Ruestung anwenden
	float armor = m_base_attr_mod.m_armor;

	// Faehigkeit Turm in der Schlacht
	if (checkAbility(Action::STEADFAST) && m_base_attr_mod.m_max_health *0.3 > m_dyn_attr.m_health)
		armor *=1.5;


	if (armor>d->m_power && !(d->m_special_flags & Damage::IGNORE_ARMOR))
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
		res = min(m_base_attr_mod.m_resistances_cap[i],m_base_attr_mod.m_resistances[i]);
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
					m_event_mask |= Event::DATA_STATUS_MODS;
				}

				DEBUG("applying status mod %i for %f ms",i,t);
			}
		}
	}

	DEBUG5("sum dmg %f",dmg);

	// Lebenspunkte abziehen
	getDynAttr()->m_health -= dmg;

	if (dmg>0)
	{
		m_dyn_attr.m_effect_time[CreatureDynAttr::BLEEDING] = max(m_dyn_attr.m_effect_time[CreatureDynAttr::BLEEDING],150.0f);
		m_event_mask |= Event::DATA_HP | Event::DATA_EFFECTS;
	}

	// Statikschild wenn mehr als 2% der Lebenspunkte verloren
	if ((m_base_attr_mod.m_special_flags & STATIC_SHIELD) && dmg > m_base_attr_mod.m_max_health * 0.02)
	{
		// Projektil Statikschild erzeugen
		DmgProjectile* pr = new DmgProjectile(getWorld(),Projectile::STATIC_SHIELD,getTypeInfo()->m_fraction, getWorld()->getValidProjectileId());

		// Schaden festlegen
		Damage dmg;
		dmg.m_status_mod_power[Damage::PARALYZED] = m_base_attr_mod.m_magic_power;
		dmg.m_min_damage[Damage::AIR] = m_base_attr_mod.m_magic_power*0.2;
		dmg.m_max_damage[Damage::AIR] = m_base_attr_mod.m_magic_power*0.3;
		dmg.m_multiplier[Damage::AIR]=1;
		dmg.m_attacker_fraction = getTypeInfo()->m_fraction;
		memcpy(pr->getDamage(),&dmg,sizeof(Damage));
		pr->getGeometry()->m_radius =getGeometry()->m_shape.m_radius+1;
		getWorld()->insertProjectile(pr,getGeometry()->m_shape.m_coordinate_x,getGeometry()->m_shape.m_coordinate_y,getGridLocation()->m_region);
	}
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
		m_event_mask |= Event::DATA_HP;
	}


	for (int i = 0;i< NR_STATUS_MODS;i++)
	{
		if (mod->m_dstatus_mod_immune_time[i]>0)
		{
			m_dyn_attr.m_status_mod_immune_time[i] = max(m_dyn_attr.m_status_mod_immune_time[i],mod->m_dstatus_mod_immune_time[i]);
			m_dyn_attr.m_status_mod_time[i]=0;
			m_event_mask |= Event::DATA_STATUS_MODS;
		}
	}
}

void Creature::applyBaseAttrMod(CreatureBaseAttrMod* mod, bool add)
{
	int i;
	// Deltawerte dazu addieren
	m_base_attr_mod.m_armor +=mod->m_darmor;
	m_base_attr_mod.m_block +=mod->m_dblock;
	m_base_attr_mod.m_strength +=mod->m_dstrength;
	m_base_attr_mod.m_dexterity +=mod->m_ddexterity;
	m_base_attr_mod.m_willpower +=mod->m_dwillpower;
	m_base_attr_mod.m_magic_power +=mod->m_dmagic_power;
	m_base_attr_mod.m_walk_speed +=mod->m_dwalk_speed;
	m_base_attr_mod.m_max_health += mod->m_dmax_health;
	m_base_attr_mod.m_max_health += mod->m_dstrength*5;
	m_base_attr_mod.m_attack_speed += mod->m_ddexterity*3;

	
	m_base_attr_mod.m_attack_speed +=mod->m_dattack_speed;
	
	// Modifikationen feststellen
	if (mod->m_dwalk_speed!=0 )
	{
		m_event_mask |= Event::DATA_WALK_SPEED;
	}
	if (mod->m_dattack_speed !=0 || mod->m_ddexterity!=0)
	{
		m_event_mask |= Event::DATA_ATTACK_SPEED;
	}
	if (mod->m_dmax_health !=0 || mod->m_dstrength!=0)
	{
		m_event_mask |= Event::DATA_MAX_HP;
	}

	// einige Untergrenzen pruefen
	m_base_attr_mod.m_strength = max(m_base_attr_mod.m_strength,(short) 1);
	m_base_attr_mod.m_dexterity = max(m_base_attr_mod.m_dexterity,(short) 1);
	m_base_attr_mod.m_willpower = max(m_base_attr_mod.m_willpower,(short) 1);
	m_base_attr_mod.m_magic_power = max(m_base_attr_mod.m_magic_power,(short) 1);
	m_base_attr_mod.m_walk_speed = max(m_base_attr_mod.m_walk_speed,(short) 200);
	m_base_attr_mod.m_attack_speed = max(m_base_attr_mod.m_attack_speed,(short) 200);

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
			m_event_mask |= Event::DATA_ABILITIES;
		}
	}

	// Flags mit OR hinzufuegen
	m_base_attr_mod.m_special_flags |= mod->m_xspecial_flags;
	m_base_attr_mod.m_immunity |= mod->m_ximmunity;
	
	if (mod->m_xspecial_flags!=0)
	{
		m_event_mask |= Event::DATA_FLAGS;
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
	m_base_attr_mod.m_attack_speed -= mod->m_ddexterity*3;
	
	// Modifikationen feststellen
	if (mod->m_dwalk_speed!=0)
	{
		m_event_mask |= Event::DATA_WALK_SPEED;
	}
	if (mod->m_dattack_speed !=0 || mod->m_ddexterity!=0)
	{
		m_event_mask |= Event::DATA_ATTACK_SPEED;
	}
	if (mod->m_dmax_health !=0 || mod->m_dstrength!=0)
	{
		m_event_mask |= Event::DATA_MAX_HP;
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
			m_event_mask |= Event::DATA_ABILITIES;
		}
	}

	// Wenn Flags veraendert wurden neu berechnen
	if (mod->m_xspecial_flags!=0)
	{
		m_event_mask |= Event::DATA_FLAGS;
		ret = true;
	}

	if( mod->m_ximmunity!=0)
		ret = true;

	// Schaden neu berechnen
	recalcDamage();

	return ret;
}

void Creature::getPathDirection(float x_start, float y_start,short region, float base_size, short layer, float dir[2])
{
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

	if ((layer & Geometry::LAYER_AIR) ==0)
	{
		DEBUG5("switching to flying info");
		pi = &m_small_flying_path_info;
		bsize=1;
		fly = true;
	}

	Geometry* geom = getGeometry();
	// true, wenn einfach direkter Weg zum Ziel gewaehlt werden soll
	bool direct = false;

	// true, wenn Wegfindeinformation neu berechnet werden muss
	bool recalc = false;

	// Position es Wegsuchenden Objekts
	float x = geom->m_shape.m_coordinate_x;
	float y = geom->m_shape.m_coordinate_y;


	if (*pi == 0)
	{
		// Wegfindeinfo existiert noch nicht
		*pi = new PathfindInfo;
		// Qualitaet der Suche
		int qual=4;
		int dim = 20 * qual / bsize +1;

		// Potenzialmatrix
		(*pi)->m_pot = new Matrix2d<float>(dim,dim);
		// Blockmatrix
		(*pi)->m_block = new Matrix2d<char>(dim,dim);

		(*pi)->m_dim = dim;

		// Ebene ist Base und Air fuer normale, nur AIR fuer fliegende Lebewesen
		(*pi)->m_layer= Geometry::LAYER_BASE | Geometry::LAYER_AIR;
		if (fly)
		{
			(*pi)->m_layer= Geometry::LAYER_AIR;
		}
		(*pi)->m_region=getGridLocation()->m_region;
		(*pi)->m_base_size = bsize;
		(*pi)->m_quality=qual;
		(*pi)->m_id = getId();
		recalc = true;
		DEBUG5("recalc: not pathinfo so far");
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
			dir[0] =0;
			dir[1] =0;
			return;
		}

		// Abstand aktuelle Position zu Position fuer die die Wegfindeinformation erstellt wurde
		float d = sqr((*pi)->m_start_x -x) +sqr((*pi)->m_start_y -y);

		// Abstand des wegsuchenden Objektes zur einen Position
		float d2 = sqr((*pi)->m_start_x -x_start) +sqr((*pi)->m_start_y -y_start);
		if (fabs((*pi)->m_center_x -x_start)>10 || fabs((*pi)->m_center_y -y_start)>10)
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

		// Zentrum und Senke sind die aktuelle Position
		(*pi)->m_start_x= x;
		(*pi)->m_start_y= y;
		(*pi)->m_center_x= x;
		(*pi)->m_center_y= y;
		(*pi)->m_timer =0;

		// Blockmatrix berechnen
		bool suc;
		suc = getWorld()->calcBlockmat(*pi);
		if (suc!=true)
			direct = true;
		else
		{
			// Potentialfeld berechnen
			suc = getWorld()->calcPotential(*pi);
			if (suc!=true)
				direct = true;
		}
	}

	if (direct)
	{
		// Direkte Richtung
		// Vektor vom Start zum Ziel, normiert
		dir[0] = x-x_start;
		dir[1] = y-y_start;

		float rezsq = 1/(sqr(dir[0]) + sqr(dir[1]));
		dir[0] *= rezsq;
		dir[1] *= rezsq;

	}
	else
	{
		// Potentialfeld verwenden um die Richtung zu berechnen
		DEBUG5("calcing direction from %f %f",x_start,y_start);
		getWorld()->calcPathDirection(*pi, x_start, y_start, dir);
	}
}

void Creature::toString(CharConv* cv)
{
	DEBUG5("Creature::tostring");
	WorldObject::toString(cv);
	cv->toBuffer((short) getTypeInfo()->m_category);
	cv->toBuffer((char) getTypeInfo()->m_fraction);
	
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

	cv->toBuffer(getMoveInfo()->m_speed_x);
	cv->toBuffer(getMoveInfo()->m_speed_y);
		

}

void Creature::fromString(CharConv* cv)
{
	WorldObject::fromString(cv);
	char ctmp;
	short stmp;
	cv->fromBuffer<short>(stmp);
	getTypeInfo()->m_category = (TypeInfo::Category) stmp;
	
	cv->fromBuffer<char>(ctmp);
	getTypeInfo()->m_fraction = (TypeInfo::Fraction) ctmp;
	
	m_action.fromString(cv);
	m_command.fromString(cv);
	m_next_command.fromString(cv);
	
	cv->fromBuffer(m_dyn_attr.m_health);
	cv->fromBuffer(m_base_attr_mod.m_max_health);
	DEBUG5("read offset: %i",cv->getBitStream()->GetReadOffset());

	
	// Statusveraenderungen
	cv->fromBuffer<char>(ctmp);
	char c=0;
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
	
	cv->fromBuffer(getMoveInfo()->m_speed_x);
	cv->fromBuffer(getMoveInfo()->m_speed_y);
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


void Creature::writeEvent(Event* event, CharConv* cv)
{
	if (event->m_data & Event::DATA_COMMAND)
	{
		m_command.toString(cv);
	}
	
	if (event->m_data & Event::DATA_ACTION)
	{
		m_action.toString(cv);
		cv->toBuffer(getGeometry()->m_shape.m_coordinate_x);
		cv->toBuffer(getGeometry()->m_shape.m_coordinate_y);
		

		if (m_action.m_type!=0)
		{
			float acttime = m_action.m_time - m_action.m_elapsed_time;
			float goalx = getGeometry()->m_shape.m_coordinate_x + getMoveInfo()->m_speed_x*(acttime);
			float goaly = getGeometry()->m_shape.m_coordinate_y + getMoveInfo()->m_speed_y*(acttime);
			DEBUG5("sending action: %i at %f %f, dir %f %f goal %f %f",m_action.m_type, getGeometry()->m_shape.m_coordinate_x,	getGeometry()->m_shape.m_coordinate_y, getMoveInfo()->m_speed_x,getMoveInfo()->m_speed_y,goalx,goaly);
		}
	}
	
	if (event->m_data & Event::DATA_HP)
	{
		cv->toBuffer(getDynAttr()->m_health);
	}
	
	if (event->m_data & Event::DATA_MAX_HP)
	{
		cv->toBuffer(getBaseAttrMod()->m_max_health);
	}
	
	if (event->m_data & Event::DATA_EFFECTS)
	{
		for (int i=0;i<NR_EFFECTS;i++)
		{
			cv->toBuffer(m_dyn_attr.m_effect_time[i]);
		}
	}

	if (event->m_data & Event::DATA_STATUS_MODS)
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
	
	if (event->m_data & Event::DATA_TIMER)
	{
		cv->toBuffer(m_timer1_max);
		cv->toBuffer(m_timer2_max);
	}
	
	if (event->m_data & Event::DATA_STATE)
	{
		cv->toBuffer((char) getState());
	}
	
	if (event->m_data & Event::DATA_WALK_SPEED)
	{
		cv->toBuffer(getBaseAttrMod()->m_walk_speed);
	}
	
	if (event->m_data & Event::DATA_ATTACK_SPEED)
	{
		cv->toBuffer(getBaseAttrMod()->m_attack_speed);
	}
	
	if (event->m_data & Event::DATA_NEXT_COMMAND)
	{
		m_next_command.toString(cv);
	}
	
	if (event->m_data & Event::DATA_ABILITIES)
	{
		for (int i=0; i<6; i++)
		{
			cv->toBuffer(getBaseAttrMod()->m_abilities[i]);	
		}	
	}
	
	if (event->m_data & Event::DATA_FLAGS )
	{
		cv->toBuffer(getBaseAttrMod()->m_special_flags);
	}
	
	if (event->m_data & Event::DATA_EXPERIENCE)
	{
		cv->toBuffer(getDynAttr()->m_experience);
	}
	
	if (event->m_data & Event::DATA_MOVE_INFO)
	{
		cv->toBuffer(getMoveInfo()->m_speed_x);	
		cv->toBuffer(getMoveInfo()->m_speed_y);	
	}
	
	if (event->m_data & Event::DATA_LEVEL)
	{
		cv->toBuffer(getBaseAttr()->m_level);	
	}
}


void Creature::processEvent(Event* event, CharConv* cv)
{
	DEBUG5("object %i processing Event %i data %i",getId(),event->m_type, event->m_data);
	if (event->m_data & Event::DATA_COMMAND)
	{
		m_command.fromString(cv);
		DEBUG5("got Command %i",m_command.m_type);
	}
	
	char oldact = m_action.m_type;
	float posx = getGeometry()->m_shape.m_coordinate_x;
	float posy = getGeometry()->m_shape.m_coordinate_y;
	float movex = getMoveInfo()->m_speed_x;
	float movey = getMoveInfo()->m_speed_y;
	float newx = getGeometry()->m_shape.m_coordinate_x,newy= getGeometry()->m_shape.m_coordinate_y;
	float newmovex,newmovey;
	
	bool newact= false;
	bool newmove = false;
	if (event->m_data & Event::DATA_ACTION)
	{
		
		
		m_action.fromString(cv);
		
		float x,y;
		cv->fromBuffer(newx);
		cv->fromBuffer(newy);
		newact = true;
	}
	
	
	
	if (event->m_data & Event::DATA_HP)
	{
		cv->fromBuffer(getDynAttr()->m_health);
	}
	
	if (event->m_data & Event::DATA_MAX_HP)
	{
		cv->fromBuffer(getBaseAttrMod()->m_max_health);
	}
	
	if (event->m_data & Event::DATA_EFFECTS)
	{
		for (int i=0;i<NR_EFFECTS;i++)
		{
			cv->fromBuffer(m_dyn_attr.m_effect_time[i]);
		}
	}

	if (event->m_data & Event::DATA_STATUS_MODS)
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
	
	if (event->m_data & Event::DATA_TIMER)
	{
		cv->fromBuffer(m_timer1_max);
		cv->fromBuffer(m_timer2_max);
		m_timer1 = m_timer1_max;
		m_timer2 = m_timer2_max;
			
	}
	
	if (event->m_data & Event::DATA_STATE)
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
	
	if (event->m_data & Event::DATA_WALK_SPEED)
	{
		cv->fromBuffer(getBaseAttrMod()->m_walk_speed);
	}
	
	if (event->m_data & Event::DATA_ATTACK_SPEED)
	{
		cv->fromBuffer(getBaseAttrMod()->m_attack_speed);
	}
	
	if (event->m_data & Event::DATA_NEXT_COMMAND)
	{
		m_next_command.fromString(cv);
	}
	
	if (event->m_data & Event::DATA_ABILITIES)
	{
		for (int i=0; i<6; i++)
		{
			cv->fromBuffer(getBaseAttrMod()->m_abilities[i]);	
		}	
	}
	
	if (event->m_data & Event::DATA_FLAGS )
	{
		cv->fromBuffer(getBaseAttrMod()->m_special_flags);
	}
	
	if (event->m_data & Event::DATA_EXPERIENCE)
	{
		cv->fromBuffer(getDynAttr()->m_experience);
	}
	
	if (event->m_data & Event::DATA_MOVE_INFO)
	{
		cv->fromBuffer(newmovex);	
		cv->fromBuffer(newmovey);
		newmove = true;
		
	}
	
	if (event->m_data & Event::DATA_LEVEL)
	{
		cv->fromBuffer(getBaseAttr()->m_level);	
	}
	
	if (newmove)
	{
		// Zielort der Aktion berechnen
		float acttime = m_action.m_time - m_action.m_elapsed_time;
		float goalx = newx + newmovex*(acttime);
		float goaly = newy + newmovey*(acttime);
		
		DEBUG5("goal %f %f current pos %f %f",goalx,goaly,getGeometry()->m_shape.m_coordinate_x,getGeometry()->m_shape.m_coordinate_y);
		
		// Zeit die zum erreichen des Zieles uebrig ist
		float goaltime = acttime;
		if (event->m_data & Event::DATA_ACTION)
		{
			goaltime -= cv->getDelay();
		}
		DEBUG5("time to reach goal %f",goaltime);
		
		if (goaltime <0)
		{
			// wenn man schon lange da sein muesste, Objekt an den Zielort versetzen
			moveTo(goalx,goaly);
		}
		else
		{
			// Bewegungsgeschwindigkeit so setzen, dass
			getMoveInfo()->m_speed_x = (goalx-getGeometry()->m_shape.m_coordinate_x) / goaltime;
			getMoveInfo()->m_speed_y = (goaly-getGeometry()->m_shape.m_coordinate_y) / goaltime;
			DEBUG5("new speed %f %f",getMoveInfo()->m_speed_x,getMoveInfo()->m_speed_y);
		}
			
	}
	
	if (newact)
	{
		m_action.m_elapsed_time += cv->getDelay();	
		if (!newmove)
		{
			moveTo(newx,newy);
		}
	}
}


