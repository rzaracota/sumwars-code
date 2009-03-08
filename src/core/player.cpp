
/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Hans Wulf, Alexander Boehm

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

#include "player.h"
#include "eventsystem.h"

PlayerCamera::PlayerCamera()
{
	m_distance = 20;
	m_theta = 70;
	m_phi =270;

	m_goal_distance = 20;
	m_goal_theta = 70;
	m_goal_phi =270;
	m_time =0;
}

void PlayerCamera::moveTo(float distance, float theta, float phi, float time)
{
	m_goal_distance = std::max(std::min(distance,50.0f),5.0f);
	m_goal_theta = std::max(std::min(theta,90.0f),5.0f);
	m_goal_phi = fmod(phi+360,360);

	// Phi so anpassen, dass jeweils der kuerzere Teilkreis genutzt wird
	if (m_phi - m_goal_phi > 180)
		m_phi -= 360;

	if (m_phi - m_goal_phi < -180)
		m_phi += 360;

	m_time = time;
}

void PlayerCamera::update(float time)
{
	if (m_time <= time)
	{
		m_distance = m_goal_distance;
		m_theta = m_goal_theta;
		m_phi = m_goal_phi;
	}
	else
	{
		float frac = time/m_time;
		m_distance = (1-frac)*m_distance + frac* m_goal_distance;
		m_theta =  (1-frac)*m_theta + frac*m_goal_theta;
		m_phi = (1-frac)*m_phi + frac* m_goal_phi;

		m_time -= time;
	}
}

//Constructors/Destructors

Player::Player( int id, TypeInfo::ObjectSubtype subtype) : Creature( id)
{
	getTypeInfo()->m_subtype = subtype;
	bool tmp=Player::init();
	if (!tmp)
	{
		DEBUG("Initialiserung des Players fehlgeschlagen!");
	}
}


Player::~Player()
{
	
}

bool Player::destroy()
{
	DEBUG5("leave Party");
	DEBUG5("destroy");
	World::getWorld()->getPartyFrac(m_fraction)->removeMember(getId());
	return Creature::destroy();
}

//Operations

bool Player::init()
{
	if (World::getWorld() !=0)
	{
		m_revive_position = World::getWorld()->getPlayerStartLocation();
	}
	else
	{
		m_revive_position.first = "none";
		m_revive_position.second = "none";
			
	}
	
	DEBUG5("Player::init");
	//eigene Initialisierung
	CreatureBaseAttr* bas = getBaseAttr();
	CreatureDynAttr* dyn = getDynAttr();
	
	m_network_slot=-1;
	m_package_number =0;
	getTypeInfo()->m_type = TypeInfo::TYPE_PLAYER;
	m_category = HUMAN;

	getShape()->m_type = Shape::CIRCLE;
	getShape()->m_radius = 0.5;
	m_layer = (LAYER_BASE | LAYER_AIR);
	getShape()->m_angle =0;

	bas->m_step_length=1.5;
	bas->m_attack_range=0.5;
	bas->m_level=1;

	m_attribute_points=0;
	m_skill_points=0;

	m_fraction = NOFRACTION;

	m_secondary_equip = false;

	m_equipement = new Equipement(5,14,30);
	m_equipement->setGold(50);

	m_save_timer= 3000;

	m_candidate_party = -1;

	// Attribute auf Basiswerte setzen
	int i;
	for (i=1;i<6;i++)
		bas->m_abilities[i]=0;
	
	dyn->m_experience=0;
	
	bas->m_max_experience = 100;
	bas->m_level =1;
	bas->m_block=0;
	bas->m_resistances[0] =0;
	bas->m_resistances[1] =0;
	bas->m_resistances[2] =0;
	bas->m_resistances[3] =0;
	bas->m_resistances_cap[0] =50;
	bas->m_resistances_cap[1] =50;
	bas->m_resistances_cap[2] =50;
	bas->m_resistances_cap[3] =50;
	bas->m_special_flags=0;
	bas->m_abilities[0] = 0xffffff;
	
	bool init = false;
	
	Item* si;
	Equipement* equ = getEquipement();
	if (getTypeInfo()->m_subtype == "warrior")
	{
		dyn->m_health = 200;
		bas->m_max_health = 200;
		bas->m_armor = 20;
		bas->m_attack = 40;
		bas->m_strength = 50;
		bas->m_dexterity = 20;
		bas->m_magic_power = 10;
		bas->m_willpower = 25;
		bas->m_walk_speed = 3000;
		bas->m_attack_speed=2000;
		
		bas->m_attack_range =20;
		
		m_base_action = Action::ATTACK;
		m_left_action = Action::ATTACK;
		m_right_action = Action::ATTACK;
		init = true;

		// Debugging
		m_look = "warrior_m";
		m_name.assign("Boromir");
		bas->m_abilities[1] = 0xffffff;
		
		

		si = ItemFactory::createItem(Item::WEAPON,"dagger_sw");
		equ->swapItem(si,Equipement::WEAPON);

		
	}
	else if (getTypeInfo()->m_subtype == "archer")
	{
		dyn->m_health = 150;
		bas->m_max_health = 150;
		bas->m_armor = 15;
		bas->m_attack = 30;
		bas->m_strength = 25;
		bas->m_dexterity = 45;
		bas->m_magic_power = 15;
		bas->m_willpower = 15;
		bas->m_walk_speed = 3000;
		bas->m_attack_speed=1000;
		
		m_look = "archer_f";
		
		bas->m_attack_range =20;
		
		m_base_action = Action::RANGE_ATTACK;
		m_left_action = Action::RANGE_ATTACK;
		m_right_action = Action::RANGE_ATTACK;
		init = true;

		
		// Debugging
		m_name.assign("Legolas");
		bas->m_abilities[2] = 0xffffff;

		si = ItemFactory::createItem(Item::WEAPON,"trainy_sbw");
		equ->swapItem(si,Equipement::WEAPON);
		
		
	}
	else if (getTypeInfo()->m_subtype == "mage")
	{
		dyn->m_health = 100;
		bas->m_max_health = 100;
		bas->m_armor = 15;
		bas->m_attack = 10;
		bas->m_strength = 15;
		bas->m_dexterity = 15;
		bas->m_magic_power = 45;
		bas->m_willpower = 25;
		bas->m_attack_range =20;

		m_name.assign("Gandalf");
		bas->m_walk_speed = 3000;
		bas->m_attack_speed=1500;
		m_base_action = Action::MAGIC_ATTACK;
		m_left_action = Action::MAGIC_ATTACK;
		m_right_action = Action::MAGIC_ATTACK;
		init = true;
		m_look = "mage_m";
	
		
	
		si = ItemFactory::createItem(Item::WEAPON,"ice_wnd");
		equ->swapItem(si,Equipement::WEAPON);

		// Debugging
		bas->m_abilities[3] = 0xffffff;
	}
	else if (getTypeInfo()->m_subtype == "priest")
	{
		dyn->m_health = 200;
		bas->m_max_experience = 100;
		bas->m_level =1;
		bas->m_max_health = 200;
		bas->m_armor = 20;
		bas->m_attack = 10;
		bas->m_strength = 25;
		bas->m_dexterity = 15;
		bas->m_magic_power = 15;
		bas->m_willpower = 45;
		bas->m_walk_speed = 3000;
		bas->m_attack_speed=2000;
		bas->m_attack_range =1;

		m_base_action = Action::HOLY_ATTACK;
		m_left_action = Action::HOLY_ATTACK;
		m_right_action = Action::HOLY_ATTACK;
		init = true;

		m_look = "priest_f";

		// Debugging
		bas->m_abilities[4] = 0xffffff;
		m_name.assign("Elrond");
		
		si = ItemFactory::createItem(Item::WEAPON,"hammer_hmr");
		equ->swapItem(si,Equipement::WEAPON);
	}

	
	si = ItemFactory::createItem(Item::POTION,"heal_1");
	equ->swapItem(si,Equipement::SMALL_ITEMS);

	si = ItemFactory::createItem(Item::POTION,"heal_1");
	equ->swapItem(si,Equipement::SMALL_ITEMS+1);
	
	getBaseAttrMod()->m_max_health = getBaseAttr()->m_max_health;
	
	if (init)
	{
		// Modifizierte Basisattribute erzeugen
		calcBaseAttrMod();
	}
	
	return true;
}

