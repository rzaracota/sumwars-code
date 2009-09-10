#ifndef TREASUREBASE_H
#define TREASUREBASE_H

#include "dropslot.h"
#include "worldobject.h"
#include "fixedbase.h"

/**
 * \struct TreasureBasicData
 * \brief Informationen zum Erstellen eines Treasure Objekts
 */
struct TreasureBasicData
{
	/**
	 * \var GameObject::Type m_subtype
	 * \brief Informationen zum Subtyp
	 */
	GameObject::Type m_subtype;
	
	/**
	 * \var DropSlot m_drop_slots[4]
	 * \brief Beschreibung der Items die der Schatz droppen kann
	 */
	DropSlot m_drop_slots[4];
	
	/**
	 * \var std::string m_name
	 * \brief Name des Monsters
	 */
	std::string m_name;
	
	/**
	 * \var FixedObjectData m_fixed_data
	 * \brief Basisdaten analog zu FixedObject
	 */
	FixedObjectData m_fixed_data;
};

#endif

