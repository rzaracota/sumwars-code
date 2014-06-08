/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SUMWARS_CORE_DAMAGE_H__
#define __SUMWARS_CORE_DAMAGE_H__


#include "worldobject.h"
#include <stdlib.h>
#include <string>

#define NR_STATUS_MODS 8

/**
 * \enum AIMods
 * \brief AI modifications
 */
	enum AIMods
{
	TAUNT =0,
 	NR_AI_MODS,
};

/**
 * \struct Damage
 * \brief Parameter for the damage dealt
 */
struct Damage 
{
	
	
	/**
	 * \var m_min_damage[4]
	 * \brief Holds the minimum damage for each of the four damage types.
	 */
	float m_min_damage[4];
	
	/**
	 * \var m_max_damage[4]
	 * \brief Holds the maximum damage for each of the four damage types.
	 */
	float m_max_damage[4];
	
	/**
	 * \var m_multiplier[4]
	 * \brief Multiplier for the each damage type.
	 */
	float m_multiplier[4];
	
	/**
	 * \var m_attack
	 * \brief Specifies the attack value. The attack value influences the hit chance.
	 */
	float m_attack;
	
	/**
	 * \var m_power
	 * \brief Specifies the piercing power. The power influences how much the damage is reduced by the armor.
	 */
	float m_power;
	
	/**
	 * \var  m_attacker_id
	 * \brief ID of the attacker.
	 */
	int m_attacker_id;
	
	/**
	 * \var Fraction::Id m_attacker_fraction
	 * \brief Faction of the attacker
	 */
	Fraction::Id m_attacker_fraction;
	

	/**
	 * \var float m_crit_perc
	 * \brief Chance for critical hit
	 */
	float m_crit_perc;
	
	/**
	 * \var m_status_mod_power
	 * \brief Specifies the spell power for all the status changes. Spell power of 0 means that the effect will have no effect at all to the target.
	 */
	short m_status_mod_power[NR_STATUS_MODS];
	
	/**
	 * \var short m_ai_mod_power[NR_AI_MODS]
	 * \brief Influence of the effect to an AI NPC
	 */
	short m_ai_mod_power[NR_AI_MODS];
	
	/**
	 * \var WorldObject::Race m_extra_dmg_race
	 * \brief Specifies the race to whom the spell does tripple damage. 
	 */
	WorldObject::Race m_extra_dmg_race;
	
	/**
	 * \var m_special_flags
	 * \brief Bitcoded summary of the damage properties. Like extra damage to specified types of monsters.
	 */
	short m_special_flags;
	
	/**
	 * \enum DamageType
	 * \brief Enumeration of all damage types.
	 */
	enum DamageType
	{
		PHYSICAL =0,
		AIR =1,
		ICE =2,
		FIRE =3
	};
	
	/**
	 * \enum StatusMods
	 * \brief Enumeration of all status mods.
	 */
	enum StatusMods
	{
		BLIND =0,
		POISONED =1,
		BERSERK = 2,
		CONFUSED = 3,
		MUTE = 4,
		PARALYZED=5,
		FROZEN = 6,
		BURNING = 7,
		
	};
	
	/**
	 * \enum SpecialFlags
	 * \brief Enumeration of all special properties of a damage type.
	 */
	enum SpecialFlags
	{
		NOFLAGS =0,
		UNBLOCKABLE = 0x1,
		IGNORE_ARMOR = 0x2,
		NOVISUALIZE = 0x4,
		VISUALIZE_SMALL = 0x8,
	};
	
	/**
	 * \enum Usage
	 * \brief Usage of the damage description. // Verwendung der Beschreibung von Schaden (unterschiedlich ausfuehrlich)
	 */
	enum Usage
	{
		NORMAL =0,
		ABILITY =1,
		ITEM =2,
	};	
		
  
	/**
	 * \fn void toString(CharConv* cv)
	 * \brief Converts the object into a string and writes it to a given buffer
	 * \param cv Output buffer
	 */
	void toString(CharConv* cv);
	
	/**
	 * \fn void fromString(CharConv* cv)
	 * \brief Creates the object from a string
	 * \param cv Input buffer
	 */
	void fromString(CharConv* cv);
	
	/**
	 * \fn void init()
	 * \brief Initializes all datafields
	 */
	void init();
	
	/**
	 * \fn void normalize()
	 * \brief Applies all multipliers to the damage values
	 */
	void normalize();
	
	/**
	 * \fn Damage()
	 * \brief Constructor. Creates all fields with 0.
	 */
	Damage();
	
	/**
	 * \fn static std::string getDamageTypeName(DamageType dt)
	 * \brief Returns damage type name as a string.
	 */
	static std::string getDamageTypeName(DamageType dt);
	
	/**
	 * \fn static std::string getDamageResistanceName(DamageType dt)
	 * \brief Returns the resistance name of a damage type as a string.
	 */
	static std::string getDamageResistanceName(DamageType dt);
	
	/**
	 * \fn static std::string getStatusModName(StatusMods sm)
	 * \brief Returns the status change name as a string.
	 */
	static std::string getStatusModName(StatusMods sm);
	
	/**
	 * \fn float getSumMinDamage()
	 * \brief Returns the sum of the minimal damage for all damage types.
	 */
	float getSumMinDamage();
	
	/**
	 * \fn float getSumMaxDamage()
	 * \brief Returns the sum of the maximal damage for all damage types.
	 */
	float getSumMaxDamage();
	
	/**
	 * \fn std::string getDamageString((Usage usage= NORMAL)
	 * \brief Creates a string which displays the specified damage.
	 * \param usage Specifies what the string is used for. This influences which data will be returned.
	 * \param rarityColor color used to display magically enhanced values
	 * \param magic_mods bitset with ones set for magically enhanced values (see \ref ItemFactory::ItemMod )
	 */
	std::string getDamageString(Usage usage= NORMAL, std::string rarityColor="", std::bitset<32>* magic_mods=0);
	
	/**
	 * \fn int getValue(std::string valname)
	 * \brief Pushes the value of an attribute to the lua stack
	 * \param valname Value name
	 */
	int getValue(std::string valname);
	
	/**
	 * \fn virtual bool setValue(std::string valname)
	 * \brief Sets the attribute value of an object
	 * \param valname Value name
	 */
	bool setValue(std::string valname);
	
	/**
	 * \fn void operator=(Damage& other)
	 * \brief Assignment operator overload
	 */
	void operator=(Damage& other);
};
	
#endif // __SUMWARS_CORE_DAMAGE_H__