void  Player::revive()
{
	DEBUG("reviving");

	getRegion()->changeObjectGroup(this,PLAYER);
	getDynAttr()->m_health = getBaseAttrMod()->m_max_health;

	// keine Aktion/Kommando
	getAction()->m_type = Action::NOACTION;
	getCommand()->m_type = Action::NOACTION;
	getCommand()->m_damage_mult=1;
	getNextCommand()->m_type = Action::NOACTION;

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

	getDynAttr()->m_last_attacker_id=0;

	// Statusmods auf null setzen
	int i;
	for (i=0;i<NR_STATUS_MODS;i++)
	{
		getDynAttr()->m_status_mod_time[i]=0;
		getDynAttr()->m_status_mod_immune_time[i]=0;

	}
	getDynAttr()->m_temp_mods.clear();

	for (i=0;i<NR_EFFECTS;i++)
	{
		getDynAttr()->m_effect_time[i]=0;
	}

	setState(STATE_ACTIVE);
	clearCommand(false);
	getNextCommand()->m_type = Action::NOACTION;
	getNextCommand()->m_damage_mult = 1;
	getNextCommand()->m_goal = Vector(0,0);
	getNextCommand()->m_goal_object_id =0;
	getNextCommand()->m_range =1;

	getAction()->m_animation_number=0;
	getAction()->m_action_equip = Action::NO_WEAPON;
	getAction()->m_time =0;
	getAction()->m_elapsed_time =0;

	calcBaseAttrMod();
}

bool Player::onGamefieldClick(ClientCommand* command)
{
	// keine Aktionen waehrend eines Dialogs oder Handel
	if (getDialogueId()!=0 || getTradeInfo().m_trade_partner !=0)
	{
		return true;
	}
	
	if (command->m_action>=192)
	{
		ERRORMSG("invalid action");
		return false;
	}

	Command* com = getNextCommand();
	int dist;
	bool meleedir = false;
	WorldObject* wo;
	WorldObject::Relation rel;

	// Actionen auf self brauchen kein Zielobjekt
	dist = Action::getActionInfo(command->m_action)->m_distance;
	if ( dist == Action::SELF || dist == Action::PARTY_MULTI)
		command->m_id=0;

	// TODO ???
	if (dist == Action::PARTY)
		command->m_id = getId();

	// bei Take Item wird in der ID die ID des Items gespeichert
	if ( command->m_id!=0 && command->m_action != Action::TAKE_ITEM)
	{

		DEBUG5("Kommando erhalten, zielid: %i",command->m_id);
		wo = getRegion()->getObject(command->m_id);

		// Unterscheidung Zielobject vs kein Zielobject
		if (wo !=0)
		{
			rel = World::getWorld()->getRelation(m_fraction,wo);

			if (command->m_button == LEFT_MOUSE_BUTTON)
			{
				// Linke Maustaste: Aktion nur ausfuehren, wenn tatsaechlich ein passendenes Objekt vorhanden ist

				// Fuer Aktionen der Art Nah oder Fernkampf braucht man eine feindliche Kreatur
				if (rel == WorldObject::HOSTILE && (dist == Action::MELEE || dist == Action::RANGED))
				{

					if (wo->getState()==STATE_ACTIVE)
					{

						com->m_type =command->m_action;

						com->m_goal_object_id = command->m_id;
						com->m_goal = command->m_goal;
						com->m_range = getBaseAttrMod()->m_attack_range;
						DEBUG5("action range %f",getCommand()->m_range);
					}
					else
					{
						/*
						if (wo->getState()==STATES_DEAD)
						{
						getNextCommand()->setType(COMMAND_TRADE);
						getNextCommand()->setGoalObjectId(command->m_id);
						DEBUG("Monster auspluendern");
					}
						*/
					}
				}

				if (rel == WorldObject::ALLIED && dist == Action::PARTY)
				{
					if (wo->getState()==STATE_ACTIVE)
					{
						com->m_type =command->m_action;

						com->m_goal_object_id = command->m_id;
						com->m_goal = command->m_goal;
						com->m_range = getBaseAttrMod()->m_attack_range;
					}
				}
				else if (wo->isCreature() && ( rel == WorldObject::ALLIED || rel == WorldObject::NEUTRAL))
				{
					com->m_type =Action::SPEAK;

					com->m_goal_object_id = command->m_id;
					com->m_goal = command->m_goal;
					com->m_range = 3;
				}
				
				
				if (wo->getTypeInfo()->m_type==TypeInfo::TYPE_FIXED_OBJECT )
				{
					// festes Objekt benutzen
					com->m_type =Action::USE;
					com->m_goal_object_id = command->m_id;
					com->m_range = 0.5;
					DEBUG5("action range %f",getCommand()->m_range);
				}
						/*
				if (wo->getObjectType().getObjectType()==OBJECTTYPE_TRADER)
				{
							// mit einem Händler handeln
				getNextCommand()->setType(COMMAND_TRADE);
				getNextCommand()->setGoalObjectId(command->m_id);
			}
						*/
			}
			else if (command->m_button == RIGHT_MOUSE_BUTTON)
			{
				com->m_type =command->m_action;
				com->m_goal_object_id = command->m_id;
				com->m_goal = command->m_goal;
				com->m_range = getBaseAttrMod()->m_attack_range;
				com->m_goal_object_id = command->m_id;
			}
			else if (command->m_button == LEFT_SHIFT_MOUSE_BUTTON)
			{
				com->m_type = command->m_action;
				com->m_goal = command->m_goal;
				com->m_range = getBaseAttrMod()->m_attack_range;
				com->m_goal_object_id =0;
				if (dist == Action::MELEE)
				{

					meleedir = true;
				}
				m_event_mask |= NetEvent::DATA_NEXT_COMMAND;
			}
		}
		else
		{
			// Objekt existiert nicht ( Fehler?)
			com->m_type =Action::NOACTION;
		}

	}
	else
	{
		// Kein Zielobjekt gegeben
		DEBUG5("Kommando erhalten, Ziel (%f,%f) button %i action %i dist %i",command->m_goal.m_x, command->m_goal.m_y,command->m_button,command->m_action,dist);

		if (command->m_button == LEFT_MOUSE_BUTTON)
		{

			// linke Maustaste -> immer laufen
			if (command->m_action == Action::TAKE_ITEM)
			{
				com->m_type = Action::TAKE_ITEM;
				com->m_goal_object_id =command->m_id;
				DEBUG5("take item");
			}
			else
			{
				com->m_type = Action::WALK;
				com->m_goal_object_id =0;
			}
			com->m_goal = command->m_goal;

		}
		else if (dist == Action::SELF)
		{
			// Aktionen auf selbst auf left_shift und rechts ausfuehren
			com->m_type = command->m_action;
			com->m_goal = Vector(0,0);
			com->m_goal_object_id =0;
		}
		else if (dist == Action::PARTY_MULTI)
		{
			com->m_type = command->m_action;
			com->m_goal = command->m_goal;
			com->m_goal_object_id =0;
		}
		else if (dist == Action::PARTY)
		{
			// Aktionen auf Party nie ausfuehren, wenn kein Ziel angegeben
			com->m_type =Action::NOACTION;
			com->m_goal = Vector(0,0);
			com->m_goal_object_id =0;

		}
		else if (dist == Action::RANGED)
		{
			// Fernkampf immer starten
			com->m_type = command->m_action;
			com->m_goal = command->m_goal;
			com->m_range = getBaseAttrMod()->m_attack_range;
			com->m_goal_object_id =0;
		}
		else if (dist == Action::MELEE && (command->m_button == LEFT_SHIFT_MOUSE_BUTTON  || command->m_button == RIGHT_MOUSE_BUTTON))
		{
			// Nahkampf nur dann starten, wenn shift gedrueckt wurde
			com->m_type = command->m_action;
			com->m_goal = command->m_goal;
			com->m_range = getBaseAttrMod()->m_attack_range;
			com->m_goal_object_id =0;
			meleedir = true;
		}
		else
		{
			com->m_type =Action::NOACTION;
		}

	}

	if (!checkAbility(com->m_type))
	{
		com->m_type = Action::getActionInfo(com->m_type)->m_base_action;
		DEBUG("Basisaktion verwendet");
	}

	if (meleedir)
	{
		DEBUG5("meleedir");
		Vector dir;
		dir = com->m_goal - getShape()->m_center;
		float range = com->m_range + getShape()->m_radius;
		if ( dir.getLength()>= range)
		{
			dir.normalize();
			dir *= 0.95*range;
			com->m_goal = getShape()->m_center + dir;
		}
	}

	m_event_mask |= NetEvent::DATA_NEXT_COMMAND;
	DEBUG5("resulting command %i goal %f %f id %i",com->m_type,com->m_goal.m_x,com->m_goal.m_y, com->m_goal_object_id);

	return true;
}


