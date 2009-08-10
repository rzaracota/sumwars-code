/*
	Ein kleines Rollenspiel
	Copyright (C) 2007 Hans Wulf

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

/**
 * \file action.h
 * \defgroup action \ Aktionen
 * \ingroup command
 * \brief Stellt einzelne Aktionen zur Realisierung eines Kommandos zur Verf&uuml;gung
 * \author Hans Wulf
 * \version 1.0
 * \date 2007/05/28
 * \note Beinhaltet die Klasse Action
 */
#ifndef ACTION_H
#define ACTION_H

#include <string>
#include "gettext.h"
#include "charconv.h"
#include "debug.h"
#include "geometry.h"
#include <map>
#include <vector>
#include "../tinyxml/tinyxml.h"

/**
 * \struct Action
 * \brief Beschreibt eine Aktion, welche eine Figur ausf&uuml;hren kann. Eine Aktion ist dabei die kleinste Einheit einer Handlung.
 */
struct Action {
//Public stuff

public:

	/**
	 * \enum ActionType
	 * \brief Aufzaehlung verschiedener Aktionen
	 */
	/*
	enum ActionType
	{
		NOACTION=0,
		WALK=1,
		USE=2,
		TRADE=3,
		ATTACK=4,
		RANGE_ATTACK=5,
		MAGIC_ATTACK=6,
		HOLY_ATTACK=7,
		DEFEND=9,
		WAIT=10,
		DIE=11,
		DEAD=12,
		TAKE_ITEM =13,
  		SPEAK =14,
		SPEAK_ANSWER =15,

		BASH = 32,		//a
		HAMMER_BASH=33,		//b
		AROUND_BLOW = 34,	//c
		WHIRL_BLOW= 35,		//d
		SMASH = 36,		//e
		HATE_MAGE = 37,		//f
		CHARGE = 38,		//g
		STORM_CHARGE = 39,	//h

		BLOCK = 40,
		STEADFAST = 41,
		MONSTER_HUNTER = 42,
		MONSTER_SLAYER = 43,
		ENDURANCE = 44,
		WEAPONMASTER = 45,

		FIRESWORD = 46,		//n
		FLAMESWORD = 47,	//o
		FLAMEARMOR = 48,	//p
		BERSERK = 49,		//q
		WARCRY = 50,		//r
		REGENERATE = 51,	//s
		DECOY = 52,		//t
		SCARE = 53,		//u
		ANGER = 54,		//v
		FURY = 55,		//w

		// Schuetze Faehigkeiten
		TRIPLE_SHOT = 64,	//a
		GUIDED_TRIPLE_SHOT = 65,//b
		MULTISHOT = 66,		//c
		VOLLEY_SHOT =67,	//d
		PIERCE = 68,		//e
		WEAK_POINT = 69,	//f
		BLIND_RAGE = 70,	//g
		VACUUM = 71,		//h
		DEATH_ROULETTE = 72,	//i
		EXPLODING_ARROW = 73,	//j
		EXPLOSION_CASCADE= 74,	//k

		EVADE = 75,
		CRITICAL_STRIKE = 76,
		CONCENTRATION = 77,
		MENTAL_WALL = 78,
		RESIST_ICE = 79,
		RESIST_AIR = 80,

		AIMED_SHOT = 81,	//r
		BOW_SPIRIT = 82,	//s
		ICE_ARROWS = 83,	//t
		FREEZING_ARROWS = 84,	//u
		WIND_ARROWS = 85,	//v
		STORM_ARROWS = 86,	//w
		WIND_WALK = 87,		//x


		FIRE_BOLT =96,		//a
		FIRE_STRIKE = 97,	//b
		FIRE_WAVE = 98,		//c
		FIRE_STORM = 99,	//d
		FIRE_BALL = 100,	//e
		INFERNO_BALL = 101,	//f
		FIRE_WALL = 102,	//g
		INFLAME = 103,

		ICE_BOLT = 104,		//j
		ICE_SPIKE = 105,	//k
		SNOW_STORM = 106,	//l
		BLIZZARD = 107,		//m
		ICE_RING = 108,		//n
		FROST_RING = 109,	//o
		FREEZE = 110,		//p
		CHILL = 111,

		LIGHTNING = 112,	//r
		LIGHTNING_STRIKE = 113,	//s
		THUNDERSTORM = 114,	//t
		THUNDERSTORM2 = 115,	//u
		CHAIN_LIGHTNING = 116,	//v
		CHAIN_LIGHTNING2 = 117,	//w
		STATIC_SHIELD = 118,	//x
		IONISATION = 119,


		// Priester Faehigkeiten
		HOLY_LIGHT = 128,	//a
		HOLY_FIRE = 129,	//b
		BURNING_RAGE = 130,	//c
		CURE_BLIND_MUTE = 131,	//d
		CURE_BLIND_MUTE_PARTY = 132,	//e
		BLAZING_SHIELD = 133,	//f
		LIGHT_BEAM = 134,	//g
		BURNING_SUN = 135,	//h

		BREAK_BINDING = 136,	//i
		DISRUPT_BINDING = 137,	//j
		MAGIC_SHIELD = 138,	//k
		CURE_POIS_BURN = 139,	//l
		CURE_POIS_BURN_PARTY = 140,	//m
		ACID = 141,		//n
		HEAL = 142,		//o
		HEAL_PARTY = 143,	//p

		DIVINE_WIND = 144,	//q
		DIVINE_STORM = 145,	//r
		BLADE_STORM = 146,	//s
		CURE_CONF_BSRK = 147,	//t
		CURE_CONF_BSRK_PARTY = 148,	//u
		HYPNOSIS = 149,		//v
		HYPNOSIS2 = 150,	//w
		KEEN_MIND = 151		//x




	};
*/
	
