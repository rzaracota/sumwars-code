#include "objectfactory.h"

#include "warrior.h"
#include "mage.h"
#include "archer.h"
#include "priest.h"
#include "fixedobject.h"
#include "monster.h"

#define USE_OBJECTLOADER


World* ObjectFactory::m_world;

map<WorldObject::TypeInfo::ObjectSubtype, MonsterBasicData*> ObjectFactory::m_monster_data;

void ObjectFactory::registerMonster(WorldObject::TypeInfo::ObjectSubtype subtype, MonsterBasicData* data)
{
	DEBUG5("registered monster for subtype %s",subtype.c_str());
	m_monster_data.insert(make_pair(subtype,data));
}

void ObjectFactory::init()
{
	MonsterBasicData* mdata;
	int i;
	float p[4];
	
#ifdef USE_OBJECTLOADER
	// Objekte bzw. Monster aus XML Laden
	ObjectLoader* objectloader = 0;
	objectloader = new ObjectLoader;
	list<MonsterBasicData*>* monster_list;
	monster_list = objectloader->loadObjects("../../data/monsters.xml");
	
	if (monster_list != 0)
	{
		list<MonsterBasicData*>::iterator forward_iterator = monster_list->begin();
		while (forward_iterator != monster_list->end())
		{
			/*cout << "> > >  Testwert: " << (*forward_iterator)->m_base_attr.m_abilities[0] << "  < < <" << endl;
			if ((*forward_iterator)->m_base_attr.m_abilities[0] == 0xf1f)
				cout << "Wert stimmt ueberein" << endl;
			else
				cout << "Wert ist falsch" << endl;*/
			// FIXME WorldObject::TypeInfo::SUBTYPE_GOBLIN  soll von loadObjects zurueckgegeben werden
			registerMonster("goblin", *forward_iterator);
			//registerMonster(WorldObject::TypeInfo::SUBTYPE_GOBLIN, *forward_iterator);
			*forward_iterator++;
		}
	}
	
	delete objectloader;
#endif

#ifndef USE_OBJECTLOADER
	// Goblin
	mdata = new MonsterBasicData;

	// Goblin Type Informationen
	mdata->m_type_info.m_type = WorldObject::TypeInfo::TYPE_MONSTER;
	mdata->m_type_info.m_subtype = "goblin";
	mdata->m_type_info.m_fraction = WorldObject::TypeInfo::FRAC_MONSTER;
	mdata->m_type_info.m_category = WorldObject::TypeInfo::GOBLIN;

	// Goblin Drop Informationen
	p[0] = 0.1; p[1] = 0.2; p[2] = 0.2; p[3] = 0.2;
	mdata->m_drop_slots[0].init(p, 0,20, 0.3, 500);
	mdata->m_drop_slots[1].init(p,0, 10, 0.3, 1000);

	// Goblin Attribut werte
	mdata->m_base_attr.m_max_experience = 100000;
	mdata->m_base_attr.m_level =1;
	mdata->m_base_attr.m_max_health = 150;
	mdata->m_base_attr.m_armor = 10;
	mdata->m_base_attr.m_block=0;
	mdata->m_base_attr.m_attack = 10;
	mdata->m_base_attr.m_strength = 15;
	mdata->m_base_attr.m_dexterity = 10;
	mdata->m_base_attr.m_magic_power = 5;
	mdata->m_base_attr.m_willpower = 10;
	mdata->m_base_attr.m_resistances[Damage::PHYSICAL] =0;
	mdata->m_base_attr.m_resistances[Damage::AIR] =0;
	mdata->m_base_attr.m_resistances[Damage::ICE] =0;
	mdata->m_base_attr.m_resistances[Damage::FIRE] =0;
	mdata->m_base_attr.m_resistances_cap[Damage::PHYSICAL] =50;
	mdata->m_base_attr.m_resistances_cap[Damage::AIR] =50;
	mdata->m_base_attr.m_resistances_cap[Damage::ICE] =50;
	mdata->m_base_attr.m_resistances_cap[Damage::FIRE] =50;
	mdata->m_base_attr.m_walk_speed = 2000;
	mdata->m_base_attr.m_attack_speed=1500;
	mdata->m_base_attr.m_step_length = 0.5;
	for (i=0;i<6;i++)
		mdata->m_base_attr.m_abilities[i]=0;

	mdata->m_base_attr.m_abilities[0] = 0xf1f;
	mdata->m_base_attr.m_attack_range =1;
	mdata->m_base_attr.m_special_flags=0;
	mdata->m_base_attr.m_immunity =0;

	// Goblin Geometrie Informationen
	mdata->m_layer = (WorldObject::Geometry::LAYER_BASE | WorldObject::Geometry::LAYER_AIR)
	mdata->m_radius = 0.5;

	// Goblin AI Informationen
	mdata->m_ai_sight_range = 8;

	registerMonster("goblin",mdata);
#endif

	// Lich
	mdata = new MonsterBasicData;

	// Lich Type Informationen
	mdata->m_type_info.m_type = WorldObject::TypeInfo::TYPE_MONSTER;
	mdata->m_type_info.m_subtype = "lich";
	mdata->m_type_info.m_fraction = WorldObject::TypeInfo::FRAC_UNDEAD;
	mdata->m_type_info.m_category = WorldObject::TypeInfo::UNDEAD;

	// Lich Drop Informationen
	p[0] = 0.1; p[1] = 0.2; p[2] = 0.2; p[3] = 0.2;
	mdata->m_drop_slots[0].init(p, 10,40, 0.5,1000);
	mdata->m_drop_slots[1].init(p,0, 20, 0.5, 1000);

	// Lich Attribut werte
	mdata->m_base_attr.m_max_experience = 100000;
	mdata->m_base_attr.m_level =1;
	mdata->m_base_attr.m_max_health = 250;
	mdata->m_base_attr.m_armor = 20;
	mdata->m_base_attr.m_block=0;
	mdata->m_base_attr.m_attack = 20;
	mdata->m_base_attr.m_strength = 25;
	mdata->m_base_attr.m_dexterity = 10;
	mdata->m_base_attr.m_magic_power = 5;
	mdata->m_base_attr.m_willpower = 20;
	mdata->m_base_attr.m_resistances[Damage::PHYSICAL] =30;
	mdata->m_base_attr.m_resistances[Damage::AIR] =0;
	mdata->m_base_attr.m_resistances[Damage::ICE] =50;
	mdata->m_base_attr.m_resistances[Damage::FIRE] =-50;
	mdata->m_base_attr.m_resistances_cap[Damage::PHYSICAL] =50;
	mdata->m_base_attr.m_resistances_cap[Damage::AIR] =50;
	mdata->m_base_attr.m_resistances_cap[Damage::ICE] =50;
	mdata->m_base_attr.m_resistances_cap[Damage::FIRE] =50;
	mdata->m_base_attr.m_walk_speed = 1500;
	mdata->m_base_attr.m_attack_speed=1000;
	mdata->m_base_attr.m_step_length = 1.0;
	for (i=0;i<6;i++)
		mdata->m_base_attr.m_abilities[i]=0;

	mdata->m_base_attr.m_abilities[0] = 0xf1f;
	mdata->m_base_attr.m_abilities[3] = 0x100;
	mdata->m_base_attr.m_attack_range =2;
	mdata->m_base_attr.m_special_flags=0;
	mdata->m_base_attr.m_immunity =0xa;

	// Lich Geometrie Informationen
	mdata->m_layer = (WorldObject::Geometry::LAYER_BASE | WorldObject::Geometry::LAYER_AIR);
	mdata->m_radius = 1.0;

	// Lich AI Informationen
	mdata->m_ai_sight_range = 8;

	registerMonster("lich",mdata);


	// Goblin dog
	mdata = new MonsterBasicData;

	// Goblin dog Informationen
	mdata->m_type_info.m_type = WorldObject::TypeInfo::TYPE_MONSTER;
	mdata->m_type_info.m_subtype ="gob_dog";
	mdata->m_type_info.m_fraction = WorldObject::TypeInfo::FRAC_MONSTER;
	mdata->m_type_info.m_category = WorldObject::TypeInfo::ANIMAL;

	// Goblin dog Drop Informationen
	p[0] = 0.1; p[1] = 0.2; p[2] = 0.2; p[3] = 0.2;
	mdata->m_drop_slots[0].init(p, 10,40, 0.5,1000);
	mdata->m_drop_slots[1].init(p,0, 20, 0.5, 1000);

	// Goblin dog Attribut werte
	mdata->m_base_attr.m_max_experience = 100000;
	mdata->m_base_attr.m_level =1;
	mdata->m_base_attr.m_max_health = 100;
	mdata->m_base_attr.m_armor = 10;
	mdata->m_base_attr.m_block=0;
	mdata->m_base_attr.m_attack = 20;
	mdata->m_base_attr.m_strength = 10;
	mdata->m_base_attr.m_dexterity = 20;
	mdata->m_base_attr.m_magic_power = 5;
	mdata->m_base_attr.m_willpower = 10;
	mdata->m_base_attr.m_resistances[Damage::PHYSICAL] =0;
	mdata->m_base_attr.m_resistances[Damage::AIR] =0;
	mdata->m_base_attr.m_resistances[Damage::ICE] =10;
	mdata->m_base_attr.m_resistances[Damage::FIRE] =0;
	mdata->m_base_attr.m_resistances_cap[Damage::PHYSICAL] =50;
	mdata->m_base_attr.m_resistances_cap[Damage::AIR] =50;
	mdata->m_base_attr.m_resistances_cap[Damage::ICE] =50;
	mdata->m_base_attr.m_resistances_cap[Damage::FIRE] =50;
	mdata->m_base_attr.m_walk_speed = 2500;
	mdata->m_base_attr.m_attack_speed=2000;
	mdata->m_base_attr.m_step_length = 0.5;
	for (i=0;i<6;i++)
		mdata->m_base_attr.m_abilities[i]=0;

	mdata->m_base_attr.m_abilities[0] = 0xf1f;
	mdata->m_base_attr.m_attack_range =0.5;
	mdata->m_base_attr.m_special_flags=0;
	mdata->m_base_attr.m_immunity =0;

	// Goblin dogGeometrie Informationen
	mdata->m_layer = (WorldObject::Geometry::LAYER_BASE | WorldObject::Geometry::LAYER_AIR);
	mdata->m_radius = 0.5;

	// Goblin dog AI Informationen
	mdata->m_ai_sight_range = 8;

	registerMonster("gob_dog",mdata);
}

