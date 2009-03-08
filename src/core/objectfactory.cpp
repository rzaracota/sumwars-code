#include "objectfactory.h"

#include "fixedobject.h"
#include "monster.h"
#include "world.h"
#include "player.h"



#define USE_OBJECTLOADER


std::map<WorldObject::TypeInfo::ObjectSubtype, MonsterBasicData*> ObjectFactory::m_monster_data;

std::map<WorldObject::TypeInfo::ObjectSubtype, FixedObjectData*> ObjectFactory::m_fixed_object_data;

std::map<ObjectTemplateType, ObjectTemplate*> ObjectFactory::m_object_templates;

std::map<ObjectGroupTemplateName, ObjectGroupTemplate*> ObjectFactory::m_object_group_templates;

std::map< MonsterGroupName, MonsterGroup*>  ObjectFactory::m_monster_groups;

std::map<WorldObject::TypeInfo::ObjectSubtype, WorldObject::TypeInfo::ObjectType> ObjectFactory::m_object_types;


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
		ret = new Player(id,subtype);
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
			return 0;
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
			return 0;
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

WorldObject::TypeInfo::ObjectType ObjectFactory::getObjectBaseType(WorldObject::TypeInfo::ObjectSubtype subtype)
{
	std::map<WorldObject::TypeInfo::ObjectSubtype, WorldObject::TypeInfo::ObjectType>::iterator it;
	it = m_object_types.find(subtype);
	if (it != m_object_types.end())
		return it->second;
	
	return WorldObject::TypeInfo::TYPE_NONE;
}


void ObjectFactory::registerMonster(WorldObject::TypeInfo::ObjectSubtype subtype, MonsterBasicData* data)
{
	m_object_types.insert(std::make_pair(subtype, WorldObject::TypeInfo::TYPE_MONSTER));
	
	DEBUG5("registered monster for subtype %s",subtype.c_str());
	m_monster_data.insert(std::make_pair(subtype,data));
}

void ObjectFactory::registerFixedObject(WorldObject::TypeInfo::ObjectSubtype subtype, FixedObjectData* data)
{
	m_object_types.insert(std::make_pair(subtype, WorldObject::TypeInfo::TYPE_FIXED_OBJECT));
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
			/*int b;
			b = (*forward_iterator)->m_base_attr.m_immunity;
			std::cout << "#### immunity " << b << " ####" << std::endl;
			std::cout << "#### abilities " << (*forward_iterator)->m_base_attr.m_abilities[0] << " ####" << std::endl;
			std::cout << "#### abilities " << (*forward_iterator)->m_base_attr.m_abilities[3] << " ####" << std::endl;*/

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
	// ObjectTemplates aus XML Laden
	TemplateLoader templateloader;
	std::list<ObjectTemplate*> object_template_list;
	std::list<std::string> name_list;
	templateloader.loadObjectTemplate(file.c_str(), object_template_list, name_list);
	
	std::list<ObjectTemplate*>::iterator i = object_template_list.begin();
	std::list<std::string>::iterator k = name_list.begin();
	while (i != object_template_list.end() && k != name_list.end())
	{
		registerObjectTemplate(*k,*i);
		i++;
		k++;
	}
}


void ObjectFactory::loadObjectGroupTemplates(std::string file)
{
	// ObjectGroupTemplates aus XML Laden
	
	TemplateLoader templateloader;
	
	std::list<ObjectGroupTemplate*> object_group_template_list;
	std::list<std::string> name_list;
	templateloader.loadObjectGroupTemplate(file.c_str(), object_group_template_list, name_list);
	
	std::list<ObjectGroupTemplate*>::iterator i = object_group_template_list.begin();
	std::list<std::string>::iterator k = name_list.begin();
	
	while (i != object_group_template_list.end() && k != name_list.end())
	{
		registerObjectGroupTemplate(*k,*i);
		i++;
		k++;
	}
}

void ObjectFactory::init()
{

	
	// MonsterGroups aus XML Laden
	ObjectGroupTemplate* grouptempl;
	
	/*grouptempl = new ObjectGroupTemplate;
	grouptempl->addObject("$border",Vector(0,3.8),0);
	grouptempl->addObject("$tree",Vector(-1.84,3.2),0,0.5);
	grouptempl->addObject("$tree",Vector(-1.6,0.3),0,0.5);
	grouptempl->addObject("$tree",Vector(1.1,0.82),0,0.5);
	registerObjectGroupTemplate("border(side)",grouptempl);*/
	
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
	
	/*grouptempl = new ObjectGroupTemplate;
	grouptempl->getShape()->m_type = Shape::RECT;
	grouptempl->getShape()->m_extent = Vector(4,4);
	grouptempl->addObject("$tree",Vector(2.5,3.6),0,0.7);
	grouptempl->addObject("$tree",Vector(-1,-2.3),0,0.7);
	grouptempl->addObject("$tree",Vector(-1.7,3.3),0,0.7);
	grouptempl->addObject("$tree",Vector(0.7,-1.7),0,0.7);
	registerObjectGroupTemplate("trees3",grouptempl);*/
	
	
}

void ObjectFactory::cleanup()
{
	std::map<WorldObject::TypeInfo::ObjectSubtype, MonsterBasicData*>::iterator it1;
	for (it1 = m_monster_data.begin(); it1 != m_monster_data.end(); ++it1)
	{
		delete it1->second;
	}

	std::map<WorldObject::TypeInfo::ObjectSubtype, FixedObjectData*>::iterator it2;
	for (it2 = m_fixed_object_data.begin(); it2 != m_fixed_object_data.end(); ++it2)
	{
		delete it2->second;
	} 
	
	
	std::map<ObjectTemplateType, ObjectTemplate*>::iterator it3;
	for (it3 = m_object_templates.begin(); it3 != m_object_templates.end(); ++it3)
	{
		delete it3->second;
	} 
	
	std::map<ObjectGroupTemplateName, ObjectGroupTemplate*>::iterator it4;
	for (it4 = m_object_group_templates.begin(); it4!= m_object_group_templates.end(); ++it4)
	{
		delete it4->second;
	} 
	
	
	std::map< MonsterGroupName, MonsterGroup*>::iterator it5;
	for (it5 = m_monster_groups.begin(); it5!=m_monster_groups.end(); ++it5)
	{
		delete it5->second;
	} 
}




