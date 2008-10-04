#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H



#include <map>
#include "monsterbase.h"
#include "objectloader.h"

#include "../tinyxml/tinyxml.h"
#include <string>
#include <list>
using namespace std;

class World;

/**
 * \class ObjectFactory
 * \brief Erzeugt Objekte vom Typ WorldObject
 */
class ObjectFactory
{
	public:
		
	/**
	 * \fn static WorldObject* createObject(WorldObject::TypeInfo::ObjectType type, WorldObject::TypeInfo::ObjectSubtype subtype, int id=0)
	 * \brief Erzeugt ein Objekt des angefordertens Typs/Subtyps
	 * \param type Typ des Objektes
	 * \param subtype Subtyp des Objektes
	 * \param id ID des Objektes. Wenn id==0 wird eine neue ID generiert
	 */
	static WorldObject* createObject(WorldObject::TypeInfo::ObjectType type, WorldObject::TypeInfo::ObjectSubtype subtype, int id=0);
	
	/**
	 * \var static World* m_world
	 * \brief Welt fuer die die Objekte generiert werden
	 */
	static World* m_world;
		
	/**
	 * \var static map<WorldObject::TypeInfo::ObjectSubtype, MonsterBasicData*> m_monster_data
	 * \brief Basisdaten zu den Monstern
	 */
	static map<WorldObject::TypeInfo::ObjectSubtype, MonsterBasicData*> m_monster_data;
	
	/**
	 * \fn static init()
	 * \brief registriert die Daten der Monster
	 */
	static void init();
	
	/**
	 * \fn static registerMonster(WorldObject::TypeInfo::ObjectSubtype subtype, MonsterBasicData* data)
	 * \brief Registriert die Daten fuer ein bestimmten Monstertyp
	 * \param subtyp Subtyp des Monsters
	 * \param data Daten des Monsters
	 */
	static void registerMonster(WorldObject::TypeInfo::ObjectSubtype subtype, MonsterBasicData* data);
};


#include "world.h"

#endif