ServerWObject* ObjectFactory::createObject(WorldObject::TypeInfo::ObjectType type, WorldObject::TypeInfo::ObjectSubtype subtype)
{
	// Zeiger auf erzeugtes Objekt
	ServerWObject* ret=0;

	// ID des Objektes
	int id = m_world->getValidId();
	if (type ==WorldObject::TypeInfo::TYPE_PLAYER)
	{
		if (subtype == "warrior")
		{
			ret = new Warrior( m_world, id );
		}
		else if (subtype == "mage")
		{
			ret = new Mage( m_world, id );
		}
		else if (subtype == "archer")
		{
			ret = new Archer( m_world, id );
		}
		else if(subtype == "priest")
		{
			ret = new Priest( m_world, id );				
		}
	}
	else if (type ==WorldObject::TypeInfo::TYPE_MONSTER)
	{
		DEBUG5("requested subtype: %s",subtype.c_str());
		MonsterBasicData* mdata;
		map<WorldObject::TypeInfo::ObjectSubtype, MonsterBasicData*>::iterator i;

		i = m_monster_data.find(subtype);
		if (i== m_monster_data.end())
		{
			ERRORMSG("subtype not found: %s",subtype.c_str());
			i = m_monster_data.find("goblin");
		}
		mdata = i->second;
		ret = new Monster(m_world, id,*mdata);
		DEBUG5("Monster created");
	}
	else if (type ==WorldObject::TypeInfo::TYPE_FIXED_OBJECT)
	{
		WorldObject::Geometry* wob;
		Shape* sp;
		DEBUG5("create fixed object: %s",subtype.c_str());
		ret = new FixedObject(m_world,id,subtype);

		wob=ret->getGeometry();
		sp=&(wob->m_shape);
		wob->m_angle =0;

		ret->setState(WorldObject::STATE_STATIC);
		wob->m_layer = (WorldObject::Geometry::LAYER_BASE | WorldObject::Geometry::LAYER_AIR);

		if (subtype =="fence")
		{
			sp->m_type = Shape::RECT;
			sp->m_extent_x = 0.3;
			sp->m_extent_y = 2.5;
		}
		else if (subtype =="tree")
		{
			
				sp->m_type = Shape::CIRCLE;
				sp->m_radius = 2.8;
		}
	}
	return ret;
}


