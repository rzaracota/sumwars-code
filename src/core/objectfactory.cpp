#include "objectfactory.h"

#include "warrior.h"
#include "mage.h"
#include "archer.h"
#include "priest.h"
#include "fixedobject.h"
#include "monster.h"
#include "world.h"




#define USE_OBJECTLOADER


std::map<WorldObject::TypeInfo::ObjectSubtype, MonsterBasicData*> ObjectFactory::m_monster_data;

std::map<WorldObject::TypeInfo::ObjectSubtype, FixedObjectData*> ObjectFactory::m_fixed_object_data;

std::map<ObjectTemplateType, ObjectTemplate*> ObjectFactory::m_object_templates;

std::map<ObjectGroupTemplateName, ObjectGroupTemplate*> ObjectFactory::m_object_group_templates;

std::map< MonsterGroupName, MonsterGroup*>  ObjectFactory::m_monster_groups;


WorldObject::TypeInfo::ObjectSubtype ObjectTemplate::getObject(EnvironmentName env)
{
	// Daten aus der Map suchen
	std::map<EnvironmentName, WorldObjectTypeList >::iterator it;
	it = m_env_objects.find(env);
	
	if (it == m_env_objects.end() || it->second.empty())
	{
		// nichts gefunden
		return "";
	}
	else
	{
		WorldObjectTypeList::iterator jt;
		int r = Random::randi(it->second.size());
		
		jt = it->second.begin();
		for (int i=0; i<r; ++i)
		{
			++jt;
		}
	
		return *jt;
		
	}
}

void ObjectGroupTemplate::addObject(ObjectTemplateType objtype, Vector pos, float angle, float probability )
{
	GroupObject gobj;
	gobj.m_type = objtype;
	gobj.m_center = pos;
	gobj.m_angle = angle;
	gobj.m_probability = probability;
	m_objects.push_back(gobj);
}


WorldObject::TypeInfo::ObjectSubtype ObjectFactory::getObjectType(ObjectTemplateType generictype, EnvironmentName env)
{
	// Namen die nicht mit $ anfangen sind normale Typen
	if (generictype[0] != '$')
	{
		DEBUG5("simple subtype %s",generictype.c_str());
		return generictype;
	}
	
	// Suchen in der Datenbank
	std::map<ObjectTemplateType, ObjectTemplate*>::iterator it;
	it = m_object_templates.find(generictype);
	if (it == m_object_templates.end())
	{
		return "";
	}
	else
	{
		return it->second->getObject(env);
	}
}