bool Player::onItemClick(ClientCommand* command)
{
	short pos = command->m_id;

	if (m_secondary_equip)
	{
		if (pos == Equipement::WEAPON)
			pos = Equipement::WEAPON2;

		if (pos == Equipement::SHIELD)
			pos = Equipement::SHIELD2;

	}

	// das Item welches, des Spieler aktuell *in der Hand* hat
	Item* it = m_equipement->getItem(Equipement::CURSOR_ITEM);

	DEBUG5("got Item %p",it);

	if (command->m_button== BUTTON_ITEM_LEFT)
	{
		// Item soll verschoben werden
		if (it!=0)
		{

			// zeigt an, ob der Vorraussetzungen erfuellt sind
			bool req = true;

			if (pos <Equipement::CURSOR_ITEM)
			{

				// Item soll als Ausruestungsgegenstand benutzt werden
				req = checkItemRequirements(it);
				

				// geforderter Typ um das Item anlegen zu koennen
				Item::Type req_type = Item::NOITEM;

				if (pos == Equipement::WEAPON || pos == Equipement::WEAPON2)
					req_type = Item::WEAPON;
				if (pos == Equipement::HELMET)
					req_type = Item::HELMET;
				if (pos == Equipement::SHIELD || pos == Equipement::SHIELD2)
					req_type = Item::SHIELD;
				if (pos == Equipement::GLOVES)
					req_type = Item::GLOVES;
				if (pos == Equipement::RING_LEFT)
					req_type = Item::RING;
				if (pos == Equipement::RING_RIGHT)
					req_type = Item::RING;
				if (pos == Equipement::AMULET)
					req_type = Item::AMULET;
				if (pos == Equipement::ARMOR)
					req_type = Item::ARMOR;


				if (it->m_type != req_type)
				{
					// Item passt nicht an den entsprechenden Ausruestungsslot
					DEBUG("wrong itemtype");
					req= false;
				}



			}
			else
			{
			// Item soll im Inventar abgelegt werden
			DEBUG5("swap in inventory");

			// Groesse die das Item haben muss
				Item::Size size = Item::BIG;

				if (pos >= Equipement::MEDIUM_ITEMS)
					size = Item::MEDIUM;
				if (pos >= Equipement::SMALL_ITEMS)
					size = Item::SMALL;

				if (it->m_size != size)
				{
					// Groesse stimmt nicht
					// Item vom Cursor nehmen
					Item* itm =0;
					m_equipement->swapItem(itm, Equipement::CURSOR_ITEM);

					// ins Inventar einfuegen
					insertItem(itm);
					req = false;
				}
				else if (pos >= Equipement::SMALL_ITEMS && pos < Equipement::SMALL_ITEMS+10)
				{
					// Guertel, nur Traenke zulassen
					if (it->m_type != Item::POTION)
					{

						// Item vom Cursor nehmen
						Item* itm =0;
						m_equipement->swapItem(itm, Equipement::CURSOR_ITEM);

						// ins Inventar einfuegen
						insertItem(itm);
						req = false;
					}
				}

			}

			if (req == false)
			{
			// Vorraussetzungen nicht erfuellt
				return true;
			}
		}


		// Vertauschen von Cursoritem und angeklicktem Item
		m_equipement->swapCursorItem(pos);

		if (World::getWorld()->isServer() && pos <Equipement::CURSOR_ITEM)
		{
			// Ausruestungsgegenstand wurde getauscht
			NetEvent event;
			event.m_id = getId();
			event.m_data = pos;

			if (m_equipement->getItem(pos) == 0)
			{
				event.m_type = NetEvent::PLAYER_NOITEM_EQUIPED;
			}
			else
			{
				event.m_type = NetEvent::PLAYER_ITEM_EQUIPED;
			}
			DEBUG5("event: %i at %i",event.m_type,event.m_data);
			World::getWorld()->insertNetEvent(event);
		}

		Item* itm;
		if (pos==Equipement::WEAPON || pos == Equipement::WEAPON2)
		{
			// Waffe wurde angelegt, bei zweihaendig testen ob der Schild Slot frei ist
			Item* weapon = getWeapon();
			if (weapon!=0 && weapon->m_weapon_attr->m_two_handed)
			{
				if (getShield()!=0)
				{
					// Schild Slot ist nicht frei
					// Schild muss entfernt werden
					short shpos= Equipement::SHIELD;
					if (pos == Equipement::WEAPON2)
						shpos = Equipement::SHIELD2;

					if (World::getWorld()->isServer())
					{
						NetEvent event;
						event.m_type =  NetEvent::PLAYER_NOITEM_EQUIPED;
						event.m_data = shpos;
						event.m_id = getId();

						DEBUG5("event: no item at %i",shpos);

						World::getWorld()->insertNetEvent(event);
					}

					// Wenn aktuell kein Item am Cursor gehalten wird
					if (m_equipement->getItem(Equipement::CURSOR_ITEM)==0)
					{
						// Schild aufnehmen
						m_equipement->swapCursorItem(shpos);
					}
					else
					{
						// Schild ins Inventar verschieben
						itm =0;
						// Schild aus dem Schildslot holen
						m_equipement->swapItem( itm,shpos);

						// wenn man sich auf Serverseite befindet: NetEvent generieren


						if (!getEquipement()->insertItem(itm))
						{
							// Einfuegen ins Inventar fehlgeschlagen
							// Item fallen lassen
							getRegion()->dropItem(itm,getShape()->m_center);
						}
					}
				}
			}
		}


		if (pos==Equipement::SHIELD || pos == Equipement::SHIELD2)
		{
			// Schild soll verwendet werden, testen dass keine zweihaendige Waffe benutzt wird
			Item* weapon = getWeapon();
			if (weapon!=0 && weapon->m_weapon_attr->m_two_handed)
			{

				// zweihaendige Waffe wird verwendet, muss entfernt werden
				short wpos= Equipement::WEAPON;
				if (pos == Equipement::SHIELD2)
					wpos = Equipement::WEAPON2;

				if (World::getWorld()->isServer())
				{
					NetEvent event;
					event.m_type =  NetEvent::PLAYER_NOITEM_EQUIPED;
					event.m_data = wpos;
					event.m_id = getId();

					DEBUG5("event: no item at %i",wpos);

					World::getWorld()->insertNetEvent(event);
				}

				m_equipement->swapCursorItem(wpos);
			}
		}

		// Wenn an der Ausruestung etwas geaendert wurde
		if (pos <Equipement::CURSOR_ITEM)
		{
			// modifizierte Basisattribute neu berechnen
			calcBaseAttrMod();
		}
	}
	else
	{
		// Item soll verbraucht werden
		// oder verkauft
		it = m_equipement->getItem(pos);
		if (it!=0)
		{
			if (getTradePartner() != 0)
			{
				it =0;
				int gold = getEquipement()->getGold();
				getEquipement()->swapItem(it,pos);
				getTradePartner()->buyItem(it,gold);
				
				// wieder hinein getauscht, wenn Handel nicht erfolgreich
				getEquipement()->swapItem(it,pos);
				getEquipement()->setGold(gold);
				return true;
			}
			
			if (it->m_useup_effect==0)
			{
				// Item kann nicht verbraucht werden
				return true;
			}

			// Wirkung des Items
			applyDynAttrMod(it->m_useup_effect);

			// entfernen aus dem Inventar durch Tausch mit einem Nullpointer
			Item* swap=0;
			m_equipement->swapItem(swap,pos);

			// Typ und Subtyp des Items
			Item::Type type = it->m_type;
			Item::Subtype stype = it->m_subtype;

			// Item loeschen
			delete it;

			if (pos>=Equipement::SMALL_ITEMS && pos < Equipement::SMALL_ITEMS+10)
			{
				// Item befand sich im Guertel
				// suchen nach einem aehnlichen Item zum nachruecken
				for (int i=10;i<30;i++)
				{
					it = m_equipement->getItem(Equipement::SMALL_ITEMS+i);

					if (it==0)
						continue;

					if (it->m_type == type && it->m_subtype == stype)
					{
						// Item zum tauschen gefunden
						swap=0;

						// von der aktuellen Position wegnehmen
						m_equipement->swapItem(swap,Equipement::SMALL_ITEMS+i);
						// an der Position des verbrauchten Items einfuegen
						m_equipement->swapItem(swap,pos);

						break;
					}
				}
			}
		}
	}
	return true;
}

