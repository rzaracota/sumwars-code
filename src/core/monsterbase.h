#ifndef MONSTER_BASE_H
#define MONSTER_BASE_H

#include "dropslot.h"
#include "worldobject.h"
#include "creaturestruct.h"

/**
 * \struct MonsterBasicData
 * \brief enthaelt alle Daten die zur Erzeugung eines Monsters noetig sind
 */
struct MonsterBasicData
{
	
	/**
	 * \var CreatureBaseAttr m_base_attr
	 * \brief Basisattribute
	 */
	CreatureBaseAttr m_base_attr;
	
	/**
	 * \var WorldObject::TypeInfo m_type_info
	 * \brief Informationen zum Typ
	 */
	WorldObject::TypeInfo m_type_info;
	
	/**
	 * \var DropSlot m_drop_slots[4]
	 * \brief Beschreibung der Items die das Monster droppen kann, wenn es stirbt
	 */
	DropSlot m_drop_slots[4];
	
	/**
	 * \var float m_ai_sight_range
	 * \brief Sichtweite der AI
	 */
	float m_ai_sight_range;
	
	/**
	 * \var m_radius
	 * \brief Radius des Grundkreises
	 */
	float m_radius;
	
	/**
	 * \var short m_layer
	 * \brief Ebene in der das Monster sich befindet
	 */
	short m_layer;
};

#endif