WorldObject* ObjectFactory::createObject(WorldObject::TypeInfo::ObjectType type, WorldObject::TypeInfo::ObjectSubtype subtype, int id)
{
	// Zeiger auf erzeugtes Objekt
	WorldObject* ret=0;

	// ID des Objektes
	if (id ==0)
	{
		id = World::getWorld()->getValidId();
	}

	if (type ==WorldObject::TypeInfo::TYPE_PLAYER)
	{
		if (subtype == "warrior")
		{
			ret = new Warrior(  id );
		}
		else if (subtype == "mage")
		{
			ret = new Mage( id );
		}
		else if (subtype == "archer")
		{
			ret = new Archer( id );
		}
		else if(subtype == "priest")
		{
			ret = new Priest( id );
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
		ret = new Monster( id,*mdata);
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
		ret = new FixedObject(id,subtype);

		sp=ret->getShape();		
		memcpy(sp,&(data->m_shape),sizeof(Shape));
		sp->m_angle =0;


		ret->setState(WorldObject::STATE_STATIC);
		ret->setLayer(data->m_layer);
	}
	return ret;
}

ObjectGroupTemplate* ObjectFactory::getObjectGroupTemplate(ObjectGroupTemplateName name)
{
	std::map<ObjectGroupTemplateName, ObjectGroupTemplate*>::iterator it;
	it = m_object_group_templates.find(name);
	
	if (it == m_object_group_templates.end())
	{
		return 0;
	}
	
	return it->second;
}

MonsterGroup* ObjectFactory::getMonsterGroup(MonsterGroupName name)
{
	std::map< MonsterGroupName,MonsterGroup*>::iterator it;
	it = m_monster_groups.find(name);
	
	if (it == m_monster_groups.end())
	{
		return 0;
	}
	
	return it->second;
}


void ObjectFactory::registerMonster(WorldObject::TypeInfo::ObjectSubtype subtype, MonsterBasicData* data)
{
	DEBUG5("registered monster for subtype %s",subtype.c_str());
	m_monster_data.insert(make_pair(subtype,data));
}

void ObjectFactory::registerFixedObject(WorldObject::TypeInfo::ObjectSubtype subtype, FixedObjectData* data)
{
	m_fixed_object_data.insert(std::make_pair(subtype,data));
}

void ObjectFactory::registerObjectTemplate(ObjectTemplateType type, ObjectTemplate* templ)
{
	if (m_object_templates.count(type)>0)
	{
		ERRORMSG("Object template with name %s already exists",type.c_str());
	}
	else
	{
		m_object_templates[type] = templ;
	}
}

void ObjectFactory::registerObjectGroupTemplate(ObjectGroupTemplateName name, ObjectGroupTemplate* data)
{
	
	if (m_object_group_templates.count(name)>0)
	{
		ERRORMSG("Object group template with name %s already exists",name.c_str());
	}
	else
	{
		m_object_group_templates[name] = data;
	}
}

void ObjectFactory::registerMonsterGroup(MonsterGroupName name, MonsterGroup* data)
{
	if (m_monster_groups.count(name)>0)
	{
		ERRORMSG("Monster group with name %s already exists",name.c_str());
	}
	else
	{
		m_monster_groups[name] = data;
	}
}




void ObjectFactory::loadMonsterData(std::string file)
{
	// Objekte bzw. Monster aus XML Laden
	ObjectLoader* objectloader = 0;
	objectloader = new ObjectLoader;
	std::list<MonsterBasicData*>* monster_list;
	monster_list = objectloader->loadMonsterBasicData(file.c_str());

	if (monster_list != 0)
	{
		std::list<MonsterBasicData*>::iterator forward_iterator = monster_list->begin();
		while (forward_iterator != monster_list->end())
		{
			registerMonster((*forward_iterator)->m_type_info.m_subtype, *forward_iterator);
			*forward_iterator++;
		}
	}

	delete monster_list;
	monster_list = 0;
	delete objectloader;
	objectloader = 0;
}

void ObjectFactory::loadFixedObjectData(std::string file)
{
	// Daten fuer feste Objekte
	ObjectLoader* objectloader = 0;
	objectloader = new ObjectLoader;
	std::list<FixedObjectData*>* object_list = 0;
	std::list<std::string>* subtype_list = 0;
	
	objectloader->loadFixedObjectData(file.c_str(), object_list, subtype_list);

	if (object_list != 0)
	{
		std::list<FixedObjectData*>::iterator iter = object_list->begin();
		std::list<std::string>::iterator itersub = subtype_list->begin();
		
		while (iter != object_list->end() && itersub != subtype_list->end())
		{
			registerFixedObject(*itersub,*iter);
			*iter++;
			*itersub++;
		}
	}

	delete object_list;
	object_list = 0;
	delete subtype_list;
	subtype_list = 0;
	delete objectloader;
	objectloader = 0;
}

void ObjectFactory::loadObjectTemplates(std::string file)
{
	// TODO
	DEBUG("reading file %s",file.c_str());
}


void ObjectFactory::loadObjectGroupTemplates(std::string file)
{
	// TODO
	DEBUG("reading file %s",file.c_str());
}

void ObjectFactory::init()
{
	MonsterBasicData* mdata;
	int i;
	float p[4];


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
	
	
	// Monstergruppen
	MonsterGroup* mgdata;
	
	mgdata = new MonsterGroup;
	mgdata->addMonsters("goblin",10,0.7);
	registerMonsterGroup("goblins", mgdata);
	
	
	mgdata = new MonsterGroup;
	mgdata->addMonsters("goblin",6,0.7);
	mgdata->addMonsters("gob_dog",4,0.7);
	registerMonsterGroup("goblins_dogs", mgdata);
	
	mgdata = new MonsterGroup;
	mgdata->addMonsters("lich",1,1.0);
	mgdata->addMonsters("goblin",7,0.7);
	registerMonsterGroup("lich_goblins", mgdata);
	
	
	// feste Objekte
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
	fdata->m_shape.m_extent = Vector(4,4);
	registerFixedObject("stones3",fdata);
	

	// Daten fuer generische Objekte
	
	// Objekt Templates
	ObjectTemplate* objtempl;
	
	objtempl =new ObjectTemplate;
	objtempl->addObject("meadow","tree1");
	objtempl->addObject("meadow","tree2");
	objtempl->addObject("meadow","tree3");
	objtempl->addObject("hills","tree3");
	objtempl->m_type = WorldObject::TypeInfo::TYPE_FIXED_OBJECT;
	registerObjectTemplate("$tree",objtempl);
	
	objtempl =new ObjectTemplate;
	objtempl->addObject("meadow","fence1");
	objtempl->addObject("meadow","fence2");
	objtempl->addObject("meadow","fence3");
	objtempl->addObject("meadow","fence4");
	objtempl->addObject("meadow","fence5");
	objtempl->addObject("hills","wall1");
	objtempl->addObject("hills","wall2");
	objtempl->addObject("hills","wall3");
	objtempl->m_type = WorldObject::TypeInfo::TYPE_FIXED_OBJECT;
	registerObjectTemplate("$border",objtempl);
	
	objtempl =new ObjectTemplate;
	objtempl->addObject("meadow","tree1");
	objtempl->addObject("hills","tree3");
	objtempl->m_type = WorldObject::TypeInfo::TYPE_FIXED_OBJECT;
	registerObjectTemplate("$single_block",objtempl);
	
	// Objektgruppen Templates
	ObjectGroupTemplate* grouptempl;
	
	grouptempl = new ObjectGroupTemplate;
	grouptempl->addObject("$border",Vector(0,3.8),0);
	grouptempl->addObject("$tree",Vector(-1.84,3.2),0,0.5);
	grouptempl->addObject("$tree",Vector(-1.6,0.3),0,0.5);
	grouptempl->addObject("$tree",Vector(1.1,0.82),0,0.5);
	registerObjectGroupTemplate("border(side)",grouptempl);
	
	grouptempl = new ObjectGroupTemplate;
	grouptempl->addObject("$border",Vector(0,3.8),0);
	grouptempl->addObject("$border",Vector(3.8,0),-90*PI/180);
	grouptempl->addObject("$tree",Vector(0.57,-2.78),0,0.5);
	grouptempl->addObject("$tree",Vector(-2.5,2.3),0,0.5);
	registerObjectGroupTemplate("border(corner)",grouptempl);
	
	grouptempl = new ObjectGroupTemplate;
	grouptempl->addObject("$border",Vector(0,3.8),0);
	grouptempl->addObject("$border",Vector(0,-3.8),-180*PI/180);
	grouptempl->addObject("$tree",Vector(-0.43,1.6),0,0.5);
	grouptempl->addObject("$tree",Vector(3.5,-0.85),0,0.5);
	registerObjectGroupTemplate("border(twoside)",grouptempl);
	
	grouptempl = new ObjectGroupTemplate;
	grouptempl->addObject("$border",Vector(0,3.8),0);
	grouptempl->addObject("$border",Vector(3.8,0),-90*PI/180);
	grouptempl->addObject("$border",Vector(0,-3.8),-180*PI/180);
	grouptempl->addObject("$tree",Vector(-0.86,1.9),0,0.5);
	grouptempl->addObject("$tree",Vector(3,-1.3),0,0.5);
	registerObjectGroupTemplate("border(twocorner)",grouptempl);
	
	grouptempl = new ObjectGroupTemplate;
	grouptempl->addObject("$tree",Vector(0,0),0);
	registerObjectGroupTemplate("border(single_block)",grouptempl);
	
	grouptempl = new ObjectGroupTemplate;
	grouptempl->addObject("$tree",Vector(0,3.6),0,0.5);
	grouptempl->addObject("$tree",Vector(-1,-2.3),0,0.5);
	grouptempl->addObject("$tree",Vector(-1.2,3.3),0,0.5);
	grouptempl->addObject("$tree",Vector(0,-1.7),0,0.5);
	registerObjectGroupTemplate("border(filled)",grouptempl);
	
	
	
	grouptempl = new ObjectGroupTemplate;
	grouptempl->getShape()->m_type = Shape::RECT;
	grouptempl->getShape()->m_extent = Vector(8,8);
	grouptempl->addObject("$border",Vector(0,7),0);
	grouptempl->addObject("$border",Vector(7,0),-90*PI/180);
	grouptempl->addObject("$border",Vector(0,-7),0);
	grouptempl->addObject("$border",Vector(-7,0),-90*PI/180);
	registerObjectGroupTemplate("test1",grouptempl);
	
	grouptempl = new ObjectGroupTemplate;
	grouptempl->getShape()->m_type = Shape::RECT;
	grouptempl->getShape()->m_extent = Vector(10,10);
	grouptempl->addObject("$border",Vector(0,9),0);
	grouptempl->addObject("$border",Vector(9,0),-90*PI/180);
	grouptempl->addObject("$border",Vector(0,-9),0);
	grouptempl->addObject("$border",Vector(-9,0),-90*PI/180);
	grouptempl->addObject("$tree",Vector(0,0),0);
	registerObjectGroupTemplate("test2",grouptempl);
	
	grouptempl = new ObjectGroupTemplate;
	grouptempl->getShape()->m_type = Shape::RECT;
	grouptempl->getShape()->m_extent = Vector(16,16);
	grouptempl->addObject("$border",Vector(0,15),0);
	grouptempl->addObject("$border",Vector(15,0),-90*PI/180);
	grouptempl->addObject("$border",Vector(0,-15),0);
	grouptempl->addObject("$border",Vector(-15,0),-90*PI/180);
	grouptempl->addObject("$tree",Vector(0,0),0);
	registerObjectGroupTemplate("test3",grouptempl);
	
	
	grouptempl = new ObjectGroupTemplate;
	grouptempl->getShape()->m_type = Shape::RECT;
	grouptempl->getShape()->m_extent = Vector(2,2);
	grouptempl->addObject("$tree",Vector(0.8,-1.7),0,0.7);
	registerObjectGroupTemplate("trees1",grouptempl);
	
	grouptempl = new ObjectGroupTemplate;
	grouptempl->getShape()->m_type = Shape::RECT;
	grouptempl->getShape()->m_extent = Vector(2,2);
	grouptempl->addObject("$tree",Vector(1.5, 1.2),0,0.7);
	grouptempl->addObject("$tree",Vector(-1.2, -1.7),0,0.7);
	registerObjectGroupTemplate("trees2",grouptempl);
	
	grouptempl = new ObjectGroupTemplate;
	grouptempl->getShape()->m_type = Shape::RECT;
	grouptempl->getShape()->m_extent = Vector(4,4);
	grouptempl->addObject("$tree",Vector(2.5,3.6),0,0.7);
	grouptempl->addObject("$tree",Vector(-1,-2.3),0,0.7);
	grouptempl->addObject("$tree",Vector(-1.7,3.3),0,0.7);
	grouptempl->addObject("$tree",Vector(0.7,-1.7),0,0.7);
	registerObjectGroupTemplate("trees3",grouptempl);
	
	
}