short Player::insertItem(Item* itm)
{
	if (itm ==0)
		ERRORMSG("tried to insert null item");
	short pos = getEquipement()->insertItem(itm);

	if (World::getWorld() != 0)
	{
		if (pos != Equipement::NONE)
		{
			// Gegenstand ins Inventar aufgenommen
			if (World::getWorld()->isServer() && getId() !=0)
			{
	
				NetEvent event;
				event.m_type =  NetEvent::PLAYER_ITEM_PICKED_UP ;
				event.m_data = pos;
				event.m_id = getId();
	
				DEBUG5("event: item picked up %i",pos);
	
				World::getWorld()->insertNetEvent(event);
			}
		}
		else
		{
			// Gegenstand passt nicht ins Inventar
			// wieder fallen lassen
			getRegion()->dropItem(itm,getShape()->m_center);
		}
	}
	return pos;
}

bool Player::checkItemRequirements(Item* itm)
{
	// testen ob Level ausreicht
	if (getBaseAttr()->m_level < itm->m_level_req)
	{
		// Level Vorraussetzung nicht erfuellt
		DEBUG5("level too low: own level: %i item level: %i",getBaseAttr()->m_level,itm->m_level_req);
		return false;
	}
	
	// testen ob Item fuer die Charakterklasse zugelassen ist
	char mask = Item::REQ_WARRIOR;
	if (getTypeInfo()->m_subtype == "mage")
		mask = Item::REQ_MAGE;
	if (getTypeInfo()->m_subtype == "archer")
		mask = Item::REQ_ARCHER;
	if (getTypeInfo()->m_subtype == "priest")
		mask = Item::REQ_PRIEST;

	if (itm->m_char_req & mask != mask)
	{
		// Spieler darf das Item nicht benutzen (falsche Spielerklasse)
		DEBUG5("wrong subtype");
		return false;
	}
	return true;
}

Item* Player::getWeapon()
{
	Item* si = m_equipement->getItem(Equipement::WEAPON);
	if (m_secondary_equip)
	{
		si = m_equipement->getItem(Equipement::WEAPON2);
	}
	return si;
}

Item* Player::getShield()
{
	Item* si = m_equipement->getItem(Equipement::SHIELD);
		if (m_secondary_equip)
	{
		si = m_equipement->getItem(Equipement::SHIELD2);
	}
	return si;
}

Action::ActionEquip Player::getActionEquip()
{
	Item* weapon = getWeapon();
	if (weapon ==0)
	{
		return Action::NO_WEAPON;
	}

	if (weapon->m_weapon_attr->m_two_handed)
	{
		return Action::TWO_HANDED;
	}
	else
	{
		return Action::	ONE_HANDED;
	}

}

void Player::increaseAttribute(CreatureBaseAttr::Attribute attr)
{
	switch (attr)
	{
		case (CreatureBaseAttr::STRENGTH):
			getBaseAttr()->m_strength++;
			getBaseAttr()->m_max_health += 5;
			getBaseAttrMod()->m_max_health += 5;
			getDynAttr()->m_health +=5;
			m_event_mask |= NetEvent::DATA_HP | NetEvent::DATA_MAX_HP;
			break;
		case (CreatureBaseAttr::DEXTERITY):
			getBaseAttr()->m_dexterity++;
			getBaseAttr()->m_attack_speed +=3;
			m_event_mask |= NetEvent::DATA_ATTACK_SPEED;
			break;
		case (CreatureBaseAttr::WILLPOWER):
			getBaseAttr()->m_willpower++;
			break;
		case (CreatureBaseAttr::MAGIC_POWER):
			getBaseAttr()->m_magic_power++;
			break;

	}
	
	calcBaseAttrMod();
	m_event_mask |= NetEvent::DATA_SKILL_ATTR_POINTS;
	
}

void Player::gainLevel()
{
	Creature::gainLevel();

	// Level um 1 erhöhen
	getBaseAttr()->m_level++;

	// heilen
	getDynAttr()->m_health =getBaseAttrMod()->m_max_health;

	// Erfahrungspunkte reduzieren
	getDynAttr()->m_experience-= getBaseAttr()->m_max_experience;

	if (getDynAttr()->m_experience<0)
	{
		getDynAttr()->m_experience=0;
	}

	// maximale Erfahrungspunkte erhoehen
	getBaseAttr()->m_max_experience += getBaseAttr()->m_level*100;
	getBaseAttr()->m_max_experience *= 1.2;


	// Attributpunkte hinzufuegen
	m_attribute_points +=5;

	if (getBaseAttr()->m_level %10 ==0 || getBaseAttr()->m_level==15 || getBaseAttr()->m_level==5 || getBaseAttr()->m_level==3)
	{
		// Skillpunkt hinzufuegen
		m_skill_points ++;
	}

	// Schaden neu berechnen
	recalcDamage();

	m_event_mask |= NetEvent::DATA_LEVEL | NetEvent::DATA_HP | NetEvent::DATA_MAX_HP | NetEvent::DATA_EXPERIENCE;
	m_event_mask |= NetEvent::DATA_SKILL_ATTR_POINTS;
}


