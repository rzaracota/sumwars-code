#ifndef PLAYER_BASE_H
#define PLAYER_BASE_H

#include "worldobject.h"
#include "creaturestruct.h"

enum Gender
{
	MALE =1,
 FEMALE = 2,
};

struct LearnableAbility
{
	Action::ActionType m_type;
	
	Vector m_skilltree_position;
	
	int m_id;
	
	int m_skilltree_tab;
	
	short m_req_level;
	
	std::list<Action::ActionType> m_req_abilities;
	
	LearnableAbility& operator=(LearnableAbility other)
	{
		m_type = other.m_type;
		m_skilltree_position = other.m_skilltree_position;
		m_id = other.m_id;
		m_skilltree_tab = other.m_skilltree_tab;
		m_req_level = other.m_req_level;
		m_req_abilities = other.m_req_abilities;
		
		return *this;
	}
};

typedef  std::map<int,LearnableAbility> LearnableAbilityMap;

/**
 * \struct PlayerBasicData
 * \brief Enthaelt alle Daten, die fuer die Initialisierung eines neuen Spielers notwendig sind
 */
struct PlayerBasicData
{
	/**
	 * \var CreatureBaseAttr m_base_attr
	 * \brief Basisattribute
	 */
	CreatureBaseAttr m_base_attr;
	
	/**
	 * \var GameObject::Type m_subtype
	 * \brief Informationen zum Subtyp
	 */
	GameObject::Type m_subtype;
	
	/**
	 * \var std::string m_name
	 * \brief Name der Spielerklasse
	 */
	std::string m_name;
	
	/**
	 * \var Action::ActionType m_base_ability
	 * \brief Basisfaehigkeit der Spielerklasse
	 */
	Action::ActionType m_base_ability;
	
	/**
	 * \var LearnableAbilityMap m_learnable_abilities
	 * \brief erlernbare Faehigkeiten
	 */
	LearnableAbilityMap m_learnable_abilities;
	
	/**
	 * \var std::string m_tabnames[3]
	 * \brief Namen der Skilltreetabs
	 */
	std::string m_tabnames[3];
	
	/**
	 * \var std::list<GameObject::Subtype> m_start_items
	 * \brief Liste der Items, mit denen ein Spieler in das Spiel startet
	 */
	std::list<GameObject::Subtype> m_start_items;
};


#endif

