#include "objectfactory.h"

#include "warrior.h"
#include "mage.h"
#include "archer.h"
#include "priest.h"
#include "fixedobject.h"
#include "monster.h"

#define USE_OBJECTLOADER


World* ObjectFactory::m_world;

std::map<WorldObject::TypeInfo::ObjectSubtype, MonsterBasicData*> ObjectFactory::m_monster_data;

std::map<WorldObject::TypeInfo::ObjectSubtype, FixedObjectData*> ObjectFactory::m_fixed_object_data;

void ObjectFactory::registerMonster(WorldObject::TypeInfo::ObjectSubtype subtype, MonsterBasicData* data)
{
	DEBUG5("registered monster for subtype %s",subtype.c_str());
	m_monster_data.insert(make_pair(subtype,data));
}

void ObjectFactory::registerFixedObject(WorldObject::TypeInfo::ObjectSubtype subtype, FixedObjectData* data)
{
	m_fixed_object_data.insert(std::make_pair(subtype,data));
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
	std::list<MonsterBasicData*>* monster_list;
	monster_list = objectloader->loadObjects("../data/monsters.xml");

	if (monster_list != 0)
	{
		std::list<MonsterBasicData*>::iterator forward_iterator = monster_list->begin();
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

	delete monster_list;
	monster_list = 0;
	delete objectloader;
	objectloader = 0;
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
	mdata->m_fraction = WorldObject::FRAC_UNDEAD;
	mdata->m_category = WorldObject::UNDEAD;

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
	mdata->m_layer = (WorldObject::LAYER_BASE | WorldObject::LAYER_AIR);
	mdata->m_radius = 1.0;

	// Lich AI Informationen
	mdata->m_ai_sight_range = 8;

	registerMonster("lich",mdata);


	// Goblin dog
	mdata = new MonsterBasicData;

	// Goblin dog Informationen
	mdata->m_type_info.m_type = WorldObject::TypeInfo::TYPE_MONSTER;
	mdata->m_type_info.m_subtype ="gob_dog";
	mdata->m_fraction = WorldObject::FRAC_MONSTER;
	mdata->m_category = WorldObject::ANIMAL;

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
	mdata->m_layer = (WorldObject::LAYER_BASE | WorldObject::LAYER_AIR);
	mdata->m_radius = 0.5;

	// Goblin dog AI Informationen
	mdata->m_ai_sight_range = 8;

	registerMonster("gob_dog",mdata);
	
	FixedObjectData* fdata;
	
	fdata = new FixedObjectData;
	fdata->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
	fdata->m_shape.m_type = Shape::CIRCLE;
	fdata->m_shape.m_radius = 0.15;
	registerFixedObject("tree1",fdata);
	
	fdata = new FixedObjectData;
	fdata->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
	fdata->m_shape.m_type = Shape::CIRCLE;
	fdata->m_shape.m_radius = 0.15;
	registerFixedObject("tree2",fdata);
	
	fdata = new FixedObjectData;
	fdata->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
	fdata->m_shape.m_type = Shape::CIRCLE;
	fdata->m_shape.m_radius = 0.15;
	registerFixedObject("tree3",fdata);
	
	fdata = new FixedObjectData;
	fdata->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
	fdata->m_shape.m_type = Shape::CIRCLE;
	fdata->m_shape.m_radius = 0.15;
	registerFixedObject("tree1",fdata);
	

	fdata = new FixedObjectData;
	fdata->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
	fdata->m_shape.m_type = Shape::RECT;
	fdata->m_shape.m_extent = Vector(3.84,0.15);
	registerFixedObject("fence1",fdata);
	
	fdata = new FixedObjectData;
	fdata->m_shape.m_type = Shape::RECT;
	fdata->m_shape.m_extent = Vector(3.84,0.15);
	registerFixedObject("fence2",fdata);
	
	fdata = new FixedObjectData;
	fdata->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
	fdata->m_shape.m_type = Shape::RECT;
	fdata->m_shape.m_extent = Vector(3.84,0.15);
	registerFixedObject("fence3",fdata);
	
	fdata = new FixedObjectData;
	fdata->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
	fdata->m_shape.m_type = Shape::RECT;
	fdata->m_shape.m_extent = Vector(3.84,0.15);
	registerFixedObject("fence4",fdata);
	
	fdata = new FixedObjectData;
	fdata->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
	fdata->m_shape.m_type = Shape::RECT;
	fdata->m_shape.m_extent = Vector(3.84,0.15);
	registerFixedObject("fence5",fdata);
	
	fdata = new FixedObjectData;
	fdata->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
	fdata->m_shape.m_type = Shape::RECT;
	fdata->m_shape.m_extent = Vector(3.04,0.61);
	registerFixedObject("wall1",fdata);
	
	fdata = new FixedObjectData;
	fdata->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
	fdata->m_shape.m_type = Shape::RECT;
	fdata->m_shape.m_extent = Vector(3.04,0.61);
	registerFixedObject("wall2",fdata);
	
	fdata = new FixedObjectData;
	fdata->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
	fdata->m_shape.m_type = Shape::RECT;
	fdata->m_shape.m_extent = Vector(3.04,0.61);
	registerFixedObject("wall3",fdata);
	
	fdata = new FixedObjectData;
	fdata->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
	fdata->m_shape.m_type = Shape::RECT;
	fdata->m_shape.m_extent = Vector(0.46,0.6);
	registerFixedObject("smallWall1",fdata);
	
	fdata = new FixedObjectData;
	fdata->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
	fdata->m_shape.m_type = Shape::RECT;
	fdata->m_shape.m_extent = Vector(0.46,0.6);
	registerFixedObject("smallWall2",fdata);
	
	fdata = new FixedObjectData;
	fdata->m_layer = WorldObject::LAYER_BASE | WorldObject::LAYER_AIR;
	fdata->m_shape.m_type = Shape::RECT;
	fdata->m_shape.m_extent = Vector(3.18,2.11);
	registerFixedObject("stones3",fdata);
}

WorldObject* ObjectFactory::createObject(WorldObject::TypeInfo::ObjectType type, WorldObject::TypeInfo::ObjectSubtype subtype, int id)
{
	// Zeiger auf erzeugtes Objekt
	WorldObject* ret=0;

	// ID des Objektes
	if (id ==0)
	{
		id = m_world->getValidId();
	}

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
		std::map<WorldObject::TypeInfo::ObjectSubtype, MonsterBasicData*>::iterator i;

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
	
		DEBUG5("requested subtype: %s",subtype.c_str());
		FixedObjectData* data;
		std::map<WorldObject::TypeInfo::ObjectSubtype, FixedObjectData*>::iterator i;

		i = m_fixed_object_data.find(subtype);
		if (i== m_fixed_object_data.end())
		{
			ERRORMSG("subtype not found: %s",subtype.c_str());
			i = m_fixed_object_data.find("tree1");
		}
		data = i->second;
		
		Shape* sp;
		DEBUG5("create fixed object: %s",subtype.c_str());
		ret = new FixedObject(m_world,id,subtype);

		sp=ret->getShape();		
		memcpy(sp,&(data->m_shape),sizeof(Shape));
		sp->m_angle =0;


		ret->setState(WorldObject::STATE_STATIC);
		ret->setLayer(data->m_layer);
/*
		if (subtype =="fence1" || subtype =="fence2" || subtype =="fence3" || subtype =="fence4" || subtype =="fence5")
		{
			sp->m_type = Shape::RECT;
			sp->m_extent = Vector(3.84,0.15);
		}
		else if (subtype =="wall1" || subtype =="wall2" || subtype =="wall3")
		{
				sp->m_type = Shape::RECT;
				sp->m_extent = Vector(3.04,0.61);
		}
		else if (subtype =="smallWall1" || subtype == "smallWall2")
		{
				sp->m_type = Shape::RECT;
				sp->m_extent = Vector(0.46,0.6);
		}
		else if (subtype =="tree1")
		{

				sp->m_type = Shape::CIRCLE;
				sp->m_radius = 0.16;
		}
        else if (subtype =="tree2")
		{

				sp->m_type = Shape::CIRCLE;
				sp->m_radius = 0.14;
		}
        else if (subtype =="tree3")
		{

				sp->m_type = Shape::CIRCLE;
				sp->m_radius = 0.13;
		}
		else if (subtype =="stones3")
		{
				sp->m_type = Shape::RECT;
				sp->m_extent = Vector(3.18,2.11);
		}
		*/
	}
	return ret;
}