bool Player::onClientCommand( ClientCommand* command, float delay)
{
	DEBUG5("Kommando (%f %f) button: %i id: %i action: %i",command->m_goal.m_x,command->m_goal.m_y,command->m_button,command->m_id, command->m_action);

	Party* 	p;
	Player* pl;
	Dialogue* dia;
	int i;
	std::list< std::pair<std::string, std::string> >::iterator it;

	bool sell;
	Item* si;
	DropSlot ds;
	int gold;
	// Wahrscheinlichkeiten BIG, MEDIUM, SMALL, GOLD
	float prob[4] = {0.1, 0.2, 0.2, 0.2};

	// prob_size, min level, max_level, prob_magic, magic_power
	ds.init(prob, 0,20, 0.3, 2000);

	// Kopie des aktuellen Kommandos anlegen
	Command oldcommand;
	memcpy(&oldcommand,getNextCommand(),sizeof(Command));

	switch(command->m_button)
	{

	// je nach dem welche Eingabe getaetigt wurde
		// Behandlung rechte Maustaste

		case LEFT_MOUSE_BUTTON:
		case RIGHT_MOUSE_BUTTON:
		case LEFT_SHIFT_MOUSE_BUTTON:
			onGamefieldClick(command);
			break;

		case BUTTON_APPLY:
		case BUTTON_PEACE:
		case BUTTON_WAR:
			pl = static_cast<Player*>(World::getWorld()->getPlayer(command->m_id));
			if (pl !=0)
			{
				p = pl->getParty();
				if (command->m_button == BUTTON_APPLY)
				{
					p->addCandidate(getId());
				}
				else if (command->m_button == BUTTON_WAR)
				{
					getParty()->setRelation(p->getId() , HOSTILE);
				}
				else if (command->m_button == BUTTON_PEACE)
				{
					getParty()->setRelation(p->getId() , NEUTRAL);
				}
			}
			break;
		case BUTTON_MEMBER_REJECT:
			getParty()->removeCandidate(command->m_id);
			break;


		case BUTTON_MEMBER_ACCEPT:
			pl = static_cast<Player*>(World::getWorld()->getPlayer(command->m_id));
			if (pl !=0)
			{
				pl->getParty()->removeMember(pl->getId());
				getParty()->acceptCandidate(command->m_id);
			}
			break;

		case BUTTON_KICK:
			getParty()->removeMember(command->m_id);
			World::getWorld()->getEmptyParty()->addMember(command->m_id);
			break;

		case BUTTON_LEAVE:
			if (getParty()->getNrMembers() ==1)
			{
				if (m_candidate_party >=0)
					World::getWorld()->getParty(m_candidate_party)->removeCandidate(getId());
			}
			else
			{
				getParty()->removeMember(getId());
				World::getWorld()->getEmptyParty()->addMember(getId());
			}
			break;

		case BUTTON_SET_LEFT_ACTION:
			m_left_action = (Action::ActionType) command->m_action;
			recalcDamage();
			break;

		case BUTTON_SET_RIGHT_ACTION:
			m_right_action = (Action::ActionType) command->m_action;
			recalcDamage();
			break;

		case BUTTON_ITEM_LEFT:
		case BUTTON_ITEM_RIGHT:
			onItemClick(command);
			break;

		case BUTTON_SAVE_QUIT:
			setState(STATE_QUIT);
			DEBUG("player wants to quit");
			break;

		case BUTTON_SWAP_EQUIP:
			m_secondary_equip = !m_secondary_equip;
			calcBaseAttrMod();
			DEBUG5("switching equip");
			break;

		case REQUEST_DETAILED_ITEM:
			DEBUG5("requested item %i",command->m_id);
			sendDetailedItem(command->m_id);
			break;

		case REQUEST_ABILITY_DAMAGE:
			DEBUG5("requested info %i",command->m_id);
			sendAbilityDamage((Action::ActionType) command->m_id);
			break;

		case BUTTON_INCREASE_ATTRIBUTE:
			if (m_attribute_points>0)
			{
				// noch freie Attributpunkte vorhanden
				m_attribute_points--;

				// entsprechendes Attribut erhoehen
				increaseAttribute((CreatureBaseAttr::Attribute) command->m_id);
			}
			break;

		case BUTTON_LEARN_ABILITY:
			if (m_skill_points>0)
			{
				// Skill punkte stehen zur Verfuegung
				if (checkAbilityLearnable(( Action::ActionType) command->m_id))
				{
					// Faehigkeit kann gelernt werden

					m_skill_points--;

					// Bit fuer die Faehigkeit setzen
					getBaseAttr()->m_abilities[command->m_id/32] |= (1 << (command->m_id%32));

					DEBUG("lerne Faehigkeit %i", command->m_id);

					calcBaseAttrMod();
					
					m_event_mask |= NetEvent::DATA_SKILL_ATTR_POINTS;
				}
			}
			break;

		case DROP_ITEM:
			si = getEquipement()->getItem(Equipement::CURSOR_ITEM);
			if (si !=0)
			{
				si=0;
				getEquipement()->swapItem(si,Equipement::CURSOR_ITEM);
				getRegion()->dropItem(si,getShape()->m_center);
			}
			break;
			
		case BUTTON_ANSWER:
			if (!World::getWorld()->isServer())
				break;
			
			dia = getDialogue();
			if (dia ==0)
			{
				ERRORMSG("answer without dialogue");
				break;
			}
			
			i = command->m_id;
			it = getSpeakText().m_answers.begin();
			while (it != getSpeakText().m_answers.end() && i >0)
			{
				i--;
				it++;
			}
			
			if (it == getSpeakText().m_answers.end())
			{
				ERRORMSG("invalid answer %i",command->m_id);
				dia->changeTopic("end");
				break;
			}
			
			DEBUG5("changing topic to %s",it->second.c_str());
			clearSpeakText();
			EventSystem::setDialogue(dia);
			dia->changeTopic(it->second);
			break;
			
		case BUTTON_TRADE_END:
			if (!World::getWorld()->isServer())
				break;
			
			dia = getDialogue();
			if (dia ==0)
			{
				ERRORMSG("answer without dialogue");
				break;
			}
			dia->changeTopic("end");
			break;
			
		case BUTTON_TRADE_ITEM_LEFT:
		case BUTTON_TRADE_ITEM_RIGHT:
		case BUTTON_TRADE_SELL:
			if (getTradePartner() ==0)
			{
				break;
			}
			
			si = getEquipement()->getItem(Equipement::CURSOR_ITEM);
			sell = true;
			gold = getEquipement()->getGold();
			
			// Rechtklick kauft immer ein, Linksklick nur, wenn kein Item in der Hand
			if (command->m_button == BUTTON_TRADE_ITEM_RIGHT || command->m_button == BUTTON_TRADE_ITEM_LEFT && si ==0)
				sell = false;
			
			if (sell)
			{
				// Item wird aus dem Inventar herraus genommen
				si =0;
				getEquipement()->swapItem(si,Equipement::CURSOR_ITEM);
				getTradePartner()->buyItem(si,gold);
				
				// wieder hinein getauscht, wenn Handel nicht erfolgreich
				getEquipement()->swapItem(si,Equipement::CURSOR_ITEM);
				
			}
			else
			{
				si =0;
				getTradePartner()->sellItem(command->m_id,si, gold);
				
				if (si !=0)
				{
					if (command->m_button == BUTTON_TRADE_ITEM_RIGHT)
					{
						insertItem(si);
					}
					else
					{
						getEquipement()->swapItem(si,Equipement::CURSOR_ITEM);
					}
				}
			}
			
			getEquipement()->setGold(gold);
			break;

		case DEBUG_SIGNAL:
			// Debugging
			if (command->m_id==0)
			{
				gainLevel();
			}
			else if (command->m_id==3)
			{

				si = ItemFactory::createItem(ds);
				bool ret = getEquipement()->insertItem(si);

				if (ret == false)
				{
					getEquipement()->clear();
					getEquipement()->insertItem(si);
				}

			}
			break;

		default:
			DEBUG("unknown command: %i",command->m_button);
	}

	if (oldcommand != *getNextCommand() && delay>0)
	{
		// naechstes Kommando hat sich geaendert
		// die aktuelle Aktion basiert eventuell auf einem veralteten Kommando

		if (getAction()->m_type == Action::NOACTION || getAction()->m_elapsed_time < delay)
		{
			// Aktion basiert auf veraltetem Kommando
			// abbrechen
			abortAction();

			// neue Aktion berechnen
			calcAction();
			initAction();
			calcDamage(getAction()->m_type,*(getDamage()));

			DEBUG5("new action %i time %f",getAction()->m_type,getAction()->m_time);

			// Action entsprechend der Verzoegerung schneller ausfuehren
			// aber maximal doppelt so schnell
			float mult = std::max(getAction()->m_time-delay, getAction()->m_time/2)/getAction()->m_time;
			DEBUG5("delay %f mult %f",delay,mult);
			if (getAction()->m_type == Action::WALK)
			{
				// Laufgeschwindigkeit entsprechend erhoehen
				m_speed *= (1/mult);

			}
			getAction()->m_time *= mult;
			DEBUG5("faster action time %f",getAction()->m_time);
		}

	}

	return true;
}


