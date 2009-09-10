#include "objectfactory.h"

#include "fixedobject.h"
#include "monster.h"
#include "world.h"
#include "player.h"
#include "projectile.h"
#include "scriptobject.h"
#include "templateloader.h"
#include "treasure.h"


// globale Daten
std::map<GameObject::Subtype, MonsterBasicData*> ObjectFactory::m_monster_data;

std::map<GameObject::Subtype, FixedObjectData*> ObjectFactory::m_fixed_object_data;

std::map<ObjectTemplateType, ObjectTemplate*> ObjectFactory::m_object_templates;

std::map<ObjectGroupTemplateName, ObjectGroupTemplate*> ObjectFactory::m_object_group_templates;

std::map< MonsterGroupName, MonsterGroup*>  ObjectFactory::m_monster_groups;

std::map<GameObject::Subtype, ScriptObjectData*> ObjectFactory::m_script_object_data;

std::map<GameObject::Subtype, GameObject::Type> ObjectFactory::m_object_types;

std::map<GameObject::Subtype, PlayerBasicData*> ObjectFactory::m_player_data;

std::map<GameObject::Subtype, ProjectileBasicData*> ObjectFactory::m_projectile_data;

std::map<std::string, EmotionSet*> ObjectFactory::m_emotion_sets;

std::multimap< GameObject::Subtype, PlayerLook> ObjectFactory::m_player_look;

std::map<GameObject::Subtype, TreasureBasicData*> ObjectFactory::m_treasure_data;

GameObject::Subtype ObjectTemplate::getObject(EnvironmentName env)
{
	// Daten aus der Map suchen
	std::map<EnvironmentName, WorldObjectTypeList >::iterator it;
	it = m_env_objects.find(env);
	
	if (it == m_env_objects.end() || it->second.empty())
	{
		it = m_env_objects.find(m_default_environment);
		
		if (it == m_env_objects.end() || it->second.empty())
		{
			// nichts gefunden
			return "";
		}
	}
	
	WorldObjectTypeList::iterator jt;
	int r = Random::randi(it->second.size());
	
	jt = it->second.begin();
	for (int i=0; i<r; ++i)
	{
		++jt;
	}

	return *jt;

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


GameObject::Subtype ObjectFactory::getObjectType(ObjectTemplateType generictype, EnvironmentName env)
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
		return generictype.substr(1);
	}
	else
	{
		return it->second->getObject(env);
	}
}

