#ifndef PLAYER_BASE_H
#define PLAYER_BASE_H

#include "worldobject.h"
#include "creaturestruct.h"

enum Gender
{
	MALE =1,
 FEMALE = 2,
};

/**
 * \struct PlayerLook
 * \brief Enthaelt grundlegende Informationen fuer die Darstellung des Spielers
 */
struct PlayerLook
{
	/**
	 * \var std::string m_name
	 * \brief Geschlecht des Spielers
	 */
	Gender m_gender;
	
	/**
	 * \var std::string m_name
	 * \brief Name des Looks
	 */
	std::string m_name;
	
	/**
	 * \var std::string m_render_info
	 * \brief Name des RenderInfo
	 */
	std::string m_render_info;
	
	/**
	 * \var std::string m_emotion_set
	 * \brief Name des Satzes von Emotionen des Spielers
	 */
	std::string m_emotion_set;
	
	/**
	 * \fn void operator=(PlayerLook other)
	 * \brief Zuweisungsoperator
	 */
	void operator=(PlayerLook other)
	{
		m_gender = other.m_gender;
		m_render_info = other.m_render_info;
		m_emotion_set = other.m_emotion_set;
		m_name = other.m_name;
	}
	
	/**
	 * \fn void toString(CharConv* cv)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param cv Ausgabepuffer
	*/
	void toString(CharConv* cv)
	{
		cv->toBuffer<char>(m_gender);
		cv->toBuffer(m_render_info);
		cv->toBuffer(m_emotion_set);
	}


	/**
	 * \fn void fromString(CharConv* cv)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param cv Eingabepuffer
	 */
	void fromString(CharConv* cv)
	{
		char tmp;
		cv->fromBuffer(tmp);
		m_gender = (Gender) tmp;
		cv->fromBuffer(m_render_info);
		cv->fromBuffer(m_emotion_set);
	}
};

struct LearnableAbility
{
	Action::ActionType m_type;
	
	Vector m_skilltree_position;
	
	int m_id;
	
	int m_skilltree_tab;
	
	std::string m_image;
	
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
	 * \var std::list<std::string> m_item_req_tags
	 * \brief Liste von Tags, wenn im Itemrequirement eines Items eines der Tags auftaucht, kann das Item verwendet werden
	 */
	std::list<std::string> m_item_req_tags;
	
	/**
	 * \var std::list<GameObject::Subtype> m_start_items
	 * \brief Liste der Items, mit denen ein Spieler in das Spiel startet
	 */
	std::list<GameObject::Subtype> m_start_items;
};


#endif