void Player::abortAction()
{
	float time = getAction()->m_elapsed_time;
	Action::ActionInfo* aci = Action::getActionInfo(getAction()->m_type);

	DEBUG5("abort Action %i (elapsed time %f)",getAction()->m_type, time);


	if (getAction()->m_type == Action::WALK)
	{
		// Position zurueck setzen
		getShape()->m_center -= m_speed*time;
	}

	// Timer wieder zuruecksetzen
	if (aci->m_timer_nr==1)
	{
		 m_timer1=0;
		 m_timer1_max=0;
	}

	if (aci->m_timer_nr==2)
	{
		m_timer2=0;
		m_timer2_max=0;
	}

	m_event_mask |= NetEvent::DATA_ACTION;
}

bool Player::update(float time)
{
	// Aufruf des updates für von Creature
	Creature::update(time);

	m_camera.update(time);

	// Player spezifische Updateroutine
	DEBUG5("Update des Playerobjekts [%i] wird gestartet", getId());

	// Debugging
	//return true;
	
	return true;
}

void Player::performActionCritPart(Vector goal, WorldObject* goalobj)
{
	if (getAction()->m_type == Action::TAKE_ITEM)
	{
		DEBUG5("take item");
		// Item suchen
		Item* itm = getRegion()->getItem(getCommand()->m_goal_object_id);

		if (itm !=0)
		{
			// aus der Region entfernen
			getRegion()->deleteItem(getCommand()->m_goal_object_id);

			// Item einfuegen
			insertItem(itm);

		}
		else
		{
			DEBUG("no item found at %f %f",goal.m_x,goal.m_y);
		}
	}
	else
	{
		Creature::performActionCritPart(goal,goalobj);
	}
}

bool Player::checkRole(std::string role)
{
	// all trifft immer zu
	if (role == "all" || role == "")
	{
		return true;
	}
	
	if (role == getTypeInfo()->m_subtype)
	{
		return true;
	}
	
	bool leader = getParty()->getLeader() == getId();
	
	if (leader && role =="leader")
	{
		return true;
	}
	
	if (!leader && role == "non-leader")
	{
		return true;
	}
	
	return false;
}


void Player::sendDetailedItem(short pos)
{
	/*
	Item* it = static_cast<Item*>(m_equipement->getItem(pos));

	if (it!=0)
	{
		ServerNetwork* net = World::getWorld()->getNetwork();
		int len;

		// Puffer fuer Item
		CharConv cv;
		ServerHeader header;

		memset(&header,0,sizeof(header));
		header.m_content = PTYPE_S2C_ITEM;
		header.m_detailed_item = pos;
		header.toString(&cv);
		it->toStringComplete(&cv,pos);
		net->pushSlotMessage(m_network_slot, cv.getBitStream());

	}
	*/
}


void Player::sendAbilityDamage(Action::ActionType act)
{
	/*
	// Wertebereich fuer Aktionen pruefen
	if (act<0 || act >=192)
		return;

	ServerNetwork* net = World::getWorld()->getNetwork();
	int len;

	// Puffer
	CharConv cv;
	ServerHeader header;

	// Header erstellen
	memset(&header,0,sizeof(header));
	header.m_content = PTYPE_S2C_DAMAGE;
	// Typ der Aktion wird im Feld fuer Item untergebracht...
	header.m_detailed_item = act;
	header.toString(&cv);

	// Damage berechnen
	Damage dmg;
	calcDamage(act,dmg);

	// Daten in den Puffer schreiben
	dmg.toString(&cv);
	net->pushSlotMessage(m_network_slot, cv.getBitStream());

	*/
}


void Player::calcBaseDamage(Action::ActionType act,Damage& dmg)
{
	Creature::calcBaseDamage(act,dmg);


	Item* weapon = getWeapon();

	if (weapon!=0)
	{
		// Schaden der Waffe
		Damage& wdmg=weapon->m_weapon_attr->m_damage;

		int i;
		// Schaden uebernehmen
		for (i=0;i<4;i++)
		{
			dmg.m_min_damage[i] += wdmg.m_min_damage[i];
			dmg.m_max_damage[i] += wdmg.m_max_damage[i];
			dmg.m_multiplier[i] *= wdmg.m_multiplier[i];
		}

		// Statusmods uebernehmen
		for (i=0;i<NR_STATUS_MODS;i++)
		{
			dmg.m_status_mod_power[i] += wdmg.m_status_mod_power[i];
		}

		// weitere Attribute
		dmg.m_power += wdmg.m_power;
		dmg.m_attack += wdmg.m_attack;
		dmg.m_crit_perc += wdmg.m_crit_perc;
		dmg.m_special_flags |= wdmg.m_special_flags;

	}
}

void Player::calcBaseAttrMod()
{
	Creature::calcBaseAttrMod();
	Item* si;

	// Wirkung der Ausruestungsgegenstaende einfuegen

	// Waffe
	si = getWeapon();
	if (si!=0)
	{
		if (si->m_equip_effect !=0)
		{
			applyBaseAttrMod(si->m_equip_effect,false);
		}

		if (si->m_weapon_attr !=0)
		{
			getBaseAttrMod()->m_attack_speed += si->m_weapon_attr ->m_dattack_speed;
			getBaseAttrMod()->m_attack_range = si->m_weapon_attr ->m_attack_range;

			if (si->m_weapon_attr ->m_dattack_speed!=0)
			{
				m_event_mask |= NetEvent::DATA_ATTACK_SPEED;
			}
		}
	}



	// Ruestung
	si = m_equipement->getItem(Equipement::ARMOR);
	if (si!=0 && si->m_equip_effect !=0)
	{
		applyBaseAttrMod(si->m_equip_effect,false);
	}

	// Helm
	si = m_equipement->getItem(Equipement::HELMET);
	if (si!=0 && si->m_equip_effect !=0)
	{
		applyBaseAttrMod(si->m_equip_effect,false);
	}

	// Schild
	si = getShield();
	if (si!=0 && si->m_equip_effect !=0)
	{
		applyBaseAttrMod(si->m_equip_effect,false);
	}

	// Handschuhe
	si = m_equipement->getItem(Equipement::GLOVES);
	if (si!=0 && si->m_equip_effect !=0)
	{
		applyBaseAttrMod(si->m_equip_effect,false);
	}

	// Ring links
	si = m_equipement->getItem(Equipement::RING_LEFT);
	if (si!=0 && si->m_equip_effect !=0)
	{
		applyBaseAttrMod(si->m_equip_effect,false);
	}

	// Ring rechts
	si = m_equipement->getItem(Equipement::RING_RIGHT);
	if (si!=0 && si->m_equip_effect !=0)
	{
		applyBaseAttrMod(si->m_equip_effect,false);
	}

	// Amulett
	si = m_equipement->getItem(Equipement::AMULET);
	if (si!=0 && si->m_equip_effect !=0)
	{
		applyBaseAttrMod(si->m_equip_effect,false);
	}

	// Faehigkeiten anwenden

	// Faehigkeit ausweichen
	if (checkAbility(Action::EVADE))
	{
		getBaseAttrMod()->m_block += getBaseAttrMod()->m_dexterity/ 2;
	}

		// Faehigkeit Windwandern
	if (getBaseAttrMod()->m_special_flags & WIND_WALK)
	{
		getBaseAttrMod()->m_block += getBaseAttrMod()->m_dexterity/ 4;
	}


	// Blockbonus durch Geschick (nur wenn schon ein Blockwert vorhanden ist)
	if (getBaseAttrMod()->m_block!=0 || getTypeInfo()->m_subtype == "archer")
	{
		getBaseAttrMod()->m_block += getBaseAttrMod()->m_dexterity/ 4;
	}

	// Faehigkeit Blocken
	if (checkAbility(Action::BLOCK))
	{
		getBaseAttrMod()->m_block += (short) (0.3f*getBaseAttrMod()->m_block);
	}




	// Schaden neu berechnen
	recalcDamage();


}

