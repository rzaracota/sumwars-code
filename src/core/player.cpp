
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

//Constructors/Destructors

Player::Player(World* world, int id) : Creature(world,  id)
{
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
	getWorld()->getParty(getTypeInfo()->m_fraction)->removeMember(getId());
	Creature::destroy();
}

//Operations

bool Player::init()
{
	DEBUG5("Player::init");
	//eigene Initialisierung
	m_network_slot=-1;
	m_package_number =0;
	setTradeId(0);
	getTypeInfo()->m_type = TypeInfo::TYPE_PLAYER;
	getTypeInfo()->m_category = TypeInfo::HUMAN;
	Party* p = getWorld()->getEmptyParty();
	if (p==0)
	{
//		ERROR("cant open new party");
	}
	p->clear();
	p->addMember(getId());
	getTypeInfo()->m_fraction = (TypeInfo::Fraction) (p->getId() + TypeInfo::FRAC_PLAYER_PARTY);
	DEBUG("opened Party %i",p->getId());

	getGeometry()->m_shape.m_type = Shape::CIRCLE;
	getGeometry()->m_shape.m_radius = 0.5;
	getGeometry()->m_layer = (Geometry::LAYER_BASE | Geometry::LAYER_AIR);
	getGeometry()->m_angle =0;

	getBaseAttr()->m_step_length=1.5;
	getBaseAttr()->m_attack_range=0.5;
	getBaseAttr()->m_level=1;

	m_attribute_points=0;
	m_skill_points=25;

	m_secondary_equip = false;

	m_equipement = new Equipement(5,14,30);

	m_save_timer= 3000;

	return true;
}

