#include "damage.h"
#include "eventsystem.h"
#include "gettext.h"

Damage::Damage()
{
	init();
}

void Damage::toString(CharConv* cv)
{

	int i;
	for (i=0;i<4;i++)
	{
		cv->toBuffer(m_min_damage[i]);
	}

	for (i=0;i<4;i++)
	{
		cv->toBuffer(m_max_damage[i]);
	}

	for (i=0;i<4;i++)
	{
		cv->toBuffer(m_multiplier[i]);
	}

	cv->toBuffer(m_attack);
	cv->toBuffer(m_power);
	cv->toBuffer(m_crit_perc);

	for (i=0;i<NR_STATUS_MODS;i++)
	{
		cv->toBuffer(m_status_mod_power[i]);
	}

	cv->toBuffer(m_special_flags);


}


void Damage::fromString(CharConv* cv)
{

	int i;
	for (i=0;i<4;i++)
	{
		cv->fromBuffer<float>(m_min_damage[i]);
	}

	for (i=0;i<4;i++)
	{
		cv->fromBuffer<float>(m_max_damage[i]);
	}

	for (i=0;i<4;i++)
	{
		cv->fromBuffer<float>(m_multiplier[i]);
	}

	cv->fromBuffer<float>(m_attack);
	cv->fromBuffer<float>(m_power);
	cv->fromBuffer<float>(m_crit_perc);

	for (i=0;i<NR_STATUS_MODS;i++)
	{
		cv->fromBuffer<short>(m_status_mod_power[i]);
	}

	cv->fromBuffer<short>(m_special_flags );

}


void Damage::init()
{
	m_attack =0;
	m_power =0;
	m_attacker_id =0;
	m_crit_perc =0;
	m_special_flags =0;
	
	for (int i=0; i<NR_STATUS_MODS; i++)
	{
		m_status_mod_power[i] =0;
	}
	
	for (int i=0; i<NR_AI_MODS; i++)
	{
		m_ai_mod_power[i] =0;
	}
	
	// Multiplikatoren auf 1 setzen
	for (int i=0;i<4;i++)
	{
		m_min_damage[i] =0;
		m_max_damage[i] =0;
		m_multiplier[i]=1;
	}

	m_attacker_fraction = WorldObject::FRAC_HOSTILE_TO_ALL;
}

void Damage::operator=(Damage& other)
{
	m_attack =other.m_attack;
	m_power =other.m_power;
	m_attacker_id =other.m_attacker_id;
	m_crit_perc = other.m_crit_perc;
	m_special_flags =other.m_special_flags;
	
	for (int i=0; i<NR_STATUS_MODS; i++)
	{
		m_status_mod_power[i] =other.m_status_mod_power[i];
	}
	
	for (int i=0; i<NR_AI_MODS; i++)
	{
		m_ai_mod_power[i] = other.m_ai_mod_power[i];
	}
	
	// Multiplikatoren auf 1 setzen
	for (int i=0;i<4;i++)
	{
		m_min_damage[i] =other.m_min_damage[i];
		m_max_damage[i] =other.m_max_damage[i];
		m_multiplier[i]=other.m_multiplier[i];
	}

	m_attacker_fraction = other.m_attacker_fraction;
}

std::string Damage::getDamageTypeName(DamageType dt)
{
	switch(dt)
	{
		case PHYSICAL:
			return gettext("Physical");
		case FIRE:
			return gettext("Fire");
		case ICE:
			return gettext("Ice");
		case AIR:
			return gettext("Air");
	}
	return "";
}

std::string Damage::getDamageResistanceName(DamageType dt)
{
	switch(dt)
	{
		case PHYSICAL:
			return gettext("Physical resistance");
		case FIRE:
			return gettext("Fire resistance");
		case ICE:
			return gettext("Ice resistance");
		case AIR:
			return gettext("Air resistance");
	}
	return "";
}

std::string Damage::getStatusModName(StatusMods sm)
{
	switch(sm)
	{
		case BLIND:
			return gettext("blind");
		case POISONED:
			return gettext("poisoned");
		case MUTE:
			return gettext("mute");
		case CONFUSED:
			return gettext("confused");
		case BERSERK:
			return gettext("berserk");
		case PARALYZED:
			return gettext("paralyzed");
		case FROZEN:
			return gettext("frozen");
		case BURNING:
			return gettext("burning");

	}
	return "";
}

void Damage::normalize()
{
	for (int i=0;i<4;i++)
	{
		m_min_damage[i] *= m_multiplier[i];
		m_max_damage[i] *= m_multiplier[i];
		m_multiplier[i]=1;
	}
}

float Damage::getSumMinDamage()
{
	float s=0;
	for (int i =0;i<4;i++)
		s+=m_min_damage[i];
	return s;
}