	typedef  std::string ActionType;
	
	/**
	 * \enum TargetType
	 * \brief Aufzaehlung verschiedener Zieltypen fuer Aktionen
	 */
	enum TargetType
	{

		MELEE = 1,
		RANGED = 2,
		PASSIVE = 4,
		SELF = 8,
 		PARTY =16,
		PARTY_MULTI = 32
	};

	/**
	 * \enum Flags
	 * \brief Aufzaehlung von Sondereigenschaften von Aktionen
	 */
	enum Flags
	{
		MUTE_AFFECTED= 1,
	};

	/**
	 * \struct ActionInfo
	 * \brief Enthaelt einige grundlegende Informationen zu einer Aktion
	 */
	struct ActionInfo
	{
		/**
		 * \var int m_timer_nr
		 * \brief Nummer des Timers, der bei benutzen der Aktion gestartet wird. Moegliche Werte sind 0,1,2
		 */
		int m_timer_nr;

		/**
		 * \var float m_standard_time
		 * \brief Zeit in ms, die die Aktion ohne Modifikatoren in Anspruch nimmt
		 */
		float m_standard_time;

		/**
		 * \var float m_timer
		 * \brief Gibt die Zeitdauer in ms an, die der Timer laeuft, der beim Benutzen dieser Aktion gestartet wird.
		 */
		float m_timer;

		/**
		 * \var ActionType m_base_action
		 * \brief Gibt die Aktion an, die stattdessen ausgefuehrt wird, wenn die Aktion selbst nicht benutzt werden kann weil der Timer noch lauft
		 */
		ActionType m_base_action;

		/**
		 * \var float m_critial_perc
		 * \brief Prozentsatz zu dem die Aktion abgeschlossen ist, wenn der eigentliche Effekt der Aktion ausgeloest wird
		 */
		float m_critical_perc;

		/**
		 * \var TargetType m_target_type
		 * \brief Gibt die Art des Zielobjekts an
		 */
		TargetType m_target_type;

		/**
		 * \var char m_flags
		 * \brief verschiedene Eigenschaften der Aktion in Bitkodierung
		 */
		char m_flags;

		/**
		 * \var std::string m_name
		 * \brief Name der Aktion
		 */
		std::string m_name;

		/**
		 * \var std::string m_description
		 * \brief Abkuerzung der Beschreibung als String
		 **/
		std::string m_description;

		/**
		 * \var short m_req_level
		 * \brief Level, welches benoetigt wird, um die Faehigkeit zu erlernen
		 */
		short m_req_level;

		/**
		 * \var ActionType m_req_ability[3]
		 * \brief Faehigkeiten, die benoetigt werden um die Faehigkeit zu erlernen
		 */
		ActionType m_req_ability[3];


	};

	/**
	 * \fn enum ActionEquip
	 * \brief Aufzaehlung, welche Ausruestungsgegenstaende fuer die Aktion verwendet werden
	 */
	enum ActionEquip
	{
		NO_WEAPON=0,
		ONE_HANDED =1,
		TWO_HANDED =2,
	};