WorldObject* ObjectFactory::createObject(GameObject::Type type, GameObject::Subtype subtype, int id)
{
	// Zeiger auf erzeugtes Objekt
	WorldObject* ret=0;

	

	if (type =="PLAYER")
	{
		ret = new Player(id,subtype);
	}
	else if (type =="MONSTER" || type =="NPC")
	{
		DEBUG5("requested subtype: %s",subtype.c_str());
		MonsterBasicData* mdata;
		std::map<GameObject::Subtype, MonsterBasicData*>::iterator i;

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
	else if (type =="FIXED_OBJECT")
	{
	
		DEBUG5("requested subtype: %s",subtype.c_str());
		FixedObjectData* data;
		std::map<GameObject::Subtype, FixedObjectData*>::iterator i;

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
		ret->setState(WorldObject::STATE_STATIC,false);
		
		sp=ret->getShape();		
		memcpy(sp,&(data->m_shape),sizeof(Shape));
		sp->m_angle =0;
	
		ret->setLayer(data->m_layer);
	}
	else if (type =="SCRIPTOBJECT")
	{
		ScriptObjectData* data;
		std::map<GameObject::Subtype, ScriptObjectData*>::iterator i;

		i = m_script_object_data.find(subtype);
		if (i== m_script_object_data.end())
		{
			ERRORMSG("subtype not found: %s",subtype.c_str());
			return 0;
		}
		data = i->second;
		
		ret = new ScriptObject(id);
		
		Shape* sp;
		sp=ret->getShape();		
		memcpy(sp,&(data->m_fixed_data.m_shape),sizeof(Shape));
		sp->m_angle =0;
		
		ret->setSubtype(subtype);
		ret->setLayer(data->m_fixed_data.m_layer);
		ScriptObject* so = static_cast<ScriptObject*>(ret);
		so->setRenderInfo(data->m_render_info);
		so->setInteractionFlags(data->m_interaction_flags);
		
		std::multimap<TriggerType, Event*>::iterator et;
		Event* ev;
		for (et = data->m_events.begin(); et != data->m_events.end(); ++et)
		{
			ev = new Event(*(et->second));
			so->addEvent(et->first,ev);
		}
		
	}
	else if (type =="TREASURE")
	{
		TreasureBasicData* data;
		std::map<GameObject::Subtype, TreasureBasicData*>::iterator i;

		i = m_treasure_data.find(subtype);
		if (i== m_treasure_data.end())
		{
			ERRORMSG("subtype not found: %s",subtype.c_str());
			return 0;
		}
		data = i->second;
		
		ret = new Treasure(id,*data);
		ret->setSubtype(subtype);
	}
	
	return ret;
}

Projectile* ObjectFactory::createProjectile(GameObject::Subtype subtype, int id)
{
	std::map<GameObject::Subtype, ProjectileBasicData*>::iterator it;
	it = m_projectile_data.find(subtype);
	
	if (it == m_projectile_data.end())
	{
		ERRORMSG("subtype for projectile not found: %s",subtype.c_str());
		return 0;
	}
	
	Projectile* pr = new Projectile(*(it->second),id);
	return pr;
}

std::string ObjectFactory::getObjectName(GameObject::Subtype subtype)
{
	std::map<GameObject::Subtype, MonsterBasicData*>::iterator i;

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

GameObject::Type ObjectFactory::getObjectBaseType(GameObject::Subtype subtype)
{
	std::map<GameObject::Subtype, GameObject::Type>::iterator it;
	it = m_object_types.find(subtype);
	if (it != m_object_types.end())
		return it->second;
	
	return "NONE";
}

PlayerBasicData* ObjectFactory::getPlayerData(GameObject::Subtype subtype)
{
	std::map<GameObject::Subtype, PlayerBasicData*>::iterator it;
	it = m_player_data.find(subtype);
	
	if (it == m_player_data.end())
	{
		return 0;
	}
	
	return it->second;
}

ProjectileBasicData* ObjectFactory::getProjectileData(GameObject::Subtype subtype)
{
	std::map<GameObject::Subtype, ProjectileBasicData*>::iterator it;
	it = m_projectile_data.find(subtype);
	
	if (it == m_projectile_data.end())
	{
		return 0;
	}
	
	return it->second;
}
		
void ObjectFactory::registerMonster(GameObject::Subtype subtype, MonsterBasicData* data)
{
	m_object_types.insert(std::make_pair(subtype, "MONSTER"));
	
	DEBUG5("registered monster for subtype %s",subtype.c_str());
	m_monster_data.insert(std::make_pair(subtype,data));
}

void ObjectFactory::registerFixedObject(GameObject::Subtype subtype, FixedObjectData* data)
{
	m_object_types.insert(std::make_pair(subtype, "FIXED_OBJECT"));
	m_fixed_object_data.insert(std::make_pair(subtype,data));
}

void ObjectFactory::registerScriptObject(GameObject::Subtype subtype, ScriptObjectData* data)
{
	m_object_types.insert(std::make_pair(subtype, "SCRIPTOBJECT"));
	m_script_object_data.insert(std::make_pair(subtype,data));
}

void ObjectFactory::registerProjectile(GameObject::Subtype subtype, ProjectileBasicData* data)
{
	m_object_types.insert(std::make_pair(subtype, "PROJECTILE"));
	m_projectile_data.insert(std::make_pair(subtype,data));
}

void ObjectFactory::registerTreasure(GameObject::Subtype subtype, TreasureBasicData* data)
{
	m_object_types.insert(std::make_pair(subtype, "TREASURE"));
	m_treasure_data.insert(std::make_pair(subtype,data));
}

void ObjectFactory::registerPlayer(GameObject::Subtype subtype, PlayerBasicData* data)
{
	m_object_types.insert(std::make_pair(subtype, "PLAYER"));
	
	DEBUG5("registered playerclass for subtype %s",subtype.c_str());
	m_player_data.insert(std::make_pair(subtype,data));
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

void ObjectFactory::registerEmotionSet(std::string name, EmotionSet* set)
{
	if (m_emotion_sets.count(name)>0)
	{
		ERRORMSG("Emotionset with name %s already exists",name.c_str());
	}
		else
	{
		m_emotion_sets[name] = set;
	}
}

void ObjectFactory::registerPlayerLook(GameObject::Subtype subtype, PlayerLook look)
{
	m_player_look.insert(std::make_pair(subtype, look));
}


EmotionSet* ObjectFactory::getEmotionSet(std::string name)
{
	std::map<std::string, EmotionSet*>::iterator it;
	it = m_emotion_sets.find(name);
	if (it == m_emotion_sets.end())
		return 0;
  
	return it->second;
}

void ObjectFactory::getPlayerLooks(GameObject::Subtype subtype, std::list<  PlayerLook> &looks)
{
	std::multimap< GameObject::Subtype,  PlayerLook >::iterator it,jt;
	it = m_player_look.lower_bound(subtype);
	jt = m_player_look.upper_bound(subtype);

	while (it != jt)
	{
		looks.push_back(it->second);
		++it;
	}
}

PlayerLook* ObjectFactory::getPlayerLook(GameObject::Subtype subtype, std::string name)
{
	std::multimap< GameObject::Subtype,  PlayerLook >::iterator it,jt;
	it = m_player_look.lower_bound(subtype);
	jt = m_player_look.upper_bound(subtype);

	while (it != jt)
	{
		if (it->second.m_name == name)
			return &(it->second);
		++it;
	}
	return 0;
}

void ObjectFactory::init()
{

	
	
}

void ObjectFactory::cleanup()
{
	cleanupObjectData();
	
	std::map<ObjectTemplateType, ObjectTemplate*>::iterator it3;
	for (it3 = m_object_templates.begin(); it3 != m_object_templates.end(); ++it3)
	{
		delete it3->second;
	} 
	m_object_templates.clear();
	
	std::map<ObjectGroupTemplateName, ObjectGroupTemplate*>::iterator it4;
	for (it4 = m_object_group_templates.begin(); it4!= m_object_group_templates.end(); ++it4)
	{
		delete it4->second;
	} 
	m_object_group_templates.clear();
	

	
	std::map<GameObject::Subtype, PlayerBasicData*>::iterator it6;
	for (it6= m_player_data.begin(); it6 != m_player_data.end(); ++it6)
	{
		delete it6->second;
	}
	m_player_data.clear();
	
}

void ObjectFactory::cleanupObjectData()
{
	std::map<GameObject::Subtype, MonsterBasicData*>::iterator it1;
	for (it1 = m_monster_data.begin(); it1 != m_monster_data.end(); ++it1)
	{
		delete it1->second;
	}
	m_monster_data.clear();

	std::map<GameObject::Subtype, FixedObjectData*>::iterator it2;
	for (it2 = m_fixed_object_data.begin(); it2 != m_fixed_object_data.end(); ++it2)
	{
		delete it2->second;
	} 
	m_fixed_object_data.clear();
	
	std::map< MonsterGroupName, MonsterGroup*>::iterator it5;
	for (it5 = m_monster_groups.begin(); it5!=m_monster_groups.end(); ++it5)
	{
		delete it5->second;
	}
	m_monster_groups.clear();
	
	std::map<GameObject::Subtype, ProjectileBasicData*>::iterator it7;
	for (it7= m_projectile_data.begin(); it7 != m_projectile_data.end(); ++it7)
	{
		delete it7->second;
	} 
	m_projectile_data.clear();
	
	// TODO: Emotionset wirklich loeschen ???
	std::map<std::string, EmotionSet*>::iterator it8;
	for (it8 = m_emotion_sets.begin(); it8 != m_emotion_sets.end(); ++it8)
	{
		delete it8->second;
	}
	m_emotion_sets.clear();
	
	std::map<GameObject::Subtype, ScriptObjectData*>::iterator it9;
	for (it9 = m_script_object_data.begin(); it9 !=m_script_object_data.end(); ++it9)
	{
		delete it9->second;
	}
	m_script_object_data.clear();
	
	std::map<GameObject::Subtype, TreasureBasicData*>::iterator it10;
	for (it10 = m_treasure_data.begin(); it10 != m_treasure_data.end(); ++it10)
	{
		delete it10->second;
	}
	m_treasure_data.clear();
}



