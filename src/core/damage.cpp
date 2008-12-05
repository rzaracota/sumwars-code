#include "damage.h"
#include "eventsystem.h"

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
		// alles nullen
	memset(this,0, sizeof(Damage));
	int i;
		
		// Multiplikatoren auf 1 setzen
	for (i=0;i<4;i++)
		m_multiplier[i]=1;
}

std::string Damage::getDamageTypeName(DamageType dt)
{
	switch(dt)
	{
		case PHYSICAL:
			return "Phys.";
		case FIRE:
			return "Feuer";
		case ICE:
			return "Eis";
		case AIR:
			return "Luft";
	}
	return "";
}

std::string Damage::getStatusModName(StatusMods sm)
{
	switch(sm)
	{
		case BLIND:
			return "blind";
		case POISONED:
			return "vergiftet";
		case MUTE:
			return "stumm";
		case CONFUSED:
			return "verwirrt";
		case BERSERK:
			return "Berserker";
		case PARALYZED:
			return "gelaehmt";
		case FROZEN:
			return "eingefroren";
		case BURNING:
			return "brennend";
			
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
				out_stream <<getDamageTypeName((DamageType) i) <<": +"<<(int) (100*(m_multiplier[i]-1))<< "% Schaden";
			}
			else
			{
				out_stream <<getDamageTypeName((DamageType) i) <<": -"<<(int) (100*(1-m_multiplier[i]))<< "% Schaden";
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
				out_stream <<"trifft immer" ;
			}
			else
			{
				out_stream <<"Attacke: "<<(int) m_attack;
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
				out_stream << "ignoriert Ruestung";
			}
			else
			{
				out_stream <<"Durchschlagskraft: "<<(int) m_power;
			}
		}
		
		// Chance auf kritische Treffer
		if (m_crit_perc>0 && !first)
		{
			out_stream << "\n";
			
			out_stream << "Chance auf krit. Treffer: "<<(int) (100*m_crit_perc)<<"%";
			
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
	else if (valname =="ice_dmg")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_min_damage[ICE]);
		lua_pushnumber(EventSystem::getLuaState() , m_max_damage[ICE]);
		return 2;
	}
	else if (valname =="air_dmg")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_min_damage[AIR]);
		lua_pushnumber(EventSystem::getLuaState() , m_max_damage[AIR]);
		return 2;
	}
	else if (valname =="phys_dmg")
	{
		lua_pushnumber(EventSystem::getLuaState() , m_min_damage[PHYSICAL]);
		lua_pushnumber(EventSystem::getLuaState() , m_max_damage[PHYSICAL]);
		return 2;
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
	//(19:37:01)  Esmil:  Lastmerlin: lua_createtable(L, 3, 0); lua_pushnumber(L, x); lua_rawseti(L, -2, 1); ... lua_pushnumber(L,  z); lua_rawseti(L, -2, 3);
	return 0;
}

bool Damage::setValue(std::string valname)
{
	int argc = lua_gettop(EventSystem::getLuaState());
	
	if (valname =="fire_dmg")
	{
		float d1,d2;
		d2 = d1 = lua_tonumber(EventSystem::getLuaState(),3);
		if (argc>=4)
			d2 = lua_tonumber(EventSystem::getLuaState(),4);
		m_min_damage[FIRE] = d1;
		m_max_damage[FIRE] = d2;
		return true;
	}
	else if (valname =="ice_dmg")
	{
		float d1,d2;
		d2= d1 = lua_tonumber(EventSystem::getLuaState(),3);
		if (argc>=4)
			d2 = lua_tonumber(EventSystem::getLuaState(),4);
		m_min_damage[ICE] = d1;
		m_max_damage[ICE] = d2;
		return true;
	}
	else if (valname =="air_dmg")
	{
		float d1,d2;
		d2= d1 = lua_tonumber(EventSystem::getLuaState(),3);
		if (argc>=4)
			d2 = lua_tonumber(EventSystem::getLuaState(),4);
		m_min_damage[AIR] = d1;
		m_max_damage[AIR] = d2;
		return true;
	}
	else if (valname =="phys_dmg")
	{
		float d1,d2;
		d2= d1 = lua_tonumber(EventSystem::getLuaState(),3);
		if (argc>=4)
			d2 = lua_tonumber(EventSystem::getLuaState(),4);
		m_min_damage[PHYSICAL] = d1;
		m_max_damage[PHYSICAL] = d2;
		return true;
	}
	else if (valname =="attack")
	{
		float f;
		f = lua_tonumber(EventSystem::getLuaState(),3);
		m_attack = f;
		return true;
	}
	else if (valname =="power")
	{
		float f;
		f = lua_tonumber(EventSystem::getLuaState(),3);
		m_power = f;
		return true;
	}
	else if (valname =="crit_chance")
	{
		float f;
		f = lua_tonumber(EventSystem::getLuaState(),3);
		m_crit_perc = f;
		return true;
	}
	else if (valname =="blind")
	{
		float f;
		f = lua_tonumber(EventSystem::getLuaState(),3);
		m_status_mod_power[BLIND] = f;
		return true;
	}
	else if (valname =="poison")
	{
		float f;
		f = lua_tonumber(EventSystem::getLuaState(),3);
		m_status_mod_power[POISONED] = f;
		return true;
	}
	else if (valname =="berserk")
	{
		float f;
		f = lua_tonumber(EventSystem::getLuaState(),3);
		m_status_mod_power[BERSERK] = f;
		return true;
	}
	else if (valname =="confuse")
	{
		float f;
		f = lua_tonumber(EventSystem::getLuaState(),3);
		m_status_mod_power[CONFUSED] = f;
		return true;
	}
	else if (valname =="mute")
	{
		float f;
		f = lua_tonumber(EventSystem::getLuaState(),3);
		m_status_mod_power[MUTE] = f;
		return true;
	}
	else if (valname =="paralyze")
	{
		float f;
		f = lua_tonumber(EventSystem::getLuaState(),3);
		m_status_mod_power[PARALYZED] = f;
		return true;
	}
	else if (valname =="freeze")
	{
		float f;
		f = lua_tonumber(EventSystem::getLuaState(),3);
		m_status_mod_power[FROZEN] = f;
		return true;
	}
	else if (valname =="burning")
	{
		float f;
		f = lua_tonumber(EventSystem::getLuaState(),3);
		m_status_mod_power[BURNING] = f;
		return true;
	}
	else if (valname =="blockable")
	{
		bool b;
		b = lua_toboolean(EventSystem::getLuaState(),3);
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
		b = lua_toboolean(EventSystem::getLuaState(),3);
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
		i = lua_tointeger(EventSystem::getLuaState(),3);
		m_attacker_id = i;
		return true;
	}
	
	return false;
}