	//Fields
	//Constructors
	/**
	 * \fn Action(ActionType type)
	 * \brief Legt ein neues Action Objekt an
	 * \param type initialisiert den Typ der Aktion
	 */
	Action(ActionType type)
	{
		m_type = type;
		m_time =0;
		m_elapsed_time =0;
	}


	/**
	 * \fn Action()
	 * \brief Legt eine leere Aktion an
	 */
	Action()
	{
		m_type = "noaction";
		m_time =0;
		m_elapsed_time =0;
	}

	/**
	 * \fn static void init()
	 * \brief initialisiert die statischen Variablen
	 */
	static void init();

	/**
	 * \fn static ActionInfo* getActionInfo(ActionType type)
	 * \brief Gibt zu einer Aktion die Basisinformationen aus
	 * \param type die Aktion
	 * \return Zeiger auf die Daten
	 */
	static ActionInfo* getActionInfo(ActionType type)
	{
		std::map<ActionType,ActionInfo>::iterator it;
		it = m_action_info.find(type);
		if (it != m_action_info.end())
			return &(it->second);
		
		return 0;
	}

	/**
	 * \fn static std::string getName(ActionType type)
	 * \brief Gibt zu einer Aktion den Namen aus
	 * \param type die Aktion
	 * \return Name der Aktion
	 */
	static std::string getName(ActionType type);
	
	/**
	 * \fn static std::string getDescription(ActionType type)
	 * \brief Gibt zu einer Aktion die Beschreibung aus
	 * \param type die Aktion
	 * \return Beschreibung der Aktion
	 */
	static std::string getDescription(ActionType type);
	
	/**
	 * \fn static ActionType getType(std::string name)
	 * \brief Gibt zu einem Name einer Aktion den enum Wert aus
	 * \param name Name der Aktion
	 */
	static ActionType getActionType(std::string name);
	
	/**
	 * \fn void toString(CharConv* cv)
	 * \brief Konvertiert das Objekt in einen String und schreibt ihn in der Puffer
	 * \param cvAusgabepuffer
	 */
	void toString(CharConv* cv);
	

	/**
	 * \fn void fromString(CharConv* cv)
	 * \brief Erzeugt das Objekt aus einem String
	 * \param cv Eingabepuffer
	 */
	void fromString(CharConv* cv);
	
	/**
	 * \fn static void loadAbilities(TiXmlNode* node)
	 * \brief Laedt die Faehigkeiten aus dem angegebenen XML Dokument
	 * \param node XML Dokument
	 */
	static void loadAbilities(TiXmlNode* node);
	
	/**
	 * \fn 	static void loadAbility(TiXmlNode* node)
	 * \brief Laedt eine Faehigkeit aus dem angegebenen XML Knoten
	 * \param node XML Knoten
	 */
	static void loadAbility(TiXmlNode* node);
	
	/**
	 * \fn loadAbilityData(const char* pFilename);
	 * \brief Liest die Daten zu einer Faehigkeit aus einer XML Datei
	 * \param pFilename Name der XML Datei
	 */
	static bool loadAbilityData(const char* pFilename);
	
	static void toxml();

	//Fields
	/**
	 * \var m_type;
	 * \brief Typ der Aktion
	 */
	ActionType m_type;

	/**
	 * \var m_elapsed_time;
	 * \brief bereits vergangene Zeit der Aktion
	*/
	float m_elapsed_time;

	/**
	 * \var m_time;
	 * \brief Gesamtdauer der Aktion
	 */
	float m_time;
	
	/**
	 * \var ActionEquip m_action_equip
	 * \brief Ausruestung mit der die Aktion ausgefuehrt wird
	 */
	ActionEquip m_action_equip;

	
	/**
	 * \var Vector m_goal
	 * \brief Ziel der Aktion
	 */
	Vector m_goal;
	
	/**
	 * \var m_goal_object_id;
	 * \brief ID des Zielobjekts
	 */
	int  m_goal_object_id;

	/**
	 * \var static std::map<ActionType,ActionInfo> m_action_info
	 * \brief Enthaelt Basisinformationen fuer alle Aktionen
	 */
	static std::map<ActionType,ActionInfo> m_action_info;

};



#endif //ACTION_H
