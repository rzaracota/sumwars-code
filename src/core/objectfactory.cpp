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
	else if (type ==WorldObject::TypeInfo::TYPE_MONSTER || type ==WorldObject::TypeInfo::TYPE_NPC)
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
		ret->setState(WorldObject::STATE_STATIC);
		
		sp=ret->getShape();		
		memcpy(sp,&(data->m_shape),sizeof(Shape));
		sp->m_angle =0;
	
		ret->setLayer(data->m_layer);
	}
	return ret;
}

std::string ObjectFactory::getObjectName(WorldObject::TypeInfo::ObjectSubtype subtype)
{
	std::map<WorldObject::TypeInfo::ObjectSubtype, MonsterBasicData*>::iterator i;

	i = m_monster_data.find(subtype);
	if (i== m_monster_data.end())
	{
		return subtype;
	}
	return i->second->m_name;
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