bool Player::onGamefieldClick(ClientCommand* command)
{
	if (command->m_action>=192)
	{
		ERRORMSG("invalid action");
		return false;
	}

	Command* com = getNextCommand();
	int dist;
	bool meleedir = false;
	Party* p, *p2;
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

		DEBUG4("Kommando erhalten, zielid: %i",command->m_id);
		wo = getWorld()->getSWObject(command->m_id,getGridLocation()->m_region);

		// Unterscheidung Zielobject vs kein Zielobject
		if (wo !=0)
		{
			rel = getWorld()->getRelation(getTypeInfo()->m_fraction,wo);
			
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
						com->m_goal_coordinate_x =command->m_coordinate_x;
						com->m_goal_coordinate_y =command->m_coordinate_y;
						com->m_range = getBaseAttr()->m_attack_range;
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
						com->m_goal_coordinate_x =command->m_coordinate_x;
						com->m_goal_coordinate_y =command->m_coordinate_y;
						com->m_range = getBaseAttr()->m_attack_range;
					}
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
				com->m_goal_coordinate_x =command->m_coordinate_x;
				com->m_goal_coordinate_y =command->m_coordinate_y;
				com->m_range = getBaseAttr()->m_attack_range;
				com->m_goal_object_id = command->m_id;
			}
			else if (command->m_button == LEFT_SHIFT_MOUSE_BUTTON)
			{
				com->m_type = command->m_action;
				com->m_goal_coordinate_x =command->m_coordinate_x;
				com->m_goal_coordinate_y =command->m_coordinate_y;
				com->m_range = getBaseAttr()->m_attack_range;
				com->m_goal_object_id =0;
				if (dist == Action::MELEE)
				{
					
					meleedir = true;
				}
				m_event_mask |= Event::DATA_NEXT_COMMAND;
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
		DEBUG5("Kommando erhalten, Ziel (%f,%f) button %i action %i dist %i",command->m_coordinate_x, command->m_coordinate_y,command->m_button,command->m_action,dist);

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
			com->m_goal_coordinate_x =command->m_coordinate_x;
			com->m_goal_coordinate_y =command->m_coordinate_y;

		}
		else if (dist == Action::SELF)
		{
			// Aktionen auf selbst auf left_shift und rechts ausfuehren
			com->m_type = command->m_action;
			com->m_goal_coordinate_x =0;
			com->m_goal_coordinate_y =0;
			com->m_goal_object_id =0;
		}
		else if (dist == Action::PARTY_MULTI)
		{
			com->m_type = command->m_action;
			com->m_goal_coordinate_x =command->m_coordinate_x;
			com->m_goal_coordinate_y =command->m_coordinate_y;
			com->m_goal_object_id =0;
		}
		else if (dist == Action::PARTY)
		{
			// Aktionen auf Party nie ausfuehren, wenn kein Ziel angegeben
			com->m_type =Action::NOACTION;
			com->m_goal_coordinate_x =0;
			com->m_goal_coordinate_y =0;
			com->m_goal_object_id =0;

		}
		else if (dist == Action::RANGED)
		{
			// Fernkampf immer starten
			com->m_type = command->m_action;
			com->m_goal_coordinate_x =command->m_coordinate_x;
			com->m_goal_coordinate_y =command->m_coordinate_y;
			com->m_range = getBaseAttr()->m_attack_range;
			com->m_goal_object_id =0;
		}
		else if (dist == Action::MELEE && command->m_button == LEFT_SHIFT_MOUSE_BUTTON)
		{
			// Nahkampf nur dann starten, wenn shift gedrueckt wurde
			com->m_type = command->m_action;
			com->m_goal_coordinate_x =command->m_coordinate_x;
			com->m_goal_coordinate_y =command->m_coordinate_y;
			com->m_range = getBaseAttr()->m_attack_range;
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
		float dir[2];
		float d;
		float x = getGeometry()->m_shape.m_coordinate_x;
		float y = getGeometry()->m_shape.m_coordinate_y;
		dir[0] = com->m_goal_coordinate_x-x;
		dir[1] = com->m_goal_coordinate_y-y;
		d = sqr(dir[0]) + sqr(dir[1]);
		float range = com->m_range + getGeometry()->m_shape.m_radius;
		if ( d>= sqr(range))
		{
			if (d>0)
			{
				d = 1/sqrt(d);
				dir[0]*= d*0.95*range;
				dir[1]*= d*0.95*range;
				com->m_goal_coordinate_x = x+dir[0];
				com->m_goal_coordinate_y = y+dir[1];
				DEBUG5("new goal %f %f", com->m_goal_coordinate_x,com->m_goal_coordinate_y);
			}
			else
			{
				com->m_type =Action::NOACTION;
			}
		}
	}
	
	m_event_mask |= Event::DATA_NEXT_COMMAND;
	DEBUG5("resulting command %i goal %f %f id %i",com->m_type,com->m_goal_coordinate_x,com->m_goal_coordinate_y, com->m_goal_object_id);
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

				// testen ob Level ausreicht
				if (getBaseAttr()->m_level < it->m_level_req)
				{
					// Level Vorraussetzung nicht erfuellt
					DEBUG("level too low: own level: %i item level: %i",getBaseAttr()->m_level,it->m_level_req);
					req= false;
				}


				// testen ob Item fuer die Charakterklasse zugelassen ist
				char mask = Item::REQ_WARRIOR;
				if (getTypeInfo()->m_subtype == "mage")
					mask = Item::REQ_MAGE;
				if (getTypeInfo()->m_subtype == "archer")
					mask = Item::REQ_ARCHER;
				if (getTypeInfo()->m_subtype == "priest")
					mask = Item::REQ_PRIEST;

				if (it->m_char_req & mask != mask)
				{
					// Spieler darf das Item nicht benutzen (falsche Spielerklasse)
					DEBUG("wrong subtype");
					req =false;
				}

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
					DEBUG("wrong size");
					req = false;
				}

				if (pos >= Equipement::SMALL_ITEMS && pos < Equipement::SMALL_ITEMS+10)
				{
					// Guertel, nur Traenke zulassen
					if (it->m_type != Item::POTION)
						req = false;
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
		
		if (getWorld()->isServer() && pos <Equipement::CURSOR_ITEM)
		{
			// Ausruestungsgegenstand wurde getauscht
			Event event;
			event.m_id = getId();
			event.m_data = pos;
			
			if (m_equipement->getItem(pos) == 0)
			{
				event.m_type = Event::PLAYER_NOITEM_EQUIPED;
			}
			else
			{
				event.m_type = Event::PLAYER_ITEM_EQUIPED;
			}
			DEBUG5("event: %i at %i",event.m_type,event.m_data);
			getWorld()->insertEvent(event);
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
					
					if (getWorld()->isServer())
					{
						Event event;
						event.m_type =  Event::PLAYER_NOITEM_EQUIPED;
						event.m_data = shpos;
						event.m_id = getId();
							
						DEBUG5("event: no item at %i",shpos);
							
						getWorld()->insertEvent(event);
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
						
						// wenn man sich auf Serverseite befindet: Event generieren
						
						
						if (!getEquipement()->insertItem(itm))
						{
							// Einfuegen ins Inventar fehlgeschlagen
							// Item fallen lassen
							getRegion()->dropItem(itm,getGeometry()->m_shape.m_coordinate_x,getGeometry()->m_shape.m_coordinate_y);	
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
				
				if (getWorld()->isServer())
				{
					Event event;
					event.m_type =  Event::PLAYER_NOITEM_EQUIPED;
					event.m_data = wpos;
					event.m_id = getId();
							
					DEBUG5("event: no item at %i",wpos);
							
					getWorld()->insertEvent(event);
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
		it = m_equipement->getItem(pos);
		if (it!=0)
		{
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
	
	if (pos != Equipement::NONE)
	{
		// Gegenstand ins Inventar aufgenommen
		if (getWorld()->isServer())
		{
			Event event;
			event.m_type =  Event::PLAYER_ITEM_PICKED_UP ;
			event.m_data = pos;
			event.m_id = getId();
								
			DEBUG("event: item picked up %i",pos);
								
			getWorld()->insertEvent(event);
		}
	}
	else
	{
		// Gegenstand passt nicht ins Inventar
		// wieder fallen lassen
		getRegion()->dropItem(itm,getGeometry()->m_shape.m_coordinate_x,getGeometry()->m_shape.m_coordinate_y);
	}
	return pos;
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
			getDynAttr()->m_health +=5;
			m_event_mask |= Event::DATA_HP | Event::DATA_MAX_HP;
			break;
		case (CreatureBaseAttr::DEXTERITY):
			getBaseAttr()->m_dexterity++;
			getBaseAttr()->m_attack_speed +=3;
			m_event_mask |= Event::DATA_ATTACK_SPEED;
			break;
		case (CreatureBaseAttr::WILLPOWER):
			getBaseAttr()->m_willpower++;
			break;
		case (CreatureBaseAttr::MAGIC_POWER):
			getBaseAttr()->m_magic_power++;
			break;

	}
	calcBaseAttrMod();
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

	m_event_mask |= Event::DATA_LEVEL | Event::DATA_HP | Event::DATA_MAX_HP | Event::DATA_EXPERIENCE;
}


bool Player::onClientCommand( ClientCommand* command, float delay)
{
	DEBUG5("Kommando (%f %f) button: %i id: %i action: %i",command->m_coordinate_x,command->m_coordinate_y,command->m_button,command->m_id, command->m_action);

	Party* p, *p2;
	WorldObject* wo;

	Item* si;
	DropSlot ds;
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

		case BUTTON_PARTY_APPLY:
			DEBUG("apply to party %i",command->m_id);
			p = getWorld()->getParty((WorldObject::TypeInfo::Fraction) (command->m_id + TypeInfo::FRAC_PLAYER_PARTY));
			if (p->getNrMembers()==0)
				break;
			p->addCandidate(getId());

			break;

		case BUTTON_PARTY_ACCEPT:
			DEBUG("accept %i",command->m_id)

			p = getWorld()->getParty(getTypeInfo()->m_fraction);
			p ->acceptCandidate(command->m_id);
			// FIXME: Spieler ausgeben lassen !
			wo = getWorld()->getSWObject(command->m_id,getGridLocation()->m_region);
			if (wo !=0)
			{
				p2 = getWorld()->getParty(wo->getTypeInfo()->m_fraction);
				p2->removeMember(command->m_id);
				wo->getTypeInfo()->m_fraction = getTypeInfo()->m_fraction;
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
				}
			}
			break;

		case DROP_ITEM:
			si = getEquipement()->getItem(Equipement::CURSOR_ITEM);
			if (si !=0)
			{
				si=0;
				getEquipement()->swapItem(si,Equipement::CURSOR_ITEM);
				getRegion()->dropItem(si,getGeometry()->m_shape.m_coordinate_x,getGeometry()->m_shape.m_coordinate_y);
			}

		case DEBUG_SIGNAL:
			// Debugging
			if (command->m_id==0)
			{
				gainLevel();
			}
			if (command->m_id==2)
			{
				si = ItemFactory::createItem(Item::WEAPON,"short_sw",0,1000);
				getRegion()->dropItem(si,10,20);
			}
			if (command->m_id==1)
			{
				// Debugging 2
				si = ItemFactory::createItem(Item::WEAPON,"short_sw",0,1000);
				getEquipement()->swapItem(si,Equipement::MEDIUM_ITEMS+4);
				if (si!=0)
					delete si;

				si = ItemFactory::createItem(Item::HELMET,"steel_hlm",0,100);
				getEquipement()->swapItem(si,Equipement::MEDIUM_ITEMS+5);
				if (si!=0)
					delete si;

				si = ItemFactory::createItem(Item::GLOVES,"leath_gl",0,100);
				getEquipement()->swapItem(si,Equipement::MEDIUM_ITEMS+6);
				if (si!=0)
					delete si;

				si = ItemFactory::createItem(Item::ARMOR,"heavy_arm",0,500);
				getEquipement()->swapItem(si,Equipement::BIG_ITEMS+3);
				if (si!=0)
					delete si;

				si = ItemFactory::createItem(Item::SHIELD,"iron_sh",0,100);
				getEquipement()->swapItem(si,Equipement::BIG_ITEMS+4);
				if (si!=0)
					delete si;

				si = ItemFactory::createItem(Item::RING,"ring",0,100);
				getEquipement()->swapItem(si,Equipement::SMALL_ITEMS+15);
				if (si!=0)
					delete si;

				si = ItemFactory::createItem(Item::RING,"ring",0,400);
				getEquipement()->swapItem(si,Equipement::SMALL_ITEMS+16);
				if (si!=0)
					delete si;

				si = ItemFactory::createItem(Item::AMULET,"amulet",0,100);
				getEquipement()->swapItem(si,Equipement::SMALL_ITEMS+17);
				if (si!=0)
					delete si;

				si = ItemFactory::createItem(Item::AMULET,"amulet",0,800);
				getEquipement()->swapItem(si,Equipement::SMALL_ITEMS+18);
				if (si!=0)
					delete si;

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

		default: ;
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
				getMoveInfo()->m_speed_x /= mult;
				getMoveInfo()->m_speed_y /= mult;
					
			}
			getAction()->m_time *= mult;
			DEBUG5("faster action time %i",getAction()->m_time);
		}
		
	}
}


void Player::abortAction()
{
	float time = getAction()->m_elapsed_time;
	Action::ActionInfo* aci = Action::getActionInfo(getAction()->m_type);

	DEBUG5("abort Action %i (elapsed time %f)",getAction()->m_type, time);

	
	if (getAction()->m_type == Action::WALK)
	{
		// Position zurueck setzen
		getGeometry()->m_shape.m_coordinate_x -= getMoveInfo()->m_speed_x*time;
		getGeometry()->m_shape.m_coordinate_y -= getMoveInfo()->m_speed_y*time;
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
	
	m_event_mask |= Event::DATA_ACTION;
}

bool Player::update(float time)
{
	// Aufruf des updates für von Creature
	Creature::update(time);

	// Player spezifische Updateroutine
	DEBUG5("Update des Playerobjekts [%i] wird gestartet", getId());

	// Debugging
	//return true;

	Trade* trade = 0;
	
	/*
	// handle inputs form network
	ServerNetwork* net = getWorld()->getNetwork();
	if( net->getSlotStatus( m_network_slot )!=NET_CONNECTED )
	{
		// disconnect, Spieler sichern und aus der Welt entfernen
		// TODO
		//saveObject( false );
		DEBUG("disconnect");
		setDestroyed(true);

		return false;
	}

	Packet* data;
	int len;

	if (getState() == STATE_REGION_ENTERED)
	{
		// Spieler hat Region betreten und ist in der Region noch nicht aktiviert

		Region* reg = getRegion();
		
		ServerHeader header;

		CharConv cv;
		// Header erstellen
		memset(&header,0,sizeof(header));
		header.m_content = PTYPE_S2C_REGION;
		header.toString(&cv);

		reg->getRegionDataString(&cv);

		net->pushSlotMessage(m_network_slot, cv.getBitStream());


		// Spieler aktivieren
		setState(STATE_ACTIVE);
	}

	ClientHeader headerp;
	CharConv* cv;
	while( net->numberSlotMessages( m_network_slot )>0 )
	{
		DEBUG5("received msg");
		data=0;
		net->popSlotMessage( m_network_slot,data );
		
		cv = new CharConv(data);

		headerp.fromString(cv);

		if( headerp.m_content == PTYPE_C2S_DATA && !headerp.m_chatmessage)
		{
			ClientCommand comp;

			comp.fromString(cv);
			DEBUG5("Kommando (%f %f) %i %i",comp.m_coordinate_x,comp.m_coordinate_y,comp.m_button, comp.m_id);

			//for (char* c =(char*) comp; c< sizeof(ClientCommand) + (char*) comp; c++)
			//	printf("%02x",*c);
			//printf("\n");
			
			onClientCommand( &comp );

		}
		

		// Chatmessage erhalten
		if(  headerp.m_content == PTYPE_C2S_DATA && headerp.m_chatmessage	)
		{
			DEBUG("Chatmessage erhalten");
			
			
			char* msgp = data + sizeof(ClientHeader);
			int len = sizeof(PackageHeader)+sizeof(Chatmessage);
			char sendmsg[len];
			Chatmessage cmsg;
			PackageHeader header;
			memset(&header,0,sizeof(PackageHeader));

			// Weiterleiten an alle Player
			WorldObjectSelector* sel = new WorldObjectSelector;
			sel->getObjectType().setObjectType(OBJECTTYPE_USER);
			sel->setSelectObjectType(true);

			list<WorldObject*>* ret = new list<WorldObject*>;

			getWorld()->getWorldObjectsInCircle(getCoordinateX(),getCoordinateY(),1000, sel,ret);

			list<WorldObject*>::iterator i;


			header.m_chatmessages++;
			header.m_mark = MARK;

			string msg =getName() ;
			msg += " :> ";
			msg.append(msgp,CHATMESSAGE_LEN-msg.size());

			memcpy(&cmsg.m_message,msg.c_str(),CHATMESSAGE_LEN);
			cmsg.m_message[CHATMESSAGE_LEN-1]=0;


			memcpy(sendmsg,&header, sizeof(PackageHeader));
			memcpy(sendmsg+sizeof(PackageHeader),&cmsg,sizeof(Chatmessage));



			Player* usr;

			//DEBUG4("zu sendende nachricht %s", chat.m_message);
			for (i=ret->begin(); i!=ret->end();i++)
			{
					if ((*i)->getObjectType().getObjectType() == OBJECTTYPE_USER)
					{

						usr = (Player*) (*i);

						DEBUG4("sende nachricht an Player %i", usr->getId());
						net->pushSlotMessage(usr->getNetworkSlot(), sendmsg, len );
					}
			}
			DEBUG3("Senden abgeschlossen");
			
		}

		delete cv;
		net->deallocatePacket(data);

	}
*/

	// Behandlung des Handels
	if (getTradeId() !=0)
	{
		trade = getWorld()->getTrade(getTradeId());

		/*
		// Wenn der Handel nicht existiert Fehler ausgeben
		if (trade == 0)
		{
			setTradeId(0);
		}
		else
		{
			// eigene Position bei dem Handel bestimmen
			int idx = trade->getPlayerIndex(getId());

			// Wenn spieler gestorben Handel abbrechen
			if (getState()==STATES_DEAD)
			{
				trade->abortTrade();
			}

			DEBUG5("Playerobjekt [%i] hat das Handelsobjekt [%i] als sein Handel erkannt", getId() ,getTradeId());
			// Abfragen ob der Handel bereits abgeschlossen ist.
			if (idx ==-1)
			{
				setTradeId(0);
			}
			else if (trade->getFinished()==true)
			{
				// Abfragen ob der Handel erfolgreich war
				if (trade->getSuccessful()==true)
				{
					// Wenn Gegenstände noch nicht ausgetauscht
					if (trade->getGotItems(idx)==false)
					{
						// Items auf erhalten setzen
						trade->setGotItems(true, idx);

						// Items austauschen
						ItemList* tmp = trade->getInventory(idx);
						trade->setInventory(getItems(),idx);
						setItems(tmp);

						calcBaggage();
					}

				}
				DEBUG5("Handel [%i] für Player [%i] abgeschlossen", getTradeId(), getId());
				// Handel ist erledigt, id auf 0 setzen
				setTradeId(0);
			}

		}
		if (getCommand()->getType() == COMMAND_TRADE && getTradeId()==0)
		{
			getCommand()->setType(COMMAND_NONE);
			getObjectType().setActionType(ACTION_NONE);
			getAction()->setType(ACTION_NONE);
		}
		*/

	}


	// Wenn ausreichend lange tot
	/*
	if (getState()==STATES_DEAD && getAction()->getProgress()==1)
	{
		//respawnen
		setState(STATES_ACTIVE);
		setHealth(getMaxHealth());

		// RespawnPoint, das muss dann nochmal verbessert werden
		// aktuell gibts nen Fehler wenn (0,0) nicht frei ist

		float x,y;
		getWorld()->getClosestFreeSquare(0,0,x,y);

		MoveTo(x,y);

		setTradeId(0);
		getCommand()->setType(COMMAND_NONE);
		getObjectType().setActionType(ACTION_NONE);
		getAction()->setType(ACTION_NONE);


	}
	*/
	/*
	int len = 5+rand()%50;
	char* tmp=new char[len];
	memset(tmp,'a',len);
	net->pushSlotMessage( m_network_slot,tmp,len);
	*/

	/*
	sendGameData();




	m_save_timer -= time;
	if (m_save_timer <=0)
	{
		m_save_timer = 10000;
		sendSavegame();
	}
*/
	return true;
}

void Player::performActionCritPart(float goalx, float goaly, WorldObject* goal)
{
	if (getAction()->m_type == Action::TAKE_ITEM)
	{
		// Item suchen
		Item* itm = getRegion()->getItem(getCommand()->m_goal_object_id);

		if (itm !=0)
		{
			// aus der Region entfernen
			getRegion()->deleteItem(getCommand()->m_goal_object_id);
			
			// Item einfuegen
			short pos = insertItem(itm);

		}
		else
		{
			DEBUG("no item found at %f %f",goalx,goaly);
		}
	}
	else
	{
		Creature::performActionCritPart(goalx,goaly,goal);
	}
}

void Player::sendGameData()
{
	/*
	// handle inputs form network
	ServerNetwork* net = getWorld()->getNetwork();

	Trade* trade = 0;
	WorldObject* obj=0;
	int len;

	ServerHeader header;

	header.m_content=PTYPE_S2C_DATA;
	header.m_objects = 0;
	header.m_items=0;
	header.m_drop_items=0;
	header.m_projectiles =0;
	header.m_chatmessage=false;
	header.m_trade= false;
	if (getTradeId()!=0 && trade != 0 )
		header.m_trade = true;
	header.m_detailed_item = 0;

	CharConv cv;
	//DEBUG5("packed bytes: %i bytes",cv.getBitStream()->GetNumberOfBitsUsed());
	
	list<WorldObject*> wobjs;
	wobjs.clear();

	// Ausschnitt der Welt, der fuer den Spieler sichtbar ist
	Shape* sh = &(getGeometry()->m_shape);
	Shape shs;

	shs.m_coordinate_x = sh->m_coordinate_x;
	shs.m_coordinate_y = sh->m_coordinate_y;
	shs.m_type = Shape::RECT;
	shs.m_extent_x = 12;
	shs.m_extent_y = 12;

	// alle sichtbaren Objekte holen
	if( !getWorld()->getSWObjectsInShape(&shs,getGridLocation()->m_region,&wobjs ) )
		return;
	
	
	
	// Objekte einpacken
	for( list<WorldObject*>::iterator i=wobjs.begin() ; i!=wobjs.end() ; )
	{
		// Objekt in das Paket packen
		obj = (WorldObject*) (*i);

		// Objekte die statisch sind nicht übertragen
		if (obj->getState() == STATE_STATIC)
		{
			i = wobjs.erase(i);
		}
		else if (obj->getId() == getId())
		{
			i = wobjs.erase(i);
		}
		else
		{
			++i;
		}

	}
	header.m_objects= wobjs.size();
	
	list<DmgProjectile*> res2;
	list<DmgProjectile*>::iterator i2;
	// Liste der sichtbaren Projektile
	getWorld()->getProjectilesOnScreen(sh->m_coordinate_x,sh->m_coordinate_y,getGridLocation()->m_region,&res2);
	header.m_projectiles = res2.size();
	
	header.m_items = m_equipement->getNumberItems(m_secondary_equip);
	
	// am Boden liegende Gegenstaende
	list<DropItem*> res3;
	list<DropItem*>::iterator it3;
	getRegion()->getItemsOnScreen(sh->m_coordinate_x,sh->m_coordinate_y,&res3);
	header.m_drop_items = res3.size();
	header.toString(&cv);
		
	// Begin packet erstellen
	toStringComplete(&cv);

	//DEBUG("bits written %i",cv.getBitStream()->GetNumberOfBitsUsed());
	
	


	// Objekte einpacken
	for( list<WorldObject*>::iterator i=wobjs.begin() ; i!=wobjs.end() ; ++i)
	{
		// Objekt in das Paket packen
		obj = (WorldObject*) (*i);

		obj->toString(&cv);
		DEBUG5("packed object %i",obj->getId());


	}


	// Projektile einpacken
	for( i2=res2.begin() ; i2!=res2.end() ; ++i2)
	{
		(*i2)->toString(&cv);
	}
	DEBUG5("anzahl projektile %i",header.m_projectiles);

	// Gegenstaende senden
	// Inventar
	int nr;
	m_equipement->toString(&cv,nr,m_secondary_equip);
	
	
	
	// Gegenstaende am Boden senden
	for (it3 = res3.begin(); it3 != res3.end(); ++it3)
	{
		(*it3)->toString(&cv);
	}

	DEBUG5("anzahl gedroppter items %i",header.m_drop_items);

//	int len = ch.getBitStream()->GetNumberOfBytesUsed();
//	cv.backToStart();
//	header.toString(&cv);
//	ch.getBitStream()->

	//DEBUG5("sending %i byte of data",len);
	//DEBUG5("objects %i projectiles %i",header.m_objects,header.m_projectiles);

	
	
	DEBUG5("sending %i bytes",cv.getBitStream()->GetNumberOfBytesUsed());
	net->pushSlotMessage(m_network_slot, cv.getBitStream(),HIGH_PRIORITY, UNRELIABLE_SEQUENCED);
	*/

}

void Player::sendDetailedItem(short pos)
{
	/*
	Item* it = static_cast<Item*>(m_equipement->getItem(pos));

	if (it!=0)
	{
		ServerNetwork* net = getWorld()->getNetwork();
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

	ServerNetwork* net = getWorld()->getNetwork();
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

void Player::sendSavegame()
{
	/*
	ServerNetwork* net = getWorld()->getNetwork();
	int len;

	// Puffer fuer Savegame
	CharConv cv;
	ServerHeader header;

	memset(&header,0,sizeof(header));
	header.m_content = PTYPE_S2C_SAVEGAME;
	header.toString(&cv);
	toSavegame(&cv);
	net->pushSlotMessage(m_network_slot,  cv.getBitStream());
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
				m_event_mask |= Event::DATA_ATTACK_SPEED;
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
		delete item;
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
	
	DEBUG("reading Item for pos %i type %i subtype %s",pos,type,subtype);
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
	
	
	
	
	// Party Informationen
	getWorld()->getParty(getTypeInfo()->m_fraction)->toString(cv);

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

void Player::toSavegame(CharConv* cvin)
{
	
	int i;
	// immer binaere Speicherung
	cvin->toBuffer((char) 1);
	// TODO Version richtig setzen
	cvin->toBuffer((short) 1);
	// Laenge der Datei
	
	CharConv* cv = new CharConv;
	char stmp[11];
	stmp[10] = '\0';
	strncpy(stmp,getTypeInfo()->m_subtype.c_str(),10);
	cv->toBuffer(stmp,10);
	char ntmp[32];
	strncpy(ntmp,m_name.c_str(),32);
	cv->toBuffer(ntmp,32);
	cv->toBuffer(getBaseAttr()->m_level);
	cv->toBuffer(getBaseAttr()->m_max_health);

	cv->toBuffer(getBaseAttr()->m_strength);
	cv->toBuffer(getBaseAttr()->m_dexterity);
	cv->toBuffer(getBaseAttr()->m_magic_power);
	cv->toBuffer(getBaseAttr()->m_willpower);

	cv->toBuffer(m_attribute_points);
	cv->toBuffer(m_skill_points);

	cv->toBuffer(getBaseAttr()->m_resistances[0]);
	cv->toBuffer(getBaseAttr()->m_resistances[1]);
	cv->toBuffer(getBaseAttr()->m_resistances[2]);
	cv->toBuffer(getBaseAttr()->m_resistances[3]);

	cv->toBuffer(getBaseAttr()->m_max_experience);
	cv->toBuffer(getDynAttr()->m_experience);
	for (i=0;i<6;i++)
	{
		cv->toBuffer(getBaseAttr()->m_abilities[i]);
	}
	// TODO: letzte Stadt eintragen
	cv->toBuffer((short) 1);

	cv->toBuffer((short) m_base_action);
	cv->toBuffer((short) m_left_action);
	cv->toBuffer((short) m_right_action);

	
	// TODO: Questinformationen

	// Items
	writeEquipement(cv);
	
	int len = cv->getBitStream()->GetNumberOfBytesUsed();
	DEBUG5("length of savegame %i",len+7);
	cvin->toBuffer(len+7);
	
	cvin->toBuffer((char*) cv->getBitStream()->GetData() , len);
	
	delete cv;
	
	//int len = bp - buf;
	//cv->toBuffer(bp2,len);
	
	
}


void Player::fromSavegame(CharConv* cv)
{
	
	char binsave;
	cv->fromBuffer<char>(binsave);
	short version;
	cv->fromBuffer<short>(version);
	int len;
	cv->fromBuffer<int>(len);
	int i;

	char ctmp[11];;
	// Typ (schon eher gesetzt)
	cv->fromBuffer(ctmp,10);

	char name[32];
	cv->fromBuffer(name,32);
	name[31]=0;
	m_name.assign(name);
	cv->fromBuffer<char>(getBaseAttr()->m_level);
	cv->fromBuffer<float>(getBaseAttr()->m_max_health);
	getDynAttr()->m_health = getBaseAttr()->m_max_health;

	cv->fromBuffer<short>(getBaseAttr()->m_strength);
	cv->fromBuffer<short>(getBaseAttr()->m_dexterity);
	cv->fromBuffer<short>(getBaseAttr()->m_magic_power);
	cv->fromBuffer<short>(getBaseAttr()->m_willpower);

	cv->fromBuffer<short>(m_attribute_points);
	cv->fromBuffer<short>(m_skill_points );


	cv->fromBuffer<short>(getBaseAttr()->m_resistances[0]);
	cv->fromBuffer<short>(getBaseAttr()->m_resistances[1]);
	cv->fromBuffer<short>(getBaseAttr()->m_resistances[2]);
	cv->fromBuffer<short>(getBaseAttr()->m_resistances[3]);
	
	cv->fromBuffer<float>(getBaseAttr()->m_max_experience);
	cv->fromBuffer<float>(getDynAttr()->m_experience);

	for (i=0;i<6;i++)
	{
		cv->fromBuffer<int>(getBaseAttr()->m_abilities[i]);
	}

	// TODO: letzte Stadt auslesen
	short tmp;
	cv->fromBuffer<short>(tmp);

	cv->fromBuffer<short>(tmp);
	m_base_action = (Action::ActionType) tmp;
	cv->fromBuffer<short>(tmp);
	m_left_action = (Action::ActionType) tmp;
	cv->fromBuffer<short>(tmp);
	m_right_action = (Action::ActionType) tmp;


	// TODO: Questinformationen

	// Items
	loadEquipement(cv);

	
	//DEBUG("size of savegame %i",len);

	calcBaseAttrMod();

}

void Player::writeEquipement(CharConv* cv)
{
	/*
	Item* it;

	short nr=0;
	short i;
	for (i=1;i<=Equipement::SMALL_ITEMS+30;i++)
	{
		if (m_equipement->getItem(i)!=0)
			nr++;
	}
	
	// Platz fuer die Anzahl
	cv->toBuffer(nr);
	

	for (i=1;i<=Equipement::SMALL_ITEMS+30;i++)
	{
		it = static_cast<Item*>(m_equipement->getItem(i));
		if (it!=0)
		{
			DEBUG5("saving item at pos %i",i);
			it->toStringComplete(cv,i);
		}
	}

	DEBUG5("written %i items",nr);
	*/
}

void Player::loadEquipement(CharConv* cv)
{
	/*
	m_equipement->clear();
	Item* it;
	Item* itm;
	short pos;

	int i;
	short nr;
	cv->fromBuffer<short>(nr);

	DEBUG5("reading %i items",nr);
	for (i=0;i<nr;i++)
	{
		it = new Item();
		it->fromStringComplete(cv,pos);
		DEBUG5("loaded Item at pos %i",pos);
		itm  = it;
		m_equipement->swapItem(itm,pos);
	}
	*/
}