float Damage::getSumMaxDamage()
{
	float s=0;
	for (int i =0;i<4;i++)
		s+=m_max_damage[i];
	return s;
}

std::string Damage::getDamageString(Damage::Usage usage)
{
	std::ostringstream out_stream;
	out_stream.str("");
	bool first = true;
	int i;
	// Schaden anzeigen
	for (i=0;i<4;i++)
	{
		if (m_max_damage[i]>0)
		{
			if (!first)
				out_stream << "\n";

			first = false;
			out_stream <<getDamageTypeName((DamageType) i) <<": "<<(int) m_min_damage[i]<<"-"<< (int) m_max_damage[i];
		}
	}

	// Multiplikatoren anzeigen
	for (i=0;i<4;i++)
	{
		if (m_multiplier[i]!=1 && (m_max_damage[i]>0 || usage ==ITEM))
		{
			if (!first)
				out_stream << "\n";

			first = false;
			if (m_multiplier[i]>1)
			{
				out_stream <<getDamageTypeName((DamageType) i) <<": +"<<(int) (100*(m_multiplier[i]-1))<< "% "<< gettext("damage");
			}
			else
			{
				out_stream <<getDamageTypeName((DamageType) i) <<": -"<<(int) (100*(1-m_multiplier[i]))<< "% "<< gettext("damage");
			}
		}
	}
	if (usage != NORMAL)
	{
		// Attackewert anzeigen
		if (m_attack>0)
		{
			if (!first)
				out_stream << "\n";

			first = false;
			if (m_special_flags & UNBLOCKABLE)
			{
				out_stream <<gettext("can not be blocked");
			}
			else
			{
				out_stream <<gettext("Attack")<<": "<<(int) m_attack;
			}
		}

		// Durchschlagskraft anzeigen
		if (m_power>0)
		{
			if (!first)
				out_stream << "\n";

			first = false;
			if (m_special_flags & IGNORE_ARMOR)
			{
				out_stream << gettext("ignores armor");
			}
			else
			{
				out_stream <<gettext("Power")<<": "<<(int) m_power;
			}
		}

		// Chance auf kritische Treffer
		if (m_crit_perc>0 && !first)
		{
			out_stream << "\n";

			out_stream << gettext("Chance for critical hit")<<": "<<(int) (100*m_crit_perc)<<"%";

		}
	}

	// Statusveraenderungen
	for (i=0;i<8;i++)
	{
		if (m_status_mod_power[i]>0)
		{
			if (!first)
				out_stream << "\n";

			first = false;

			out_stream <<getStatusModName((StatusMods) i) <<": "<<m_status_mod_power[i];
		}
 	}

	return out_stream.str();

}

int Damage::getValue(std::string valname)
{
	if (valname =="fire_dmg")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_min_damage[FIRE]);
		lua_pushnumber(EventSystem::getLuaState() , m_max_damage[FIRE]);
		return 2;
	}
	else if (valname =="fire_mult")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_multiplier[FIRE]);
		return 1;
	}
	else if (valname =="ice_dmg")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_min_damage[ICE]);
		lua_pushnumber(EventSystem::getLuaState() , m_max_damage[ICE]);
		return 2;
	}
	else if (valname =="ice_mult")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_multiplier[ICE]);
		return 1;
	}
	else if (valname =="air_dmg")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_min_damage[AIR]);
		lua_pushnumber(EventSystem::getLuaState() , m_max_damage[AIR]);
		return 2;
	}
	else if (valname =="air_mult")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_multiplier[AIR]);
		return 1;
	}
	else if (valname =="phys_dmg")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_min_damage[PHYSICAL]);
		lua_pushnumber(EventSystem::getLuaState() , m_max_damage[PHYSICAL]);
		return 2;
	}
	else if (valname =="phys_mult")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_multiplier[PHYSICAL]);
		return 1;
	}
	else if (valname =="attack")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_attack);
		return 1;
	}
	else if (valname =="power")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_power);
		return 1;
	}
	else if (valname =="crit_chance")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_crit_perc);
		return 1;
	}
	else if (valname =="blind")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_power[BLIND]);
		return 1;
	}
	else if (valname =="poison")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_power[POISONED]);
		return 1;
	}
	else if (valname =="berserk")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_power[BERSERK]);
		return 1;
	}
	else if (valname =="confuse")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_power[CONFUSED]);
		return 1;
	}
	else if (valname =="mute")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_power[MUTE]);
		return 1;
	}
	else if (valname =="paralyze")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_power[PARALYZED]);
		return 1;
	}
	else if (valname =="freeze")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_power[FROZEN]);
		return 1;
	}
	else if (valname =="burning")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_status_mod_power[BURNING]);
		return 1;
	}
	else if (valname =="blockable")
	{
		lua_pushboolean(EventSystem::getLuaState() , !(m_special_flags & UNBLOCKABLE));
		return 1;
	}
	else if (valname =="ignore_armor")
	{
		lua_pushboolean(EventSystem::getLuaState() , (m_special_flags & IGNORE_ARMOR));
		return 1;
	}
	else if (valname =="attacker")
	{
		lua_pushinteger(EventSystem::getLuaState() , m_attacker_id);
		return 1;
	}
	return 0;
}

