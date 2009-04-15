#include "creaturestruct.h"
#include "damage.h"
#include "eventsystem.h"


int CreatureBaseAttr::getValue(std::string valname)
{
	if (valname =="level")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_level );
		return 1;
	}
	else if (valname =="strength")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_strength );
		return 1;
	}
	else if (valname =="dexterity")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_dexterity );
		return 1;
	}
	else if (valname =="willpower")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_willpower );
		return 1;
	}
	else if (valname =="magic_power")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_magic_power );
		return 1;
	}
	else if (valname =="max_health")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_max_health );
		return 1;
	}
	else if (valname =="block")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_block );
		return 1;
	}
	else if (valname =="attack")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_attack );
		return 1;
	}
	else if (valname =="armor")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_armor );
		return 1;
	}
	else if (valname =="resist_fire")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_resistances[Damage::FIRE] );
		return 1;
	}
	else if (valname =="resist_ice")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_resistances[Damage::ICE] );
		return 1;
	}
	else if (valname =="resist_air")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_resistances[Damage::AIR] );
		return 1;
	}
	else if (valname =="resist_phys")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_resistances[Damage::PHYSICAL] );
		return 1;
	}
	else if (valname =="max_resist_ice")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_resistances_cap[Damage::ICE] );
		return 1;
	}
	else if (valname =="max_resist_air")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_resistances_cap[Damage::AIR] );
		return 1;
	}
	else if (valname =="max_resist_fire")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_resistances_cap[Damage::FIRE] );
		return 1;
	}
	else if (valname =="max_resist_phys")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_resistances_cap[Damage::PHYSICAL] );
		return 1;
	}
	else if (valname =="attack_speed")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_attack_speed );
		return 1;
	}
	else if (valname =="walk_speed")
	{
		lua_pushinteger(EventSystem::getLuaState() ,m_walk_speed );
		return 1;
	}
	
	return 0;
}

bool CreatureBaseAttr::setValue(std::string valname, int& event_mask)
{
	if (valname =="level")
	{
		m_level = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_ATTRIBUTES_LEVEL;
		return true;
	}
	else if (valname =="strength")
	{
		m_strength = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_ATTRIBUTES_LEVEL;
		return true;
	}
	else if (valname =="dexterity")
	{
		m_dexterity = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_ATTRIBUTES_LEVEL;
		return true;
	}
	else if (valname =="willpower")
	{
		m_willpower = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_ATTRIBUTES_LEVEL;
		return true;
	}
	else if (valname =="magic_power")
	{
		m_magic_power = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_ATTRIBUTES_LEVEL;
		return true;
	}
	else if (valname =="max_health")
	{
		m_max_health = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_ATTRIBUTES_LEVEL;
		return true;
	}
	else if (valname =="block")
	{
		m_block = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="attack")
	{
		m_attack = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="armor")
	{
		m_armor = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="resist_fire")
	{
		m_resistances[Damage::FIRE] = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
		
	}
	else if (valname =="resist_ice")
	{
		m_resistances[Damage::ICE] = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="resist_air")
	{
		m_resistances[Damage::AIR] = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="resist_phys")
	{
		m_resistances[Damage::PHYSICAL] = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="max_resist_ice")
	{
		m_resistances_cap[Damage::ICE] = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="max_resist_air")
	{
		m_resistances_cap[Damage::AIR] = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="max_resist_fire")
	{
		m_resistances_cap[Damage::FIRE] = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="max_resist_phys")
	{
		m_resistances_cap[Damage::PHYSICAL] = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="attack_speed")
	{
		m_attack_speed = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_ATTACK_WALK_SPEED;
		return true;
	}
	else if (valname =="walk_speed")
	{
		m_walk_speed = lua_tointeger(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_ATTACK_WALK_SPEED;
		return true;
	}
	
	return false;
}


int CreatureDynAttr::getValue(std::string valname)
{
	if (valname =="health")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_health );
		return 1;
	}
	else if (valname =="experience")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_experience );
		return 1;
	}
	else if (valname =="blind_time")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_time[Damage::BLIND]);
		return 1;
		
	}
	else if (valname =="poisoned_time")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_time[Damage::POISONED]);
		return 1;
	}
	else if (valname =="berserk_time")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_time[Damage::BERSERK]);
		return 1;
	}
	else if (valname =="confused_time")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_time[Damage::CONFUSED]);
		return 1;
	}
	else if (valname =="mute_time")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_time[Damage::MUTE]);
		return 1;
	}
	else if (valname =="paralyzed_time")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_time[Damage::PARALYZED]);
		return 1;
	}
	else if (valname =="frozen_time")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_time[Damage::FROZEN]);
		return 1;
	}
	else if (valname =="burning_time")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_time[Damage::BURNING]);
		return 1;
	}
	
	return 0;
}

bool CreatureDynAttr::setValue(std::string valname, int& event_mask)
{
	if (valname =="health")
	{
		m_health = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_HP;
		return true;
	}
	else if (valname =="experience")
	{
		m_experience = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_EXPERIENCE;
		return true;
	}
	else if (valname =="blind_time")
	{
		m_status_mod_time[Damage::BLIND] = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_STATUS_MODS;
		return true;
	}
	else if (valname =="poisoned_time")
	{
		m_status_mod_time[Damage::POISONED] = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_STATUS_MODS;
		return true;
	}
	else if (valname =="berserk_time")
	{
		m_status_mod_time[Damage::BERSERK] = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_STATUS_MODS;
		return true;
	}
	else if (valname =="confused_time")
	{
		m_status_mod_time[Damage::CONFUSED] = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_STATUS_MODS;
		return true;
	}
	else if (valname =="mute_time")
	{
		m_status_mod_time[Damage::MUTE] = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_STATUS_MODS;
		return true;
	}
	else if (valname =="paralyzed_time")
	{
		m_status_mod_time[Damage::PARALYZED] = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_STATUS_MODS;
		return true;
	}
	else if (valname =="frozen_time")
	{
		m_status_mod_time[Damage::FROZEN] = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_STATUS_MODS;
		return true;
	}
	else if (valname =="burning_time")
	{
		m_status_mod_time[Damage::BURNING] = lua_tonumber(EventSystem::getLuaState() ,-1);
		lua_pop(EventSystem::getLuaState(), 1);
		event_mask |= NetEvent::DATA_STATUS_MODS;
		return true;
	}
	
	return false;
}

void CreatureSpeakText::operator=( CreatureSpeakText& other)
{
	m_text = other.m_text;
	m_time = other.m_time;
	m_answers = other.m_answers;
}

bool CreatureSpeakText::empty()
{
	return (m_text == "" && m_answers.empty());
}

void CreatureSpeakText::clear()
{
	m_text = "";
	m_answers.clear();
}