void Player::addMessage(std::string msg)
{
	if (msg == "")
		return;

	m_messages += msg;
	m_messages += "\n";

	// Anzahl Newlines zaehlen
	int cnt =0;
	int pos = 0;
	while (pos != -1)
	{

		cnt ++;
		pos=m_messages.find_first_of('\n',pos+1);
	}
	cnt --;
	while (cnt >20)
	{
		pos=m_messages.find_first_of('\n');
		m_messages = m_messages.substr(pos+1);
		cnt --;
	}
}


void Player::toString(CharConv* cv)
{
	DEBUG5("Player::tostring");
	Creature::toString(cv);

	cv->toBuffer((char*) m_name.c_str(),32);

	cv->toBuffer(getBaseAttr()->m_level);

	// Items
	char cnt =0;
	Item* item;
	for ( short i = Equipement::ARMOR; i<= Equipement::SHIELD2; i++)
	{
		item = getEquipement()->getItem(i);
		if (item !=0)
			cnt++;
	}
	DEBUG5("number of items: %i",cnt);
	cv->toBuffer(cnt);


	for ( short i = Equipement::ARMOR; i<= Equipement::SHIELD2; i++)
	{
		item = getEquipement()->getItem(i);
		if (item !=0)
		{
			cv->toBuffer(i);
			item->toString(cv);

		}
	}

}


void Player::fromString(CharConv* cv)
{
	Creature::fromString(cv);
	char tmp[32];
	cv->fromBuffer(tmp,32);
	m_name = tmp;

	cv->fromBuffer(getBaseAttr()->m_level);

	char cnt;
	cv->fromBuffer(cnt);
	DEBUG5("number of items: %i",cnt);


	for ( short i = 0; i< cnt; i++)
	{
		readItem(cv);
	}

}

void Player::readItem(CharConv* cv)
{
	short pos;
	char type;
	char subtype[11];
	int id;
	subtype[10] ='\0';
	Item* item;

	cv->fromBuffer<short>(pos);
	cv->fromBuffer<char>(type);
	cv->fromBuffer(subtype,10);
	cv->fromBuffer(id);

	item = ItemFactory::createItem((Item::Type) type, std::string(subtype),id);
	item->fromString(cv);
	getEquipement()->swapItem(item,pos);

	if (item !=0)
	{
		delete item;
	}
}

void Player::readItemComplete(CharConv* cv)
{
	short pos;
	char type;
	char subtype[11];
	int id;
	subtype[10] ='\0';
	Item* item;

	cv->fromBuffer<short>(pos);
	cv->fromBuffer<char>(type);
	cv->fromBuffer(subtype,10);
	cv->fromBuffer(id);

	DEBUG5("reading Item for pos %i type %i subtype %s",pos,type,subtype);
	item = ItemFactory::createItem((Item::Type) type, std::string(subtype),id);
	item->fromStringComplete(cv);
	getEquipement()->swapItem(item,pos);

	if (item !=0)
		delete item;
}

void Player::toStringComplete(CharConv* cv)
{
	this->toString(cv);
	cv->toBuffer(getBaseAttr()->m_level);
	cv->toBuffer(getBaseAttrMod()->m_max_health);
	cv->toBuffer(getDynAttr()->m_health);
	cv->toBuffer(getBaseAttrMod()->m_strength);
	cv->toBuffer(getBaseAttrMod()->m_dexterity);
	cv->toBuffer(getBaseAttrMod()->m_magic_power);
	cv->toBuffer(getBaseAttrMod()->m_willpower);
	cv->toBuffer(m_attribute_points);


	int i;
	for (i=0;i<4;i++)
		cv->toBuffer(getBaseAttrMod()->m_resistances[i]);
	for (i=0;i<4;i++)
		cv->toBuffer(getBaseAttrMod()->m_resistances_cap[i]);


	cv->toBuffer(getBaseAttr()->m_max_experience);
	cv->toBuffer(getDynAttr()->m_experience);
	cv->toBuffer(getBaseAttrMod()->m_armor);
	cv->toBuffer(getBaseAttrMod()->m_block);
	cv->toBuffer(getBaseAttrMod()->m_attack);
	cv->toBuffer(getBaseAttrMod()->m_attack_speed);
	cv->toBuffer(getBaseAttr()->m_attack_range);
	cv->toBuffer(m_base_damage.m_power);
	m_base_damage.toString(cv);
	m_left_damage.toString(cv);
	m_right_damage.toString(cv);

	/*
	for (i=0;i<4;i++)
	{
		printf("%s %f - %f\n",Damage::getDamageTypeName((Damage::DamageType) i).c_str(),m_left_damage.m_min_damage[i],m_left_damage.m_max_damage[i]);
		printf("%s %f - %f\n",Damage::getDamageTypeName((Damage::DamageType) i).c_str(),m_right_damage.m_min_damage[i],m_right_damage.m_max_damage[i]);
	}
	*/

	for (i=0;i<6;i++)
		cv->toBuffer(getBaseAttrMod()->m_abilities[i]);

	float trel1=0,trel2=0;
	if (m_timer1_max>0)
	{
		trel1 = m_timer1 / m_timer1_max;
	}

	if (m_timer2_max>0)
	{
		trel2 = m_timer2 / m_timer2_max;
	}

	DEBUG5("trel %f %f",trel1,trel2);
	cv->toBuffer(trel1);
	cv->toBuffer(trel2);
	// TODO: Questinformationen


}

void Player::recalcDamage()
{
	calcDamage(m_base_action,m_base_damage);
	calcDamage(m_left_action,m_left_damage);
	calcDamage(m_right_action,m_right_damage);

	/*
	for (int i=0;i<4;i++)
	{
		printf("%s %f - %f\n",Damage::getDamageTypeName((Damage::DamageType) i).c_str(),m_left_damage.m_min_damage[i],m_left_damage.m_max_damage[i]);
	}

	for (int i=0;i<4;i++)
	{
		printf("%s %f - %f\n",Damage::getDamageTypeName((Damage::DamageType) i).c_str(),m_right_damage.m_min_damage[i],m_right_damage.m_max_damage[i]);
	}
*/

}