bool Damage::setValue(std::string valname)
{
	int argc = lua_gettop(EventSystem::getLuaState());

	if (valname =="fire_dmg")
	{
		Vector v = EventSystem::getVector(EventSystem::getLuaState(),-1);
		m_min_damage[FIRE] = v.m_x;
		m_max_damage[FIRE] = v.m_y;
		return true;
	}
	else if (valname =="fire_mult")
	{
		m_multiplier[FIRE] = lua_tonumber(EventSystem::getLuaState(),-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="ice_dmg")
	{
		Vector v = EventSystem::getVector(EventSystem::getLuaState(),-1);
		m_min_damage[ICE] = v.m_x;
		m_max_damage[ICE] = v.m_y;
		return true;
	}
	else if (valname =="ice_mult")
	{
		m_multiplier[ICE] = lua_tonumber(EventSystem::getLuaState(),-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="air_dmg")
	{
		Vector v = EventSystem::getVector(EventSystem::getLuaState(),-1);
		m_min_damage[AIR] = v.m_x;
		m_max_damage[AIR] = v.m_y;
		return true;
	}
	else if (valname =="air_mult")
	{
		m_multiplier[AIR] = lua_tonumber(EventSystem::getLuaState(),-1);
		lua_pop(EventSystem::getLuaState(), 1);
	}
	else if (valname =="phys_dmg")
	{
		Vector v = EventSystem::getVector(EventSystem::getLuaState(),-1);
		m_min_damage[PHYSICAL] = v.m_x;
		m_max_damage[PHYSICAL] = v.m_y;
		return true;
	}
	else if (valname =="phys_mult")
	{
		m_multiplier[PHYSICAL] = lua_tonumber(EventSystem::getLuaState(),-1);
		lua_pop(EventSystem::getLuaState(), 1);
	}
	else if (valname =="attack")
	{
		m_attack = lua_tonumber(EventSystem::getLuaState(),-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="power")
	{
		m_power = lua_tonumber(EventSystem::getLuaState(),-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="crit_chance")
	{
		m_crit_perc = lua_tonumber(EventSystem::getLuaState(),-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="blind")
	{
		m_status_mod_power[BLIND] = (short) lua_tonumber(EventSystem::getLuaState(),-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="poison")
	{
		m_status_mod_power[POISONED] = (short) lua_tonumber(EventSystem::getLuaState(),-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="berserk")
	{
		m_status_mod_power[BERSERK] = (short) lua_tonumber(EventSystem::getLuaState(),-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="confuse")
	{
		m_status_mod_power[CONFUSED] = (short) lua_tonumber(EventSystem::getLuaState(),-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="mute")
	{
		m_status_mod_power[MUTE] = (short) lua_tonumber(EventSystem::getLuaState(),-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="paralyze")
	{
		m_status_mod_power[PARALYZED] = (short) lua_tonumber(EventSystem::getLuaState(),-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="freeze")
	{
		m_status_mod_power[FROZEN] = (short) lua_tonumber(EventSystem::getLuaState(),-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="burning")
	{
		m_status_mod_power[BURNING] = (short) lua_tonumber(EventSystem::getLuaState(),-1);
		lua_pop(EventSystem::getLuaState(), 1);
		return true;
	}
	else if (valname =="blockable")
	{
		bool b;
		b = lua_toboolean(EventSystem::getLuaState(),-1);
		lua_pop(EventSystem::getLuaState(), 1);
		if (!b)
		{
			m_special_flags |= UNBLOCKABLE ;
		}
		else
		{
			m_special_flags &= ~UNBLOCKABLE ;
		}
		return true;

	}
	else if (valname =="ignore_armor")
	{
		bool b;
		b = lua_toboolean(EventSystem::getLuaState(),-1);
		lua_pop(EventSystem::getLuaState(), 1);
		if (b)
		{
			m_special_flags |= IGNORE_ARMOR ;
		}
		else
		{
			m_special_flags &= ~IGNORE_ARMOR ;
		}
		return true;
	}
	else if (valname =="attacker")
	{
		int i;
		i = lua_tointeger(EventSystem::getLuaState(),-1);
		lua_pop(EventSystem::getLuaState(), 1);
		m_attacker_id = i;
		return true;
	}

	return false;
}

