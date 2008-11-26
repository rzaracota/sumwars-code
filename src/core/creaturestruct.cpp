#include "creaturestruct.h"
#include "damage.h"

void CreatureBaseAttr::getMemberReference(VariableRef& ref, std::string member)
{
	if (member =="level")
	{
		ref.setVariable(m_level);
	}
	else if (member =="strength")
	{
		ref.setVariable(m_strength);
	}
	else if (member =="dexterity")
	{
		ref.setVariable(m_dexterity);
	}
	else if (member =="willpower")
	{
		ref.setVariable(m_willpower);
	}
	else if (member =="magic_power")
	{
		ref.setVariable(m_magic_power);
	}
	else if (member =="max_health")
	{
		ref.setVariable(m_max_health);
	}
	else if (member =="block")
	{
		ref.setVariable(m_block);
	}
	else if (member =="attack")
	{
		ref.setVariable(m_attack);
	}
	else if (member =="armor")
	{
		ref.setVariable(m_armor);
	}
	else if (member =="resist_fire")
	{
		ref.setVariable(m_resistances[Damage::FIRE]);
	}
	else if (member =="resist_ice")
	{
		ref.setVariable(m_resistances[Damage::ICE]);
	}
	else if (member =="resist_air")
	{
		ref.setVariable(m_resistances[Damage::AIR]);
	}
	else if (member =="resist_phys")
	{
		ref.setVariable(m_resistances[Damage::PHYSICAL]);
	}
	else if (member =="max_resist_ice")
	{
		ref.setVariable(m_resistances_cap[Damage::ICE]);
	}
	else if (member =="max_resist_air")
	{
		ref.setVariable(m_resistances_cap[Damage::AIR]);
	}
	else if (member =="max_resist_fire")
	{
		ref.setVariable(m_resistances_cap[Damage::FIRE]);
	}
	else if (member =="max_resist_phys")
	{
		ref.setVariable(m_resistances_cap[Damage::PHYSICAL]);
	}
	else if (member =="attack_speed")
	{
		ref.setVariable(m_attack_speed);
	}
	else if (member =="walk_speed")
	{
		ref.setVariable(m_walk_speed);
	}
		
}

void CreatureDynAttr::getMemberReference(VariableRef& ref, std::string member)
{
	if (member =="health")
	{
		ref.setVariable(m_health);
	}
	else if (member =="experience")
	{
		ref.setVariable(m_experience);
	}
	else if (member =="blind_time")
	{
		ref.setVariable(m_status_mod_time[Damage::BLIND]);
	}
	else if (member =="poisoned_time")
	{
		ref.setVariable(m_status_mod_time[Damage::POISONED]);
	}
	else if (member =="berserk_time")
	{
		ref.setVariable(m_status_mod_time[Damage::BERSERK]);
	}
	else if (member =="confused_time")
	{
		ref.setVariable(m_status_mod_time[Damage::CONFUSED]);
	}
	else if (member =="mute_time")
	{
		ref.setVariable(m_status_mod_time[Damage::MUTE]);
	}
	else if (member =="paralyzed_time")
	{
		ref.setVariable(m_status_mod_time[Damage::PARALYZED]);
	}
	else if (member =="frozen_time")
	{
		ref.setVariable(m_status_mod_time[Damage::FROZEN]);
	}
	else if (member =="burning_time")
	{
		ref.setVariable(m_status_mod_time[Damage::BURNING]);
	}

}