void Player::toSavegame(CharConv* cv)
{

	int i;
	// Version richtig setzen
	cv->toBuffer((short) 10);
	
	cv->toBuffer(getTypeInfo()->m_subtype,10);
	cv->toBuffer(m_name,32);
	cv->toBuffer(m_look,20);
	cv->printNewline();
	
	cv->toBuffer(getBaseAttr()->m_level);
	cv->toBuffer(getBaseAttr()->m_max_health);
	cv->printNewline();

	cv->toBuffer(getBaseAttr()->m_armor);
	cv->toBuffer(getBaseAttr()->m_attack);
	cv->toBuffer(getBaseAttr()->m_strength);
	cv->toBuffer(getBaseAttr()->m_dexterity);
	cv->toBuffer(getBaseAttr()->m_magic_power);
	cv->toBuffer(getBaseAttr()->m_willpower);
	cv->printNewline();
	
	cv->toBuffer(m_attribute_points);
	cv->toBuffer(m_skill_points);
	cv->printNewline();
	
	cv->toBuffer(getBaseAttr()->m_walk_speed);
	cv->toBuffer(getBaseAttr()->m_attack_speed);
	cv->toBuffer(getBaseAttr()->m_attack_range);
	cv->printNewline();
	
	cv->toBuffer(getBaseAttr()->m_resistances[0]);
	cv->toBuffer(getBaseAttr()->m_resistances[1]);
	cv->toBuffer(getBaseAttr()->m_resistances[2]);
	cv->toBuffer(getBaseAttr()->m_resistances[3]);
	cv->printNewline();
	
	cv->toBuffer(getBaseAttr()->m_max_experience);
	cv->toBuffer(getDynAttr()->m_experience);
	cv->printNewline();
	
	for (i=0;i<6;i++)
	{
		cv->toBuffer(getBaseAttr()->m_abilities[i]);
	}
	cv->printNewline();
	
	cv->toBuffer((short) m_base_action);
	cv->toBuffer((short) m_left_action);
	cv->toBuffer((short) m_right_action);
	cv->printNewline();
	
	cv->toBuffer(m_revive_position.first);
	cv->toBuffer(m_revive_position.second);
	cv->printNewline();

	// Items
	writeEquipement(cv);
	cv->printNewline();

	// Questinformationen
	// Daten werden aus der lua Umgebung genommen, wenn die Welt schon laeuft
	// sonst werden sie im Spieler zwischengespeichert
	if (World::getWorld() == 0 || getId()==0)
	{
		std::list<std::string>::iterator it;
		for (it =m_lua_instructions.begin(); it !=m_lua_instructions.end(); ++it)
		{
			cv->toBuffer<char>(1);
			cv->toBuffer(*it);
			cv->printNewline();
		}
		cv->toBuffer<char>(0);
		
	}
	else
	{
		EventSystem::writeSavegame(cv);
		cv->printNewline();
	}
}


void Player::fromSavegame(CharConv* cv, bool local)
{

	
	short version;
	cv->fromBuffer<short>(version);

	cv->fromBuffer(getTypeInfo()->m_subtype,10);
	cv->fromBuffer(m_name,32);
	cv->fromBuffer(m_look,20);
	
	cv->fromBuffer(getBaseAttr()->m_level);
	cv->fromBuffer<float>(getBaseAttr()->m_max_health);
	getDynAttr()->m_health = getBaseAttr()->m_max_health;

	cv->fromBuffer(getBaseAttr()->m_armor);
	cv->fromBuffer(getBaseAttr()->m_attack);
	cv->fromBuffer<short>(getBaseAttr()->m_strength);
	cv->fromBuffer<short>(getBaseAttr()->m_dexterity);
	cv->fromBuffer<short>(getBaseAttr()->m_magic_power);
	cv->fromBuffer<short>(getBaseAttr()->m_willpower);

	cv->fromBuffer<short>(m_attribute_points);
	cv->fromBuffer<short>(m_skill_points );

	cv->fromBuffer(getBaseAttr()->m_walk_speed);
	cv->fromBuffer(getBaseAttr()->m_attack_speed);
	cv->fromBuffer(getBaseAttr()->m_attack_range);
	
	cv->fromBuffer<short>(getBaseAttr()->m_resistances[0]);
	cv->fromBuffer<short>(getBaseAttr()->m_resistances[1]);
	cv->fromBuffer<short>(getBaseAttr()->m_resistances[2]);
	cv->fromBuffer<short>(getBaseAttr()->m_resistances[3]);

	cv->fromBuffer<float>(getBaseAttr()->m_max_experience);
	cv->fromBuffer<float>(getDynAttr()->m_experience);

	for (int i=0;i<6;i++)
	{
		cv->fromBuffer<int>(getBaseAttr()->m_abilities[i]);
	}

	DEBUG5("name %s class %s level %i",m_name.c_str(), getTypeInfo()->m_subtype.c_str(), getBaseAttr()->m_level);
	
	short tmp;
	cv->fromBuffer<short>(tmp);
	m_base_action = (Action::ActionType) tmp;
	cv->fromBuffer<short>(tmp);
	m_left_action = (Action::ActionType) tmp;
	cv->fromBuffer<short>(tmp);
	m_right_action = (Action::ActionType) tmp;
	
	cv->fromBuffer(m_revive_position.first);
	cv->fromBuffer(m_revive_position.second);
	// Items
	loadEquipement(cv);

	getBaseAttrMod()->m_max_health = getBaseAttr()->m_max_health;
	calcBaseAttrMod();
	
	// Questinformationen
	// Daten werden aus der lua Umgebung genommen, wenn die Welt schon laeuft
	// sonst werden sie im Spieler zwischengespeichert
	if (World::getWorld() == 0 || getId()==0)
	{
		m_lua_instructions.clear();
		char c=1;
		std::string instr;
		while (c==1)
		{
			cv->fromBuffer(c);
			if (c==0)
				break;
			
			cv->fromBuffer(instr);
			m_lua_instructions.push_back(instr);
			DEBUG5("instructions: %s",instr.c_str());
		}
	}
	else
	{
		EventSystem::readSavegame(cv,getId(), local);
		cv->printNewline();
	}
}

void Player::writeEquipement(CharConv* cv)
{
	getEquipement()->toStringComplete(cv);
}

void Player::loadEquipement(CharConv* cv)
{
	getEquipement()->fromStringComplete(cv);
}


int Player::getValue(std::string valname)
{
	if (valname == "name")
	{
		lua_pushstring(EventSystem::getLuaState() , m_name.c_str() );
		return 1;
	}
	else if (valname =="attribute_points")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_attribute_points );
		return 1;
	}
	else if (valname =="skill_points")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_skill_points );
		return 1;
	}
	else
	{
		int ret = Creature::getValue(valname);

		return ret;
	}
	return 0;
}

bool Player::setValue(std::string valname)
{
	if (valname == "attribute_points")
	{
		m_attribute_points = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		m_event_mask |= NetEvent::DATA_SKILL_ATTR_POINTS;
		return true;
	}
	else if (valname =="skill_points")
	{
		m_skill_points = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		m_event_mask |= NetEvent::DATA_SKILL_ATTR_POINTS;
		return true;
	}
	else
	{
		return  Creature::setValue(valname);
	}

	return false;
}

void Player::writeNetEvent(NetEvent* event, CharConv* cv)
{
	Creature::writeNetEvent(event,cv);
	
	if (event->m_data & NetEvent::DATA_REVIVE_LOCATION)
	{
		cv->toBuffer(m_revive_position.first);
		cv->toBuffer(m_revive_position.second);
		DEBUG5("writing revive position %s %s",m_revive_position.first.c_str(), m_revive_position.second.c_str())
	}
	
	if (event->m_data & NetEvent::DATA_SKILL_ATTR_POINTS)
	{
		cv->toBuffer(m_attribute_points);
		cv->toBuffer(m_skill_points);
	}
}

void Player::processNetEvent(NetEvent* event, CharConv* cv)
{
	Creature::processNetEvent(event,cv);
	
	if (event->m_data & NetEvent::DATA_REVIVE_LOCATION)
	{
		cv->fromBuffer(m_revive_position.first);
		cv->fromBuffer(m_revive_position.second);
		DEBUG5("reading revive position %s %s",m_revive_position.first.c_str(), m_revive_position.second.c_str())
	}
	
	if (event->m_data & NetEvent::DATA_SKILL_ATTR_POINTS)
	{
		cv->fromBuffer(m_attribute_points);
		cv->fromBuffer(m_skill_points);
	}
}


void Player::setRevivePosition(RegionLocation regloc)
{
	DEBUG("changed revive position");
	m_revive_position = regloc;
	m_event_mask |= NetEvent::DATA_REVIVE_LOCATION;
}


