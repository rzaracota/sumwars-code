#include "damage.h"

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

